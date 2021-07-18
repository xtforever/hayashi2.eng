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

extern Boolean CvtStrToBitmap(); /* Resource Converter */

static GC gc; /* Graphics Context (necessary for Xlib drawing functions) */
static Widget label;

static void 
ChangeBitmapCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    char *filename; /* bitmap file name  (source data of conversion) */
    XrmValue source, dest; /* resource value structure */
    Pixmap bitmap;

    /* (1) Input new bitmap file name */
    filename = XmTextFieldGetString(text);
    /* (2) Without input, set focus to input area and bell */
    if (filename == NULL || *filename == 0 ) {
        XmProcessTraversal(text, XmTRAVERSE_CURRENT);
        XBell(XtDisplay(w), 0);
        return;
    }

    /* Prepare resource value struct for pixmap */
    source.size = strlen(filename) + 1;  /* size for source data */
    source.addr = filename;              /* source data */
    dest.size = sizeof(Pixmap);   /* size of target data (integer) */
    dest.addr = (XtPointer)&bitmap;          /* storage for target data */

    /* (3) Convert bitmap file name to Pixmap */
    if (!XtConvertAndStore(label, XtRString, &source, XtRBitmap, &dest)) {
        /* (4) If conversion fails, set focus to input area and bell */
        XmProcessTraversal(text, XmTRAVERSE_CURRENT);
        XBell(XtDisplay(w), 0);
        return;
    }

    /* (5) Show the created pixmap on the label */
    XtVaSetValues(label, XmNlabelPixmap, bitmap, NULL);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text, button;
    Arg    al[20];
    int    ac;
    XtConvertArgRec args[2];  /* Auxiliary data passed to converter */
    Screen *screen;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;       /* Resource database is created here */
    toplevel = XtAppInitialize(&app_context, "Bitmapcnv", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Prepare auxiliary data for resource converter */
    screen = XtScreen(toplevel);
    args[0].address_mode = XtAddress;
    args[0].address_id = (XtPointer)&screen;
    args[0].size = sizeof(Screen *);
    args[1].address_mode = XtAddress;
    args[1].address_id = (XtPointer)&label;
    args[1].size = sizeof(Widget);

    /* (6) Register resource converter from bitmap file name to Pixmap */
    XtAppSetTypeConverter(app_context, XtRString, XtRBitmap, CvtStrToBitmap,
                          args, XtNumber(args), XtCacheByDisplay, NULL);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    XtSetArg(al[ac], XmNlabelType, XmPIXMAP); ac++;
    label = XmCreateLabel(panel, "label", al, ac);
    XtManageChild(label);

    ac = 0;
    text = XmCreateTextField(panel, "text", al, ac);
    XtManageChild(text);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);

    XtAddCallback(button, XmNactivateCallback, ChangeBitmapCB, text);

    XtVaSetValues(label, XmNbottomWidget, text,   NULL);
    XtVaSetValues(text,  XmNbottomWidget, button, NULL);

    XtRealizeWidget(toplevel);
    gc = XCreateGC(XtDisplay(label), XtWindow(label), 0, NULL);
    XtAppMainLoop(app_context);
}
