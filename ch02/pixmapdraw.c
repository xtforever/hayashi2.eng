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
static Pixmap pixmap;
static Dimension width, height;

static void DrawEH(w, client_data, event, dispatch)
    Widget     w;
    XtPointer  client_data;
    XEvent    *event;
    Boolean   *dispatch;
{
    static int x0, y0;  /* start of line */
    int x, y;           /* end of line */

    if (event->type == ButtonPress && event->xbutton.button == Button1) {
        x0 = event->xbutton.x;  /* Initialize start of line */
        y0 = event->xbutton.y;
    }
    else if (event->type == MotionNotify && event->xmotion.state & Button1Mask) {
        x = event->xmotion.x;  /* end of line at this time */
        y = event->xmotion.y;

        /* draw line from previous position to current position */
        XDrawLine(XtDisplay(w), XtWindow(w), gc, x0, y0, x, y);
        /* draw the line into pixmap to save the contents */
        XDrawLine(XtDisplay(w), pixmap, gc, x0, y0, x, y);
        x0 = x; y0 = y;  /* set the start position of next line */
    }
}

/* Copy contents of pixmap onto drawing area when it is exposed */
static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XCopyArea(XtDisplay(w), pixmap, XtWindow(w), gc, 0, 0, width, height, 0, 0);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas;
    Arg al[20];
    int ac;
    int depth;

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

    XtAddEventHandler(canvas, ButtonPressMask | Button1MotionMask, False,
                      DrawEH, NULL);

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);
    XSetLineAttributes(XtDisplay(canvas), gc, 3, LineSolid, CapButt, JoinMiter);

    /* check size and depth of drawing area window */
    XtVaGetValues(canvas, XmNwidth, &width,
                          XmNheight, &height,
                          XmNdepth, &depth, NULL);

    /* create a pixmap with same size and depth as drawing area window */
    pixmap = XCreatePixmap(XtDisplay(canvas), XtWindow(canvas),
                           width, height, depth);

    /* clear pixmap by white */
    XSetForeground(XtDisplay(canvas), gc, WhitePixelOfScreen(XtScreen(canvas)));
    XFillRectangle(XtDisplay(canvas), pixmap, gc, 0, 0, width, height);

    /* line color is black */
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
