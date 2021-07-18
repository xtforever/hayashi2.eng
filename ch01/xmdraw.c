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
#define X 20
#define Y 20
static GC gc;
static Dimension width, height;
static unsigned int rw = 0, rh = 0;
static int px, py;

/* Recalculate position and size of graphics */
static void 
ResetSizeAndPos (Widget w)
{
    unsigned int old_w, old_h;
    old_w = rw;
    old_h = rh;

    /* Query size of drawing area */
    XtVaGetValues(w, XmNwidth, &width, XmNheight, &height, NULL);

    /* calculate size of rectangle */
    rw = width - X * 2;
    rh = height - Y * 2;

    /* calculate position of arc */
    if (old_w == 0) px = width / 2;
    else            px = (double)(px - X) * (double)rw / (double)old_w + X;
    if (old_h == 0) py = height / 2;
    else            py = (double)(py - Y) * (double)rh / (double)old_h + Y;
}

/* Draw graphic on window */
static void 
Redraw (Widget w)
{
    XDrawRectangle(XtDisplay(w), XtWindow(w), gc, X, Y, rw, rh);
    XFillArc(XtDisplay(w), XtWindow(w), gc, px - ARC/2, py - ARC/2, ARC, ARC,
             0, 360*64);
}

/* (5) Draw in event function (callback) */

/* Recalculate size of graphics when size of drawing area changed */
static void 
ResizeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    ResetSizeAndPos(w);
    XClearWindow(XtDisplay(w), XtWindow(w));
    Redraw(w);
}

/* Redraw graphics when drawing area is exposed */
static void 
ExposeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Redraw(w);
}

/* Draw graphic at button is pressed on drawing area */
static void 
InputCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    XmDrawingAreaCallbackStruct *cb = (XmDrawingAreaCallbackStruct *)call_data;
    if (cb->event->type == ButtonPress) {
        px = cb->event->xbutton.x;
        py = cb->event->xbutton.y;
        XClearWindow(XtDisplay(w), XtWindow(w));
        Redraw(w);
    }
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
    XtAddCallback(canvas, XmNresizeCallback, ResizeCB, NULL);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);
    XtAddCallback(canvas, XmNinputCallback,  InputCB,  NULL);

    /* Calculate initial position and size */
    ResetSizeAndPos(canvas);

    XtRealizeWidget(toplevel);
    /* (2') Window is created here */

    /* (4) Create Graphics Context and set attribute */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);
    XSetLineAttributes(XtDisplay(canvas), gc, 2, LineSolid, CapButt, JoinMiter);

    XtAppMainLoop(app_context);
}
