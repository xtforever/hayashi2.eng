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

static void CopyCB();
static void PasteCB();

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text, control, copy, paste;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Clipboard", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (1) Text input area */
    ac = 0;
    text = XmCreateText(panel, "text", al, ac);
    XtManageChild(text);

    /* Command area */
    ac = 0;
    control = XmCreateRowColumn(panel, "control", al, ac);
    XtManageChild(control);

    /* [Copy] --> copy data to clipboard */
    ac = 0;
    copy = XmCreatePushButton(control, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, text);
    XtManageChild(copy);

    /* [Paste] --> retrieve data from clipboard */
    ac = 0;
    paste = XmCreatePushButton(control, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, text);
    XtManageChild(paste);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* (2) [Copy] button callback */
static void 
CopyCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    char *string;
    XmString label;
    long item_id;
    static int private_id = 0;
    long data_id;

    /* (3) retrieve content of text widget */
    string = XmTextGetString(text);
    if (string == NULL || *string == 0 ) {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* Create a compound string with any name */
    label = XmStringCreateLocalized("_my_text");

    /* (4) Start copy to clipboard */
    if (XmClipboardStartCopy(XtDisplay(text), XtWindow(text), label,
                             XtLastTimestampProcessed(XtDisplay(text)),
                             NULL, NULL, &item_id) != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(string);
        return;
    }
    /* (5) copy data to clipboard */
    if (XmClipboardCopy(XtDisplay(text), XtWindow(text), item_id,
                        "STRING", string, strlen(string),
                        private_id, &data_id) != ClipboardSuccess) {
        XmClipboardCancelCopy(XtDisplay(text), XtWindow(text), item_id);
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(string);
        return;
    }
    /* (6) Finish the copy to the clipboard (the data is actually sent here) */
    if (XmClipboardEndCopy(XtDisplay(text), XtWindow(text), item_id)
                                                  != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(string);
        return;
    }
    private_id++;
    XmStringFree(label);
    XtFree(string);
}

/* (7) [Paste] button callback */
static void 
PasteCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    char buff[BUFSIZ];
    unsigned long num_bytes;
    long private_id;

    /* (8) Retrieve data from clipboard */
    if (XmClipboardRetrieve(XtDisplay(text), XtWindow(text),
                         "STRING", buff, sizeof(buff), &num_bytes,
                         &private_id) != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(text), 0);
        return;
    }
    buff[num_bytes] = '\0';  /* append NULL */

    /* (9) Paste the data into text widget */
    XmTextSetString(text, buff);
}
