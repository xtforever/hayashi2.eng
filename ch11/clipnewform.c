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
#include <time.h>
#include <Xm/XmAll.h>

#define _MY_TIME    "_MY_TIME"

static void CopyCB();
static void PasteCB();

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, clock, control, copy, paste;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Clipnewform", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Label for time */
    ac = 0;
    clock = XmCreateLabel(panel, "clock", al, ac);
    XtManageChild(clock);

    /* Command area */
    ac = 0;
    control = XmCreateRowColumn(panel, "control", al, ac);
    XtManageChild(control);

    /* [Copy] --> copy data to clipboard */
    ac = 0;
    copy = XmCreatePushButton(control, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, clock);
    XtManageChild(copy);

    /* [Paste] --> retrieve data from clipboard */
    ac = 0;
    paste = XmCreatePushButton(control, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, clock);
    XtManageChild(paste);

    /* (1) Register new format */
    if (XmClipboardRegisterFormat(XtDisplay(toplevel), _MY_TIME, 32)
                                                    != ClipboardSuccess) {
        fprintf(stderr, "Cannot register format %s\n", _MY_TIME);
        exit(1);
    }

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* [Copy] button callback */
static void 
CopyCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget clock = (Widget)client_data;
    XmString label;
    long item_id;
    static int private_id = 0;
    long data_id;
    time_t current;
    char *time_str;
    XmString string;

    /* Get current time */
    current = time(NULL);

    /* Convert the time to string and show it on label */
    time_str = ctime(&current);
    if (time_str[strlen(time_str) - 1] == '\n')
        time_str[strlen(time_str) - 1] = '\0';
    string = XmStringCreateLocalized(time_str);
    XtVaSetValues(clock, XmNlabelString, string, NULL);
    XmStringFree(string);

    /* Create a compound string with any name */
    label = XmStringCreateLocalized("_my_clock");

    /* Start copy to clipboard */
    if (XmClipboardStartCopy(XtDisplay(clock), XtWindow(clock), label,
                             XtLastTimestampProcessed(XtDisplay(clock)),
                             NULL, NULL, &item_id) != ClipboardSuccess) {
        XBell(XtDisplay(clock), 0);
        XmStringFree(label);
        return;
    }
    /* (2) Copy data with new format  */
    if (XmClipboardCopy(XtDisplay(clock), XtWindow(clock), item_id,
                        _MY_TIME, &current, sizeof(current),
                        private_id, &data_id) != ClipboardSuccess) {
        XmClipboardCancelCopy(XtDisplay(clock), XtWindow(clock), item_id);
        XBell(XtDisplay(clock), 0);
        XmStringFree(label);
        return;
    }
    /* Finish the copy to the clipboard (the data is actually sent here) */
    if (XmClipboardEndCopy(XtDisplay(clock), XtWindow(clock), item_id)
                                                  != ClipboardSuccess) {
        XBell(XtDisplay(clock), 0);
        XmStringFree(label);
        return;
    }
    private_id++;
    XmStringFree(label);
}

/* [Paste] button callback */
static void 
PasteCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget clock = (Widget)client_data;
    char buff[BUFSIZ];
    unsigned long num_bytes;
    long private_id;
    time_t copied_time;
    char *time_str;
    XmString string;

    /* (3) Retrieve data from clipboard in new format */
    if (XmClipboardRetrieve(XtDisplay(clock), XtWindow(clock),
                      _MY_TIME, &copied_time, sizeof(copied_time), &num_bytes,
                      &private_id) != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(clock), 0);
        return;
    }

    /* Convert the retrieved value to string and show it on label */
    time_str = ctime(&copied_time);
    if (time_str[strlen(time_str) - 1] == '\n')
        time_str[strlen(time_str) - 1] = '\0';
    string = XmStringCreateLocalized(time_str);
    XtVaSetValues(clock, XmNlabelString, string, NULL);
    XmStringFree(string);
}
