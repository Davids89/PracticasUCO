from django import template
from web.models import Usuario

register = template.Library()

@register.filter(name='isFollowing')
def isFollowing(user, userId_Following):
	UsuarioAComprobar = Usuario.objects.get(pk=int(userId_Following))
	return UsuarioAComprobar.following.filter(id=user.id).count() > 0