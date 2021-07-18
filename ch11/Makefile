CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11

PROGRAMS = textcopy clipboard clipctext cliplength clipincr \
           clipinquire clip2type clipnewform clipstruct clipbyname

all : $(PROGRAMS)

clean :
	-$ rm -f $(PROGRAMS)
