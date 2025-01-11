import json
import os
from django.shortcuts import render, redirect, get_object_or_404
from django.http import HttpResponseForbidden, HttpResponse, JsonResponse
from django.contrib.auth import authenticate, login, logout
from django.template.loader import render_to_string
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from django.conf import settings
from .models import RobModel
from .models import SiteSettings
from .utils import get_runs_for_user_model
from .tasks import run_binary_task
from celery.result import AsyncResult
from django.views.decorators.http import require_POST


def index(request):
    """
    Landing page view
    """
    site_settings = SiteSettings.objects.first()
    return render(request, 'main/index.html', {'site_settings': site_settings})


def footer(request):
    """
    footer view
    """
    site_settings = SiteSettings.objects.first()
    return render(request, 'main/footer.html', {'site_settings': site_settings})

def login_view(request):
    """
    Handle login (with username & password).
    This is typically loaded in a modal via HTMX or returned in full if user has no JS.
    """
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        user = authenticate(request, username=username, password=password)
        if user is not None:
            login(request, user)
            messages.success(request, 'Logged in successfully!')
            return redirect('index')
        else:
            messages.error(request, 'Invalid credentials. Please try again.')
            return redirect('index')
    # If GET, just show index or you could return partial_login_form if using HTMX
    return redirect('index')


def logout_view(request):
    """
    Handle logout. Possibly triggered from a modal confirmation.
    """
    if request.method == 'POST':
        logout(request)
        messages.success(request, 'Logged out successfully!')
    return redirect('index')


@login_required
def model_dashboard(request, model_id):
    rob_model = get_object_or_404(RobModel, id=model_id)

    # Check if user can access
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("You don't have access to this model.")

    # Parse parameters
    try:
        param_list = json.loads(rob_model.parameters)

    except (ValueError, TypeError):
        param_list = []

    try:
        graph_config_list = json.loads(rob_model.graph_config) if rob_model.graph_config else []
    except (ValueError, TypeError):
        graph_config_list = []

    # Get existing runs
    runs = get_runs_for_user_model(request.user.id, rob_model.id)

    context = {
        'rob_model': rob_model,
        'param_list': param_list,
        'runs': runs,
        'graph_config_list': graph_config_list
    }
    return render(request, 'main/model_dashboard.html', context)


def set_nested_value(root_dict, keys, value):
    """
    Given something like keys = ["mass", "Frame"] and value=85.0,
    build or set root_dict["mass"]["Frame"] = 85.0
    """
    current = root_dict
    for k in keys[:-1]:
        if k not in current:
            current[k] = {}
        current = current[k]
    current[keys[-1]] = value


@login_required
def simulate_run(request, model_id):
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("You don't have access to this model.")

    # Parse param_list
    try:
        param_list = json.loads(rob_model.parameters)
    except (ValueError, TypeError):
        param_list = []

    if request.method == "POST":
        run_name = request.POST.get('run_name', '').strip()

        # Build the conf_dict
        conf_dict = {}
        for param in param_list:
            param_name = param.get("name")
            param_type = param.get("type", "str")
            param_keys = param.get("keys_to_rob_conf", [])
            user_input_str = request.POST.get(param_name, "")

            # Convert user_input_str -> int/float if needed
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

            # Insert into nested dict
            if param_keys:
                set_nested_value(conf_dict, param_keys, user_value)
            else:
                conf_dict[param_name] = user_value

        # Create the folder structure
        user_folder = os.path.join(settings.RUN_DATA_ROOT, str(request.user.id))
        model_folder = os.path.join(user_folder, str(rob_model.id))
        safe_run_name = run_name.replace(' ', '_') or 'run'
        run_folder = os.path.join(model_folder, safe_run_name)
        os.makedirs(run_folder, exist_ok=True)

        # Write sim.conf
        conf_path = os.path.join(run_folder, 'sim.conf')
        with open(conf_path, 'w') as f:
            json.dump(conf_dict, f, indent=4)

        # Queue the Celery task
        # The binary takes 2 arguments: the conf path & the results dir
        task = run_binary_task.delay(
            rob_model.path,  # path to the binary
            conf_path,       # .conf file path
            run_folder       # results directory
        )
        # task.id is the Celery task ID

        # Return a snippet that shows "simulation running" and a script or method to poll
        return render(
            request,
            'main/partial_generate_div.html',
            {
                'rob_model': rob_model,
                'param_list': param_list,
                'message': f"Simulation '{run_name}' has started...",
                'task_id': task.id,  # pass the Celery task ID to the template
            }
        )

    # If GET
    return redirect('model_dashboard', model_id=model_id)

# main/views.py (revised check_task_status)


@login_required
def check_task_status(request, task_id):
    result = AsyncResult(task_id)
    rob_model = get_object_or_404(RobModel, id=2)
    status = result.status  # PENDING, STARTED, SUCCESS, FAILURE, etc.

    context = {
        'status': status,
        'task_result': result.result if result.ready() else None,
        'task_id': task_id,
        'rob_model': rob_model,
    }
    # We'll return a small HTML snippet
    html_snippet = render_to_string('main/partial_task_status.html', context)
    return HttpResponse(html_snippet)


@login_required
def show_conf(request, model_id, run_name):
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    run_folder = os.path.join(settings.RUN_DATA_ROOT, str(request.user.id), str(model_id), run_name)
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
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    run_folder = os.path.join(settings.RUN_DATA_ROOT, str(request.user.id), str(model_id), run_name)
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
    Returns the partial_list_runs.html snippet
    with the updated list of runs for the user & model.
    Useful when we want to refresh the 'list' div.
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
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    return render(request, 'main/partial_delete_confirm.html', {
        'model_id': model_id,
        'run_name': run_name
    })


@login_required
def delete_run(request, model_id, run_name):
    rob_model = get_object_or_404(RobModel, id=model_id)
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("Access denied.")

    if request.method == "POST":
        run_folder = os.path.join(settings.RUN_DATA_ROOT, str(request.user.id), str(model_id), run_name)
        import shutil
        if os.path.isdir(run_folder):
            shutil.rmtree(run_folder)

        runs = get_runs_for_user_model(request.user.id, rob_model.id)
        return render(
            request,
            'main/partial_list_runs.html',
            {
                'runs': runs,
                'rob_model': rob_model,
                # add a flag so we can hide the modal automatically
                'close_delete_modal': True
            }
        )
    return redirect('model_dashboard', model_id=model_id)


@login_required
@require_POST
def draw_graph(request, model_id):
    """
    Builds the data & layout for a Plotly chart and returns JSON.
    The client will call Plotly.newPlot(...) with this JSON.
    """
    rob_model = get_object_or_404(RobModel, id=model_id)
    # Check access
    if rob_model not in request.user.profile.robmodels.all():
        return HttpResponseForbidden("You don't have access to this model")

    runs = request.POST.getlist('runs[]', [])
    graph_name = request.POST.get('graph_name', '')

    # Parse graph_config
    try:
        graph_config_list = json.loads(rob_model.graph_config or '[]')
    except (ValueError, TypeError):
        graph_config_list = []

    # Find the relevant graph definition
    selected_graph = None
    for gconf in graph_config_list:
        if gconf.get('name') == graph_name:
            selected_graph = gconf
            break

    if not selected_graph:
        return JsonResponse({
            'error': f"Graph '{graph_name}' not found in config."
        }, status=400)

    # Extract relevant fields
    result_file = selected_graph.get("result_file", "_q")  # e.g. "_q", "_qdd", ...
    column_id = selected_graph.get("column_id", 1)
    decimation = selected_graph.get("decimation", 1)
    xlabel = selected_graph.get("xlabel", "Time")
    ylabel = selected_graph.get("ylabel", "")
    title = selected_graph.get("title", "My Plot")

    # We'll build a "data" list of Plotly-like traces
    traces = []

    for run_name in runs:
        # Figure out which file in <run_folder> ends with f"{result_file}.res"
        run_folder = os.path.join(settings.RUN_DATA_ROOT, str(request.user.id), str(model_id), run_name,'result')
        chosen_file = None
        if os.path.isdir(run_folder):
            for fname in os.listdir(run_folder):
                if fname.endswith(f"{result_file}.res"):
                    chosen_file = os.path.join(run_folder, fname)
                    break

        if not chosen_file or not os.path.isfile(chosen_file):
            # skip if file not found
            continue

        # Parse the chosen_file line by line
        times = []
        values = []
        with open(chosen_file, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                parts = line.split()
                # Make sure we have enough columns
                if len(parts) <= column_id:
                    continue
                try:
                    t = float(parts[0])
                    v = float(parts[column_id])
                    times.append(t)
                    values.append(v)
                except ValueError:
                    pass

        # Decimate if needed
        if decimation > 1:
            times = times[::decimation]
            values = values[::decimation]

        # Build a "trace" object as JSON
        trace = {
            "x": times,
            "y": values,
            "mode": "lines",
            "type": "scatter",
            "name": run_name  # e.g. show run_name in the legend
        }
        if times:
            traces.append(trace)

    # Build layout
    layout = {
        "title": title,
        "xaxis": {"title": xlabel},
        "yaxis": {"title": ylabel}
    }
    # If you want a config dictionary, e.g. `plotly_config`, you could return it too.

    # Return as JSON
    # The client will do: Plotly.newPlot("someDiv", data, layout)
    return JsonResponse({
        "data": traces,
        "layout": layout
    })
