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

#define ARC 10
static GC gc;
static int x = -ARC, y;

/* (2) Event handler when button is pressed or pointer is moved */
static void MoveArcEH(w, client_data, event, dispatch)
    Widget     w;
    XtPointer  client_data;
    XEvent    *event;
    Boolean   *dispatch;
{
    /* (3) Only left mouse button */
    if (event->type == ButtonPress && event->xbutton.button != Button1)
        return;
    /* (4) Erase previous arc */
    if (x >= 0)
        XClearArea(XtDisplay(w), XtWindow(w), x-ARC/2, y-ARC/2, ARC, ARC, False);
    /* (5) Remember current mouse position */
    if (event->type == ButtonPress) {
        x = event->xbutton.x;
        y = event->xbutton.y;
    } else if (event->type == MotionNotify) {
        x = event->xmotion.x;
        y = event->xmotion.y;
    }
    /* (6) Draw at current mouse position (looks as if arc is moved) */
    XFillArc(XtDisplay(w), XtWindow(w), gc,
             x - ARC/2, y - ARC/2, ARC, ARC, 0, 360*64);
}

/* (7) Redraw arc when the drawing area is exposed */
static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XFillArc(XtDisplay(w), XtWindow(w), gc,
             x - ARC/2, y - ARC/2, ARC, ARC, 0, 360*64);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget  toplevel, panel, canvas;
    Arg     al[20];
    int     ac;

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

    /* (1) Register event handler */
    XtAddEventHandler(canvas, ButtonPressMask | Button1MotionMask, False,
                      MoveArcEH, NULL);

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
