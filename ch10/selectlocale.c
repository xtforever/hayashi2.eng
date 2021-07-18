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

static void SelectCB();
static void PasteCB();
static Boolean ConvertProc();
static void LoseProc();
static void DoneProc();
static void GetValueProc();

static Atom xa_locale;

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text, select, paste;
    Arg al[20];
    int ac;
    XTextProperty textprop;
    char *lst=NULL;

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

    /* (1) Query character code of current locale */
    if (XmbTextListToTextProperty(XtDisplay(toplevel), &lst, 1,
                                  XTextStyle, &textprop) != Success) {
        fprintf(stderr, "Cannot get encoding info.\n");
        exit(1);
    }
    xa_locale = textprop.encoding;

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
        /* If it can own the selection, invert the text */
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

    /* (2) Request selection data in the format of character code of current locale */
    XtGetSelectionValue(text, XA_PRIMARY, xa_locale, GetValueProc, NULL,
                        XtLastTimestampProcessed(XtDisplay(text)));
}

/* Owner: Called when selection data is requested */
static Boolean 
ConvertProc (Widget w, Atom *selection, Atom *target, Atom *type_ret, XtPointer *val_ret, unsigned long *len_ret, int *format_ret)
{
    char *string;

    /* prepare the transferred data */
    if (*selection == XA_PRIMARY && *target == xa_locale) {
        string = XmTextGetString(w);   /* send the entered text */
        /* (3) The character code matches.  Send the text in my character code */
        *val_ret = string;
        *len_ret = strlen(string) + 1; /* append final NULL */
        *type_ret = xa_locale;         /* use char code of my locale */
        *format_ret = 8;               /* format of string is 8 */
        return True;   /* notify success of preparing data */
    }
    else
        return False;  /* the type does not match with the requested selection */
}

/* Requestor: Called when selection data is received */
static void 
GetValueProc (Widget w, XtPointer client_data, Atom *selection, Atom *type, XtPointer val, unsigned long *len, int *format)
{
    if (*len == 0 || val == NULL) { /* Failed to receive selection data */
        XBell(XtDisplay(w), 0);  /* notify the fail by bell */
        return;
    }

    if (*selection == XA_PRIMARY && *type == xa_locale) {
        /* (4) The char code matches.
               Paste it in text widget without conversion */
        XmTextSetString(w, (char *)val);
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
    /* If this callback is registered, application must free the selection data.  */
    /* Otherwise, Xt free the data. */
}
