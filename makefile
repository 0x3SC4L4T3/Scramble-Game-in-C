CC=gcc
CFLAGS= -I.
DEPS =
OBJ = Scramble.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Scramble: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
     
