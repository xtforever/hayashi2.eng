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

    XtManageChild(dialog);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, button, dialog;
    Arg al[20];
    int ac;
    /* (1) delete Resize, Maximize and Close from mwm fnction */
    int functions = MWM_FUNC_ALL | MWM_FUNC_RESIZE | MWM_FUNC_MAXIMIZE
                                 | MWM_FUNC_CLOSE;
    /* (2) delete Resize handle and Maximize button from mwm decoration */
    int decorations = MWM_DECOR_ALL | MWM_DECOR_RESIZEH | MWM_DECOR_MAXIMIZE;

    XtSetLanguageProc(NULL, NULL, NULL);

    /* (3) restrict mwm function and decoration of toplevel shell */
    ac = 0;
    XtSetArg(al[ac], XmNmwmFunctions, functions); ac++;
    XtSetArg(al[ac], XmNmwmDecorations, decorations); ac++;
    toplevel = XtAppInitialize(&app_context, "Mwmhints", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (4) restrict mwm function and decoration of dialog */
    ac = 0;
    XtSetArg(al[ac], XmNmwmFunctions, functions); ac++;
    XtSetArg(al[ac], XmNmwmDecorations, decorations); ac++;
    dialog = XmCreateMessageDialog(toplevel, "dialog", al, ac);

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
