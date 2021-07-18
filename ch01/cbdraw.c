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
static Dimension width, height;

/* (2) Callback of drawing area */
static void 
DrawingCB (Widget w, XtPointer client_data, XtPointer call_data)
{
  XmDrawingAreaCallbackStruct *cb = (XmDrawingAreaCallbackStruct *)call_data;

  switch (cb->reason) {
  case XmCR_RESIZE: /* (3) Recalculate size of rectangle */
    XtVaGetValues(w, XmNwidth, &width, XmNheight, &height, NULL);
    XClearWindow(XtDisplay(w), XtWindow(w));
  case XmCR_EXPOSE: /* (4) Redraw rectangle */
    XDrawRectangle(XtDisplay(w), XtWindow(w), gc, 20, 20, width-40, height-40);
    break;
  case XmCR_INPUT: /* (5) Draw arc at button pressed position */
    if (cb->event->type == ButtonPress)
        XFillArc(XtDisplay(w), XtWindow(w), gc,
                 cb->event->xbutton.x - 10/2, cb->event->xbutton.y - 10/2,
                 10, 10, 0, 360*64);
    break;
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
    toplevel = XtAppInitialize(&app_context, "XMdraw", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    /* (1) Register event function (callback) */
    XtAddCallback(canvas, XmNresizeCallback, DrawingCB, NULL);
    XtAddCallback(canvas, XmNexposeCallback, DrawingCB, NULL);
    XtAddCallback(canvas, XmNinputCallback,  DrawingCB, NULL);

    XtRealizeWidget(toplevel);

    /* Check initial size of drawing area */
    XtVaGetValues(canvas, XmNwidth, &width, XmNheight, &height, NULL);

    /* Create Graphics Context and set line attributes */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);
    XSetLineAttributes(XtDisplay(canvas), gc, 2, LineSolid, CapButt, JoinMiter);

    XtAppMainLoop(app_context);
}
