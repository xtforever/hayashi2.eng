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

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, button, dialog;
    Arg    al[20];
    int    ac;
    static char *text = "English multi byte Text";
    XTextProperty ct1, ct2;
    Atom xa_ct;
    char **list;
    int count;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cvtctext", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Convert multi byte string to COMPOUND_TEXT */
    if (XmbTextListToTextProperty(XtDisplay(toplevel), &text, 1,
                                        XStdICCTextStyle, &ct1) != Success) {
        fprintf(stderr, "Cannot convert to Compound Text.\n");
        exit(1);
    }

    /* Create atom of COMPOUND_TEXT */
    xa_ct = XmInternAtom(XtDisplay(toplevel), "COMPOUND_TEXT", False);
    /* Set title and title encoding */
    XtVaSetValues(toplevel, XmNtitle, ct1.value,
                            XmNtitleEncoding, xa_ct, NULL);

    /* Convert back COMPOUND_TEXT to multi byte string */
    ct2.value = ct1.value;
    ct2.encoding = xa_ct;
    ct2.format = 8;
    ct2.nitems = ct1.nitems;
    if (XmbTextPropertyToTextList(XtDisplay(toplevel), &ct2, &list, &count)
                                                                != Success) {
        fprintf(stderr, "Cannot convert from Compound Text.\n");
        exit(1);
    }
    printf("The original text might be: %s\n", list[0]);


    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
