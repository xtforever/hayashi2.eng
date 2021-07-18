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
static Colormap colormap;
static unsigned long planes[1], pixels[4];

/* Event handler called on mouse operation */
static void DrawEH(w, client_data, event, dispatch)
    Widget     w;
    XtPointer  client_data;
    XEvent    *event;
    Boolean   *dispatch;
{
    static int x0, y0;  /* start of line */
    static int x1, y1;  /* end of line */
    static unsigned long color;  /* final color of line */

    switch (event->type) {
    case ButtonPress:
    /* Start rubber band */
        /* reset start of line */
        x0 = x1 = event->xbutton.x;
        y0 = y1 = event->xbutton.y;
        /* set GC for rubberband */
        XSetFunction(XtDisplay(w), gc, GXxor);
        XSetPlaneMask(XtDisplay(w), gc, planes[0]);
        XSetForeground(XtDisplay(w), gc, planes[0]);
        XSetLineAttributes(XtDisplay(w), gc, 2, LineOnOffDash, CapButt, JoinMiter);
        /* draw initial rubber band */
        XDrawLine(XtDisplay(w), XtWindow(w), gc, x0, y0, x1, y1);
        /* select color according with button number (rubber band is yellow) */
        switch (event->xbutton.button) {
            case Button1: color = pixels[0]; break;
            case Button2: color = pixels[1]; break;
            case Button3: color = pixels[2]; break;
        }
        break;
    case MotionNotify:
    /* draw the line by rubber band */
        /* draw again at the same location to erase previous rubber band */
        XDrawLine(XtDisplay(w), XtWindow(w), gc, x0, y0, x1, y1);
        /* set current pointer position to the end of line */
        x1 = event->xbutton.x;
        y1 = event->xbutton.y;
        /* draw rubber band from original start to current pointer position */
        XDrawLine(XtDisplay(w), XtWindow(w), gc, x0, y0, x1, y1);
        break;
    case ButtonRelease:
    /* draw final line with correct color */
        /* draw again to erase previous rubber band */
        XDrawLine(XtDisplay(w), XtWindow(w), gc, x0, y0, x1, y1);
        /* set current pointer position to the end of line */
        x1 = event->xmotion.x;
        y1 = event->xmotion.y;
        /* set GC to normal */
        XSetFunction(XtDisplay(w), gc, GXcopy);
        XSetPlaneMask(XtDisplay(w), gc, AllPlanes);
        XSetLineAttributes(XtDisplay(w), gc, 2, LineSolid, CapButt, JoinMiter);
        /* set color appropreately for button number */
        XSetForeground(XtDisplay(w), gc, color);
        /* draw final line */
        XDrawLine(XtDisplay(w), XtWindow(w), gc, x0, y0, x1, y1);
        break;
    }
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas;
    Arg al[20];
    int ac;
    int flags = DoRed | DoGreen | DoBlue;
    Display *display;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Rubberband", NULL, 0,
                               &argc, argv, NULL, al, ac);
    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);

    /* Register event handler */
    XtAddEventHandler(canvas,
              ButtonPressMask | ButtonReleaseMask | ButtonMotionMask, False,
              DrawEH, NULL);

    XtRealizeWidget(toplevel);

    /* Create Graphics Context */
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);

    /* Get default color map */
    colormap = DefaultColormapOfScreen(XtScreen(canvas));

    /* Allocate color cells for rubber band (4 colors, 1 plane mask) */
    if (XAllocColorCells(XtDisplay(canvas), colormap,
                         False, planes, 1, pixels, 4) == 0) {
        fprintf(stderr, "Cannot allocate color cells.\n");
        exit(1);
    }

    display = XtDisplay(canvas);
    /* Allocate colors of final lines */
    XStoreNamedColor(display, colormap, "red", pixels[0], flags);
    XStoreNamedColor(display, colormap, "blue", pixels[1], flags);
    XStoreNamedColor(display, colormap, "green", pixels[2], flags);
    /* Allocate color of background of drawing area */
    XStoreNamedColor(display, colormap, "white", pixels[3], flags);

    /* Allocate colosr for rubber band */
    XStoreNamedColor(display, colormap, "yellow3", pixels[0] | planes[0], flags);
    XStoreNamedColor(display, colormap, "yellow3", pixels[1] | planes[0], flags);
    XStoreNamedColor(display, colormap, "yellow3", pixels[2] | planes[0], flags);
    XStoreNamedColor(display, colormap, "yellow3", pixels[3] | planes[0], flags);

    /* Set background of drawing area to white */
    XtVaSetValues(canvas, XmNbackground, pixels[3], NULL);

    XtAppMainLoop(app_context);
}
