# myproject/celery.py
import os
from celery import Celery

# Set default Django settings module
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'cycloweb_new.settings')

app = Celery('cycloweb_new')
app.config_from_object('django.conf:settings', namespace='CELERY')
app.autodiscover_tasks()

