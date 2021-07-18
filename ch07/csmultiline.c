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

extern XmString StringListToXmString();

static void ShowMessageCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget dialog = (Widget)client_data;
    XtManageChild(dialog);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget   toplevel, button, dialog;
    Arg      al[20];
    int      ac;
    XmString string;
    /* (1) Multi line message */
    static char *message[] = {"First line", "Second line", "Final line"};

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Csmultiline", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    button = XmCreatePushButton(toplevel, "button", al, ac);
    XtManageChild(button);

    /* (2) Create multi line compound string */
    string = StringListToXmString(message, 3);
    ac = 0;
    XtSetArg(al[ac], XmNmessageString, string); ac++; /* (3) set resource */
    dialog = XmCreateMessageDialog(toplevel, "dialog", al, ac);
    XmStringFree(string);  /* (4) free unnecessary compound string */

    XtAddCallback(button, XmNactivateCallback, ShowMessageCB, dialog);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
