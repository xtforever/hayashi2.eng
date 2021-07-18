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
static void Paste0CB();
static void Paste1CB();
static Boolean ConvertProc();
static void LoseProc();
static void GetValueProc();

static Atom xa_filename;

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text, copy, paste0, paste1;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtOpenApplication(&app_context, "Selectparam", NULL, 0,
                     &argc, argv, NULL, sessionShellWidgetClass, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Text input area */
    ac = 0;
    text = XmCreateText(panel, "text", al, ac);
    XtManageChild(text);

    /* [Copy] button --> Owner: owns the selection */
    ac = 0;
    copy = XmCreatePushButton(panel, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, text);
    XtManageChild(copy);

    /* (1) [Absolute Path] button --> Owner: Own the selection */
    ac = 0;
    paste0 = XmCreatePushButton(panel, "paste0", al, ac);
    XtAddCallback(paste0, XmNactivateCallback, Paste0CB, text);
    XtManageChild(paste0);

    /* (2) [File Name] button --> Owner: Own the selection */
    ac = 0;
    paste1 = XmCreatePushButton(panel, "paste1", al, ac);
    XtAddCallback(paste1, XmNactivateCallback, Paste1CB, text);
    XtManageChild(paste1);

    /* (3) Create atom for "_FILENAME" */
    xa_filename = XmInternAtom(XtDisplay(toplevel), "_FILENAME", False);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* Owner: [Copy] button callback */
static void 
CopyCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;

    /* Own the selection (register three callbacks ) */
    if (XtOwnSelection(text, XA_PRIMARY,
                       XtLastTimestampProcessed(XtDisplay(text)),
                       ConvertProc, LoseProc, NULL))
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

/* Owner: [Absolute Path] button callback */
static void 
Paste0CB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    int which = 0;

    /* (4) Add parameter to selection request (0: absolute path) */
    XtSetSelectionParameters(text, XA_PRIMARY, XA_INTEGER, &which, 1, 32);

    /* Request selection data (the data will be received in callback) */
    XtGetSelectionValue(text, XA_PRIMARY, xa_filename, GetValueProc, NULL,
                        XtLastTimestampProcessed(XtDisplay(text)));
}

/* Owner: [File Name] button callback */
static void 
Paste1CB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    int which = 1;

    /* (5) Add parameter to selection request (1: only filename) */
    XtSetSelectionParameters(text, XA_PRIMARY, XA_INTEGER, &which, 1, 32);

    /* Request selection data (the data will be received in callback) */
    XtGetSelectionValue(text, XA_PRIMARY, xa_filename, GetValueProc, NULL,
                        XtLastTimestampProcessed(XtDisplay(text)));
}

/* Owner: Called when selection data is requested */
static Boolean 
ConvertProc (Widget w, Atom *selection, Atom *target, Atom *type_ret, XtPointer *val_ret, unsigned long *len_ret, int *format_ret)
{
    char *string;
    Atom param_type;
    XtPointer param_val;
    unsigned long param_len;
    int param_format;

    /* prepare the transferred data */
    if (*selection == XA_PRIMARY && *target == xa_filename) {
        string = XmTextGetString(w);   /* send the entered text */
        XtGetSelectionParameters(w, XA_PRIMARY, NULL, &param_type,
                                 &param_val, &param_len, &param_format);
        /* (6) If parameter is 1, extract only file name */
        if (param_type == XA_INTEGER && param_len > 0 && *(int*)param_val == 1) {
            string = strrchr(string, '/');
            string++;
        }
        *val_ret = string;
        *len_ret = strlen(string) + 1; /* append final NULL */
        *type_ret = XA_STRING;         /* this example handles only English text */
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

    /* Paste the received text in the text widget */
    if (*selection == XA_PRIMARY && *type == XA_STRING)
        XmTextSetString(w, (char *)val);
    else
        XBell(XtDisplay(w), 0);  /* Notify by bell if the type is illegal */

    XtFree((char *)val);
}
