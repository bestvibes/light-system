.PHONY: all clean light_system

CC=gcc
CFLAGS=

ODIR=obj
_OBJ=leds.o spi.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

all: light_system

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

light_system: $(OBJ)
	gcc -o $@.out $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o
