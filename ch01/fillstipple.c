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
    Pixmap stipple = XmGetPixmapByDepth(XtScreen(w), "xlogo32",
                                WhitePixelOfScreen(XtScreen(w)),
                                BlackPixelOfScreen(XtScreen(w)), 1);
    XSetFillStyle(XtDisplay(w), gc, FillStippled);
    XSetStipple(XtDisplay(w), gc, stipple);
    XSetForeground(XtDisplay(w), gc, BlackPixelOfScreen(XtScreen(w)));
    XSetBackground(XtDisplay(w), gc, WhitePixelOfScreen(XtScreen(w)));
    XFillArc(XtDisplay(w), XtWindow(w), gc,  50, 50, 100, 100, 0*64, 360*64);
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

    XtAppMainLoop(app_context);
}