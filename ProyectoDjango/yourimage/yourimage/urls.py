#encoding: utf-8
from django.conf.urls import patterns, include, url
from django.views.generic import TemplateView
from django.conf import settings
from django.conf.urls.static import static
from yourimage import views

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),

    #Users
    url(r'^users/', include('users.urls')),

    #Cosas de la web
    url(r'^web/', include('web.urls')),
    
    url(r'^main$', include('main.urls')), #portada principal

    #Principal
    url(r'^$', TemplateView.as_view(template_name='index.html'), name='index'),
) + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT) + static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)