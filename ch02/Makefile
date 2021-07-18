CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11

PROGRAMS = colorname colorval changecolor rubberband \
           bgpixmap labelpixmap tilepixmap iconpixmap \
           pixmapdraw putimage getimage \
           animtiming animpixmap animplane \
           cliprect addregion inregion

all : $(PROGRAMS)

clean :
	-$ rm -f $(PROGRAMS)
