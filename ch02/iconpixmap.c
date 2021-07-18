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

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, label;
    Arg al[20];
    int ac;
    Pixmap bitmap;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "XMdraw", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    label = XmCreateLabel(panel, "label", al, ac);
    XtManageChild(label);

    XtRealizeWidget(toplevel);

    /* Read bitmap file for icon */
    bitmap = XmGetPixmapByDepth(XtScreen(toplevel), "terminal",
                                WhitePixelOfScreen(XtScreen(toplevel)),
                                BlackPixelOfScreen(XtScreen(toplevel)), 1);

    /* Set the pixmap for icon */
    XtVaSetValues(toplevel, XmNiconPixmap, bitmap, NULL);

    XtAppMainLoop(app_context);
}
