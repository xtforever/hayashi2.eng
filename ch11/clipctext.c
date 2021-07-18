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
static void 
CopyCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    char *string;
    XTextProperty textprop;
    XmString label;
    long item_id;
    static int private_id = 0;
    long data_id;

    /* retrieve content of text widget */
    string = XmTextGetString(text);
    if (string == NULL || *string == 0 ) {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* (1) Convert multi byte string to compound text */
    if (XmbTextListToTextProperty(XtDisplay(w), &string, 1, XStdICCTextStyle,
                             &textprop) != Success || textprop.value == NULL) {
        XBell(XtDisplay(w), 0);  /* failed to convert (illegal character may be entered) */
        return;
    }
    XtFree(string);

    /* Create a compound string with any name */
    label = XmStringCreateLocalized("_my_text");

    /* Start copy to clipboard */
    if (XmClipboardStartCopy(XtDisplay(text), XtWindow(text), label,
                             XtLastTimestampProcessed(XtDisplay(text)),
                             NULL, NULL, &item_id) != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(textprop.value);
        return;
    }
    /* (2) copy data to clipboard (format: "COMPOUND_TEXT") */
    if (XmClipboardCopy(XtDisplay(text), XtWindow(text), item_id,
                        "COMPOUND_TEXT", textprop.value, strlen(textprop.value),
                        private_id, &data_id) != ClipboardSuccess) {
        XmClipboardCancelCopy(XtDisplay(text), XtWindow(text), item_id);
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(textprop.value);
        return;
    }
    /* Finish the copy to the clipboard (the data is actually sent here) */
    if (XmClipboardEndCopy(XtDisplay(text), XtWindow(text), item_id)
                                                  != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        XtFree(textprop.value);
        return;
    }
    private_id++;
    XmStringFree(label);
    XtFree(textprop.value);
}

/* [Paste] button callback */
static void 
PasteCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    char buff[BUFSIZ];
    unsigned long num_bytes;
    long private_id;
    XTextProperty textprop;
    char **clist;
    int count;

    /* (3) Retrieve data from clipboard (format: "COMPOUND_TEXT") */
    if (XmClipboardRetrieve(XtDisplay(text), XtWindow(text),
                         "COMPOUND_TEXT", buff, sizeof(buff), &num_bytes,
                         &private_id) != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(text), 0);
        return;
    }
    buff[num_bytes] = '\0';  /* append NULL */

    /* (4) Convert back the compound text to multi byte string */
    textprop.value = buff;
    textprop.encoding = XmInternAtom(XtDisplay(w), "COMPOUND_TEXT", False);
    textprop.format = 8;
    textprop.nitems = num_bytes;
    if (XmbTextPropertyToTextList(XtDisplay(w),
                                  &textprop, &clist, &count) != Success) {
        XBell(XtDisplay(w), 0);  /* notify the fail by bell */
        return;
    }

    /* Paste the clipboard data into text widget */
    XmTextSetString(text, clist[0]);
    XFreeStringList(clist);  /* free unnecessary data */
}
