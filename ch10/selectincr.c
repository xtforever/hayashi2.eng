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
static Boolean ConvertIncProc();
static void LoseIncProc();
static void DoneIncProc();
static void CancelProc();
static void GetValueProc();

#define MAX_LEN 1024
static Widget text;
static char *string;

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
    toplevel = XtAppInitialize(&app_context, "Selectincr", NULL, 0,
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
    /* [Copy] button --> Owner: Own the selection */
    ac = 0;
    copy = XmCreatePushButton(control, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, NULL);
    XtManageChild(copy);
    /* [Paste] button --> Requestor: request selection data */
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

/* (7) Owner: [Copy] button callback */
static void CopyCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    /* (8) Own the selection (register callbacks) */
    if (XtOwnSelectionIncremental(text, XA_PRIMARY,
                   XtLastTimestampProcessed(XtDisplay(text)),
                   ConvertIncProc, LoseIncProc, DoneIncProc, CancelProc, NULL))
        /* (9) If it can own the selection, invert the text */
        XmTextSetHighlight(text, 0, XmTextGetLastPosition(text),
                           XmHIGHLIGHT_SELECTED);
}

/* Owner: Called when ownership is lost */
static void LoseIncProc(w, selection, client_data)
    Widget w;
    Atom *selection;
    XtPointer client_data;
{
    /* (10) If ownership is lost, reset the inversion */
    if (*selection == XA_PRIMARY)
        XmTextSetHighlight(w, 0, XmTextGetLastPosition(w), XmHIGHLIGHT_NORMAL);
}

/* (11) Requestor: [Paste] button callback */
static void PasteCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    /* (12) Request selection data (the data will be received in callback) */
    XtGetSelectionValueIncremental(text, XA_PRIMARY, XA_STRING, GetValueProc,
                              NULL, XtLastTimestampProcessed(XtDisplay(text)));
}

/* (13) Owner: Called when selection data is requested */
static Boolean ConvertIncProc(w, selection, target,
                              type_ret, val_ret, len_ret, format_ret,
                              max_len, client_data, request_id)
    Widget w;
    Atom *selection;
    Atom *target;
    Atom *type_ret;
    XtPointer *val_ret;
    unsigned long *len_ret;
    int *format_ret;
    unsigned long *max_len;
    XtPointer client_data;
    XtRequestId *request_id;
{
    static char *p;
    static Boolean start = True;
    static XtRequestId id;
    unsigned long size;

    if (*selection != XA_PRIMARY || *target != XA_STRING)
        return False;  /* the type does not match with the requested selection */

    /* (14) Start incremental transfer */
    if (start) {
        p = string = XmTextGetString(w);  /* retrieve text */
        id = *request_id;  /* remember request ID */
        start = False;
    }

    /* (15) ignore other request during the incremental transfer */
    if (id != *request_id)
        return False;

    /* (16) return a fragment */
    size = strlen(p);  /* remaining size */
    if (size > MAX_LEN) size = MAX_LEN;    /* restrict the size of one transfer */
    if (size > *max_len) size = *max_len;  /* restrict the size less than limit */
    *val_ret = p;           /* pointer of this transfer */
    *len_ret = size;        /* size of this transfer (size = 0 if end of transfer) */
    *type_ret = XA_STRING;  /* this example handles only English text */
    *format_ret = 8;        /* format of string is  8 */

    if (size == 0)  /* (17) there is no remaining data (transfer is completed) */
        start = True;
    else
        p += size;  /* (18) progress the pointer to next transfer */

    return True;   /* notify success of preparing data */
}

/* (19) Requestor: Called when selection data is received */
static void GetValueProc(w, client_data, selection, type, val, len, format)
    Widget w;
    XtPointer client_data;
    Atom *selection;
    Atom *type;
    XtPointer val;
    unsigned long *len;
    int *format;
{
    static Boolean start = True;

    if (*selection != XA_PRIMARY || *type != XA_STRING || val == NULL) {
        XBell(XtDisplay(w), 0);  /* notify fail by bell */
        return;
    }

    /* (20) finish of incremental transfer */
    if (*len == 0) {
        start = True;
        return;
    }

    /* (21) clear text content at first */
    if (start) {
        XmTextSetString(w, "");
        start = False;
    }

    /* (22) append the received data in text widget */
    XmTextInsert(text, XmTextGetLastPosition(text), (char *)val);
    XtFree((char *)val);
}

/* (23) Owner: Called after requestor received the selection data */
static void DoneIncProc(w, selection, target, request_id, client_data)
    Widget w;
    Atom *selection;
    Atom *target;
    XtRequestId *request_id;
    XtPointer client_data;
{
    /* (24) Free the transferred data */
    XtFree(string);
}

/* (25) Owner: Called when the transfer is cancelled */
static void CancelProc(w, selection, target, request_id, client_data)
    Widget w;
    Atom *selection;
    Atom *target;
    XtRequestId *request_id;
    XtPointer client_data;
{
    /* (26) Free the transferred data */
    XtFree(string);
}
