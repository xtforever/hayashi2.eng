/***********************************************************************
 *      This example program is an excerpt from the book:              *
 *                                                                     *
 *          X Window OSF/Motif Toolkit Programming (II)                *
 *            - Graphics, Custom Resource, Japanese, Communications -  *
 *                                                                     *
 *      Copyright 1995, Hideyuki Hayashi                               *
 *                                                                     *
 *      This program can be distributed without fee, provided          *
 *      that the above copyright notice appear in all copies.          *
 ***********************************************************************/
#include <stdlib.h>
#include <Xm/XmAll.h>

#define XOFFSET 20
#define YOFFSET 60
static GC gc;
static XmFontList fontlist[3];
static XmString string[100];
static String fallback_resources[] = {
  "*canvas.topAttachment:       ATTACH_FORM",
  "*canvas.bottomAttachment:    ATTACH_FORM",
  "*canvas.leftAttachment:      ATTACH_FORM",
  "*canvas.rightAttachment:     ATTACH_FORM",
  "*canvas.width:               200",
  "*canvas.height:              200",
  NULL
};

/* (1) Drawn text.  Newline at NULL.  Two NULL at end */
static char *text[] = { "When", "drawing", "multi-line", NULL,
                        "text,", "care for", "line and", NULL,
                        "character", "spacing !", NULL, NULL };

/* (2) Use different for each line */
static char *fontname[] = { "*-medium-r-normal--16-*",
                            "*-medium-r-normal--14-*",
                            "*-medium-r-normal--24-*" };

/* (6) Redraw text when the window is exposed */
static void 
ExposeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    int tx, ty;  /* Starting position of each text */
    int i, line = 0;
    Dimension height = 0;
    Boolean newline = True;

    ty = YOFFSET;

    /* (7) Repeat for each text segment */
    for (i = 0; ; i++) {
        if (string[i] == NULL) { /* (8) end of line */
            i++;
            if (string[i] == NULL) /* (9) end of text */
                break;
            line++;
            newline = True;
        }
        if (newline) {
            /* (10) Move to next line by adding the height of previous line */
            ty += height;
            /* (11) Remember the height of next line */
            height = XmStringHeight(fontlist[line], string[i]);
            /* (12) Return caridge to the left of the line */
            tx = XOFFSET;
            newline = False;
        }
        /* (13) Draw one text segment */
        XmStringDraw(XtDisplay(w), XtWindow(w), fontlist[line], string[i], gc,
                     tx, ty, XmStringWidth(fontlist[line], string[i]),
                     XmALIGNMENT_BEGINNING, XmSTRING_DIRECTION_L_TO_R, NULL);
        /* (14) Move to next character position */
        tx += XmStringWidth(fontlist[line], string[i]);
    }
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas;
    Arg    al[20];
    int    ac;
    int    i;
    XmFontListEntry entry;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "XMdraw", NULL, 0,
                               &argc, argv, fallback_resources, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    /* (3) Load font list */
    for (i = 0; i < 3; i++) {
        if ((entry = XmFontListEntryLoad(XtDisplay(canvas), fontname[i],
                         XmFONT_IS_FONTSET, XmFONTLIST_DEFAULT_TAG)) == NULL) {
            fprintf(stderr, "Cannot load fontlist entry: %s\n", fontname[i]);
	    exit(1);
        }
        fontlist[i] = XmFontListAppendEntry(NULL, entry);
        XmFontListEntryFree(&entry);
    }

    /* (4) Create compound string */
    for (i = 0; ; i++) {
        if (text[i] == NULL) { /* end of line */
            string[i] = NULL;
            i++;
            if (text[i] == NULL) { /* end of text */
                string[i] = NULL;
                break;
            }
        }
        string[i] = XmStringCreateLocalized(text[i]);
    }
    
    XtRealizeWidget(toplevel);

    /* (5) Create Graphics Context and set attribute */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
