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

static Atom xa_my_text;
static Window window;

/* Called when client message event is received */
static void 
ClientMsgEH (Widget w, XtPointer client_data, XEvent *event, Boolean *dispatch)
{
    XmString string;

    /* Check if the event is client message and the type is _MY_TEXT */
    if (event->type == ClientMessage &&
        event->xclient.message_type == xa_my_text) {
        /* Show the received data on label */
        string = XmStringCreateLocalized(event->xclient.data.b);
        XtVaSetValues(w, XmNlabelString, string, NULL);
        XmStringFree(string);
    }
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, label;
    Arg al[20];
    int ac;
    Atom atom;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Recvclmsg", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XtManageChild(label);
    /* Event handler for client message */
    XtAddEventHandler(label, NoEventMask, True, ClientMsgEH, NULL);

    XtRealizeWidget(toplevel);

    /* Create an atom for property name of the client message */
    xa_my_text = XmInternAtom(XtDisplay(label), "_MY_TEXT", False);

    /* Create atom of property name for window ID */
    atom = XmInternAtom(XtDisplay(label), "_LABEL_WINDOW", False);

    /* Store window ID in "_LABEL_WINDOW" of root window */
    window = XtWindow(label);
    XChangeProperty(XtDisplay(label), RootWindowOfScreen(XtScreen(label)),
                    atom, XA_WINDOW, 32, PropModeReplace, (const unsigned char *)&window, 1);

    XtAppMainLoop(app_context);
}
