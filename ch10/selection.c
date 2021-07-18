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

static void SelectCB();
static void PasteCB();
static Boolean ConvertProc();
static void LoseProc();
static void DoneProc();
static void GetValueProc();

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text, select, paste;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Selection", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (1) text input area */
    ac = 0;
    text = XmCreateText(panel, "text", al, ac);
    XtManageChild(text);

    /* (2) [Select Text] button --> Owner: Own the selection */
    ac = 0;
    select = XmCreatePushButton(panel, "select", al, ac);
    XtAddCallback(select, XmNactivateCallback, SelectCB, text);
    XtManageChild(select);

    /* (3) [Paste] button --> Requestor: request selection data */
    ac = 0;
    paste = XmCreatePushButton(panel, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, text);
    XtManageChild(paste);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* (4) Owner: [Select Text] button callback */
static void 
SelectCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;

    /* (5) Own the selection (register three callbacks) */
    if (XtOwnSelection(text, XA_PRIMARY,
                       XtLastTimestampProcessed(XtDisplay(text)),
                       ConvertProc, LoseProc, DoneProc))
        /* (6) If it can own the selection, invert the text */
        XmTextSetHighlight(text, 0, XmTextGetLastPosition(text),
                           XmHIGHLIGHT_SELECTED);
}

/* (7) Owner: Called when ownership is lost */
static void 
LoseProc (Widget w, Atom *selection)
{
    /* (8) If ownership is lost, reset the inversion */
    if (*selection == XA_PRIMARY)
        XmTextSetHighlight(w, 0, XmTextGetLastPosition(w), XmHIGHLIGHT_NORMAL);
}

/* (9) Requestor: [Paste] button callback */
static void 
PasteCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;

    /* (10) Request selection data (the data will be received in callback) */
    XtGetSelectionValue(text, XA_PRIMARY, XA_STRING, GetValueProc, NULL,
                        XtLastTimestampProcessed(XtDisplay(text)));
}

/* (11) Owner: Called when selection data is requested */
static Boolean 
ConvertProc (Widget w, Atom *selection, Atom *target, Atom *type_ret, XtPointer *val_ret, unsigned long *len_ret, int *format_ret)
{
    char *string;

    /* (12) prepare the transferred data */
    if (*selection == XA_PRIMARY && *target == XA_STRING) {
        string = XmTextGetString(w);   /* send the entered text */
        *val_ret = string;
        *len_ret = strlen(string) + 1; /* append final NULL */
        *type_ret = XA_STRING;         /* this example handles only English text */
        *format_ret = 8;               /* format of string is 8 */
        return True;   /* notify success of preparing data */
    }
    else
        return False;  /* the type does not match with the requested selection */
}

/* (13) Requestor: Called when selection data is received */
static void 
GetValueProc (Widget w, XtPointer client_data, Atom *selection, Atom *type, XtPointer val, unsigned long *len, int *format)
{
    if (*len == 0 || val == NULL) { /* Failed to receive selection data */
        XBell(XtDisplay(w), 0);  /* notify the fail by bell */
        return;
    }

    /* (14) Paste the received text in the text widget */
    if (*selection == XA_PRIMARY && *type == XA_STRING) {
        XmTextSetString(w, (char *)val);
        XtFree((char *)val);
    }
    else
        XBell(XtDisplay(w), 0);  /* Notify by bell if the type is illegal */
}

/* (15) Owner: Called after requestor received the selection data */
static void 
DoneProc (Widget w, Atom *selection, Atom *target)
{
    /* Do the necessary thing after transferring the data. */
    /* If this callback is registered, application must free the selection data. */
    /* Otherwise, Xt free the data. */
}
