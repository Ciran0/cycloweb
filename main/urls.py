"""
URL configuration for the 'main' app.

Defines routes that map URLs to views within this Django application.
"""
from django.urls import path
from . import views

urlpatterns = [
    # Home and auth endpoints
    path('', views.index, name='index'),
    path('login/', views.login_view, name='login'),
    path('logout/', views.logout_view, name='logout'),

    # RobModel dashboard & simulation endpoints
    path('dashboard/<int:model_id>/', views.model_dashboard, name='model_dashboard'),
    path('simulate-run/<int:model_id>/', views.simulate_run, name='simulate_run'),
    path('check-task-status/<str:task_id>/', views.check_task_status, name='check_task_status'),

    # Viewing and deleting runs
    path('show-conf/<int:model_id>/<str:run_name>/', views.show_conf, name='show_conf'),
    path('show-log/<int:model_id>/<str:run_name>/', views.show_log, name='show_log'),
    path('confirm-delete-run/<int:model_id>/<str:run_name>/', views.confirm_delete_run, name='confirm_delete_run'),
    path('delete-run/<int:model_id>/<str:run_name>/', views.delete_run, name='delete_run'),
    path('refresh-runs/<int:model_id>/', views.refresh_runs, name='refresh_runs'),

    # Graphing endpoint
    path('draw-graph/<int:model_id>/', views.draw_graph, name='draw_graph'),
]

