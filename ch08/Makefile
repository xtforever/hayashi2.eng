CFLAGS = 
LDFLAGS = -lXm -lXt -lX11

PROGRAMS = flsingle flmulti flcreate flinfo

OBJS1 = csmultiline.o strlist2xms.o

all : $(PROGRAMS)

csmultiline : $(OBJS1)
	cc $(OBJS1) $(LDFLAGS) -o $@

clean :
	-$ rm -f $(PROGRAMS)
