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
    if (string == NULL || *string == 0) {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* Convert multi byte string to compound text */
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
    /* copy data to clipboard */
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
    int count, index;
    unsigned long format_len, copied_len, data_len;
    char *format_name;
    Boolean ctext_found = False, string_found = False;
    char *buff;
    unsigned long num_bytes;
    long private_id;
    XTextProperty textprop;
    char **clist;

    /* (1) Inquire number of formats in clipboard */
    if (XmClipboardInquireCount(XtDisplay(text), XtWindow(text), &count,
                              &format_len) != ClipboardSuccess || count == 0) {
        XBell(XtDisplay(text), 0);
        return;
    }
    /* (2) Prepare buffer for format name */
    if ((format_name = malloc(format_len + 1)) == NULL) {
        XBell(XtDisplay(text), 0);
        return;
    }
    /* (3) Inquire format name in clipboard */
    for (index = 1; index < count + 1; index++) {
        if (XmClipboardInquireFormat(XtDisplay(text), XtWindow(text),
                              index, format_name, format_len, &copied_len)
                                     != ClipboardSuccess || copied_len == 0)
            continue;
        format_name[copied_len] = '\0';  /* append NULL */
        /* (4) check if the format is of text data */
        if (strcmp(format_name, "COMPOUND_TEXT") == 0)
            ctext_found = True;
        if (strcmp(format_name, "STRING") == 0)
            string_found = True;
    }
    free(format_name);

    /* (5) select format name */
    if (ctext_found)
        format_name = "COMPOUND_TEXT";
    else if (string_found)
        format_name = "STRING";
    else {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* (6) inquire data length */
    if (XmClipboardInquireLength(XtDisplay(text), XtWindow(text), format_name,
                            &data_len) != ClipboardSuccess || data_len == 0) {
        XBell(XtDisplay(text), 0);
        return;
    }
    /* (7) prepare buffer to store the retrieved data */
    if ((buff = malloc(data_len + 1)) == NULL) {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* (8) retrieve data from clipboard */
    if (XmClipboardRetrieve(XtDisplay(text), XtWindow(text), format_name,
                            buff, data_len, &num_bytes, &private_id)
                                      != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(text), 0);
        free(buff);
        return;
    }
    buff[num_bytes] = '\0';  /* append NULL */

    if (ctext_found) {
        /* (9) Convert back the compound text to multi byte string */
        textprop.value = buff;
        textprop.encoding = XmInternAtom(XtDisplay(w), "COMPOUND_TEXT", False);
        textprop.format = 8;
        textprop.nitems = num_bytes;
        if (XmbTextPropertyToTextList(XtDisplay(w),
                                      &textprop, &clist, &count) != Success) {
            XBell(XtDisplay(w), 0);  /* notify the fail by bell */
            free(buff);
            return;
        }
        /* Paste the converted data into text widget */
        XmTextSetString(text, clist[0]);
        XFreeStringList(clist);  /* free unnecessary data */
    }
    else if (string_found) {
        /* (10) Paste the clipboard data into text widget */
        XmTextSetString(text, buff);
    }

    free(buff);
}
