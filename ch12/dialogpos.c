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

static void ShowDialogCB(w, client_data, call_data)
    Widget    w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget dialog = (Widget) client_data;
    Position root_x, root_y;
    Dimension height;

    /* (3) Check height of push button */
    XtVaGetValues(w, XmNheight, &height, NULL);
    /* (4) Translate the bottom of the push button into the coordinate of root window */
    XtTranslateCoords(w, 0, height, &root_x, &root_y);
    /* (5) show dialog at the translated location */
    XtVaSetValues(dialog, XmNx, root_x, XmNy, root_y, NULL);
    XtManageChild(dialog);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, button1, button2, button3, dialog;
    Arg    al[20];
    int    ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Dialogpos", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (1) Create a dialog */
    ac = 0;
    dialog = XmCreateMessageDialog(toplevel, "dialog", al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (2) Create three pushbuttons and register callbacks */
    ac = 0;
    button1 = XmCreatePushButton(panel, "button1", al, ac);
    XtAddCallback(button1, XmNactivateCallback, ShowDialogCB, dialog);
    XtManageChild(button1);
    ac = 0;
    button2 = XmCreatePushButton(panel, "button2", al, ac);
    XtAddCallback(button2, XmNactivateCallback, ShowDialogCB, dialog);
    XtManageChild(button2);
    ac = 0;
    button3 = XmCreatePushButton(panel, "button3", al, ac);
    XtAddCallback(button3, XmNactivateCallback, ShowDialogCB, dialog);
    XtManageChild(button3);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
