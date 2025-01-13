# Cycloweb Project

A Django-based web application that lets users run simulations (via binaries), generate runs with dynamic parameters, store results, visualize them with Plotly, and manage everything through a clean, modular interface. This README encapsulates the project setup from start to finish.

## Table of Contents

1. [Project Overview](#project-overview)  
2. [Features](#features)  
3. [Architecture & File Structure](#architecture--file-structure)  
4. [Models](#models)  
5. [Authentication & User Profiles](#authentication--user-profiles)  
6. [Views & Templates Overview](#views--templates-overview)  
7. [Running Simulations (Celery + Binaries)](#running-simulations-celery--binaries)  
8. [HTMX Integration](#htmx-integration)  
9. [Dynamic Forms for Parameters](#dynamic-forms-for-parameters)  
10. [Listing Runs & Managing Results](#listing-runs--managing-results)  
11. [Graphs & Plotly Integration](#graphs--plotly-integration)  
12. [Editable Site Settings](#editable-site-settings)  
13. [Deployment Guidelines](#deployment-guidelines)  

---

## 1. Project Overview

**Cycloweb** is a Django web application that simulates physical bodies (e.g., a bike or a pendulum). Each “model” (a **RobModel**) corresponds to a binary and can have dynamic parameters. A user:

1. Logs in, selects a **RobModel** from a navbar dropdown.  
2. Fills in a form (“Generate” section) with parameter values, which produces a `.conf` file on the server and asynchronously runs a binary.  
3. Views and manages all runs in a “List” section.  
4. Plots results in a “Graph” section using Plotly.

Additionally, you can manage user access, model definitions, and site settings through the Django admin.

---

## 2. Features

- **Authentication**: Users can only be created by admins in Django’s admin interface.  
- **RobModel**: Stores binary path, parameters, graph configurations, etc.  
- **Profile-based authorization**: A user’s `Profile` model holds which `RobModel`s they can access.  
- **Dynamic parameter form**: Each RobModel has a JSON describing parameter fields.  
- **Asynchronous tasks**: Celery workers run the binaries in the background.  
- **File Storage**: Each run is stored in `run_data/<user_id>/<model_id>/<run_name>`.  
  - A `.conf` file is generated for each run.  
  - Output is written in a `result` subfolder; a `.log` captures console output.  
- **HTMX** usage for modals, partial page updates (login, logout, run creation, polling, etc.).  
- **Plotly** charting: Compare multiple runs with a single graph definition.  
- **Editable site settings**: Control homepage content, etc., via admin.

---

## 3. Architecture & File Structure

Below is a representative file structure:

```
cycloweb_new/
├── cycloweb_new/
│   ├── __init__.py
│   ├── settings.py             # Could be settings_dev.py
│   ├── settings_prod.py        # Production settings (recommended)
│   ├── urls.py
│   ├── wsgi.py
│   └── asgi.py                 # If you ever use Channels or Daphne
├── main/
│   ├── __init__.py
│   ├── admin.py
│   ├── apps.py
│   ├── models.py
│   ├── tasks.py                # Celery tasks
│   ├── signals.py              # Auto-create profiles
│   ├── utils.py                # Helper functions (e.g. listing runs)
│   ├── views.py                # Core logic
│   ├── context_processors.py
│   ├── urls.py
│   ├── templates/
│   │   └── main/
│   │       ├── base.html
│   │       ├── index.html
│   │       ├── dashboard.html
│   │       ├── partial_*       # partial templates (for modals, forms, etc.)
│   │       └── ...
│   ├── static/
│   │   └── main/
│   │       └── css/
│   └── migrations/
├── run_data/
│   └── ...                     # Contains user-specific model/run data
├── manage.py
└── README.md                   # This document
```

---

## 4. Models

### 4.1 **RobModel**

```python
class RobModel(models.Model):
    name = models.CharField(max_length=100)
    description = models.TextField(blank=True)
    path = models.CharField(max_length=255, help_text="Path to the binary")
    parameters = models.TextField(blank=True, help_text="JSON string for parameters")
    graph_config = models.TextField(blank=True, help_text="JSON for graph definitions")

    def __str__(self):
        return self.name
```

- **parameters**: JSON describing dynamic form fields.  
- **graph_config**: JSON describing available graphs.

### 4.2 **Profile**

```python
class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    robmodels = models.ManyToManyField('RobModel', blank=True)

    def __str__(self):
        return f"Profile of {self.user.username}"
```

- Ties each user to **allowed** `RobModel`s.  

### 4.3 **SiteSettings** (Optional)

```python
class SiteSettings(models.Model):
    homepage_title = models.CharField(max_length=200, default="Welcome to Cycloweb")
    homepage_subtitle = models.CharField(max_length=500, default="Quandyga's first web-based simulation tool")
    def __str__(self):
        return "Site Settings"
```

- Lets an admin manage site-wide text fields.

---

## 5. Authentication & User Profiles

- Users are **created** in the Django admin (`/admin/`).  
- A **Profile** is auto-created via a `post_save` signal.  
- Admin assigns which `RobModel`s each user can access (via the Profile’s `robmodels` many-to-many).

In templates, the navbar shows models a user can access. If logged in, they see “Logout” and a link to the “Dashboard.”

---

## 6. Views & Templates Overview

1. **`index`**: A basic landing page (or uses `SiteSettings` for content).  
2. **`model_dashboard`**: The main page for each RobModel:
   - “Generate” (dynamic form to run a simulation).  
   - “List” (list of runs with param/log modals, delete).  
   - “Graph” (pick runs + a graph definition => see Plotly chart).  
3. **Login/Logout**: Done via modals, with HTMX partials if desired.  
4. **Partial Templates**: 
   - `partial_generate_div.html`, `partial_list_runs.html`, `partial_graph_div.html`—each piece can be dynamically updated.

---

## 7. Running Simulations (Celery + Binaries)

**Celery** handles the binary execution asynchronously:

- **`simulate_run`** view:
  1. Builds a `.conf` file from user input & `parameters` JSON.  
  2. Queues a Celery task `run_binary_task.delay(...)`.  
  3. Returns a partial to show “Simulation started…” + a polling mechanism.  

- **Celery task** (`run_binary_task` in `tasks.py`):
  1. Receives the binary path, the `.conf` path, and a run folder.  
  2. Spawns `subprocess.run([binary_path, conf_path, result_dir])`.  
  3. Captures stdout/stderr into a `.log` file.  
  4. Writes binary outputs into a `result` subfolder.

---

## 8. HTMX Integration

- **Modal** logic for login, logout, param/log viewing, delete confirmation: 
  - `hx-get` or `hx-post` on partial endpoints.  
  - Swaps content in a modal or a target div.  
- **Polling** for Celery task status: 
  - Polls a `check_task_status` endpoint every few seconds, and once it’s `SUCCESS`, updates UI.

HTMX helps reduce page reloads by swapping only relevant sections.

---

## 9. Dynamic Forms for Parameters

Each `RobModel.parameters` is a JSON array describing fields, e.g.:

```json
[
  {
    "name": "frame_mass",
    "type": "float",
    "min": 1.0,
    "max": 500.0,
    "default": 85.0,
    "label": "Mass of the cycle frame",
    "keys_to_rob_conf": ["mass", "Frame"]
  },
  ...
]
```

At runtime, `partial_generate_div.html` loops over the array to produce `<input>` or `<select>` elements. Upon form submission, we build a nested dictionary and write `sim.conf`.

---

## 10. Listing Runs & Managing Results

Runs are stored in:

```
run_data/
└── <user_id>/
    └── <rob_model_id>/
        └── <run_name>/
            ├── sim.conf
            ├── run.log
            └── result/
                └── ...
```

- A **List** div displays all runs for the current model (scanning the folder).  
- Each row has:
  - A **checkbox** (used for graph selection).  
  - A **“param”** button => loads `.conf` in a modal.  
  - A **“log”** button => loads `.log` in a modal.  
  - A **“delete”** button => confirm & remove the entire run folder.  
- After a new run finishes, the “List” auto-updates.

---

## 11. Graphs & Plotly Integration

1. Each **RobModel** has `graph_config` JSON describing available plots, e.g.:

   ```json
   [
     {
       "name": "position_T2_plot",
       "label": "Position T2 Frame",
       "xlabel": "Time [s]",
       "ylabel": "Position [m]",
       "result_file": "_q",
       "column_id": 3,
       "decimation": 1,
       "title": "Lateral position over time"
     }
   ]
   ```

2. The user checks runs in the “List,” picks a graph type from a `<select>` in the “Graph” div, and clicks “Draw Graph.”  
3. A POST to `draw_graph` returns JSON containing Plotly data/layout.  
4. The frontend calls `Plotly.newPlot(...)` to render lines for each selected run.

---

## 12. Editable Site Settings

A `SiteSettings` model allows you to edit homepage text in the admin:

```python
site_settings = SiteSettings.objects.first()
```

Then in `index.html`:

```html
{% if site_settings %}
<h1>{{ site_settings.homepage_title }}</h1>
<p>{{ site_settings.homepage_subtitle }}</p>
{% endif %}
```

---

## 13. Deployment Guidelines

### 13.1 Dev vs Production

- **Development**: Typically run `python manage.py runserver` with `settings_dev.py`.  
- **Production**:  
  - Use `settings_prod.py`, set `DEBUG=False`, and configure `ALLOWED_HOSTS`.  
  - Serve with **Gunicorn** (if synchronous) or **Daphne** (if async/Channels).  
  - Often behind **Nginx** as a reverse proxy on port 80/443.  
  - Migrate & collectstatic on the VPS.

### 13.2 Celery in Production

- **Celery** runs as a separate service or process:
  - `celery -A cycloweb_new worker -l info`.  
  - Typically managed by **systemd** or **supervisor** so it’s always running.
