# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = mundo.o libconjunto.o liblef.o libfila.o libmundo.o libevento.o
     
mundo: $(objects)
	$(CC) -o mundo $(objects) $(LDFLAGS)

libconjunto.o: libs/libconjunto.c
	$(CC) -c $(CFLAGS) libs/libconjunto.c

liblef.o: libs/liblef.c
	$(CC) -c $(CFLAGS) libs/liblef.c

libfila.o: libs/libfila.c
	$(CC) -c $(CFLAGS) libs/libfila.c

libmundo.o: libs/libmundo.c
	$(CC) -c $(CFLAGS) libs/libmundo.c

libevento.o: libs/libevento.c
	$(CC) -c $(CFLAGS) libs/libevento.c

mundo.o: mundo.c
	$(CC) -c $(CFLAGS) mundo.c

clean:
	rm -f $(objects) mundo


