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

static XmString string;
static XmFontList fontlist;
static Dimension width, height, x, y;
static GC gc;

/* (1) Calculate text position from the size of drawing area */
static void ResetPos(w)
    Widget w;
{
    Dimension canvas_w, canvas_h;
    XtVaGetValues(w, XmNwidth, &canvas_w, XmNheight, &canvas_h, NULL);
    x = (canvas_w - width) / 2;
    y = (canvas_h - height) / 2;
}

/* (2) Draw compound string on drawing area */
static void Redraw(w)
    Widget w;
{
    XmStringDraw(XtDisplay(w), XtWindow(w), fontlist, string, gc, x, y,
                 width, XmALIGNMENT_CENTER, XmSTRING_DIRECTION_L_TO_R, NULL);
}

/* (3) Called when the size of drawing area changes */
static void ResizeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    ResetPos(w);
    XClearWindow(XtDisplay(w), XtWindow(w));
    Redraw(w);
}

/* (4) Called when the drawing area is exposed */
static void ExposeCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Redraw(w);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget   toplevel, panel, canvas;
    Arg      al[20];
    int      ac;
    static char *text = "Text";

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Csdraw", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    /* (5) Register callbacks for resize and expose of drawing area */
    XtAddCallback(canvas, XmNresizeCallback, ResizeCB, NULL);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    /* (6) Get fontlist resource value from widget's resource */
    XtVaGetValues(toplevel, XmNtextFontList, &fontlist, NULL);

    /* (7) Create compound string */
    string = XmStringCreateLocalized(text);

    /* (8) Query size of the text */
    XmStringExtent(fontlist, string, &width, &height);

    /* (9) Calculate initial position of text */
    ResetPos(canvas);

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), NULL, NULL);
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));

    XtAppMainLoop(app_context);
}
