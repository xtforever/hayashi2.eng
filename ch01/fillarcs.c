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
static String fallback_resources[] = {
  "*canvas.topAttachment:       ATTACH_FORM",
  "*canvas.bottomAttachment:    ATTACH_FORM",
  "*canvas.leftAttachment:      ATTACH_FORM",
  "*canvas.rightAttachment:     ATTACH_FORM",
  "*canvas.width:               200",
  "*canvas.height:              200",
  NULL
};

static void 
ExposeCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    static XArc arcs[] = {
        { 20, 80, 40, 40,  0 * 64, 360 * 64},
        { 80, 60, 40, 80,  0 * 64, 360 * 64},
        {140, 80, 40, 40, 30 * 64, 120 * 64}
    };
    XSetForeground(XtDisplay(w), gc, BlackPixelOfScreen(XtScreen(w)));
    XFillArcs(XtDisplay(w), XtWindow(w), gc,  arcs, 3);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget   toplevel, panel, canvas;
    Arg      al[20];
    int      ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "XMdraw", NULL, 0,
                               &argc, argv, fallback_resources, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);
    XtAddCallback(canvas, XmNexposeCallback, ExposeCB, NULL);

    XtRealizeWidget(toplevel);

    gc = XCreateGC(XtDisplay(canvas), XtWindow(canvas), 0, NULL);

    XtAppMainLoop(app_context);
}
