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
    /* (1) Prepare strings for each font */
    static char *smalltext = "Small Character", *largetext = "Large Character";

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Csmultifont", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (2) Create compound string with tags */
    small = XmStringCreate(smalltext, "SMALL");
    large = XmStringCreate(largetext, "LARGE");
    /* (3) Concatenate the two compound strings */
    string = XmStringConcat(small, large);
    XmStringFree(small); /* Free the original compound strings */
    XmStringFree(large);

    ac = 0;
    XtSetArg(al[ac], XmNlabelString, string); ac++;  /* (4) Set resource */
    label = XmCreateLabel(toplevel, "label", al, ac);
    XmStringFree(string);  /* Free unnecessary compound string */
    XtManageChild(label);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
