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
#include <stdlib.h>
#include <Xm/XmAll.h>

static void 
DialogCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Boolean answer = (Boolean)client_data;
    /* (7) Select return value in accordance with the answer from user */
    if (answer) exit(0);  /* the user answered [Yes] */
    else        exit(1);  /* the user answered [No] */
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, dialog;
    Arg al[20];
    int ac;
    Position x, y;
    XmString string;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    /* (1) Do not show toplevel window */
    XtSetArg(al[ac], XmNmappedWhenManaged, False); ac++;
    toplevel = XtAppInitialize(&app_context, "Getanswer", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (2) Convert question string to compound string */
    if (argc == 2)
        string = XmStringCreateLocalized(argv[1]);
    else {
        fprintf(stderr, "Usage: %s 'Question'\n", argv[0]);
        exit(2);
    }

    /* (3) Calculate dialog position from screen size */
    x = WidthOfScreen(XtScreen(toplevel)) / 2 - 100;
    y = HeightOfScreen(XtScreen(toplevel)) / 2 - 50;


    /* (4) Create dialog with specifying the location */
    ac = 0;
    XtSetArg(al[ac], XmNx, x); ac++;  /* dialog position */
    XtSetArg(al[ac], XmNy, y); ac++;
    XtSetArg(al[ac], XmNdefaultPosition, False); ac++;
    XtSetArg(al[ac], XmNmessageString, string); ac++;  /* question message */
    dialog = XmCreateQuestionDialog(toplevel, "dialog", al, ac);
    /* do not show [Help] button */
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));
    /* (5) manage dialog to show it by default */
    XtManageChild(dialog);

    /* (6) Register callbacks for [Yes] and [No] button */
    XtAddCallback(dialog, XmNokCallback, DialogCB, True);
    XtAddCallback(dialog, XmNcancelCallback, DialogCB, False);

    XmStringFree(string);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
