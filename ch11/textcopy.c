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
static void CutCB();
static void PasteCB();

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, text, control, copy, cut, paste;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Textcopy", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Text input area */
    ac = 0;
    text = XmCreateText(panel, "text", al, ac);
    XtManageChild(text);

    /* Command area */
    ac = 0;
    control = XmCreateRowColumn(panel, "control", al, ac);
    XtManageChild(control);

    /* [Copy] --> copy data to clipboard */
    ac = 0;
    copy = XmCreatePushButton(control, "copy", al, ac);
    XtAddCallback(copy, XmNactivateCallback, CopyCB, text);
    XtManageChild(copy);

    /* [Cut] --> cut data and copy it to clipboard */
    ac = 0;
    cut = XmCreatePushButton(control, "cut", al, ac);
    XtAddCallback(cut, XmNactivateCallback, CutCB, text);
    XtManageChild(cut);

    /* [Paste] --> retrieve data from clipboard */
    ac = 0;
    paste = XmCreatePushButton(control, "paste", al, ac);
    XtAddCallback(paste, XmNactivateCallback, PasteCB, text);
    XtManageChild(paste);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* [Copy] button callback */
static void CopyCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget text = (Widget)client_data;
    Boolean stat;
    /* Copy selected data to clipboard */
    stat = XmTextCopy(text, XtLastTimestampProcessed(XtDisplay(text)));
}

/* [Cut] button callback */
static void CutCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget text = (Widget)client_data;

    Boolean stat;
    /* Cut selected data to clipboard */
    stat = XmTextCut(text, XtLastTimestampProcessed(XtDisplay(text)));
}

/* [Paste] button callback */
static void PasteCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget text = (Widget)client_data;

    Boolean stat;
    /* Paste clipboard data into text widget */
    stat = XmTextPaste(text, XtLastTimestampProcessed(XtDisplay(text)));
}
