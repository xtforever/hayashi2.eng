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

/* (5) Draw in event function (callback) */
static void 
ExposeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    XSetForeground(XtDisplay(w), gc, BlackPixelOfScreen(XtScreen(w)));
    XSetLineAttributes(XtDisplay(w), gc, 3, LineSolid, CapButt, JoinMiter);
    XDrawArc(XtDisplay(w), XtWindow(w), gc,  20, 80, 40, 40, 0 * 64, 360 * 64);
    XSetLineAttributes(XtDisplay(w), gc, 3, LineOnOffDash, CapButt, JoinMiter);
    XDrawArc(XtDisplay(w), XtWindow(w), gc,  80, 60, 40, 80, 0 * 64, 360 * 64);
    XSetLineAttributes(XtDisplay(w), gc, 6, LineSolid, CapRound, JoinMiter);
    XDrawArc(XtDisplay(w), XtWindow(w), gc, 140, 80, 40, 40, 30 * 64, 120 * 64);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget   toplevel, panel, canvas;
    Arg      al[20];
    int      ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    /* (1) Initialize X Toolkit (open display) */
    toplevel = XtAppInitialize(&app_context, "XMdraw", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    /* (2) Create and manage widget */
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    /* (3) Register event function (callback) */
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    XtRealizeWidget(toplevel); /* (2') Window is created here */

    /* (4) Create GC  */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);

    XtAppMainLoop(app_context);
}
