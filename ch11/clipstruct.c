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

#define _FIG_AND_TITLE   "_FIG_AND_TITLE"

/* (1) Define data structure for transfer */
typedef struct {
    char figure[24];
    char title[100];
} FigAndTitle;

static void CopyCB();
static void PasteCB();
static Widget box, figure, title;
static Pixmap pixmap;

int 
main (int argc, char **argv)
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

    /* Box which contains figure and Label */
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

    /* [Copy] --> copy data to clipboard */
    ac = 0;
    copy = XmCreatePushButton(panel, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, NULL);
    XtManageChild(copy);

    /* [Paste] --> retrieve data from clipboard */
    ac = 0;
    paste = XmCreatePushButton(panel, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, NULL);
    XtManageChild(paste);

    /* (2) Register new format */
    if (XmClipboardRegisterFormat(XtDisplay(toplevel), _FIG_AND_TITLE, 8)
                                                        != ClipboardSuccess) {
        fprintf(stderr, "Cannot register format %s\n", _FIG_AND_TITLE);
        exit(1);
    }

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* [Copy] button callback */
static void 
CopyCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    FigAndTitle fig_title;
    XmString label;
    long item_id;
    static int private_id = 0;
    long data_id;
    XmString string;
    char *text;

    /* retrieve pixmap from figure */
    XtVaGetValues(figure, XmNlabelPixmap, &pixmap, NULL);

    /* retrieve label string from the title */
    XtVaGetValues(title, XmNlabelString, &string, NULL);
    XmStringGetLtoR(string, XmFONTLIST_DEFAULT_TAG, &text);
    XmStringFree(string);

    if (pixmap == XmUNSPECIFIED_PIXMAP || text == NULL) {
        XBell(XtDisplay(w), 0);
        return;
    }

    /* (3) Prepare data (convert Pixmap ID to hexa string) */
    sprintf(fig_title.figure, "%x", pixmap);
    strcpy(fig_title.title, text);
    XtFree(text);

    /* Create a compound string with any name */
    label = XmStringCreateLocalized("_my_data");

    /* Start copy to clipboard */
    if (XmClipboardStartCopy(XtDisplay(box), XtWindow(box), label,
                             XtLastTimestampProcessed(XtDisplay(box)),
                             NULL, NULL, &item_id) != ClipboardSuccess) {
        XBell(XtDisplay(w), 0);
        XmStringFree(label);
        return;
    }
    /* (4) Copy pixmap and title to clipboard */
    if (XmClipboardCopy(XtDisplay(box), XtWindow(box), item_id,
                        _FIG_AND_TITLE, &fig_title, sizeof(fig_title),
                        private_id, &data_id) != ClipboardSuccess) {
//        XmClipboardCancelCopy(XtDisplay(text), XtWindow(text), item_id);
        XmClipboardCancelCopy(XtDisplay(box), XtWindow(box), item_id);
        XBell(XtDisplay(w), 0);
        XmStringFree(label);
        return;
    }
    /* Finish the copy to the clipboard (the data is actually sent here) */
    if (XmClipboardEndCopy(XtDisplay(box), XtWindow(box), item_id)
                                                         != ClipboardSuccess) {
        XBell(XtDisplay(w), 0);
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
    FigAndTitle fig_title;
    unsigned long num_bytes;
    long private_id;
    XmString string;

    /* (5) Retrieve pixmap and title from clipboard */
    if (XmClipboardRetrieve(XtDisplay(box), XtWindow(box),
                   _FIG_AND_TITLE, &fig_title, sizeof(fig_title), &num_bytes,
                   &private_id) != ClipboardSuccess || num_bytes == 0) {
        XBell(XtDisplay(w), 0);
        return;
    }

    /* Show the pixmap on figure */
    sscanf(fig_title.figure, "%x", &pixmap);
    XtVaSetValues(figure, XmNlabelPixmap, pixmap, NULL);

    /* Show the string on title */
    string = XmStringCreateLocalized(fig_title.title);
    XtVaSetValues(title, XmNlabelString, string, NULL);
    XmStringFree(string);
}
