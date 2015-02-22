#encoding: utf-8
from django.forms import ModelForm
from django import forms
from web.models import Usuario
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm

class editaUsuario(ModelForm):
	username = forms.CharField(max_length=20)

	class Meta:
		model = Usuario
		fields = ('username', 'first_name', 'last_name')
		exclude = ["password"]

	def __init__(self, *args, **kwargs):
		super(editaUsuario, self).__init__(*args, **kwargs)
		self.fields['username'].label = 'Nombre usuario'

class registrarUsuario(ModelForm):

	password1 = forms.CharField(label='Contraseña', widget=forms.PasswordInput)
	password2 = forms.CharField(label='Repetir contraseña', widget=forms.PasswordInput)
	PAIS = (
		('ESP', 'España'),
		('FRA', 'Francia'),
		('ITA', 'Italia'),
		('ING', 'Inglaterra'),
		('ALE', 'Alemania'),
	)
	pais = forms.ChoiceField(choices = PAIS)
	GENERO = (
		('HB', 'Hombre'),
		('MJ', 'Mujer'),
		('IN', 'Indefinido'),
	)
	genero = forms.ChoiceField(choices = GENERO)
	ESTADO = (
		('SO', 'Soltero/a'),
		('CA', 'Casado/a'),
		('DV', 'Divorciado/a'),
		('VI', 'Viudo/a'),
		('IN', 'En ingenieria'),
	)
	estado = forms.ChoiceField(choices = ESTADO, initial='IN')

	class Meta:
		model = Usuario
		fields = ('username', 'email', 'first_name', 'last_name')

	def clean_username(self):
		username = self.cleaned_data["username"]
		try:
			Usuario.objects.get(username = username)
		except User.DoesNotExist:
			return username
		raise forms.ValidationError("Ya existe ese nombre de usuario")

	def clean_password2(self):
		password1 = self.cleaned_data.get("password1", "")
		password2 = self.cleaned_data["password2"]
		if password1 != password2:
			raise forms.ValidationError("Las contrasenas no coinciden")
		return password2

	def save(self, commit=True):
		user = super(registrarUsuario, self).save(commit=False)
		user.set_password(self.cleaned_data["password1"])
		user.is_staff = False
		user.is_superuser = False
		if commit:
			user.save()
		return user

class cambiaAvatar(ModelForm):

	imagen = forms.ImageField(label='Imagen de perfil')

	class Meta:
		model = Usuario
		fields = ('imagen',)