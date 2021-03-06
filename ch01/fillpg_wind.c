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
    static XPoint points[] = {
        { 30,  60},
        {170,  60},
        {170,  90},
        { 60,  90},
        { 60, 140},
        {110, 140},
        {110,  30},
        {140,  30},
        {140, 170},
        { 30, 170},
    };
    XSetForeground(XtDisplay(w), gc, BlackPixelOfScreen(XtScreen(w)));
    XSetFillRule(XtDisplay(w), gc, WindingRule);
    XFillPolygon(XtDisplay(w), XtWindow(w), gc, points, 10, Complex, CoordModeOrigin);
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
    XSetForeground(XtDisplay(canvas), gc, BlackPixelOfScreen(XtScreen(canvas)));
    XSetLineAttributes(XtDisplay(canvas), gc, 5, LineSolid, CapButt, JoinMiter);

    XtAppMainLoop(app_context);
}
