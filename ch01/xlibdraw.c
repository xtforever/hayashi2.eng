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

#include <X11/Xlib.h>

#define ARC 10
#define X 20
#define Y 20
static GC gc;
static unsigned int width, height;
static unsigned int rw = 0, rh = 0;
static int px, py;

/* Recalculate position and size of graphics */
static void ResetSizeAndPos(event)
    XEvent event;
{
    unsigned int old_w, old_h;
    old_w = rw;
    old_h = rh;
    width = event.xconfigure.width;
    height = event.xconfigure.height;

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
static void Redraw(display, window)
    Display *display;
    Window window;
{
    XDrawRectangle(display, window, gc, X, Y, rw, rh);
    XFillArc(display, window, gc, px - ARC/2, py - ARC/2, ARC, ARC,
             0, 360*64);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    Display *display;
    Window window;

    /* (1) Open display */
    display = XOpenDisplay(NULL);

    /* (2) Create and map window */
    window = XCreateSimpleWindow(display, RootWindow(display, 0),
                                400, 400, 200, 200, 0,
                                BlackPixel(display, 0), WhitePixel(display, 0));
    XMapWindow(display, window);

    /* (3) Prepare for event (set event mask) */
    XSelectInput(display, window,
                 StructureNotifyMask | ExposureMask | ButtonPressMask);

    /* (4) Create Graphics Context and set attribute */
    gc = XCreateGC(display, window, NULL, NULL);
    XSetLineAttributes(display, gc, 2, LineSolid, CapButt, JoinMiter);

    while (True) {
        XEvent event;
        XNextEvent(display, &event);

        /* (5) Process drawing for each event */
        switch (event.type) {
        case ConfigureNotify:
            ResetSizeAndPos(event);
            XClearWindow(display, window);
            Redraw(display, window);
        case Expose:
            Redraw(display, window);
            break;
        case ButtonPress:
            px = event.xbutton.x;
            py = event.xbutton.y;
            XClearWindow(display, window);
            Redraw(display, window);
            break;
        }
    }
}
