#encoding: utf-8
from django.db import models
from django.contrib.auth.models import User

class Usuario(User):
	imagen = models.ImageField(upload_to='media', blank=True, null=True, default='avatar/avatar.jpg')
	PAIS = (
		('ESP', 'Espana'),
		('FRA', 'Francia'),
		('ITA', 'Italia'),
		('ING', 'Inglaterra'),
		('ALE', 'Alemania'),
	)
	pais = models.CharField(max_length=3, choices = PAIS, null = True, blank = True)
	GENERO = (
		('HB', 'Hombre'),
		('MJ', 'Mujer'),
		('IN', 'Indefinido'),
	)
	genero = models.CharField(max_length=2, choices = GENERO, null = True, blank = True)
	ESTADO = (
		('CA', 'Casado/a'),
		('SO', 'Soltero/a'),
		('DV', 'Divorciado/a'),
		('VI', 'Viudo/a'),
		('IN', 'En ingenieria'),
	)
	estado = models.CharField(max_length=2, choices = ESTADO, null = True, blank = True)
	following = models.ManyToManyField('web.Usuario', symmetrical = False, null = True, blank = True, related_name="followers")
	followingt = models.ManyToManyField('web.Tablero', symmetrical = False, null = True, blank = True, related_name="followerst")
	def __unicode__(self):
		return self.username

class Tablero(models.Model):
	nombre = models.CharField(max_length=20)
	autor = models.ForeignKey(Usuario, related_name='autorTablero')
	seguidores = models.ManyToManyField(Usuario, blank=True, null = True, related_name='seguidoresTablero')
	CATEGORIA = (
		('Alimentacion y bebidas', 'Alimentacion y bebidas'),
		('Animales', 'Animales'),
		('Arquitectura', 'Arquitectura'),
		('Arte', 'Arte'),
		('Bodas', 'Bodas'),
		('Bricolaje', 'Bricolaje'),
		('Carteles e ilustraciones', 'Carteles e ilustraciones'),
		('Ciencia y naturaleza', 'Ciencia y naturaleza'),
		('Citas celebres', 'Citas celebres'),
		('Coches y motocicletas', 'Coches y motocicletas'),
		('Decoracion del hogar', 'Decoracion del hogar'),
		('Deportes', 'Deportes'),
		('Diseño', 'Diseño'),
		('Educacion', 'Educacion'),
		('Famosos', 'Famosos'),
		('Festivos', 'Festivos'),
		('Fotografia', 'Fotografia'),
		('Historia', 'Historia'),
		('Humor', 'Humor'),
		('Jardineria', 'Jardineria'),
		('Libros, musica y cine', 'Libros, musica y cine'),
		('Moda para hombres', 'Moda para hombres'),
		('Moda para mujeres', 'Moda para mujeres'),
		('Niños', 'Niños'),
		('Peluqueria y belleza', 'Peluqueria y belleza'),
		('Productos', 'Productos'),
		('Salud y deporte', 'Salud y deporte'),
		('Tatuajes', 'Tatuajes'),
		('Tecnologia', 'Tecnologia'),
		('Viajes', 'Viajes'),
	)
	categoria = models.CharField(choices = CATEGORIA, max_length=100)
	imagen = models.ManyToManyField('web.Imagen', blank=True, null=True, related_name = 'imagenTablero')
	like = models.ManyToManyField(Usuario, blank = True, null = True, related_name = 'usuariosGustaTablero')

	def __unicode__(self):
		return self.nombre

class Imagen(models.Model):
	nombre = models.CharField(max_length=20)
	CATEGORIA = (
		('Alimentacion y bebidas', 'Alimentacion y bebidas'),
		('Animales', 'Animales'),
		('Arquitectura', 'Arquitectura'),
		('Arte', 'Arte'),
		('Bodas', 'Bodas'),
		('Bricolaje', 'Bricolaje'),
		('Carteles e ilustraciones', 'Carteles e ilustraciones'),
		('Ciencia y naturaleza', 'Ciencia y naturaleza'),
		('Citas celebres', 'Citas celebres'),
		('Coches y motocicletas', 'Coches y motocicletas'),
		('Decoracion del hogar', 'Decoracion del hogar'),
		('Deportes', 'Deportes'),
		('Diseño', 'Diseño'),
		('Educacion', 'Educacion'),
		('Famosos', 'Famosos'),
		('Festivos', 'Festivos'),
		('Fotografia', 'Fotografia'),
		('Historia', 'Historia'),
		('Humor', 'Humor'),
		('Jardineria', 'Jardineria'),
		('Libros, musica y cine', 'Libros, musica y cine'),
		('Moda para hombres', 'Moda para hombres'),
		('Moda para mujeres', 'Moda para mujeres'),
		('Niños', 'Niños'),
		('Peluqueria y belleza', 'Peluqueria y belleza'),
		('Productos', 'Productos'),
		('Salud y deporte', 'Salud y deporte'),
		('Tatuajes', 'Tatuajes'),
		('Tecnologia', 'Tecnologia'),
		('Viajes', 'Viajes'),
	)
	categoria = models.CharField(choices = CATEGORIA, max_length=100)
	imagen = models.ImageField(upload_to='media')
	like = models.ManyToManyField(Usuario, blank = True, null = True, related_name = 'usuariosGusta')

	def __unicode__(self):
		return self.nombre

class Comentario(models.Model):
	texto = models.CharField(max_length=200)
	imagen = models.ForeignKey(Imagen, related_name='comentarioImagen', blank=True, null = True)
	reportado = models.BooleanField(default=False)
	puntuacion = models.IntegerField(default=0, blank=True)

	def __unicode__(self):
		return self.texto

class Reporte(models.Model):
	observacion = models.CharField(max_length=500, blank=True, null=True)
	imagen = models.ForeignKey(Imagen, related_name='reporteImagen', blank=True, null=True)
	usuario = models.ForeignKey(Usuario, related_name='reporteUsuario', blank=True, null=True)

	def __unicode__(self):
		return self.observacion

class Notificacion(models.Model):
	usuario = models.ForeignKey(Usuario, related_name='usuarioNotificacion', blank=True, null=True)
	texto = models.CharField(max_length=200)
	visto = models.BooleanField(default=False)
	fecha = models.DateTimeField(auto_now_add=True)

# Create your models here.
