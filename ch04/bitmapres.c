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

extern Boolean CvtStrToBitmap(); /* Resource Converter */

static GC gc; /* Graphics Context (necessary for Xlib drawing functions) */

typedef struct {  /* (1) Resource value structure */
    Pixmap bitmap;      /* pixmap */
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* (2) Custom Resource */
  {"bitmap", "Bitmap", XtRBitmap, sizeof(Pixmap), XtOffsetOf(CustomRec, bitmap),
   XtRImmediate, (XtPointer) XtUnspecifiedPixmap},
};

static XrmOptionDescRec opt[] = { /* (3) Custom Option */
  {"-bm", "*canvas.bitmap", XrmoptionSepArg, NULL},
};

/* (4) Redraw pixmap when drawing area is exposed or resized */
static void RedrawCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Pixmap bitmap = (Pixmap)client_data;
    XmDrawingAreaCallbackStruct *cb = (XmDrawingAreaCallbackStruct *)call_data;
    Window r;
    int x, y;
    unsigned int pw, ph, bw, d;
    Dimension dw, dh;

    if (bitmap == XtUnspecifiedPixmap)
        return;
    /* Query size of bitmap */
    if (XGetGeometry(XtDisplay(w), bitmap, &r, &x, &y, &pw, &ph, &bw, &d) == 0)
        return;

    /* Once clear the window when the drawing area is resized */
    if (cb->reason == XmCR_RESIZE)
        XClearWindow(XtDisplay(w), cb->window);

    /* Query size of drawing area */
    XtVaGetValues(w, XmNwidth,  &dw, XmNheight, &dh, NULL);

    /* Show the pixmap at the center of drawing area */
    XCopyArea(XtDisplay(w), bitmap, cb->window, gc, 0, 0, pw, ph,
              (dw - pw) / 2, (dh - ph) / 2);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, canvas;
    Arg    al[20];
    int    ac;
    CustomRec data;
    XtConvertArgRec args[2];  /* Auxiliary data passed to converter */
    Screen *screen;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;       /* Resource database is created here */
    toplevel = XtAppInitialize(&app_context, "Bitmapres", opt, XtNumber(opt),
                               &argc, argv, NULL, al, ac);

    /* (5) Prepare auxiliary data for resource converter */
    screen = XtScreen(toplevel);
    args[0].address_mode = XtAddress;
    args[0].address_id = (XtPointer)&screen;
    args[0].size = sizeof(Screen *);
    args[1].address_mode = XtAddress;
    args[1].address_id = (XtPointer)&canvas;
    args[1].size = sizeof(Widget);

    /* (6) Register resource converter from bitmap file name to Pixmap */
    XtAppSetTypeConverter(app_context, XtRString, XtRBitmap, CvtStrToBitmap,
                          args, XtNumber(args), XtCacheByDisplay, NULL);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);

    /* (7) Get custom resource (Resouce converter is called) */
    XtGetApplicationResources(canvas, &data, res, XtNumber(res), NULL, 0);
            /* The created pixmap ID is stored in data.bitmap */
    XtAddCallback(canvas, XmNexposeCallback, RedrawCB, data.bitmap);
    XtAddCallback(canvas, XmNresizeCallback, RedrawCB, data.bitmap);

    XtRealizeWidget(toplevel);
    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);
    XtAppMainLoop(app_context);
}
