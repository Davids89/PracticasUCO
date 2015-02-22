from django.contrib import admin
from web.models import Usuario, Tablero, Imagen, Comentario, Reporte, Notificacion

admin.site.register(Usuario)
admin.site.register(Tablero)
admin.site.register(Imagen)
admin.site.register(Comentario)
admin.site.register(Reporte)
admin.site.register(Notificacion)
