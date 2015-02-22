from django.conf.urls import patterns, include, url
from django.views.generic import TemplateView
from web import views

urlpatterns = patterns('',
	
    url(r'^verTablero$', views.verTablero, name='verTablero'),#hecho
    url(r'^entraTablero/(?P<tablero_id>\d+)/$', views.entraTablero, name='entraTablero'),#hecho
    url(r'^subirImagen/(?P<tablero_id>\d+)/$', views.subirImagen, name='subirImagen'),#hecho
    url(r'^subirImagen$', views.subeImagenSin, name='subeImagen'),
    url(r'^nuevoTablero$', views.creaTablero, name='nuevoTablero'),#hecho
    url(r'^editarTablero/(?P<tablero_id>\d+)/$', views.editarTablero, name='editarTablero'),#hecho
    url(r'^borraTablero/(?P<tablero_id>\d+)/$', views.borraTablero, name='borraTablero'),#hecho
    url(r'^meGusta/(?P<imagen_id>\d+)/$', views.meGusta, name='meGusta'), #hecho
    url(r'^imagenGusta/(?P<imagen_id>\d+)/$', views.meGusta2, name='meGusta'), #hecho
    url(r'^borrarImagen/(?P<imagen_id>\d+)/(?P<tablero_id>\d+)/$', views.borrarImagen, name='borrarImagen'),
    url(r'^search/$', views.search, name='search'),
    url(r'^verImagen/(?P<imagen_id>\d+)/$', views.verImagen, name='verImagen'),
)