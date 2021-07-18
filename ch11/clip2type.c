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
static Widget box, figure, title;
static Pixmap pixmap;

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, copy, paste;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Clip2type", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (1) Box which contains figure and Label */
    ac = 0;
    box = XmCreateRowColumn(panel, "box", al, ac);
    XtManageChild(box);
    /* Figure */
    ac = 0;
    figure = XmCreateLabel(box, "figure", al, ac);
    XtManageChild(figure);
    /* Label */
    ac = 0;
    title = XmCreateLabel(box, "title", al, ac);
    XtManageChild(title);

    /* (2) [Copy] --> copy data to clipboard */
    ac = 0;
    copy = XmCreatePushButton(panel, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, NULL);
    XtManageChild(copy);

    /* (3) [Paste] --> retrieve data from clipboard */
    ac = 0;
    paste = XmCreatePushButton(panel, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, NULL);
    XtManageChild(paste);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* (4) [Copy] button callback */
static void CopyCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    XmString label;
    long item_id;
    static int private_id = 0;
    long data_id;
    XmString string;
    char *text;

    /* (5) retrieve pixmap from figure */
    XtVaGetValues(figure, XmNlabelPixmap, &pixmap, NULL);

    /* (6) retrieve label string from the title */
    XtVaGetValues(title, XmNlabelString, &string, NULL);
    XmStringGetLtoR(string, XmFONTLIST_DEFAULT_TAG, &text);
    XmStringFree(string);

    if (pixmap == XmUNSPECIFIED_PIXMAP || text == NULL) {
        XBell(XtDisplay(w), 0);
        return;
    }

    /* Create a compound string with any name */
    label = XmStringCreateLocalized("_my_data");

    /* (7) Start copy to clipboard */
    if (XmClipboardStartCopy(XtDisplay(box), XtWindow(box), label,
                             XtLastTimestampProcessed(XtDisplay(box)),
                             NULL, NULL, &item_id) != ClipboardSuccess) {
        XBell(XtDisplay(w), 0);
        XmStringFree(label);
        XtFree(text);
        return;
    }
    /* (8) Copy Pixmap ID to clipboard */
    if (XmClipboardCopy(XtDisplay(box), XtWindow(box), item_id,
                        "PIXMAP", &pixmap, sizeof(pixmap), private_id,
                        &data_id) != ClipboardSuccess) {
        XmClipboardCancelCopy(XtDisplay(text), XtWindow(text), item_id);
        XBell(XtDisplay(w), 0);
        XmStringFree(label);
        XtFree(text);
        return;
    }
    /* (9) Copy title string to clipboard */
    if (XmClipboardCopy(XtDisplay(box), XtWindow(box), item_id,
                        "STRING", text, strlen(text), private_id,
                        &data_id) != ClipboardSuccess) {
        XmClipboardCancelCopy(XtDisplay(text), XtWindow(text), item_id);
        XBell(XtDisplay(w), 0);
        XmStringFree(label);
        XtFree(text);
        return;
    }
    /* (10) Finish the copy to the clipboard (the data is actually sent here) */
    if (XmClipboardEndCopy(XtDisplay(box), XtWindow(box), item_id)
                                                         != ClipboardSuccess) {
        XBell(XtDisplay(w), 0);
        XmStringFree(label);
        XtFree(text);
        return;
    }
    private_id++;
    XmStringFree(label);
    XtFree(text);
}

/* (11) [Paste] button callback */
static void PasteCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    char buff[BUFSIZ];
    unsigned long num_bytes;
    long private_id;
    XmString string;

    /* (12) Start retrieve */
    if (XmClipboardStartRetrieve(XtDisplay(box), XtWindow(box),
              XtLastTimestampProcessed(XtDisplay(box))) != ClipboardSuccess) {
        XBell(XtDisplay(w), 0);
        return;
    }

    /* (13) Retrieve Pixmap ID */
    if (XmClipboardRetrieve(XtDisplay(box), XtWindow(box),
                         "PIXMAP", &pixmap, sizeof(pixmap), &num_bytes,
                         &private_id) != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(w), 0);
        return;
    }
    /* (14) Show the pixmap on figure */
    XtVaSetValues(figure, XmNlabelPixmap, pixmap, NULL);

    /* (15) Retrieve title string */
    if (XmClipboardRetrieve(XtDisplay(box), XtWindow(box),
                         "STRING", buff, sizeof(buff), &num_bytes,
                         &private_id) != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(w), 0);
        return;
    }
    buff[num_bytes] = '\0';  /* append NULL */

    /* (16) Show the string on title */
    string = XmStringCreateLocalized(buff);
    XtVaSetValues(title, XmNlabelString, string, NULL);
    XmStringFree(string);

    /* (17) Finish the retrieve */
    XmClipboardEndRetrieve(XtDisplay(box), XtWindow(box));
}
