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
static String fallback_resources[] = {
  "*canvas.topAttachment:       ATTACH_FORM",
  "*canvas.bottomAttachment:    ATTACH_FORM",
  "*canvas.leftAttachment:      ATTACH_FORM",
  "*canvas.rightAttachment:     ATTACH_FORM",
  "*canvas.width:               200",
  "*canvas.height:              200",
  NULL
};

/* (5) Draw in event function (Callback) */
static void 
ExposeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    XSetForeground(XtDisplay(w), gc, BlackPixelOfScreen(XtScreen(w)));

    XSetLineAttributes(XtDisplay(w), gc, 20, LineSolid, CapButt, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 40, 150, 40);

    XSetLineAttributes(XtDisplay(w), gc, 20, LineSolid, CapRound, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 80, 150, 80);

    XSetLineAttributes(XtDisplay(w), gc, 20, LineSolid, CapProjecting, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 120, 150, 120);

    XSetLineAttributes(XtDisplay(w), gc, 20, LineSolid, CapNotLast, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 160, 150, 160);

    XSetForeground(XtDisplay(w), gc, WhitePixelOfScreen(XtScreen(w)));
    XSetLineAttributes(XtDisplay(w), gc, 1, LineOnOffDash, CapButt, JoinMiter);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 50, 20, 50, 180);
    XDrawLine(XtDisplay(w), XtWindow(w), gc, 150, 20, 150, 180);
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
    /* (1) Initialize X Toolkit (Open display) */
    toplevel = XtAppInitialize(&app_context, "XMdraw", NULL, 0,
                               &argc, argv, fallback_resources, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    /* (2) Create and manage widget */
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    /* (3) Register event function (Callback) */
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    XtRealizeWidget(toplevel); /* (2') Window is created here */

    /* (4) Create Graphics Context */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);

    XtAppMainLoop(app_context);
}
