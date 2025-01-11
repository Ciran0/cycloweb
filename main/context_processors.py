# main/context_processors.py
def accessible_robmodels(request):
    if request.user.is_authenticated:
        return {
            'robmodels': request.user.profile.robmodels.all()
        }
    return {'robmodels': None}

