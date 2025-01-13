"""
Contains Celery tasks for the 'main' app.

Defines asynchronous jobs that handle running binaries
and capturing outputs without blocking the main Django process.
"""

import os
import subprocess
from celery import shared_task

@shared_task
def run_binary_task(binary_path, conf_path, run_folder):
    """
    Launches the given binary with two arguments:
      1) The .conf file path
      2) The directory where results must be written.

    Captures stdout/stderr, writes them to a .log file in run_folder,
    and returns a status dict.
    """
    try:
        # 1) Create a "result" subfolder for binary outputs
        result_dir = os.path.join(run_folder, "result")
        os.makedirs(result_dir, exist_ok=True)

        # 2) Run the binary with subprocess
        completed_proc = subprocess.run(
            [binary_path, conf_path, result_dir],
            capture_output=True,  # captures stdout/stderr
            text=True,
            check=True,           # raises CalledProcessError if exit code != 0
            cwd=os.path.dirname(binary_path)  # ensures we run in the binary's directory
        )

        # 3) Write stdout/stderr to a run.log file
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
        # Non-zero exit code from the binary
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
        # Any other error (permissions, missing file, etc.)
        log_path = os.path.join(run_folder, "run.log")
        with open(log_path, "w", encoding="utf-8") as log_file:
            log_file.write(f"Exception: {str(e)}\n")

        return {
            "status": "error",
            "message": str(e)
        }

