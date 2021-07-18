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

static void 
RecvNotifyEH (Widget w, XtPointer client_data, XEvent *event, Boolean *dispatch)
{
    XmString string;

    /* (4) Check if the event is PropertyNotify on root window */
    if (event->type == PropertyNotify &&
        event->xany.window == RootWindowOfScreen(XtScreen(w))) {
        /* (5) Change the label string and notify by bell */
        string = XmStringCreateLocalized("Received the Notify");
        XtVaSetValues(w, XmNlabelString, string, NULL);
        XmStringFree(string);
        XBell(XtDisplay(w), 0);
    }
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, label;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtOpenApplication(&app_context, "Proprecv", NULL, 0,
                        &argc, argv, NULL, sessionShellWidgetClass, al, ac);

    ac = 0;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XtManageChild(label);

    /* Create atom for the property name */
    atom = XmInternAtom(XtDisplay(label), "_NOTIFY", False);

    /* (1) Set event mask of property change on root window */
    XSelectInput(XtDisplay(label), RootWindowOfScreen(XtScreen(label)),
                 PropertyChangeMask);

    /* (2) register root window to widget */
    XtRegisterDrawable(XtDisplay(label),
                       RootWindowOfScreen(XtScreen(label)), label);

    /* (3) register an event handler for property change */
    XtAddEventHandler(label, PropertyChangeMask, False, RecvNotifyEH, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
