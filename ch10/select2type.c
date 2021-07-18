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
static Boolean ConvertProc();
static void LoseProc();
static void GetValueProc();

static Widget box, figure, title;
static Boolean owner = False;
static Atom xa_figure, xa_title;
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
    toplevel = XtAppInitialize(&app_context, "Select2type", NULL, 0,
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

    /* [Copy] button --> Owner: owns the selection */
    ac = 0;
    copy = XmCreatePushButton(panel, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, NULL);
    XtManageChild(copy);

    /* [Paste] button --> Requestor: request selection data */
    ac = 0;
    paste = XmCreatePushButton(panel, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, NULL);
    XtManageChild(paste);

    /* (2) Create atoms for "_FIGURE" and "_TITLE" */
    xa_figure = XmInternAtom(XtDisplay(toplevel), "_FIGURE", False);
    xa_title = XmInternAtom(XtDisplay(toplevel), "_TITLE", False);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* Owner: [Copy] button callback */
static void CopyCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Pixel fg, bg;

    if (owner)    /* do nothing if this program already is owner */
        return;

    /* (3) Own the selection (register three callbacks ) */
    if (XtOwnSelection(box, XA_PRIMARY,
                       XtLastTimestampProcessed(XtDisplay(box)),
                       ConvertProc, LoseProc, NULL)) {
        /* If it can own the selection, invert the figure and label */
        XtVaGetValues(figure, XmNforeground, &fg, XmNbackground, &bg, NULL);
        XtVaSetValues(figure, XmNforeground, bg, XmNbackground, fg, NULL);
        XtVaGetValues(title, XmNforeground, &fg, XmNbackground, &bg, NULL);
        XtVaSetValues(title, XmNforeground, bg, XmNbackground, fg, NULL);
        owner = True;
    }
}

/* Owner: Called when ownership is lost */
static void LoseProc(w, selection)
    Widget w;
    Atom *selection;
{
    Pixel fg, bg;

    /* (4) If ownership is lost, reset the inversion */
    if (*selection == XA_PRIMARY) {
        XtVaGetValues(figure, XmNforeground, &fg, XmNbackground, &bg, NULL);
        XtVaSetValues(figure, XmNforeground, bg, XmNbackground, fg, NULL);
        XtVaGetValues(title, XmNforeground, &fg, XmNbackground, &bg, NULL);
        XtVaSetValues(title, XmNforeground, bg, XmNbackground, fg, NULL);
        owner = False;
    }
}

/* Requestor: [Paste] button callback */
static void PasteCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Atom atoms[2];

    /* (5) Request selection data (label and figure) */
    atoms[0] = xa_figure;
    atoms[1] = xa_title;
    XtGetSelectionValues(box, XA_PRIMARY, atoms, 2, GetValueProc, NULL,
                         XtLastTimestampProcessed(XtDisplay(box)));
}

/* Owner: Called when selection data is requested */
static Boolean ConvertProc(w, selection, target,
                           type_ret, val_ret, len_ret, format_ret)
    Widget w;
    Atom *selection;
    Atom *target;
    Atom *type_ret;
    XtPointer *val_ret;
    unsigned long *len_ret;
    int *format_ret;
{
    if (*selection != XA_PRIMARY)  /* Handle only "PRIMARY" selection */
        return False;

    /* return requested target type */
    if (*target == xa_figure) {
        /* (6) If requested target is "_FIGURE" , send the figure */
        /* retrieve pixmap from figure */
        XtVaGetValues(figure, XmNlabelPixmap, &pixmap, NULL);
        /* send the pixmap ID */
        *val_ret = &pixmap;
        *len_ret = 1;
        *type_ret = XA_PIXMAP;  /* type is Pixmap */
        *format_ret = 32;       /* format of Pixmap is 32 */
        return True;   /* Success to prepare the data */
    }
    else if (*target == xa_title) {
        /* (7) If requested target is "_TITLE", send string of the title */
        XmString string;
        char *text;
        /* retrieve label string from the title */
        XtVaGetValues(title, XmNlabelString, &string, NULL);
        /* extract normal string from the compound string */
        XmStringGetLtoR(string, XmFONTLIST_DEFAULT_TAG, &text);
        XmStringFree(string);
        /* send the string */
        *val_ret = text;
        *len_ret = strlen(text) + 1;  /* append final NULL */
        *type_ret = XA_STRING;        /* this example treats English only */
        *format_ret = 8;              /* format of string is  8 */
        return True;   /* Notify success of preparation */
    }
    else
        return False;  /* unknown type */
}

/* Requestor: Called when selection data is received */
static void GetValueProc(w, client_data, selection, type, val, len, format)
    Widget w;
    XtPointer client_data;
    Atom *selection;
    Atom *type;
    XtPointer val;
    unsigned long *len;
    int *format;
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
    if (*type == XA_PIXMAP) {
        /* (8) if the type is "PIXMAP", show it on figure */
        XtVaSetValues(figure, XmNlabelPixmap, *(Pixmap*)val, NULL);
    }
    else if (*type == XA_STRING) {
        /* (9) if the type is "STRING", show the string on title */
        XmString string;
        string = XmStringCreateLocalized((char *)val);
        XtVaSetValues(title, XmNlabelString, string, NULL);
        XmStringFree(string);
    }
    else
        XBell(XtDisplay(w), 0);  /* Notify by bell if the type is illegal */

    XtFree((char *)val);
}
