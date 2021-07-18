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

main(argc, argv)
    int  argc;
    char **argv;
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

    /* Text input area */
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

/* [Copy] button callback */
static void CopyCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget text = (Widget)client_data;
    char *string;
    XmString label;
    long item_id;
    static int private_id = 0;
    long data_id;

    /* retrieve content of text widget */
    string = XmTextGetString(text);
    if (string == NULL || *string == NULL) {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* Create a compound string with any name */
    label = XmStringCreateLocalized("_my_text");

    /* Start copy to clipboard */
    if (XmClipboardStartCopy(XtDisplay(text), XtWindow(text), label,
                             XtLastTimestampProcessed(XtDisplay(text)),
                             NULL, NULL, &item_id) != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(string);
        return;
    }
    /* copy data to clipboard */
    if (XmClipboardCopy(XtDisplay(text), XtWindow(text), item_id,
                        "STRING", string, strlen(string),
                        private_id, &data_id) != ClipboardSuccess) {
        XmClipboardCancelCopy(XtDisplay(text), XtWindow(text), item_id);
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(string);
        return;
    }
    /* Finish the copy to the clipboard (the data is actually sent here) */
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

/* [Paste] button callback */
static void PasteCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget text = (Widget)client_data;
    unsigned long data_len;
    char *buff;
    unsigned long num_bytes;
    long private_id;

    /* (1) Inquire data length */
    if (XmClipboardInquireLength(XtDisplay(text), XtWindow(text), "STRING",
                            &data_len) != ClipboardSuccess || data_len == 0) {
        XBell(XtDisplay(text), 0);
        return;
    }
    /* (2) Prepare buffer for retrieved data */
    if ((buff = malloc(data_len + 1)) == NULL) {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* (3) Retrieve data from clipboard */
    if (XmClipboardRetrieve(XtDisplay(text), XtWindow(text),
                         "STRING", buff, data_len, &num_bytes,
                         &private_id) != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(text), 0);
        free(buff);
        return;
    }
    buff[num_bytes] = '\0';  /* append NULL */

    /* Paste the clipboard data into text widget */
    XmTextSetString(text, buff);

    /* (4) free the prepared buffer area */
    free(buff);
}
