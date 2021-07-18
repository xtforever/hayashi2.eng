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

static Atom xa_targets, xa_locale, xa_ctext;
static Atom atoms[4];

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

    /* (1) Create atoms for selection target */

    /* atom for "TARGETS" */
    xa_targets = XmInternAtom(XtDisplay(toplevel), "TARGETS", False);

    /* atom for COMPOUND_TEXT */
    xa_ctext = XmInternAtom(XtDisplay(toplevel), "COMPOUND_TEXT", False);

    /* atom for current character code */
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

    /* (2) request for "TARGETS" to inquire data types owner can offer */
    XtGetSelectionValue(text, XA_PRIMARY, xa_targets, GetValueProc, NULL,
                        XtLastTimestampProcessed(XtDisplay(text)));
}

/* Owner: Called when selection data is requested */
static Boolean 
ConvertProc (Widget w, Atom *selection, Atom *target, Atom *type_ret, XtPointer *val_ret, unsigned long *len_ret, int *format_ret)
{
    char *string;

    if (*selection != XA_PRIMARY)  /* handle only "PRIMARY" selection */
        return False;

    /* Do what is appropreate for the received type */
    if (*target == xa_targets) {
        /* (3) "TARGETS" is requested.  Return list of available targets */
        atoms[0] = xa_targets;  /* atom of "TARGETS" */
        atoms[1] = xa_locale;   /* atom of owner's encoding */
        atoms[2] = xa_ctext;    /* atom of compound text */
        atoms[3] = XA_STRING;   /* atomof "STRING" (English text) */
        *val_ret = atoms;     /* list of available types */
        *len_ret = 4;         /* four types are available */
        *type_ret = XA_ATOM;  /* data type of "type list" is Atom */
        *format_ret = 32;     /* format of Atom is 32 */
    }
    else if (*target == xa_locale) {
        /* (5) Requested type is same encoding name of owner */
        string = XmTextGetString(w);   /* send the entered text */
        /* The character code matches.  Send the text in my character code */
        *val_ret = string;
        *len_ret = strlen(string) + 1; /* append final NULL */
        *type_ret = xa_locale;         /* use char code of my locale */
        *format_ret = 8;               /* format of string is 8 */
    }
    else if (*target == xa_ctext) {
        /* (6) Requested type is compound text */
        XTextProperty textprop;
        string = XmTextGetString(w);  /* send the entered text */
        /* Convert multi byte string to compound text */
        if (XmbTextListToTextProperty(XtDisplay(w), &string, 1,
                                      XStdICCTextStyle, &textprop) != Success)
            return False;  /* failed to convert (illegal character may be entered) */
        XtFree(string);
        *val_ret = textprop.value;    /* send the compound text */
        *len_ret = strlen(textprop.value);
        *type_ret = xa_ctext;         /* atom for compound text */
        *format_ret = 8;              /* format of string is 8 */
    }
    else if (*target == XA_STRING) {
        /* (7) Requested type is "STRING" (English text) */
        string = XmTextGetString(w);   /* send the entered text */
        *val_ret = string;
        *len_ret = strlen(string) + 1; /* append final NULL */
        *type_ret = XA_STRING;         /* handles only English text */
        *format_ret = 8;               /* format of string is 8 */
    }
    else {
        return False;  /* unknown type */
    }

    return True;   /* Notify success of preparation */
}

/* Requestor: Called when selection data is received */
static void 
GetValueProc (Widget w, XtPointer client_data, Atom *selection, Atom *type, XtPointer val, unsigned long *len, int *format)
{
    if (*len == 0 || val == NULL) { /* Failed to receive selection data */
        XBell(XtDisplay(w), 0);  /* notify by bell */
        return;
    }

    if (*selection != XA_PRIMARY) { /* handle "PRIMARY" selection only */
        XBell(XtDisplay(w), 0);  /* notify by bell */
        return;
    }

    /* Do what is appropreate for the received type */
    if (*type == XA_ATOM) {
        /* (4) The received type is Atom */
        /* It implies that it is the answer for "TARGETS" request */
        int i, j;
        /* The atom list is the list of types which owner can provide */
        Atom *atomlist = val;
        atoms[0] = xa_locale;
        atoms[1] = xa_ctext;
        atoms[2] = XA_STRING;
        /* Select a type from the list as next target */
        for (i = 0; i < 3; i++) {
            for (j = 0; j < *len; j++) {
                if (atoms[i] == atomlist[j]) {
                    /* request data with the selected target */
                    XtGetSelectionValue(w, XA_PRIMARY, atoms[i],
                                        GetValueProc, NULL,
                                        XtLastTimestampProcessed(XtDisplay(w)));
                    XtFree((char *)val);
                    return;
                }
            }
        }
    }
    else if (*type == xa_locale) {
        /* (8) The received type matches with my charset encoding */
        /* Paste it in text widget without conversion */
        XmTextSetString(w, (char *)val);
    }
    else if (*type == xa_ctext) {
        /* (9) The received type is compound text */
        XTextProperty textprop;
        char **clist;
        int count;
        /* Convert back the compound text to multi byte string */
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
    }
    else if (*type == XA_STRING) {
        /* (10) The received type is "STRING" (English text) */
        XmTextSetString(w, (char *)val);
    }
    else {
        XBell(XtDisplay(w), 0);  /* Notify by bell if the type is illegal */
    }
    XtFree((char *)val);
}

/* Owner: Called after requestor received the selection data */
static void 
DoneProc (Widget w, Atom *selection, Atom *target)
{
    /* Do the necessary thing after transferring the data. */
    /* If this callback is registered, application must free the selection data. */
    /* Otherwise, Xt free the data. */
}
