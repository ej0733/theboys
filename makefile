# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = mundo.o libconjunto.o liblef.o libfila.o libmundo.o libevento.o
     
mundo: $(objects)
	$(CC) -o mundo $(objects) $(LDFLAGS)

libconjunto.o: libconjunto.c
	$(CC) -c $(CFLAGS) libconjunto.c

liblef.o: liblef.c
	$(CC) -c $(CFLAGS) liblef.c

libfila.o: libfila.c
	$(CC) -c $(CFLAGS) libfila.c

libmundo.o: libmundo.c
	$(CC) -c $(CFLAGS) libmundo.c

libevento.o: libevento.c
	$(CC) -c $(CFLAGS) libevento.c

mundo.o: mundo.c
	$(CC) -c $(CFLAGS) mundo.c

clean:
	rm -f $(objects) mundo


