CC = tcc
CFLAGS = -std=c99 -pedantic -D _POSIX_C_SOURCE=200809L
DEPS =
OBJ = main.o
LIBS =
XNAME = $(notdir $(CURDIR))

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(XNAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(XNAME) $^ $(LIBS)

clean:
	-rm $(OBJ)
	-rm $(XNAME)

