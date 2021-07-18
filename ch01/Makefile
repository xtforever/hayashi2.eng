#CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
#LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11
CFLAGS = -g  -I/usr/include/X11R5 -I/usr/include/Xm
LDFLAGS = -L/usr/lib/X11R5 -lXm -lXt -lX11


PROGRAMS = xlibdraw xmdraw drawpoint drawps_org drawps_pre \
           drawline drawsegments drawls_org drawls_pre \
           capstyle joinstyle linestyle setdashes \
           drawrect drawrects fillrect fillrects \
           drawarc drawarcs fillarc fillarcs \
           fillpg_conv fillpg_nonc fillpg_comp fillpg_wind \
           filltile fillopaque fillstipple drawstring drawxmstr  \
           cbdraw evhdraw actdraw expose

all : $(PROGRAMS)

clean :
	-$ rm -f $(PROGRAMS)
