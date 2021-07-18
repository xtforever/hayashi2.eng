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

#define INTERVAL   50    /* Timing for next frame */
#define ARC        40    /* size of arc */
#define LEFT        0    /* x coordinate of left wall */
#define RIGHT     400    /* x coordinate of right wall */
#define Y          80    /* y coordinate of arc */
#define STEP        5    /* distance of motion by one frame */
#define LtoR        0    /* direction: left to right */
#define RtoL        1    /* direction: right to left */

static GC gc;
static XtIntervalId timeout_id;
static int position = LEFT;  /* current x position of arc */
static int direction = LtoR;  /* LtoR: left to right, RtoL: right to left */
static Pixmap front, back;  /* pixmap buffer */
static Dimension width, height;

static void TimeoutCB(client_data, id)
    XtPointer     client_data;
    XtIntervalId *id;
{
    Widget w = (Widget)client_data;
    Pixmap tmp;

    /* (3) Clear buffer */
    XSetForeground(XtDisplay(w), gc, WhitePixelOfScreen(XtScreen(w)));
    XFillRectangle(XtDisplay(w), back, gc, 0, 0, width, height);

    /* Calculate next position of arc */
    if (direction == LtoR) {             /* previous direction is L to R */
        position += STEP;                /* proceed to next position */
        if (position > (RIGHT - ARC)) {  /* reverse direction at wall */
            position = (RIGHT - ARC) - (position - (RIGHT - ARC));
            direction = RtoL;
        }
    }
    else { /* direction == RtoL */  /* previous direction is R to L */
        position -= STEP;           /* proceed to next position */
        if (position < LEFT) {      /* reverse direction at wall */
            position = LEFT - (position - LEFT);
            direction = LtoR;
        }
    }

    /* The arc is black */
    XSetForeground(XtDisplay(w), gc, BlackPixelOfScreen(XtScreen(w)));
    /* (4) Draw arc at next position */
    XFillArc(XtDisplay(w), back, gc, position, Y, ARC, ARC, 0*64, 360*64);

    /* Call this timeout callback again */
    timeout_id = XtAppAddTimeOut(XtWidgetToApplicationContext(w),
                                 INTERVAL, TimeoutCB, w);

    /* (5) Switch buffer */
    tmp = front;
    front = back;
    back = tmp;

    /* (6) Copy buffer contents to drawing area*/
    XCopyArea(XtDisplay(w), front, XtWindow(w), gc, 0, 0, width, height, 0, 0);
}

/* (7) Copy buffer contents when drawing area is exposed */
static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XCopyArea(XtDisplay(w), front, XtWindow(w), gc, 0, 0, width, height, 0, 0);
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
    toplevel = XtAppInitialize(&app_context, "Animation", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);
    XtManageChild(canvas);

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);

    /* query size and depth of drawing area window */
    XtVaGetValues(canvas, XmNwidth, &width,
                          XmNheight, &height,
                          XmNdepth, &depth, NULL);

    /* (1) Create two pixmaps with the same size and depth as drawing area */
    front = XCreatePixmap(XtDisplay(canvas), XtWindow(canvas),
                          width, height, depth);
    back = XCreatePixmap(XtDisplay(canvas), XtWindow(canvas),
                         width, height, depth);

    /* (2) Clear pixmaps by white */
    XSetForeground(XtDisplay(canvas), gc, WhitePixelOfScreen(XtScreen(canvas)));
    XFillRectangle(XtDisplay(canvas), front, gc, 0, 0, width, height);
    XFillRectangle(XtDisplay(canvas), back, gc, 0, 0, width, height);

    /* Regsiter timeout callback */
    timeout_id = XtAppAddTimeOut(app_context, INTERVAL, TimeoutCB, canvas);

    XtAppMainLoop(app_context);
}
