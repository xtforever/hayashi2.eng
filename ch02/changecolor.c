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

static Widget sample;
static GC gc;
static Colormap colormap;
static unsigned long pixel;
static unsigned long red = 0, green = 0, blue = 0;

/* Callback of scale */
static void ChangeColorCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XmScaleCallbackStruct *cb = (XmScaleCallbackStruct *)call_data;
    XColor color;

    /* Set new value */
    switch ((int)client_data) {  /* Which scale is changed ? */
        case 0: red   = cb->value; break;  /* red scale */
        case 1: green = cb->value; break;  /* green scale */
        case 2: blue  = cb->value; break;  /* blue scale */
    }
    /* Set new color info */
    color.pixel = pixel;  /* pixel value to be changed (private color cell) */
    color.red   = red   * 256;  /* RGB value: 0 - 65535. */ 
    color.green = green * 256;  /* Scale range is 0 - 255 */
    color.blue  = blue  * 256;  /* multiply 256 and use as RGB value */
    color.flags = DoRed | DoGreen | DoBlue;  /* change RGB element */

    /* Allocate color by color value */
    if (XStoreColor(XtDisplay(w), colormap, &color) == 0) {
        fprintf(stderr, "Cannot allocate the new color\n");
        exit(1);
    }

    /* Set color pixel to the background of sample widget */
    XtVaSetValues(sample, XmNbackground, color.pixel, NULL);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, control, scales[3];
    Arg al[20];
    int ac;
    int i;
    char wname[10];
    unsigned long plane;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Changecolor", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Widget to be shown sample color */
    ac = 0;
    sample = XmCreateLabel(panel, "sample", al, ac);
    XtManageChild(sample);

    ac = 0;
    control = XmCreateRowColumn(panel, "control", al, ac);
    XtManageChild(control);

    for (i = 0; i < 3; i++) {
        ac = 0;
        sprintf(wname, "scale%d", i);
        scales[i] = XmCreateScale(control, wname, al, ac);
        XtAddCallback(scales[i], XmNdragCallback, ChangeColorCB, i);
        XtAddCallback(scales[i], XmNvalueChangedCallback, ChangeColorCB, i);
    }
    XtManageChildren(scales, 3);

    /* Get default color map */
    colormap = DefaultColormapOfScreen(XtScreen(sample));
    /* Own one private color cell */
    if (XAllocColorCells(XtDisplay(sample), colormap,
                         False, &plane, 0, &pixel, 1) == 0) {
        fprintf(stderr, "Cannot allocate color cell.\n");
        exit(1);
    }

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(sample), XtWindow(sample), NULL, NULL);

    XtVaSetValues(sample,
                  XmNbackground, BlackPixelOfScreen(XtScreen(sample)), NULL);

    XtAppMainLoop(app_context);
}
