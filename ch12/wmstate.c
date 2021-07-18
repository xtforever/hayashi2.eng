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

#define INTERVAL  (3 * 1000)

typedef struct _WmStateHint {
    unsigned long state;
    Window        icon;
} WmStateHint;

static void WithdrawCB(), Withdraw2Icon(), Icon2Normal();
static void IconifyCB(), Icon2Withdraw(), Withdraw2Normal();
static void MappedEH(), StateEH();

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, withdraw, iconify;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Wmstate", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (1) Event handler to wait until the top level window is mapped */
    XtAddEventHandler(toplevel, StructureNotifyMask, False, MappedEH, NULL);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (2) [Withdrawn -> Iconic -> Normal] button */
    ac = 0;
    withdraw = XmCreatePushButton(panel, "withdraw", al, ac);
    XtManageChild(withdraw);
    XtAddCallback(withdraw, XmNactivateCallback, WithdrawCB, toplevel);

    /* (3) [Iconic -> Withdrawn -> Normal] button */
    ac = 0;
    iconify = XmCreatePushButton(panel, "iconify", al, ac);
    XtManageChild(iconify);
    XtAddCallback(iconify, XmNactivateCallback, IconifyCB, toplevel);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* (4) [Withdrawn -> Iconic -> Normal] button callback */
static void 
WithdrawCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget toplevel = (Widget)client_data;
    XtIntervalId timeout_id;

    /* (5) Unmap to become Withdrawn state */
    XtUnmapWidget(toplevel);

    /* (6) call Withdraw2Icon() 3 seconds later */
    timeout_id = XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
                                 INTERVAL, Withdraw2Icon, toplevel);
}

/* (7) Called 3 seconds after WithdrawCB() */
static void 
Withdraw2Icon (XtPointer client_data, XtIntervalId *id)
{
    Widget toplevel = (Widget)client_data;
    XtIntervalId timeout_id;

    /* (8) Change from Withdrawn state to Icon state */
    XtVaSetValues(toplevel, XmNinitialState, IconicState, NULL);
    XtMapWidget(toplevel);

    /* (9) Call Icon2Normal() 3 seconds later */
    timeout_id = XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
                                 INTERVAL, Icon2Normal, toplevel);
}

/* (10) called 3 seconds after Withdraw2Icon() */
static void 
Icon2Normal (XtPointer client_data, XtIntervalId *id)
{
    Widget toplevel = (Widget)client_data;

    /* (11) Normalize from icon state */
    XtMapWidget(toplevel);
}

/* (12) [Iconic -> Withdrawn -> Normal] button callback */
static void 
IconifyCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget toplevel = (Widget)client_data;
    XtIntervalId timeout_id;

    /* (13) Iconify */
    XIconifyWindow(XtDisplay(toplevel), XtWindow(toplevel),
                   XScreenNumberOfScreen(XtScreen(toplevel)));

    /* (14) call Icon2Withdraw() 3 seconds later */
    timeout_id = XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
                                 INTERVAL, Icon2Withdraw, toplevel);
}

/* (15) called 3 seconds after IconifyCB() */
static void 
Icon2Withdraw (XtPointer client_data, XtIntervalId *id)
{
    Widget toplevel = (Widget)client_data;
    XtIntervalId timeout_id;
    XUnmapEvent unmap;

    /* (16) Change from icon state to Withdrawn state */
    /* prepare Unmap event structure */
    unmap.type = UnmapNotify;
    unmap.display = XtDisplay(toplevel);
    unmap.event = RootWindowOfScreen(XtScreen(toplevel));
    unmap.window = XtWindow(toplevel);
    unmap.from_configure = False;
    /* send the event to root window */
    XSendEvent(XtDisplay(toplevel), RootWindowOfScreen(XtScreen(toplevel)),
	       False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *)&unmap);

    /* (17) call Withdraw2Normal() 3 seconds later */
    timeout_id = XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
                                 INTERVAL, Withdraw2Normal, toplevel);
}

/* (18) called 3 seconds after Icon2Withdraw() */
static void 
Withdraw2Normal (XtPointer client_data, XtIntervalId *id)
{
    Widget toplevel = (Widget)client_data;

    /* (19) back to normal state from Withdraw state */
    XtVaSetValues(toplevel, XmNinitialState, NormalState, NULL);
    XtMapWidget(toplevel);
}

/* (20) Called at first time the top level window is mapped */
static void 
MappedEH (Widget w, XtPointer client_data, XEvent *event, Boolean *dispatch)
{
    if (event->type == MapNotify) {
        /* (21) To know the property change of top level window */
        XtAddEventHandler(w, PropertyChangeMask, False, StateEH, NULL);
        XtRemoveEventHandler(w, StructureNotifyMask, False, MappedEH, NULL);
    }
}

/* (22) Called when propert of toplevel window is changed */
static void 
StateEH (Widget w, XtPointer client_data, XEvent *event, Boolean *dispatch)
{
    Atom xa_wm_state, actual_type;
    int actual_format;
    unsigned long nitems, left;
    WmStateHint *state_hint;

    /* (23) Create atom of "WM_STATE" */
    xa_wm_state = XmInternAtom(XtDisplay(w), "WM_STATE", False);

    /* (24) Read "WM_STATE" property of top level window */
    if (XGetWindowProperty(XtDisplay(w), XtWindow(w),
                           xa_wm_state, 0, 2, False, xa_wm_state,
                           &actual_type, &actual_format, &nitems, &left,
                           (unsigned char **)&state_hint)
                == Success && actual_type == xa_wm_state && nitems == 2) {

        /* (25) determine window state from "WM_STATE" */
        switch (state_hint->state) {
        case WithdrawnState:
                printf("Withdrawn State\n");
                break;
        case NormalState:
                printf("Normal State\n");
                break;
        case IconicState:
                printf("Iconic State\n");
                break;
        default:
                printf("Unknown State\n");
                break;
        }
    }
    else {
        fprintf(stderr, "Couldn't get window state.\n");
    }
}
