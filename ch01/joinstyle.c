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
    static XPoint points1[] = { {50, 80}, {100, 40}, {150, 80} };
    static XPoint points2[] = { {50, 120}, {100, 80}, {150, 120} };
    static XPoint points3[] = { {50, 160}, {100, 120}, {150, 160} };

    XSetLineAttributes(XtDisplay(w), gc, 20, LineSolid, CapButt, JoinMiter);
    XDrawLines(XtDisplay(w), XtWindow(w), gc, points1, 3, CoordModeOrigin);

    XSetLineAttributes(XtDisplay(w), gc, 20, LineSolid, CapButt, JoinRound);
    XDrawLines(XtDisplay(w), XtWindow(w), gc, points2, 3, CoordModeOrigin);

    XSetLineAttributes(XtDisplay(w), gc, 20, LineSolid, CapButt, JoinBevel);
    XDrawLines(XtDisplay(w), XtWindow(w), gc, points3, 3, CoordModeOrigin);
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
