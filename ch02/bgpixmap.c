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


int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, label;
    Arg al[20];
    int ac;
    Pixmap pixmap;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Bgpixmap", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    pixmap = XmGetPixmap(XtScreen(toplevel), "xlogo32",
                         BlackPixelOfScreen(XtScreen(toplevel)),
                         WhitePixelOfScreen(XtScreen(toplevel)));
    ac = 0;
    XtSetArg(al[ac], XmNbackgroundPixmap, pixmap); ac++;
    label = XmCreateLabel(panel, "label", al, ac);
    XtManageChild(label);

    XtRealizeWidget(toplevel);

    XtAppMainLoop(app_context);
}
