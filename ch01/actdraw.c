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

/* (1) Action called when buttom press and mouse motion */
static void MoveArcAC(w, event, params, num_params)
    Widget    w;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
{
    /* (2) Erase arc drawn at previous time */
    if (x >= 0)
        XClearArea(XtDisplay(w), XtWindow(w), x-ARC/2, y-ARC/2, ARC, ARC, False);
    /* (3) Remember current mouse position */
    if (event->type == ButtonPress) {
        x = event->xbutton.x;
        y = event->xbutton.y;
    } else if (event->type == MotionNotify) {
        x = event->xmotion.x;
        y = event->xmotion.y;
    }
    /* (4) Draw arc at current mouse position (looks as the arc moves) */
    XFillArc(XtDisplay(w), XtWindow(w), gc,
             x - ARC/2, y - ARC/2, ARC, ARC, 0, 360*64);
}

/* (5) Define Action table */
static XtActionsRec actions[] = {
    { "move-arc", MoveArcAC },
};

/* Redraw arc when the drawing area is exposed */
static void 
ExposeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    XFillArc(XtDisplay(w), XtWindow(w), gc,
             x - ARC/2, y - ARC/2, ARC, ARC, 0, 360*64);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget  toplevel, panel, canvas;
    Arg     al[20];
    int     ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Actdraw", NULL, 0,
                               &argc, argv, NULL, al, ac);
    /* (6) Register action table */
    XtAppAddActions(app_context, actions, XtNumber(actions));
    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    XtRealizeWidget(toplevel);

    /* Create Graphcs Context and set color to black */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
