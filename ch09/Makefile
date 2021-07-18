CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11

PROGRAMS = atom setprop getprop propnotify proprecv cutbuffer \
           recvxevent sendxevent recvclmsg sendclmsg \
           outputdata inputdata multidisp

all : $(PROGRAMS)

clean :
	-$ rm -f $(PROGRAMS)
