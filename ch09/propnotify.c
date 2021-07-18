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

static Atom atom;

static void NotifyCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    /* (2) update "_NOTIFY" of root window */
    XChangeProperty(XtDisplay(w), RootWindowOfScreen(XtScreen(w)),
                    atom, XA_STRING, 8, PropModeReplace, NULL, 0);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, button;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Propnotify", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    button = XmCreatePushButton(toplevel, "button", al, ac);
    XtAddCallback(button, XmNactivateCallback, NotifyCB, NULL);
    XtManageChild(button);

    /* (1) Create atom for property name */
    atom = XmInternAtom(XtDisplay(toplevel), "_NOTIFY", False);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
