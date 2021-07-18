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

static void 
ExposeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    XSetLineAttributes(XtDisplay(w), gc, 4, LineSolid, CapButt, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 50, 150, 50);

    XSetLineAttributes(XtDisplay(w), gc, 4, LineOnOffDash, CapButt, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 100, 150, 100);

    XSetLineAttributes(XtDisplay(w), gc, 4, LineDoubleDash, CapButt, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 150, 150, 150);
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

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));
    XSetBackground(XtDisplay(canvas), gc, WhitePixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
