"""
Utility/helper functions for the 'main' app.

Contains file/folder scanning logic to list simulation runs
stored in the 'run_data' directory structure.
"""

import os
from django.conf import settings

def get_runs_for_user_model(user_id, model_id):
    """
    Returns a list of dicts describing each run folder for the given user & model.

    Directory structure:
      run_data/<user_id>/<model_id>/
        └── <run_name>/
            ├── sim.conf
            ├── run.log
            └── result/  (binary outputs)

    :param user_id: ID of the user
    :param model_id: ID of the RobModel
    :return: A sorted list of dicts like:
             [
               {
                 'name': run_name,
                 'path': full_path_to_run_folder
               },
               ...
             ]
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

    # Optional: sort by run_name or last modified
    runs.sort(key=lambda r: r['name'])
    return runs

