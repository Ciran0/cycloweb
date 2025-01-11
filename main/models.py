# main/models.py
from django.db import models
from django.contrib.auth.models import User


class SiteSettings(models.Model):
    """
    Model to store site-wide text or configurations.
    We assume there's only one row for now.
    """
    homepage_title = models.CharField(max_length=200, default="Welcome to Cycloweb")
    homepage_subtitle = models.CharField(max_length=500, default="Quandyga's first web based simulation tool")
    footer_info = models.CharField(max_length=500, default="QuandygaEngeneering")

    def __str__(self):
        return "Site Settings"


class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    robmodels = models.ManyToManyField('RobModel', blank=True)

    def __str__(self):
        return f"Profile of {self.user.username}"


class RobModel(models.Model):
    name = models.CharField(max_length=100)
    description = models.TextField(blank=True)
    path = models.CharField(max_length=255, help_text="Path to the binary")
    parameters = models.TextField(
        blank=True,
        help_text="JSON string for parameters",
    )
    graph_config = models.TextField(blank=True, help_text="JSON string for graph config")

    def __str__(self):
        return self.name
