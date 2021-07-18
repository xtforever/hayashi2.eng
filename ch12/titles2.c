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

static void 
ShowDialogCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget dialog = (Widget) client_data;

    XtManageChild(dialog);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, button, dialog;
    Arg al[20];
    int ac;
    XmString str1, str2, str3;
    char *ct1, *ct2, *ct3;
    Atom xa_ct;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Titles", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Create compound string for titles of window and icon */
    str1 = XmStringCreateLocalized("Application Name");
    str2 = XmStringCreateLocalized("IconName");
    /* Convert the compound strings to compound text */
    ct1 = XmCvtXmStringToCT(str1);
    ct2 = XmCvtXmStringToCT(str2);
    /* Create atom for COMPOUND_TEXT */
    xa_ct = XmInternAtom(XtDisplay(toplevel), "COMPOUND_TEXT", False);
    /* Set titles and title encodings for window and icons */
    XtVaSetValues(toplevel, XmNtitle, ct1,
                            XmNiconName, ct2,
                            XmNtitleEncoding, xa_ct,
                            XmNiconNameEncoding, xa_ct, NULL);

    ac = 0;
    dialog = XmCreateMessageDialog(toplevel, "dialog", al, ac);

    /* Create a compound string for dialog title */
    str3 = XmStringCreateLocalized("Dialog Name");
    /* Convert the compound strings to compound text */
    ct3 = XmCvtXmStringToCT(str3);
    /* Set the compound text to the parent (dialog shell) */
    XtVaSetValues(XtParent(dialog), XmNtitle, ct3,
                                    XmNtitleEncoding, xa_ct, NULL);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtAddCallback(button, XmNactivateCallback, ShowDialogCB, dialog);
    XtManageChild(button);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
