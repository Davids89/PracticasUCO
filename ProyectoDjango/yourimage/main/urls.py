#encoding: utf-8
from django.conf.urls import patterns, include, url
from django.views.generic import TemplateView
from django.conf import settings
from django.conf.urls.static import static
from main import views

urlpatterns = patterns('',

	url(r'^$', views.portada, name='muestraPortada'),

)