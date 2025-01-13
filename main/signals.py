"""
Signal handlers for the 'main' app, ensuring a Profile is created
whenever a new Django User is added.
"""

from django.db.models.signals import post_save
from django.dispatch import receiver
from django.contrib.auth.models import User
from .models import Profile

@receiver(post_save, sender=User)
def create_user_profile(sender, instance, created, **kwargs):
    """
    Signal receiver that fires after a User is saved.
    If the User is newly created, also create a matching Profile.
    """
    if created:
        Profile.objects.create(user=instance)

