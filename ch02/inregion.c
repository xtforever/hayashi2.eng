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
static Region region = NULL;

static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XmDrawingAreaCallbackStruct *cb = (XmDrawingAreaCallbackStruct *)call_data;
    int i, j;
    int x, y;
    unsigned int width, height;

    /* Create region if not created */
    if (region == NULL)
        region = XCreateRegion();

    /* Add this Expose area into region */
    XtAddExposureToRegion(cb->event, region);

    /* Do not draw if Expose events follow */
    if (cb->event->xexpose.count > 0)
        return;

    width = height = 20;
    /* draw many graphics */
    for (i = 0; i < 5; i++) {
        x = i * 40 + 10;
        for (j = 0; j < 5; j++) {
            y = j * 40 + 10;
            /* draw when the graphic is in region */
            if (XRectInRegion(region, x, y, width, height) != RectangleOut)
                XFillArc(XtDisplay(w), XtWindow(w), gc,
                         x, y, width, height, 0 * 64, 360 * 64);
        }
    }

    /* free region and prepare for next time */
    XDestroyRegion(region);
    region = NULL;
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
