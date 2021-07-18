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

static GC gc;
static Colormap colormap;
static XColor colors[7];
/* prepare color names */
static char *colornames[] = {
    "red", "orange", "yellow", "green", "light sea green", "blue", "violet"
/*
    "#ff0000", "#ff8000", "#ffff00", "#00ff00", "#00a090", "#0000ff", "#9000e0"
*/
};

static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    int i;

    for (i = 0; i < 7; i++) {
        /* Set color pixel into GC and fill rectangle with the color */
        XSetForeground(XtDisplay(w), gc, colors[i].pixel);
        XFillRectangle(XtDisplay(w), XtWindow(w), gc, 30, 30 + i*20, 140, 20);
    }
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas;
    Arg al[20];
    int ac;
    int i;
    XColor exact;

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

    /* Get default color map */
    colormap = DefaultColormapOfScreen(XtScreen(canvas));

    /* Allocate colors by color name */
    for (i = 0; i < 7; i++) {
        if (XAllocNamedColor(XtDisplay(canvas), colormap,
                             colornames[i], &colors[i], &exact) == 0) {
            fprintf(stderr, "Cannot allocate color `%s'\n", colornames[i]);
            exit(1);
        }
    }

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);

    XtAppMainLoop(app_context);
}
