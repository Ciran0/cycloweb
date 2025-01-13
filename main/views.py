import json
import os
import shutil

from django.shortcuts import render, redirect, get_object_or_404
from django.http import (
    HttpResponseForbidden,
    HttpResponse,
    JsonResponse
)
from django.template.loader import render_to_string
from django.contrib import messages
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.conf import settings
from django.views.decorators.http import require_POST

from celery.result import AsyncResult

from .models import RobModel, SiteSettings
from .utils import get_runs_for_user_model
from .tasks import run_binary_task


def index(request):
    """
    Renders the landing page of the site, optionally including site-wide text
    from the first (and only) SiteSettings record.
    """
    site_settings = SiteSettings.objects.first()
    return render(request, 'main/index.html', {'site_settings': site_settings})


def footer(request):
    """
    Renders the footer template. If the SiteSettings model includes footer-related
    fields, they can be fetched here.
    """
    site_settings = SiteSettings.objects.first()
    return render(request, 'main/footer.html', {'site_settings': site_settings})


def login_view(request):
    """
    Authenticates a user with a username and password, typically invoked via a modal.
    If credentials are valid, the user is logged in and redirected to the index page.
    Otherwise, an error message is set and the user is redirected back to index.
    """
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        user = authenticate(request, username=username, password=password)
        if user:
            login(request, user)
            messages.success(request, 'Logged in successfully!')
        else:
            messages.error(request, 'Invalid credentials. Please try again.')
        return redirect('index')
    # Fallback for GET requests
    return redirect('index')


def logout_view(request):
    """
    Logs out the current user. Typically triggered from a modal confirmation.
    Always redirects to the index page.
    """
    if request.method == 'POST':
        logout(request)
        messages.success(request, 'Logged out successfully!')
    return redirect('index')


@login_required
def model_dashboard(request, model_id):
    """
    Main dashboard for a specific RobModel. Shows:
      - A dynamic "Generate" form (using rob_model.parameters)
      - A list of previous runs
      - Graph configuration options
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("You don't have access to this model.")

    # Parse parameter JSON for the "Generate" section
    try:
        param_list = json.loads(rob_model.parameters)
    except (ValueError, TypeError):
        param_list = []

    # Parse JSON for available graphs
    try:
        graph_config_list = (
            json.loads(rob_model.graph_config)
            if rob_model.graph_config else []
        )
    except (ValueError, TypeError):
        graph_config_list = []

    # Gather runs the user has created for this model
    runs = get_runs_for_user_model(request.user.id, rob_model.id)

    context = {
        'rob_model': rob_model,
        'param_list': param_list,
        'graph_config_list': graph_config_list,
        'runs': runs
    }
    return render(request, 'main/model_dashboard.html', context)


def set_nested_value(root_dict, keys, value):
    """
    Given a list of keys (e.g. ["mass", "Frame"]) and a value (e.g. 85.0),
    sets root_dict["mass"]["Frame"] = 85.0, creating nested dicts if necessary.
    """
    current = root_dict
    for k in keys[:-1]:
        if k not in current:
            current[k] = {}
        current = current[k]
    current[keys[-1]] = value


@login_required
def simulate_run(request, model_id):
    """
    Handles the creation of a new simulation run for the given RobModel:
      1. Validates access to the model.
      2. Builds a .conf file with user-specified parameters.
      3. Queues a Celery task to run the binary asynchronously.
      4. Returns a partial that indicates the simulation has started
         (optionally providing a Celery task ID for polling).
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("You don't have access to this model.")

    try:
        param_list = json.loads(rob_model.parameters)
    except (ValueError, TypeError):
        param_list = []

    if request.method == "POST":
        run_name = request.POST.get('run_name', '').strip()

        # Build nested dictionary from param_list + form input
        conf_dict = {}
        for param in param_list:
            param_name = param.get("name")
            param_type = param.get("type", "str")
            param_keys = param.get("keys_to_rob_conf", [])
            user_input_str = request.POST.get(param_name, "")

            # Convert user input to appropriate type
            if param_type == "int":
                try:
                    user_value = int(user_input_str)
                except ValueError:
                    user_value = 0
            elif param_type == "float":
                try:
                    user_value = float(user_input_str)
                except ValueError:
                    user_value = 0.0
            else:
                user_value = user_input_str

            # Insert the value into conf_dict (possibly nested)
            if param_keys:
                set_nested_value(conf_dict, param_keys, user_value)
            else:
                conf_dict[param_name] = user_value

        # Create the run folder structure
        user_folder = os.path.join(settings.RUN_DATA_ROOT, str(request.user.id))
        model_folder = os.path.join(user_folder, str(rob_model.id))
        safe_run_name = run_name.replace(' ', '_') or 'run'
        run_folder = os.path.join(model_folder, safe_run_name)
        os.makedirs(run_folder, exist_ok=True)

        # Write the .conf file
        conf_path = os.path.join(run_folder, 'sim.conf')
        with open(conf_path, 'w') as f:
            json.dump(conf_dict, f, indent=4)

        # Queue Celery task to run the binary
        task = run_binary_task.delay(
            rob_model.path,
            conf_path,
            run_folder
        )

        # Return partial template with a "simulation started" message and the task ID
        return render(
            request,
            'main/partial_generate_div.html',
            {
                'rob_model': rob_model,
                'param_list': param_list,
                'message': f"Simulation '{run_name}' has started...",
                'task_id': task.id,
            }
        )

    # If GET, redirect to model dashboard
    return redirect('model_dashboard', model_id=model_id)


@login_required
def check_task_status(request, task_id):
    """
    Polling endpoint for Celery task status. Renders a snippet describing
    whether the task is PENDING, STARTED, SUCCESS, or FAILURE.
    """
    # For demonstration, a specific RobModel is fetched. You might adjust as needed.
    rob_model = get_object_or_404(RobModel, id=2)
    result = AsyncResult(task_id)
    status = result.status  # e.g. 'PENDING', 'SUCCESS', etc.

    context = {
        'status': status,
        'task_result': result.result if result.ready() else None,
        'task_id': task_id,
        'rob_model': rob_model,
    }
    snippet = render_to_string('main/partial_task_status.html', context)
    return HttpResponse(snippet)


@login_required
def show_conf(request, model_id, run_name):
    """
    Returns the content of a run's .conf file in a small partial view.
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    run_folder = os.path.join(
        settings.RUN_DATA_ROOT,
        str(request.user.id),
        str(model_id),
        run_name
    )
    conf_path = os.path.join(run_folder, "sim.conf")
    conf_content = ""

    if os.path.isfile(conf_path):
        with open(conf_path, 'r', encoding='utf-8') as f:
            conf_content = f.read()

    return render(request, 'main/partial_conf_view.html', {
        'run_name': run_name,
        'conf_content': conf_content
    })


@login_required
def show_log(request, model_id, run_name):
    """
    Returns the content of a run's .log file in a partial view.
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    run_folder = os.path.join(
        settings.RUN_DATA_ROOT,
        str(request.user.id),
        str(model_id),
        run_name
    )
    log_path = os.path.join(run_folder, "run.log")
    log_content = ""

    if os.path.isfile(log_path):
        with open(log_path, 'r', encoding='utf-8') as f:
            log_content = f.read()

    return render(request, 'main/partial_log_view.html', {
        'run_name': run_name,
        'log_content': log_content
    })


@login_required
def refresh_runs(request, model_id):
    """
    Returns partial_list_runs.html with an updated list of runs for the user
    and model. Useful for refreshing the 'list' div via HTMX.
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    runs = get_runs_for_user_model(request.user.id, rob_model.id)
    return render(
        request,
        'main/partial_list_runs.html',
        {'rob_model': rob_model, 'runs': runs}
    )


@login_required
def confirm_delete_run(request, model_id, run_name):
    """
    Displays a confirmation partial for deleting a specific run.
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    return render(request, 'main/partial_delete_confirm.html', {
        'model_id': model_id,
        'run_name': run_name
    })


@login_required
def delete_run(request, model_id, run_name):
    """
    Deletes the specified run directory if the user confirms via POST.
    After deletion, returns an updated partial_list_runs.html snippet
    so the UI can reflect the change.
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    if request.method == "POST":
        run_folder = os.path.join(
            settings.RUN_DATA_ROOT,
            str(request.user.id),
            str(model_id),
            run_name
        )
        if os.path.isdir(run_folder):
            shutil.rmtree(run_folder)

        runs = get_runs_for_user_model(request.user.id, rob_model.id)
        return render(
            request,
            'main/partial_list_runs.html',
            {
                'runs': runs,
                'rob_model': rob_model,
                'close_delete_modal': True  # used to hide modal via JS
            }
        )
    return redirect('model_dashboard', model_id=model_id)


@login_required
@require_POST
def draw_graph(request, model_id):
    """
    Builds the data & layout structures for a Plotly chart comparing multiple runs.
    Expects:
      - 'runs[]' POST data for selected run names
      - 'graph_name' for which graph definition to use (from rob_model.graph_config)
    Returns JSON with {data: [...], layout: {...}} to be used by Plotly.newPlot().
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("You don't have access to this model")

    runs = request.POST.getlist('runs[]', [])
    graph_name = request.POST.get('graph_name', '')

    # Parse the model's graph_config JSON
    try:
        graph_config_list = json.loads(rob_model.graph_config or '[]')
    except (ValueError, TypeError):
        graph_config_list = []

    # Find the matching graph definition
    selected_graph = None
    for gconf in graph_config_list:
        if gconf.get('name') == graph_name:
            selected_graph = gconf
            break

    if not selected_graph:
        return JsonResponse({
            'error': f"Graph '{graph_name}' not found in config."
        }, status=400)

    # Extract relevant graph details
    result_file = selected_graph.get("result_file", "_q")  # e.g. "_q", "_qdd"
    column_id = selected_graph.get("column_id", 1)
    decimation = selected_graph.get("decimation", 1)
    xlabel = selected_graph.get("xlabel", "Time")
    ylabel = selected_graph.get("ylabel", "")
    title = selected_graph.get("title", "My Plot")

    # Build Plotly-like "data" list
    traces = []
    for run_name in runs:
        # By convention, result files end with e.g. "_q.res" in a "result" subfolder
        run_folder = os.path.join(
            settings.RUN_DATA_ROOT,
            str(request.user.id),
            str(model_id),
            run_name,
            'result'
        )
        chosen_file = None
        if os.path.isdir(run_folder):
            for fname in os.listdir(run_folder):
                if fname.endswith(f"{result_file}.res"):
                    chosen_file = os.path.join(run_folder, fname)
                    break

        if not chosen_file or not os.path.isfile(chosen_file):
            # If file isn't found, skip
            continue

        # Parse the chosen_file line-by-line
        times, values = [], []
        with open(chosen_file, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                parts = line.split()
                if len(parts) <= column_id:
                    continue
                try:
                    t = float(parts[0])
                    v = float(parts[column_id])
                    times.append(t)
                    values.append(v)
                except ValueError:
                    pass

        # Optionally decimate the data
        if decimation > 1:
            times = times[::decimation]
            values = values[::decimation]

        # Create a trace for this run
        if times:
            trace = {
                "x": times,
                "y": values,
                "mode": "lines",
                "type": "scatter",
                "name": run_name,
            }
            traces.append(trace)

    # Build the layout
    layout = {
        "title": title,
        "xaxis": {"title": xlabel},
        "yaxis": {"title": ylabel}
    }

    # Return as JSON for client-side Plotly
    return JsonResponse({
        "data": traces,
        "layout": layout
    })

