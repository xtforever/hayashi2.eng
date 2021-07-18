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
    Widget toplevel, panel, textf, text;
    Arg    al[20];
    int    ac;

    /* (1) Set locale (register language procedure) */
    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "XImtext", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (2) Create text widgets */

    ac = 0;
    textf = XmCreateTextField(panel, "textf", al, ac);
    XtManageChild(textf);

    ac = 0;
    text = XmCreateText(panel, "text", al, ac);
    XtManageChild(text);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
