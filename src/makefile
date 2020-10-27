IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

_DEPS = pi.h util.h file.h extract.h getmask.h maskfat.h getqual.h qualgrad.h qualpseu.h qualvar.h dxdygrad.h grad.h rbtrees.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o util.o extract.o getmask.o maskfat.o getqual.o qualgrad.o qualpseu.o qualvar.o dxdygrad.o grad.o rbtrees.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
