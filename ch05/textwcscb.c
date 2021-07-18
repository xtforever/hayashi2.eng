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

static void 
CheckInputCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    XmTextVerifyPtrWcs cb = (XmTextVerifyPtrWcs)call_data;
    int i;
    wint_t wc;

    /* (2) Check wide character text one by one */
    for (i = 0; i < cb->text->length; i++) {
        wc = (wint_t) *(cb->text->wcsptr + i);
        if ( ! iswdigit(wc) ) {  /* Check if the character is number */
            cb->doit = False;  /* Otherwise, ignore the entered character */
            break;
        }
    }
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text;
    Arg    al[20];
    int    ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Textwcscb", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    text = XmCreateTextField(panel, "text", al, ac);
    /* (1) Register a callback called whenever input occurs on the text */
    XtAddCallback(text, XmNmodifyVerifyCallbackWcs, CheckInputCB, NULL);
    XtManageChild(text);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
