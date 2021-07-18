CFLAGS = -I/usr/include/X11R5 -I/usr/include/Motif1.2
LDFLAGS = -L/usr/lib/X11R5 -L/usr/lib/Motif1.2 -lXm -lXt -lX11

PROGRAMS = setlang simple switchmsg ximtext textwcsfunc textwcscb \
           titleres titlepg titlepg2 cvtctext \
           questiondialog workingdialog promptdialog menulabels

all : $(PROGRAMS) appdef msg cat


appdef : app-defaults/ja_JP.SJIS/Simple app-defaults/ja_JP.SJIS/Switchmsg
msg : msg/ja_JP.SJIS/switchmsg.msg
cat : msg/ja_JP.eucJP/switchmsg.cat msg/ja_JP.SJIS/switchmsg.cat \
      msg/C/switchmsg.cat

app-defaults/ja_JP.SJIS/Simple : app-defaults/ja_JP.eucJP/Simple
	iconv -f ujis -t sjis $? > $@
app-defaults/ja_JP.SJIS/Switchmsg : app-defaults/ja_JP.eucJP/Switchmsg
	iconv -f ujis -t sjis $? > $@

msg/ja_JP.SJIS/switchmsg.msg : msg/ja_JP.eucJP/switchmsg.msg
	iconv -f ujis -t sjis $? > $@

msg/ja_JP.eucJP/switchmsg.cat : msg/ja_JP.eucJP/switchmsg.msg
	LANG=japanese.euc gencat $@ $?
msg/ja_JP.SJIS/switchmsg.cat : msg/ja_JP.SJIS/switchmsg.msg
	LANG=japanese gencat $@ $?
msg/C/switchmsg.cat : msg/C/switchmsg.msg
	LANG=C gencat $@ $?

clean :
	-$ rm -f $(PROGRAMS)
