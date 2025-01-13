"""
Defines the 'main' app configuration, responsible for
registering signals upon readiness.
"""

from django.apps import AppConfig

class MainConfig(AppConfig):
    """
    AppConfig for the 'main' app. Imports signals on app readiness
    so Profile objects are created whenever a new User is added.
    """
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'main'

    def ready(self):
        # Import signals here to ensure they're registered
        import main.signals

