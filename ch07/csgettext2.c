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
    XmString string, small, large;
    /* Prepare strings for each font */
    static char *smalltext = "Small Character", *largetext = "Large Character";
    char    *text;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Csgettext2", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Create compound string with tags */
    small = XmStringCreate(smalltext, "SMALL");
    large = XmStringCreate(largetext, "LARGE");
    /* Concatenate the two compound strings */
    string = XmStringConcat(small, large);
    XmStringFree(small); /* Free the original compound strings */
    XmStringFree(large);

    ac = 0;
    XtSetArg(al[ac], XmNlabelString, string); ac++;  /* Set resource */
    label = XmCreateLabel(toplevel, "label", al, ac);
    XtManageChild(label);

    /* Extract a text whose tag match */
    if (XmStringGetLtoR(string, "SMALL", &text)) {  /* ==> "Small Character" */
        printf("text for tag SMALL: %s\n", text);
        XtFree(text);  /* Free the text */
    }
    if (XmStringGetLtoR(string, "LARGE", &text)) {  /* ==> "Large Character" */
        printf("text for tag LARGE: %s\n", text);
        XtFree(text);  /* Free the text */
    }
    XmStringFree(string);  /* Free unnecessary compound string */

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
