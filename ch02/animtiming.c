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

/* (2) Timeout callback called in fixed time interval */
static void TimeoutCB(client_data, id)
    XtPointer     client_data;
    XtIntervalId *id;
{
    Widget w = (Widget)client_data;

    /* (3) Erase previous arc */
    XClearArea(XtDisplay(w), XtWindow(w), position, Y, ARC, ARC, False);

    /* (4) Calculate next position */
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

    /* (5) Draw arc at next position */
    XFillArc(XtDisplay(w), XtWindow(w), gc, position, Y, ARC, ARC, 0*64, 360*64);

    /* (6) Call this timeout callback again */
    timeout_id = XtAppAddTimeOut(XtWidgetToApplicationContext(w),
                                 INTERVAL, TimeoutCB, w);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget   toplevel, panel, canvas;
    Arg      al[20];
    int      ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Animation", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    /* (1) Regsiter timeout callback */
    timeout_id = XtAppAddTimeOut(app_context, INTERVAL, TimeoutCB, canvas);

    XtAppMainLoop(app_context);
}
