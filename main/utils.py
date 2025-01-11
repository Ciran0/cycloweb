# main/utils.py (new file, optional)
import os
from django.conf import settings

def get_runs_for_user_model(user_id, model_id):
    """
    Returns a list of dicts: [{'name': run_name, 'path': run_folder}, ...]
    by scanning run_data/<user_id>/<model_id>/ subfolders.
    """
    user_folder = os.path.join(settings.RUN_DATA_ROOT, str(user_id))
    model_folder = os.path.join(user_folder, str(model_id))

    runs = []
    if os.path.isdir(model_folder):
        for run_name in os.listdir(model_folder):
            run_path = os.path.join(model_folder, run_name)
            if os.path.isdir(run_path):
                runs.append({
                    'name': run_name,
                    'path': run_path
                })
    # Sort by name or last modified date if you want
    runs.sort(key=lambda r: r['name'])
    return runs

