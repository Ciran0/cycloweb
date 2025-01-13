"""
Custom context processors for the 'main' app.

These functions add extra variables to the template context
so they're accessible site-wide.
"""

def accessible_robmodels(request):
    """
    Provides 'robmodels' to templates if the user is authenticated.
    This enables the navbar to display only those RobModels the user can access.
    """
    if request.user.is_authenticated:
        return {
            'robmodels': request.user.profile.robmodels.all()
        }
    return {'robmodels': None}

