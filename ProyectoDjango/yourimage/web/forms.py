from django.forms import ModelForm
from django import forms
from web.models import Tablero, Imagen

#la funcion _init_ es para hacer cambios en el modelo como por ejemplo cambiar las etiquetas (nombre de los campos)

class subeImagen(ModelForm):

	imagen = forms.ImageField(label='Imagen')

	class Meta:
		model = Imagen
		fields = ('imagen', 'nombre', 'categoria')

class subeImagenSinTablero(ModelForm):

	imagen = forms.ImageField(label='Imagen')
	#imagenTablero__nombre = forms.ModelChoiceField(queryset=Tablero.objects.filter(autor=user))


	class Meta:
		model = Imagen
		#fields = ('imagen', 'imagenTablero__nombre')

	def __init__(self, user, *args, **kwargs):
		self.user = user
		super(subeImagenSinTablero, self).__init__(*args, **kwargs)
class nuevoTablero(ModelForm):

	#imagen = forms.ImageField(label='Imagen')

	class Meta:
		model = Tablero
		fields = ('nombre', 'categoria')
		exclude = ["autor"]

class editaTablero(ModelForm):

	nombre = forms.CharField(max_length=20)

	class Meta:
		model = Tablero
		fields = ('nombre', 'categoria')