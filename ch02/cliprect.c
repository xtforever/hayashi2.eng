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

static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XmDrawingAreaCallbackStruct *cb = (XmDrawingAreaCallbackStruct *)call_data;
    int i, j;
    XRectangle rectangle;

    /* Restrict drawing area by clip mask */
    rectangle.x = cb->event->xexpose.x;
    rectangle.y = cb->event->xexpose.y;
    rectangle.width = cb->event->xexpose.width;
    rectangle.height = cb->event->xexpose.height;
    XSetClipRectangles(XtDisplay(w), gc, 0, 0, &rectangle, 1, Unsorted);

    /* draw many graphics */
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            XFillArc(XtDisplay(w), XtWindow(w), gc,  i * 40 + 10, j * 40 + 10,
                     20, 20,  0 * 64, 360 * 64);
        }
    }
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget   toplevel, panel, canvas;
    Arg      al[20];
    int      ac;

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

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
