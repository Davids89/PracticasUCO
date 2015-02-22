from django import template
from web.models import Usuario, Tablero

register = template.Library()

@register.filter(name='isFollowing')
def isFollowing(user, tableroId_Following):
	TableroAComprobar = Tablero.objects.get(pk=int(tableroId_Following))
	return TableroAComprobar.following.filter(id=user.id).count() > 0