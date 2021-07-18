CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11

PROGRAMS = cslabel cslist csmultiline csmultifont cssegments \
           csgettext csgettext2 csgetseg cssize \
           csdraw csdrawimage csdrawuline

OBJS1 = csmultiline.o strlist2xms.o

all : $(PROGRAMS)

csmultiline : $(OBJS1)
	cc $(OBJS1) $(LDFLAGS) -o $@

clean :
	-$ rm -f $(PROGRAMS) $(OBJS1)
