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
    Widget toplevel, panel, list;
    Arg    al[20];
    int    ac;
    int    i;
    char  *text;
    XmString items[7], *ritems;
    /* (1) Prepare strings for list item */
    static char *weeks[] = {"Sunday", "Monday", "Tuesday", "Wednesday",
                            "Thursday", "Friday", "Saturday"};

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cslist", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (2) Create array of compound string */
    for (i = 0; i < 7; i++)
        items[i] = XmStringCreateLocalized(weeks[i]);

    /* (3) Set list resources */
    ac = 0;
    XtSetArg(al[ac], XmNitems,            items); ac++; /* listed items */
    XtSetArg(al[ac], XmNitemCount,        7); ac++;     /* number of items */
    XtSetArg(al[ac], XmNvisibleItemCount, 7); ac++;     /* number to be shown */
    list = XmCreateList(panel, "list", al, ac);
           /* (4) The list item is copied in List Widget */
    for (i = 0; i < 7; i++)
        XmStringFree(items[i]); /* (5) free unnecessary compound string */
    XtManageChild(list);

    /* (6) Retrieve list item from widget */
    XtVaGetValues(list, XmNitems, &ritems, NULL);
    for (i = 0; i < 7; i++) {
            /* Extract text from compound string */
        if (XmStringGetLtoR(ritems[i], XmFONTLIST_DEFAULT_TAG, &text)) {
            printf("label text: %s\n", text);
            XtFree(text);  /* free the unnecessary text */
        }
        /* (7) Do not free the value gotten from XmNitems resource */
        /* XmStringFree(ritems[i]) should not be called */
    }
    /* XtFree(ritems) should not be called */

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
