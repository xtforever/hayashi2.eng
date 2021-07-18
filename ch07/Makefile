CFLAGS = -g 
LDFLAGS = -lXm -lXt -lX11

PROGRAMS = cslabel cslist csmultiline csmultifont cssegments \
           csgettext csgettext2 csgetseg cssize \
           csdraw csdrawimage csdrawuline

OBJS1 = csmultiline.o strlist2xms.o

all : $(PROGRAMS)

csmultiline : $(OBJS1)
	cc $(OBJS1) $(LDFLAGS) -o $@

clean :
	-$ rm -f $(PROGRAMS) $(OBJS1)
