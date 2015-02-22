from django.conf.urls import patterns, include, url
from django.views.generic import TemplateView
from users import views

urlpatterns = patterns('',

	url(r'^login$', views.userLogin, name="UserLogin"),
	url(r'^logout$', views.userLogout, name='UserLogout'),
	url(r'^editarUsuario/(?P<usuario_id>\d+)/$', views.editarUsuario, name='editarUsuario'),
	url(r'^registro$', views.registroUsuario, name='registroUsuario'),
	url(r'^verUsuario/(?P<usuario_id>\d+)/$', views.verUsuario, name='verUsuario'),
	url(r'^seguirUsuario/(?P<usuario_id>\d+)/$', views.seguirUsuario, name='seguirUsuario'),
	url(r'^dejarSeguirUsuario/(?P<usuario_id>\d+)/$', views.dejarSeguirUsuario, name='dejarSeguirUsuario'),
	url(r'^cambiaAvatar/(?P<usuario_id>\d+)/$', views.cambiarAvatar, name='cambiarAvatar'),
)