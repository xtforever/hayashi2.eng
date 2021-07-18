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

#include <wctype.h>
#include <wchar.h>
#include <Xm/XmAll.h>

static Widget text, error;

static void 
CheckTextCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    wchar_t *wcs;
    size_t length;
    int i;
    wint_t wc;
    Boolean valid = True;

    /* (1) Retrieve text in wide character format */
    wcs = XmTextFieldGetStringWcs(text);
    length = wcslen(wcs);  /* Check number of chars */

    /* (2) Check the wide characer one by one */
    for (i = 0; i < length; i++) {
        wc = (wint_t) *(wcs + i);
        if ( ! iswdigit(wc) ) {  /* Check if the character is number */
            valid = False;  /* If non-number exists, it's illegal */
            break;
        }
    }

    /* (3) Show error when non-number is entered or no characters is entered */
    if (valid == False || length == 0) {
        XmProcessTraversal(text, XmTRAVERSE_CURRENT); /* Set focus to text */
        XBell(XtDisplay(text), 0); /* ring a bell */
        XtManageChild(error); /* show error dialog */
    }

    XtFree((char *)wcs); /* Free the retrieved text */
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, button;
    Arg    al[20];
    int    ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Textwcsfunc", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    text = XmCreateTextField(panel, "text", al, ac);
    XtManageChild(text);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtAddCallback(button, XmNactivateCallback, CheckTextCB, NULL);
    XtManageChild(button);

    ac = 0;
    error = XmCreateErrorDialog(toplevel, "error", al, ac);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
