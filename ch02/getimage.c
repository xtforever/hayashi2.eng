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
static XImage *image;
static Dimension width, height;
static Colormap colormap;
static XColor colors[7];
/* prepare color names */
static char *colornames[] = {
    "red", "orange", "yellow", "green", "light sea green", "blue", "violet"
};

static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    /* copy image to window */
    XPutImage(XtDisplay(w), XtWindow(w), gc, image, 0, 0, 0, 0, width, height);
}

/* (2) called when button is pressed */
static void InputCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XmDrawingAreaCallbackStruct *cb = (XmDrawingAreaCallbackStruct *)call_data;
    Widget label = (Widget)client_data;
    XImage *point;
    unsigned long pixel;
    XColor xcolor;
    char buff[BUFSIZ];
    XmString string;

    /* (3) only when mouse button is pressed */
    if (cb->event->type != ButtonPress)
        return;

    /* (4) dump the window contents into image (only the pressed point) */
    point = XGetImage(XtDisplay(w), XtWindow(w), cb->event->xbutton.x,
                      cb->event->xbutton.y, 1, 1, AllPlanes, ZPixmap);
    /* (5) get pixel value from image */
    pixel = XGetPixel(point, 0, 0);

    /* (6) get color element values */
    xcolor.pixel = pixel;
    XQueryColor(XtDisplay(w), colormap, &xcolor);

    /* (7) show the color element values */
    sprintf(buff, "RGB: %x, %x, %x",  xcolor.red, xcolor.green, xcolor.blue);
    string = XmStringCreateLocalized(buff);
    XtVaSetValues(label, XmNlabelString, string, NULL);
    XmStringFree(string);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas, label;
    Arg al[20];
    int ac;
    int i, j, k;
    XColor exact;
    Visual *visual;
    int depth;
    int x, y;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Getimage", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    ac = 0;
    label = XmCreateLabel(panel, "label", al, ac);
    XtManageChild(label);

    /* (1) Register callback for pressing on drawing area */
    XtAddCallback(canvas, XmNinputCallback, InputCB, label);

    /* Get default color map */
    colormap = DefaultColormapOfScreen(XtScreen(canvas));

    /* Allocate colors by color names */
    for (i = 0; i < 7; i++) {
        if (XAllocNamedColor(XtDisplay(canvas), colormap,
                             colornames[i], &colors[i], &exact) == 0) {
            fprintf(stderr, "Cannot allocate color `%s'\n", colornames[i]);
            exit(1);
        }
    }

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);

    /* Query window information of drawing area */
    XtVaGetValues(canvas, XmNvisual, &visual,
                          XmNdepth,  &depth,
                          XmNwidth,  &width,
                          XmNheight, &height, NULL);
    /* Create image */
    if ((image = XCreateImage(XtDisplay(canvas), visual, depth,
                              ZPixmap, 0, NULL, width, height,
                              BitmapPad(XtDisplay(canvas)), 0)) == NULL) {
        fprintf(stderr, "Cannot create image.\n");
        exit(1);
    }
    /* Allocate image data */
    if ((image->data = malloc(image->bytes_per_line * height)) == NULL) {
        fprintf(stderr, "Cannot allocate memory for image.\n");
        exit(1);
    }
    /* Initialize image data */
    memset(image->data, 0, image->bytes_per_line * height);

    /* draw 7 color band as image */
    for (i = 0; i < 7; i++) {  /* loop for 7 colors */
        for (j = 0; j < 140; j++) {  /* X direction */
            x = j + 30;
            for (k = 0; k < 20; k++) {  /* Y direction */
                y = k + i * 20 + 30;
                XPutPixel(image, x, y, colors[i].pixel);  /* put the color */
            }
        }
    }

    XtAppMainLoop(app_context);
}
