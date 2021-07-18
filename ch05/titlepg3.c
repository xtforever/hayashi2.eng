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
    Arg    al[20];
    int    ac;
    static char *t1 = "Application Name", *t2 = "IconName", *t3 = "Dialog Name";
    XTextProperty ct1, ct2;
    XmString str3;
    Atom xa_ct;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Titlepg", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Create compound text for titles of window and icon */
    XmbTextListToTextProperty(XtDisplay(toplevel), &t1,1,XStdICCTextStyle, &ct1);
    XmbTextListToTextProperty(XtDisplay(toplevel), &t2,1,XStdICCTextStyle, &ct2);
    /* Create atom for COMPOUND_TEXT */
    xa_ct = XmInternAtom(XtDisplay(toplevel), "COMPOUND_TEXT", False);
    /* Set titles and title encodings for window and icons */
    XtVaSetValues(toplevel, XmNtitle, ct1.value,
                            XmNiconName, ct2.value,
                            XmNtitleEncoding, xa_ct,
                            XmNiconNameEncoding, xa_ct, NULL);

    /* Create a compound string for dialog title */
    str3 = XmStringCreateLocalized(t3);
    /* The type of XmNdialogTitle is XmString (compound string) */
    ac = 0;
    XtSetArg(al[ac], XmNdialogTitle, str3); ac++;
    dialog = XmCreateMessageDialog(toplevel, "dialog", al, ac);

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
