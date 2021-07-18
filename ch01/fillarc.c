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
    XSetForeground(XtDisplay(w), gc, BlackPixelOfScreen(XtScreen(w)));

    XFillArc(XtDisplay(w), XtWindow(w), gc,  20, 80, 40, 40,  0 * 64, 360 * 64);

    XSetArcMode(XtDisplay(w), gc, ArcPieSlice);
    XFillArc(XtDisplay(w), XtWindow(w), gc,  80, 80, 40, 40, 30 * 64, 120 * 64);

    XSetArcMode(XtDisplay(w), gc, ArcChord);
    XFillArc(XtDisplay(w), XtWindow(w), gc, 140, 80, 40, 40, 30 * 64, 120 * 64);
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
