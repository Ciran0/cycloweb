"""
Project-level URL configuration for cycloweb_new.

This routes requests for:
  - /admin/ -> the Django admin
  - /       -> the 'main' app URLs
"""
from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),       # Django admin
    path('', include('main.urls')),        # Routes from the 'main' app
]

