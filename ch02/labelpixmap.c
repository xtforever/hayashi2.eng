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
    Widget toplevel, panel, button;
    Arg al[20];
    int ac;
    Pixmap pixmap;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Labelpixmap", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    pixmap = XmGetPixmap(XtScreen(toplevel), "flagup",
                         BlackPixelOfScreen(XtScreen(toplevel)),
                         WhitePixelOfScreen(XtScreen(toplevel)));
    ac = 0;
    XtSetArg(al[ac], XmNlabelType, XmPIXMAP); ac++;
    XtSetArg(al[ac], XmNlabelPixmap, pixmap); ac++;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);

    XtRealizeWidget(toplevel);

    XtAppMainLoop(app_context);
}
