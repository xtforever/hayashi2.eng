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
    Widget toplevel, label;
    Arg al[20];
    int ac;
    Atom atom;
    XEvent event;
    XmString string;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Proprecv", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XtManageChild(label);

    /* (1) Create atom for the property name */
    atom = XmInternAtom(XtDisplay(label), "_NOTIFY", False);

    /* (2) Set event mask of property change on root window */
    XSelectInput(XtDisplay(label), RootWindowOfScreen(XtScreen(label)),
                 PropertyChangeMask);

    XtRealizeWidget(toplevel);

    /* (3) Custom event loop */
    for (;;) {
        XtAppNextEvent(app_context, &event);
        /* (4) Check if the event is PropertyNotify on root window */
        if (event.type == PropertyNotify &&
            event.xany.window == RootWindowOfScreen(XtScreen(label))) {
            /* (5) Change the label string and notify by bell */
            string = XmStringCreateLocalized("Received the Notify");
            XtVaSetValues(label, XmNlabelString, string, NULL);
            XmStringFree(string);
            XBell(XtDisplay(label), 0);
        }
        else
            XtDispatchEvent(&event);
    }
}
