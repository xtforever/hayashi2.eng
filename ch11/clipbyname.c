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

static void OpenCB();
static void CopyCB();
static void PasteCB();
static void CopyByName();

static Widget text;

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, field, label, fname, open, control, copy, paste;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Clipbyname", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (1) Input file name and open the file */
    ac = 0;
    field = XmCreateRowColumn(panel, "field", al, ac);
    XtManageChild(field);
    /* "File Name: */
    ac = 0;
    label = XmCreateLabel(field, "label", al, ac);
    XtManageChild(label);
    /* input area */
    ac = 0;
    fname = XmCreateTextField(field, "fname", al, ac);
    XtManageChild(fname);
    /* [Open] */
    ac = 0;
    open = XmCreatePushButton(field, "open", al, ac);
    XtAddCallback(open, XmNactivateCallback, OpenCB, fname);
    XtManageChild(open);

    /* (2) text area to show the content of the file */
    ac = 0;
    text = XmCreateScrolledText(panel, "text", al, ac);
    XtManageChild(text);

    /* (3) command area */
    ac = 0;
    control = XmCreateRowColumn(panel, "control", al, ac);
    XtManageChild(control);
    /* [Copy] --> Copy data to clipboard */
    ac = 0;
    copy = XmCreatePushButton(control, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, NULL);
    XtManageChild(copy);
    /* [Paste] --> Retrieve data from clipboard */
    ac = 0;
    paste = XmCreatePushButton(control, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, NULL);
    XtManageChild(paste);

    XtVaSetValues(XtParent(text), XmNtopWidget, field,
                                  XmNbottomWidget, control, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* (4) [Open] button callback */
static void OpenCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget textf = (Widget)client_data;
    char *filename;
    FILE *fp;
    char buff[BUFSIZ];

    /* (5) Retrieve the entered file name */
    XtVaGetValues(textf, XmNvalue, &filename, NULL);

    /* (6) Show the content of the file on the text widget */
    if ((fp = fopen(filename, "r")) != NULL) {  /* open the file */
        XmTextSetString(text, "");  /* once clear the text */
        while (fgets(buff, BUFSIZ, fp) != NULL)
            XmTextInsert(text, XmTextGetLastPosition(text), buff); /* append */
        fclose(fp);
    }

    XtFree(filename);
}

/* (7) [Copy] button callback */
static void CopyCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XmString label;
    long item_id;
    static int private_id = 0;
    long data_id;

    /* Create a compound string with any name */
    label = XmStringCreateLocalized("_my_text");

    /* (8) Start copy by name */
    if (XmClipboardStartCopy(XtDisplay(text), XtWindow(text), label,
                             XtLastTimestampProcessed(XtDisplay(text)),
                             text, CopyByName, &item_id) != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        return;
    }
    /* (9) Copy format name */
    if (XmClipboardCopy(XtDisplay(text), XtWindow(text), item_id, "STRING",
                        NULL, XmTextGetLastPosition(text) * 2,
                        private_id, &data_id) != ClipboardSuccess) {
        XmClipboardCancelCopy(XtDisplay(text), XtWindow(text), item_id);
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        return;
    }
    /* (10) Finish copy by name */
    if (XmClipboardEndCopy(XtDisplay(text), XtWindow(text), item_id)
                                                       != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        XmStringFree(label);
        return;
    }
    private_id++;
    XmStringFree(label);
}

/* (11) Called when clipbpard data is requested */
static void CopyByName(w, data_id, private_id, reason)
    Widget w;
    int *data_id;
    int *private_id;
    int *reason;
{
    char *string;

    if (*reason == XmCR_CLIPBOARD_DATA_REQUEST) {
        /* (12) retrieve contents of text widget */
        string = XmTextGetString(text);
        /* (13) Copy actual data */
        if (XmClipboardCopyByName(XtDisplay(text), XtWindow(text), *data_id,
                    string, strlen(string), *private_id) != ClipboardSuccess) {
            XBell(XtDisplay(text), 0);
            XtFree(string);
            return;
        }
        XtFree(string);
    }
    else { /* *reason == XmCR_CLIPBOARD_DATA_DELETE */
        /* (14) Do something when deletion is requested */
    }
}

/* (15) [Paste] button callback */
static void PasteCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    char buff[BUFSIZ];
    unsigned long num_bytes;
    long private_id;
    int status;

    /* (16) Start retrieve */
    if (XmClipboardStartRetrieve(XtDisplay(text), XtWindow(text),
              XtLastTimestampProcessed(XtDisplay(text))) != ClipboardSuccess) {
        XBell(XtDisplay(text), 0);
        return;
    }

    /* (17) Clear text */
    XmTextSetString(text, "");

    /* (18) Retrieve data in fixed size */
    do {
        status = XmClipboardRetrieve(XtDisplay(text), XtWindow(text),
                       "STRING", buff, sizeof(buff), &num_bytes, &private_id);
        /* (19) Append the data in text widget */
        if (status == ClipboardSuccess || status == ClipboardTruncate) {
            buff[num_bytes] = '\0';  /* append NULL */
            XmTextInsert(text, XmTextGetLastPosition(text), buff);
        }
    } while (status == ClipboardTruncate);

    if (status != ClipboardSuccess)
        XBell(XtDisplay(text), 0); /* bell on fail */

    /* (20) Finish the retrieve */
    XmClipboardEndRetrieve(XtDisplay(text), XtWindow(text));
}
