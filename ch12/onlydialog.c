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
    Widget toplevel, dialog;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Onlydialog", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    dialog = XmCreateMessageDialog(toplevel, "dialog", al, ac);
    XtManageChild(dialog);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
