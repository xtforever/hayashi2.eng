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
static Window *destination;

static void SendEventCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget text = (Widget)client_data;
    char *message;
    XClientMessageEvent clientmsg;

    /* Retrieve data from text widget */
    message = XmTextFieldGetString(text);
    if (strlen(message) >= 20) /* up to 20 bytes can be sent */
        message[19] = NULL;

    /* Create client message event structure */
    clientmsg.type = ClientMessage;
    clientmsg.display = XtDisplay(w);
    clientmsg.window = *destination;      /* destination window ID */
    clientmsg.message_type = xa_my_text;  /* message type is _MY_TEXT */
    clientmsg.format = 8;                 /* format of string is 8 */
    strcpy(clientmsg.data.b, message);    /* copy the data */
    /* send the client message event */
    XSendEvent(XtDisplay(w), *destination, False, NoEventMask, &clientmsg);

    XtFree(message);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, text, button;
    Arg al[20];
    int ac;
    Atom atom, type;
    int format, stat;
    unsigned long nitems, left;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Sendclmsg", NULL, 0,
                               &argc, argv, NULL, al, ac);
    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    text = XmCreateTextField(panel, "text", al, ac);
    XtManageChild(text);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtAddCallback(button, XmNactivateCallback, SendEventCB, text);
    XtManageChild(button);

    /* Create atom for property name of client message */
    xa_my_text = XmInternAtom(XtDisplay(button), "_MY_TEXT", False);

    /* Create atom for window ID */
    atom = XmInternAtom(XtDisplay(button), "_LABEL_WINDOW", False);

    /* Retrieve window ID from "_LABEL_WINDOW" of root window */
    if (XGetWindowProperty(XtDisplay(button),
                  RootWindowOfScreen(XtScreen(button)), atom, 0, 1, False,
                  XA_WINDOW, &type, &format, &nitems, &left, &destination)
                                            != Success || type != XA_WINDOW)
        fprintf(stderr, "Cannot get window ID from _LABEL_WINDOW property\n");

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
