FUENTE = ipe
LEXICO = lexico


CC = gcc -g
YFLAGS = -d -v
LFLAGS = -lm -lfl
OBJS= $(FUENTE).tab.o lex.yy.o  init.o math.o symbol.o  code.o auxiliar.o

$(FUENTE)3.exe: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $(FUENTE).exe

code.o:  code.c $(FUENTE).h
	$(CC) -c code.c

init.o:  init.c $(FUENTE).h $(FUENTE).tab.h
	$(CC) -c init.c

symbol.o:  symbol.c $(FUENTE).h $(FUENTE).tab.h
	$(CC) -c symbol.c

math.o:  math.c $(FUENTE).h
	$(CC) -c math.c

lex.yy.o:  lex.yy.c $(FUENTE).tab.h $(FUENTE).h
	$(CC) -c lex.yy.c

auxiliar.o:  auxiliar.c $(FUENTE).h
	$(CC) -c auxiliar.c

lex.yy.c: $(LEXICO).l $(FUENTE).tab.h $(FUENTE).h
	flex $(LEXICO).l

$(FUENTE).tab.o:  $(FUENTE).tab.c $(FUENTE).tab.h $(FUENTE).h
	$(CC) -c $(FUENTE).tab.c

$(FUENTE).tab.c $(FUENTE).tab.h:  $(FUENTE).y $(FUENTE).h
	bison $(YFLAGS) $(FUENTE).y

#Opcion para borrar los ficheros objetos y auxiliares
clean:
	rm -f  $(OBJS) $(FUENTE).tab.[ch] lex.yy.c $(FUENTE).exe $(FUENTE).output
