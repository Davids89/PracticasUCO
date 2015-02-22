#encoding: utf-8
from django.shortcuts import render
from web.models import Imagen, Tablero, Usuario
from django.contrib.auth.decorators import login_required
from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger

@login_required
def portada(request):
	imagenes = Imagen.objects.all()
	paginator = Paginator(imagenes, 21)

	page = request.GET.get('page')

	try:
		imagenes = paginator.page(page)
	except PageNotAnInteger:
		imagenes = paginator.page(1)
	except EmptyPage:
		imagenes = paginator.page(paginator.num_pages)

	tableros = Tablero.objects.all()
	usuario = Usuario.objects.get(pk=request.user.id)
	context = {'Imagenes' : imagenes, 'Tableros' : tableros, 'Usuario' : usuario}
	return render(request, 'main/main.html', context)