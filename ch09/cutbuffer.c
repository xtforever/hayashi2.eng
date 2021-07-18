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

/* [Copy] button callback */
static void 
StoreCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    char *data;
    /* Copy data in text widget to cut buffer */
    data = XmTextFieldGetString(text);
    XStoreBytes(XtDisplay(text), data, strlen(data));
    XtFree(data);
}

/* [Paste] button callback */
static void 
FetchCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget text = (Widget)client_data;
    char *data;
    int n;
    /* Set the data in cut buffer into text widget */
    data = XFetchBytes(XtDisplay(text), &n);
    XmTextFieldSetString(text, data);
    XFree(data);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text, store, fetch;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cutbuffer", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Text input area */
    ac = 0;
    text = XmCreateTextField(panel, "text", al, ac);
    XtManageChild(text);

    /* [Copy] Button */
    ac = 0;
    store = XmCreatePushButton(panel, "store", al, ac);
    XtAddCallback(store, XmNactivateCallback, StoreCB, text);
    XtManageChild(store);

    /* [Paste] Button */
    ac = 0;
    fetch = XmCreatePushButton(panel, "fetch", al, ac);
    XtAddCallback(fetch, XmNactivateCallback, FetchCB, text);
    XtManageChild(fetch);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
