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
/* Prepare color values */
static XColor colors[] = {
    {0, 65535,     0,     0, 0, 0},
    {0, 65535, 32767,     0, 0, 0},
    {0, 65535, 65535,     0, 0, 0},
    {0,     0, 65535,     0, 0, 0},
    {0,     0, 43520, 40960, 0, 0},
    {0,     0,     0, 65535, 0, 0},
    {0, 35840,     0, 51200, 0, 0},
};

static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    int i;

    for (i = 0; i < 7; i++) {
        /* Set color pixel into GC and fill rectangle by the color */
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

    /* allocate colors by color value */
    for (i = 0; i < 7; i++) {
        if (XAllocColor(XtDisplay(canvas), colormap, &colors[i]) == 0) {
            fprintf(stderr, "Cannot allocate color.\n");
            exit(1);
        }
    }

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);

    XtAppMainLoop(app_context);
}
