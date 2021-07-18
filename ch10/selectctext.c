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

static Atom xa_ctext;

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

    /* Text input area */
    ac = 0;
    text = XmCreateText(panel, "text", al, ac);
    XtManageChild(text);

    /* [Select Text] button --> Owner: Own the selection */
    ac = 0;
    select = XmCreatePushButton(panel, "select", al, ac);
    XtAddCallback(select, XmNactivateCallback, SelectCB, text);
    XtManageChild(select);

    /* [Paste] button --> Requestor: request selection data */
    ac = 0;
    paste = XmCreatePushButton(panel, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, text);
    XtManageChild(paste);

    /* Create atom for COMPOUND_TEXT */
    xa_ctext = XmInternAtom(XtDisplay(toplevel), "COMPOUND_TEXT", False);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* Owner: [Select Text] button callback */
static void 
SelectCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;

    /* Own the selection (register three callbacks) */
    if (XtOwnSelection(text, XA_PRIMARY,
                       XtLastTimestampProcessed(XtDisplay(text)),
                       ConvertProc, LoseProc, DoneProc))
        /* If it can own the selection, invert the figure and label */
        XmTextSetHighlight(text, 0, XmTextGetLastPosition(text),
                           XmHIGHLIGHT_SELECTED);
}

/* Owner: Called when ownership is lost */
static void 
LoseProc (Widget w, Atom *selection)
{
    /* If ownership is lost, reset the inversion */
    if (*selection == XA_PRIMARY)
        XmTextSetHighlight(w, 0, XmTextGetLastPosition(w), XmHIGHLIGHT_NORMAL);
}

/* Requestor: [Paste] button callback */
static void 
PasteCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;

    /* (1) Request selection data in compound text format */
    XtGetSelectionValue(text, XA_PRIMARY, xa_ctext, GetValueProc, NULL,
                        XtLastTimestampProcessed(XtDisplay(text)));
}

/* Owner: Called when selection data is requested */
static Boolean 
ConvertProc (Widget w, Atom *selection, Atom *target, Atom *type_ret, XtPointer *val_ret, unsigned long *len_ret, int *format_ret)
{
    char *string;
    XTextProperty textprop;

    /* return requested target type */
    if (*selection == XA_PRIMARY && *target == xa_ctext) {
        string = XmTextGetString(w);
        /* (2) Convert multi byte string to compound text */
        if (XmbTextListToTextProperty(XtDisplay(w), &string, 1,
                                      XStdICCTextStyle, &textprop) != Success)
            return False;  /* failed to convert (illegal character may be entered) */
        XtFree(string);
        *val_ret = textprop.value;    /* send the compound text */
        *len_ret = strlen(textprop.value);
        *type_ret = xa_ctext;         /* atom for compound text */
        *format_ret = 8;              /* format of string is 8 */
        return True;   /* Success to prepare the data */
    }
    else
        return False;  /* the type does not match with the requested selection */
}

/* Requestor: Called when selection data is received */
static void 
GetValueProc (Widget w, XtPointer client_data, Atom *selection, Atom *type, XtPointer val, unsigned long *len, int *format)
{
    XTextProperty textprop;
    char **clist;
    int count;

    if (*len == 0 || val == NULL) { /* Failed to receive selection data */
        XBell(XtDisplay(w), 0);  /* notify the fail by bell */
        return;
    }

    if (*selection == XA_PRIMARY && *type == xa_ctext) {
        /* (3) Convert back the compound text to multi byte string */
        textprop.value = val;
        textprop.encoding = *type;
        textprop.format = *format;
        textprop.nitems = *len;
        if (XmbTextPropertyToTextList(XtDisplay(w),
                                      &textprop, &clist, &count) != Success)
            XBell(XtDisplay(w), 0);  /* notify the fail by bell */

        /* Paste the received text in the text widget */
        XmTextSetString(w, clist[0]);
        XFreeStringList(clist);  /* free unnecessary text */
	XtFree((char *)val);
    }
    else
        XBell(XtDisplay(w), 0);  /* Notify by bell if the type is illegal */
}

/* Owner: Called after requestor received the selection data */
static void 
DoneProc (Widget w, Atom *selection, Atom *target)
{
    /* Do the necessary thing after transferring the data. */
    /* If this callback is registered, application must free the selection data. */
    /* Otherwise, Xt free the data. */
}
