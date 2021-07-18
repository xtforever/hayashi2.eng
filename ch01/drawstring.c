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

#include <Xm/XmAll.h>

#define XOFFSET 20
#define YOFFSET 60
static GC gc;
static XFontSet fontset[3];

/* (1) Drawn text.  Newline at NULL.  Two NULL at end */
static char *text[] = { "When", "drawing", "multi-line", NULL,
                        "text,", "care for", "line and", NULL,
                        "character", "spacing !", NULL, NULL };

/* (2) Use different for each line */
static char *fontname[] = { "*-medium-r-normal--16-*",
                            "*-medium-r-normal--14-*",
                            "*-medium-r-normal--24-*" };

/* (5) Redraw text when the window is exposed */
static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    int tx, ty;  /* Starting position of each text */
    int i, line = 0;
    XFontSetExtents *fsextent;
    Boolean newline = True;

    ty = YOFFSET;

    /* (6) Draw text segment one by one */
    for (i = 0; ; i++) {
        if (text[i] == NULL) { /* (7) end of line */
            i++;
            if (text[i] == NULL) /* (8) end of text */
                break;
            line++;
            newline = True;
        }
        if (newline) {
            /* (9) Check height of next font set */
            fsextent = XExtentsOfFontSet(fontset[line]);
            /* (10) Go to next line */
            ty += fsextent->max_logical_extent.height;
            /* (11) Return caridge to the left of the line */
            tx = XOFFSET;
            newline = False;
        }
        /* (12) Draw one text segment */
        XmbDrawString(XtDisplay(w), XtWindow(w), fontset[line], gc,
                      tx, ty, text[i], strlen(text[i]));
        /* (13) Move to next character position */
        tx += XmbTextEscapement(fontset[line], text[i], strlen(text[i]));
    }
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas;
    Arg    al[20];
    int    ac;
    char **miss, def;
    int    n_miss, i, j;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "XMdraw", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    /* (3) Load font set */
    for (i = 0; i < 3; i++) {
        fontset[i] = XCreateFontSet(XtDisplay(canvas), fontname[i],
                                    &miss, &n_miss, &def);
        if (n_miss > 0) {
            fprintf(stderr, "Missing charsets for %s\n", fontname[i]);
            for (j = 0; j < n_miss; j++)
                fprintf(stderr, "\t%s\n", miss[j]);
            XFreeStringList(miss);
        }
    }

    XtRealizeWidget(toplevel);

    /* (4) Create Graphics Context and set attribute */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
