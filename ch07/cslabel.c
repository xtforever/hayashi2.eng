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

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget   toplevel, label;
    Arg      al[20];
    int      ac;
    char    *text;
    XmString string;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cslabel", NULL, 0,
                               &argc, argv, NULL, al, ac);

    text = malloc(strlen(argv[1])); /* This text is allocated storage */
    strcpy(text, argv[1]);
    string = XmStringCreateLocalized(text); /*(1) Create compound string */
    free(text);  /* (2) free the text */

    ac = 0;
    XtSetArg(al[ac], XmNlabelString, string); ac++;  /* (3) set resource */
    label = XmCreateLabel(toplevel, "label", al, ac);
             /* (4) The compound string is copied in widget */
    XmStringFree(string);  /* (5) free unnecessary compound string */
    XtManageChild(label);

    XtVaGetValues(label, XmNlabelString, &string, NULL); /*(6) get from resource */
        /* Extract a text from compound string */
    if (XmStringGetLtoR(string, XmFONTLIST_DEFAULT_TAG, &text)) {
        printf("label text: %s\n", text);
        XtFree(text);  /* free the unnecessary text */
    }
    XmStringFree(string);  /* (7) free unnecessary compound string */

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
