# main/tasks.py
import os
import subprocess
from celery import shared_task

@shared_task
def run_binary_task(binary_path, conf_path, run_folder):
    """
    Launch the binary with two arguments:
      1. the .conf file path
      2. the directory where results must be written
    Then capture stdout/stderr into a .log file in the run_folder.
    """
    try:
        # 1) Create a "result" subfolder
        result_dir = os.path.join(run_folder, "result")
        os.makedirs(result_dir, exist_ok=True)

        # 2) Run the binary, passing conf_path & result_dir
        completed_proc = subprocess.run(
            [binary_path, conf_path, result_dir],
            capture_output=True,  # capture stdout and stderr
            text=True,            # decode stdout/stderr as text
            check=True,            # raise CalledProcessError if exit code != 0
            cwd=os.path.dirname(binary_path)
        )

        # 3) Save stdout and stderr to a .log file in run_folder
        log_path = os.path.join(run_folder, "run.log")
        with open(log_path, "w", encoding="utf-8") as log_file:
            log_file.write("STDOUT:\n")
            log_file.write(completed_proc.stdout)
            log_file.write("\n\nSTDERR:\n")
            log_file.write(completed_proc.stderr)

        return {
            "status": "success",
            "stdout": completed_proc.stdout,
            "stderr": completed_proc.stderr
        }

    except subprocess.CalledProcessError as e:
        # If the binary fails (non-zero exit code)
        log_path = os.path.join(run_folder, "run.log")
        with open(log_path, "w", encoding="utf-8") as log_file:
            log_file.write("STDOUT:\n")
            log_file.write(e.stdout or "")
            log_file.write("\n\nSTDERR:\n")
            log_file.write(e.stderr or "")

        return {
            "status": "error",
            "stdout": e.stdout,
            "stderr": e.stderr
        }
    except Exception as e:
        # Any other error (permissions, path not found, etc.)
        log_path = os.path.join(run_folder, "run.log")
        with open(log_path, "w", encoding="utf-8") as log_file:
            log_file.write(f"Exception: {str(e)}\n")

        return {
            "status": "error",
            "message": str(e)
        }

