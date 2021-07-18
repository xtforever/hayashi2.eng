CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11

PROGRAMS = flsingle flmulti flcreate flinfo

OBJS1 = csmultiline.o strlist2xms.o

all : $(PROGRAMS)

csmultiline : $(OBJS1)
	cc $(OBJS1) $(LDFLAGS) -o $@

clean :
	-$ rm -f $(PROGRAMS)
