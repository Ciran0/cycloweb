"""
Celery application setup for the cycloweb_new project.

Configures Celery to use Django settings, auto-discovers tasks,
and sets default DJANGO_SETTINGS_MODULE if not already set.
"""

import os
from celery import Celery

# Ensure Django settings are set before initializing Celery
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'cycloweb_new.settings')

app = Celery('cycloweb_new')
app.config_from_object('django.conf:settings', namespace='CELERY')
app.autodiscover_tasks()

