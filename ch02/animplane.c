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
#define Yr         20    /* y coordinate of red arc */
#define Yg         80    /* y coordinate of green arc */
#define Yb        140    /* y coordinate of blue arc */
#define STEP        5    /* distance of motion by one frame */
#define LtoR        0    /* direction: left to right */
#define RtoL        1    /* direction: right to left */

#define BUFFER0     0
#define BUFFER1     1

static GC gc;
static XtIntervalId timeout_id;
static int position = LEFT;  /* current x position of arc */
static int direction = LtoR;
static Dimension width, height;  /* size of drawing area */

static Colormap colormap;
static unsigned long planes[4], pixels[1];  /* plane and base pixel */
/* (1) define four color structures */
static XColor yellow = {0, 65535, 65535,     0, DoRed | DoGreen | DoBlue, 0};
static XColor red    = {0, 65535,     0,     0, DoRed | DoGreen | DoBlue, 0};
static XColor green  = {0,     0, 65535,     0, DoRed | DoGreen | DoBlue, 0};
static XColor blue   = {0,     0,     0, 65535, DoRed | DoGreen | DoBlue, 0};
/* (2) array for color map definition: colors0 => BUFFER0, colors1 => BUFFER1 */
static XColor colors0[16], colors1[16];
static Boolean which = BUFFER0;  /* draw on buffer0 at first */

static void TimeoutCB();

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas;
    Arg al[20];
    int ac;
    int depth;
    int i;

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

    /* query size of drawing area window */
    XtVaGetValues(canvas, XmNwidth, &width,
                          XmNheight, &height, NULL);

    /* Create GC */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);

    /* (3) Get default color map */
    colormap = DefaultColormapOfScreen(XtScreen(canvas));

    /* (4) Allocate color cells for double buffer*/
    if (XAllocColorCells(XtDisplay(canvas), colormap,
                         False, planes, 4, pixels, 1) == 0) {
        fprintf(stderr, "Cannot allocate color cells.\n");
        exit(1);
    }

    /* (5) Prepare color array for BUFFER0 */
    /* (6) Prepare four color cells per each color */
    for (i = 0; i < 16; i++) {
        if ((i / 4) == 0)  colors0[i] = yellow; /* colors0[] :  0 -  3 */
        if ((i / 4) == 1)  colors0[i] = red;    /* colors0[] :  4 -  7 */
        if ((i / 4) == 2)  colors0[i] = green;  /* colors0[] :  8 - 11 */
        if ((i / 4) == 3)  colors0[i] = blue;   /* colors0[] : 11 - 15 */
    }
    /* (7) Select color by planes[0] and planes[1] */
    for (i = 0; i < 16; i++) {
        if ((i / 4) == 0)  colors0[i].pixel = pixels[0];
        if ((i / 4) == 1)  colors0[i].pixel = pixels[0]|planes[0];
        if ((i / 4) == 2)  colors0[i].pixel = pixels[0]|planes[1];
        if ((i / 4) == 3)  colors0[i].pixel = pixels[0]|planes[0]|planes[1];
    }
    /* (8) do not depend on the planes[2] and planes[3] of BUFFER1 */
    for (i = 0; i < 16; i++) {
        if ((i % 4) == 1)  colors0[i].pixel |= planes[2];
        if ((i % 4) == 2)  colors0[i].pixel |= planes[3];
        if ((i % 4) == 3)  colors0[i].pixel |= planes[2]|planes[3];
    }

    /* (9) Prepare color array for BUFFER1 */
    /* (10) Prepare four color cells per each color */
    for (i = 0; i < 16; i++) {
        if ((i / 4) == 0)  colors1[i] = yellow; /* colors1[] :  0 -  3 */
        if ((i / 4) == 1)  colors1[i] = red;    /* colors1[] :  4 -  7 */
        if ((i / 4) == 2)  colors1[i] = green;  /* colors1[] :  8 - 11 */
        if ((i / 4) == 3)  colors1[i] = blue;   /* colors1[] : 11 - 15 */
    }
    /* (11) Select color by planes[2] and planes[3] */
    for (i = 0; i < 16; i++) {
        if ((i / 4) == 0)  colors1[i].pixel = pixels[0];
        if ((i / 4) == 1)  colors1[i].pixel = pixels[0]|planes[2];
        if ((i / 4) == 2)  colors1[i].pixel = pixels[0]|planes[3];
        if ((i / 4) == 3)  colors1[i].pixel = pixels[0]|planes[2]|planes[3];
    }
    /* (12) do not depend on the planes[0] and planes[1] of BUFFER0 */
    for (i = 0; i < 16; i++) {
        if ((i % 4) == 1)  colors1[i].pixel |= planes[0];
        if ((i % 4) == 2)  colors1[i].pixel |= planes[1];
        if ((i % 4) == 3)  colors1[i].pixel |= planes[0]|planes[1];
    }

    /* Use color config of BUFFER1 because it draws on BUFFER0 at first */
    XStoreColors(XtDisplay(canvas), colormap, colors1, 16);

    /* Set background to yellow (base pixel value pixels[0]) */
    XtVaSetValues(canvas, XmNbackground, pixels[0], NULL);

    /* Register timeout callback */
    timeout_id = XtAppAddTimeOut(app_context, INTERVAL, TimeoutCB, canvas);

    XtAppMainLoop(app_context);
}

static void TimeoutCB(client_data, id)
    XtPointer     client_data;
    XtIntervalId *id;
{
    Widget w = (Widget)client_data;

    /* (13) Set plane mask */
    if (which == BUFFER0)
        XSetPlaneMask(XtDisplay(w), gc, planes[0] | planes[1]);
    else
        XSetPlaneMask(XtDisplay(w), gc, planes[2] | planes[3]);

    /* (14) Clear buffer */
    XSetForeground(XtDisplay(w), gc, 0);
    XFillRectangle(XtDisplay(w), XtWindow(w), gc, 0, 0, width, height);

    /* (15) Calculate next position */
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

    /* (16) Set colors for each arcs and draw arcs at next position */
    XSetForeground(XtDisplay(w), gc, planes[0]|planes[2]);
    XFillArc(XtDisplay(w), XtWindow(w), gc, position, Yr, ARC,ARC, 0*64,360*64);
    XSetForeground(XtDisplay(w), gc, planes[1]|planes[3]);
    XFillArc(XtDisplay(w), XtWindow(w), gc, position, Yg, ARC,ARC, 0*64,360*64);
    XSetForeground(XtDisplay(w), gc, planes[0]|planes[1]|planes[2]|planes[3]);
    XFillArc(XtDisplay(w), XtWindow(w), gc, position, Yb, ARC,ARC, 0*64,360*64);

    /* (17) Change color map (Exchange front and back buffers) */
    if (which == BUFFER0) {
        XStoreColors(XtDisplay(w), colormap, colors0, 16);
        which = BUFFER1;
    }
    else {
        XStoreColors(XtDisplay(w), colormap, colors1, 16);
        which = BUFFER0;
    }

    /* Call timeout callback again */
    timeout_id = XtAppAddTimeOut(XtWidgetToApplicationContext(w),
                                 INTERVAL, TimeoutCB, w);
}
