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
#include <stdlib.h>
#include <stdint.h>
#include <Xm/XmAll.h>

static Widget sample;
static GC gc;
static Colormap colormap;
static unsigned long pixel;
static unsigned long red = 0, green = 0, blue = 0;



static String fallback_resources[] = {
  "*sample.labelString:      ",
  "*sample.marginHeight:    40",
  "*sample.marginWidth:     100",
  "*XmScale.orientation:    HORIZONTAL",
  "*XmScale.showValue:      True",
  "*XmScale.minimum:        0",
  "*XmScale.maximum:        255",
  "*scale0.titleString:     Red",
  "*scale1.titleString:     Green",
  "*scale2.titleString:     Blue",
NULL
};

/* Callback of scale */
static void  
ChangeColorCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    intptr_t rgb=(intptr_t)client_data;
    XmScaleCallbackStruct *cbs = (XmScaleCallbackStruct *) call_data;
    Colormap cmap = DefaultColormapOfScreen (XtScreen (w));
    static XColor color; 

    switch (rgb) {
        case 0: color.red = (cbs->value << 8); break;
        case 1: color.green = (cbs->value << 8); break;
        case 2: color.blue = (cbs->value << 8);
    }
    /* reuse the same color again and again */
    color.flags = DoRed | DoGreen | DoBlue;  /* change RGB element */
    XFreeColors (XtDisplay (w), cmap, &color.pixel, 1, 0);

    if (!XAllocColor (XtDisplay (w), cmap, &color)) {
        puts ("Couldn't XAllocColor!"); exit(1);
    }

    XtVaSetValues (sample, XmNbackground, color.pixel, NULL);
}

int 
main (int argc, char **argv)
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
                               &argc, argv, fallback_resources, al, ac);

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
	XtAddCallback(scales[i], XmNdragCallback, ChangeColorCB,(XtPointer)(intptr_t)i);
        XtAddCallback(scales[i], XmNvalueChangedCallback, ChangeColorCB,(XtPointer)(intptr_t)i);

    }
    XtManageChildren(scales, 3);

    /* Get default color map */
    colormap = DefaultColormapOfScreen(XtScreen(sample));

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(sample), XtWindow(sample), 0, NULL);

    XtVaSetValues(sample,
                  XmNbackground, BlackPixelOfScreen(XtScreen(sample)), NULL);

    XtAppMainLoop(app_context);
}
