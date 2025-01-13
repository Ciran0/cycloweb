# main/admin.py

from django.contrib import admin
from .models import SiteSettings, RobModel, Profile

@admin.register(SiteSettings)
class SiteSettingsAdmin(admin.ModelAdmin):
    """
    Admin configuration for SiteSettings.
    Allows viewing and editing site-wide text fields,
    such as homepage title, subtitle, and footer info.
    """
    list_display = ("homepage_title", "homepage_subtitle", "footer_info")

@admin.register(RobModel)
class RobModelAdmin(admin.ModelAdmin):
    """
    Admin configuration for RobModel objects.
    Displays key fields (name, path, description, parameters, graph_config),
    and provides search functionality for name, description, and parameters.
    """
    list_display = ('name', 'path', 'description', 'parameters', 'graph_config')
    search_fields = ('name', 'description', 'parameters')

@admin.register(Profile)
class ProfileAdmin(admin.ModelAdmin):
    """
    Admin configuration for user Profile objects.
    Allows assigning RobModels to a user via a many-to-many field,
    making it easier to manage which models each user can access.
    """
    list_display = ('user', )
    filter_horizontal = ('robmodels',)  # Provides a better UI for many-to-many fields

