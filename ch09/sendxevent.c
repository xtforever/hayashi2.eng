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

/* Event handler called on button press and mouse motion */
static void 
MoveArcEH (Widget w, XtPointer client_data, XEvent *event, Boolean *dispatch)
{
    Window destination = (Window)client_data;

    /* (2) Send event to window of receiver */
    event->xany.window = destination;
    XSendEvent(XtDisplay(w), destination, False,
               ButtonPressMask | Button1MotionMask, event);

    /* Move the arc to current pointer position */

    /* Handle only mouse left button */
    if (event->type == ButtonPress && event->xbutton.button != Button1)
        return;
    /* Erase previous arc */
    if (x >= 0)
        XClearArea(XtDisplay(w), XtWindow(w), x-ARC/2, y-ARC/2, ARC, ARC, False);
    /* Remember current mouse position */
    if (event->type == ButtonPress) {
        x = event->xbutton.x;
        y = event->xbutton.y;
    } else if (event->type == MotionNotify) {
        x = event->xmotion.x;
        y = event->xmotion.y;
    }
    /* Draw an arc at current mouse position (the arc looks as if it is moved) */
    XFillArc(XtDisplay(w), XtWindow(w), gc,
             x - ARC/2, y - ARC/2, ARC, ARC, 0, 360*64);
}

/* Redraw the arc at same position when drawing area is exposed */
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
    Widget toplevel, panel, canvas;
    Arg al[20];
    int ac;
    Atom atom, type;
    int format, stat;
    unsigned long nitems, left;
    Window *window;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Sendxevent", NULL, 0,
                               &argc, argv, NULL, al, ac);
    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    XtSetArg(al[ac], XmNwidth, 200); ac++;
    XtSetArg(al[ac], XmNheight, 200); ac++;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    /* Create a graphics context and set color to black */
    atom = XmInternAtom(XtDisplay(canvas), "_CANVAS_WINDOW", False);

    /* (1) Get the window ID from "_CANVAS_WINDOW" of root window */
    if (XGetWindowProperty(XtDisplay(canvas),
                  RootWindowOfScreen(XtScreen(canvas)), atom, 0, 1, False,
			   XA_WINDOW, &type, &format, &nitems, &left,
			   (unsigned char **)&window)
                                            != Success || type != XA_WINDOW)
        fprintf(stderr, "Cannot get window ID from _CANVAS_WINDOW property\n");

    /* Register an event handler for button press and mouse motion */
    XtAddEventHandler(canvas, ButtonPressMask | Button1MotionMask, False,
                      MoveArcEH, (XtPointer)*window);

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
