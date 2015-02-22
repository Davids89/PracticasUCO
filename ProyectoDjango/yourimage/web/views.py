#encoding: utf-8
from django.shortcuts import render
from web.models import Tablero, Usuario, Imagen
from django.contrib.auth.decorators import login_required
from web.forms import subeImagen, nuevoTablero, editaTablero, subeImagenSinTablero
from django.shortcuts import redirect
from django.http import Http404
from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger


#tableros

@login_required
def verTablero(request):
	usuario = Usuario.objects.get(pk=request.user.id)
	usuario2 = Usuario.objects.get(pk=usuario.id)
	tablero = Tablero.objects.filter(autor=usuario2)
	context = {'Tablero' : tablero, 'usuario' : usuario2, 'Usuario' : usuario}
	return render(request, 'tablero/verTableros.html', context)

@login_required
def entraTablero(request, tablero_id):
	try:
		usuario = Usuario.objects.get(pk=request.user.id)
		tablero = Tablero.objects.get(pk=tablero_id)
		imagenes = Imagen.objects.filter(imagenTablero=tablero)

		paginator = Paginator(imagenes, 21)

		page = request.GET.get('page')

		try:
			imagenes = paginator.page(page)
		except PageNotAnInteger:
			imagenes = paginator.page(1)
		except EmptyPage:
			imagenes = paginator.page(paginator.num_pages)

		context = {'Tablero' : tablero , 'Imagenes' : imagenes , 'Usuario' : usuario}
	except Tablero.DoesNotExist:
		raise Http404
	return render(request, 'tablero/entraTablero.html', context)

@login_required
def creaTablero(request):
	usuario = Usuario.objects.get(pk=request.user.id)
	if request.method == 'POST':
		formulario = nuevoTablero(request.POST, request.FILES)
		if formulario.is_valid():
			usuario2 = Usuario.objects.get(pk=usuario.id)
			form = formulario.save(commit = False)
			form.autor = usuario2
			usuario.tieneTablero = form.nombre
			form.save()
			formulario.save()
			return redirect('/web/verTablero')
	else:
		formulario = nuevoTablero()
	context = {'formulario' : formulario, 'Usuario' : usuario}
	return render(request, 'tablero/nuevoTablero.html', context)

@login_required
def editarTablero(request, tablero_id):
	tablero = Tablero.objects.get(pk=tablero_id)
	usuario = Usuario.objects.get(pk=request.user.id)
	if request.method == 'POST':
		formulario = editaTablero(request.POST, request.FILES, instance=tablero)
		if formulario.is_valid():
			formulario.save()
			return redirect('/web/verTablero')
	else:
		formulario = editaTablero(instance=tablero)
	context = {'tablero' : formulario , 'Usuario' : usuario}
	return render(request, 'tablero/editaTablero.html', context)

@login_required
def borraTablero(request, tablero_id):
	tablero = Tablero.objects.get(pk=tablero_id)
	tablero.delete()
	return redirect('/web/verTablero')

#imagenes

@login_required
def subirImagen(request, tablero_id):
	usuario = Usuario.objects.get(pk=request.user.id)
	if request.method == 'POST':
		imagen = subeImagen(request.POST, request.FILES)
		tablero = Tablero.objects.get(pk=tablero_id)
		if imagen.is_valid():
			id_imagen = imagen.save()
			tablero.imagen.add(id_imagen)
			tablero.save()
    		return redirect('/web/entraTablero/'+tablero_id)
	else:
		formulario = subeImagen()
	context = {'formulario' : formulario, 'Usuario' : usuario}
	return render(request, 'tablero/subirImagen.html', context)

@login_required
def subeImagenSin(request):
	usuario = Usuario.objects.get(pk=request.user.id)
	tableros = Tablero.objects.filter(autor=usuario.id)
	if request.method == 'POST':
		imagen = subeImagenSinTablero(request.user, request.POST, request.FILES, instance=usuario)
		print "hola"
		if imagen.is_valid():
			print "hola2"
			id_imagen = imagen.save(commit=False)
			for tablero in imagen.cleaned_data['tablero']:
				print tablero
				tablero.imagen.add(id_imagen)
				tablero.save()
			return redirect('/main')
	else:
		imagen = subeImagenSinTablero(request.user, instance=usuario)
	context = {'Imagen' : imagen, 'Tableros' : tableros, 'Usuario' : usuario}
	return render(request, 'tablero/subirImagenSin.html', context)

@login_required
def meGusta(request, imagen_id):
	usuario = Usuario.objects.get(pk=request.user.id)
	imagen = Imagen.objects.get(pk=imagen_id)
	imagen.like.add(usuario)
	imagen.save()
	return redirect('/main')

@login_required
def meGusta2(request, imagen_id):
	usuario = Usuario.objects.get(pk=request.user.id)
	imagen = Imagen.objects.get(pk=imagen_id)
	imagen.like.add(usuario)
	imagen.save()
	return redirect('/web/verImagen/'+imagen_id)

@login_required
def borrarImagen(request, imagen_id, tablero_id):
	imagen = Imagen.objects.get(pk=imagen_id)
	tablero = Tablero.objects.get(pk=tablero_id)
	tablero.imagen.remove(imagen)
	imagen.delete()
	return redirect('/web/entraTablero/'+tablero_id)

@login_required
def search(request):
	usuario = Usuario.objects.get(pk=request.user.id)
	busqueda = request.POST['busqueda']
	usuarios = Usuario.objects.filter(username__icontains=busqueda)
	imagenes = Imagen.objects.filter(nombre__icontains=busqueda)
	tableros = Tablero.objects.filter(nombre__icontains=busqueda)
	context = {'Usuarios' : usuarios, 'Imagenes' : imagenes, 'Tableros' : tableros, 'Busqueda' : busqueda, 'Usuario' : usuario}
	return render(request, 'tablero/busqueda.html', context)

@login_required
def verImagen(request, imagen_id):
	usuario = Usuario.objects.get(pk=request.user.id)
	imagen = Imagen.objects.get(pk=imagen_id)
	for tablero in imagen.imagenTablero.all():
		tableroImagen = tablero
	propietario = tableroImagen.autor
	context = {'Imagen' : imagen, 'Usuario' : usuario,'Propietario' : propietario}
	return render(request, 'tablero/verImagen.html', context)