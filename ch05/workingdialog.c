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

static void 
ShowDialogCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget dialog = (Widget) client_data;

    XtManageChild(dialog);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, button, dialog;
    Arg    al[20];
    int    ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Workingdialog", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    dialog = XmCreateWorkingDialog(toplevel, "dialog", al, ac);
    /*
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
    */
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtAddCallback(button, XmNactivateCallback, ShowDialogCB, dialog);
    XtManageChild(button);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
