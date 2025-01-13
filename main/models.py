"""
Defines data models for the 'main' app, including site-wide settings,
user profiles, and simulation models (RobModel).
"""

from django.db import models
from django.contrib.auth.models import User

class SiteSettings(models.Model):
    """
    Stores global site text/configuration, such as:
      - homepage_title
      - homepage_subtitle
      - footer_info
    Typically, only one record is used.
    """
    homepage_title = models.CharField(max_length=200, default="Welcome to Cycloweb")
    homepage_subtitle = models.CharField(
        max_length=500,
        default="Quandyga's first web based simulation tool"
    )
    footer_info = models.CharField(max_length=500, default="QuandygaEngeneering")

    def __str__(self):
        return "Site Settings"

class Profile(models.Model):
    """
    Each user has one Profile, which determines which RobModels
    they can access via a many-to-many field.
    """
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    robmodels = models.ManyToManyField('RobModel', blank=True)

    def __str__(self):
        return f"Profile of {self.user.username}"

class RobModel(models.Model):
    """
    Represents a simulation model or physical system, associating:
      - A binary path (path to the executable)
      - JSON-based parameters for dynamic forms
      - JSON-based graph_config for Plotly charts
    """
    name = models.CharField(max_length=100)
    description = models.TextField(blank=True)
    path = models.CharField(max_length=255, help_text="Path to the binary")
    parameters = models.TextField(
        blank=True,
        help_text="JSON string for parameters"
    )
    graph_config = models.TextField(blank=True, help_text="JSON string for graph config")

    def __str__(self):
        return self.name

