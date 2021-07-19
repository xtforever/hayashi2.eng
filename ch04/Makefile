CFLAGS = 
LDFLAGS =  -lXm -lXt -lX11

PROGRAMS = reptype resconvert bitmapres bitmapcnv listres messageres

OBJS1 = bitmapres.o str2bitmap.o
OBJS2 = bitmapcnv.o str2bitmap.o
OBJS3 = messageres.o str2mlxms.o
OBJS4 = listres.o str2xmstable.o

all : $(PROGRAMS)

bitmapres : $(OBJS1)

bitmapcnv : $(OBJS2)

messageres : $(OBJS3)

listres : $(OBJS4)


clean :
	-$ rm -f $(PROGRAMS) $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4)
