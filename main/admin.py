# main/admin.py
from django.contrib import admin
from .models import SiteSettings, RobModel, Profile


@admin.register(SiteSettings)
class SiteSettingsAdmin(admin.ModelAdmin):
    list_display = ("homepage_title", "homepage_subtitle", "footer_info")


@admin.register(RobModel)
class RobModelAdmin(admin.ModelAdmin):
    list_display = ('name', 'path', 'description', 'parameters','graph_config')
    search_fields = ('name', 'description', 'parameters')


@admin.register(Profile)
class ProfileAdmin(admin.ModelAdmin):
    list_display = ('user', )
    # This will let you choose which RobModels this user can access
    filter_horizontal = ('robmodels',)  # optional: better UI for many-to-many
