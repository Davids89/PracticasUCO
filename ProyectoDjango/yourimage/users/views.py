from django.shortcuts import render, redirect
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth import login, authenticate, logout
from django.contrib.auth.decorators import login_required
#from django.views.generic.base import View
from web.models import Usuario, Tablero
from users.forms import editaUsuario, registrarUsuario, cambiaAvatar

def userLogin(request):
	if request.method == 'POST':
		form = AuthenticationForm(request.POST)
		if form.is_valid:
			user = request.POST['username']
			passwd = request.POST['password']
			access = authenticate(username=user, password=passwd)
			if access is not None:
				if access.is_active:
					login(request, access)
					return redirect('/main')
				else:
					return render(request, 'users/inactive.html')
			else:
				return render(request, 'users/nouser.html')
	else:
		form = AuthenticationForm()
	context = {'form': form}
	return render(request,'users/login.html', context)

@login_required(login_url='users/login')
def userLogout(request):
	logout(request)
	return redirect('/')

@login_required
def cambiarAvatar(request, usuario_id):
	usuario = Usuario.objects.get(pk=usuario_id)
	if request.method == 'POST':
		formulario = cambiaAvatar(request.POST, request.FILES, instance=usuario)
		if formulario.is_valid():
			formulario.save()
			return redirect('/users/verUsuario/'+usuario_id)
	else:
		formulario = cambiaAvatar(instance=usuario)
	context = {'formulario' : formulario , 'Usuario' : usuario}
	return render(request, 'users/cambiaAvatar.html', context)

@login_required
def editarUsuario(request, usuario_id):
	usuario = Usuario.objects.get(pk=usuario_id)
	if request.method == 'POST':
		formulario = editaUsuario(request.POST, request.FILES, instance=usuario)
		if formulario.is_valid():
			formulario.save()
			return redirect('/web/verTablero')
	else:
		formulario = editaUsuario(instance=usuario)
	context = { 'formulario' : formulario, 'Usuario' : usuario}
	return render(request, 'users/editarUsuario.html', context)

def registroUsuario(request):
    if request.method == 'POST':
    	form = registrarUsuario(request.POST)
    	if form.is_valid():
    		form.save()
    		return redirect('/')
    else:
    	form = registrarUsuario(request.POST)
    context = {'formulario':form}
    return render(request, 'users/registro.html',  context)

@login_required
def verUsuario(request, usuario_id):
	usuario = Usuario.objects.get(pk=request.user.id)
	usuario1 = Usuario.objects.get(pk=usuario_id)
	print "usuario:",usuario
	print "usuario1:",usuario1
	tableros = Tablero.objects.filter(autor=usuario1)
	context = {'Usuario1' : usuario1, 'Tableros' : tableros, 'Usuario' : usuario}
	return render(request, 'users/verUsuario.html', context)

@login_required
def seguirUsuario(request, usuario_id):
	voy_a_seguir = Usuario.objects.get(pk=usuario_id)
	yo = Usuario.objects.get(pk=request.user.id)
	yo.following.add(voy_a_seguir)
	yo.save()
	return redirect('/users/verUsuario/'+usuario_id)

@login_required
def dejarSeguirUsuario(request, usuario_id):
	dejo_seguir = Usuario.objects.get(pk=usuario_id)
	yo = Usuario.objects.get(pk=request.user.id)
	yo.following.remove(dejo_seguir)
	yo.save()
	return redirect('/users/verUsuario/'+usuario_id)