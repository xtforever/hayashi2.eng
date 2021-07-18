CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11

PROGRAMS = reptype resconvert bitmapres bitmapcnv listres messageres

OBJS1 = bitmapres.o str2bitmap.o
OBJS2 = bitmapcnv.o str2bitmap.o
OBJS3 = messageres.o str2mlxms.o
OBJS4 = listres.o str2xmstable.o

all : $(PROGRAMS)

bitmapres : $(OBJS1)
	cc $(OBJS1) $(LDFLAGS) -o $@
bitmapcnv : $(OBJS2)
	cc $(OBJS2) $(LDFLAGS) -o $@
messageres : $(OBJS3)
	cc $(OBJS3) $(LDFLAGS) -o $@
listres : $(OBJS4)
	cc $(OBJS4) $(LDFLAGS) -o $@

clean :
	-$ rm -f $(PROGRAMS) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4)
