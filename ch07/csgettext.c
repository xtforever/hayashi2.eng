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

/* Called when list item is double-clicked */
static void 
SelectCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    char *text;
    /* Callback struct of list (selected item is cb->item) */
    XmListCallbackStruct *cb = (XmListCallbackStruct *)call_data;

    /* (1) retrieve text from selected item cb->item */
    if (XmStringGetLtoR(cb->item, XmFONTLIST_DEFAULT_TAG, &text)) {
        printf("selected item: %s\n", text);
        XtFree(text);  /* (2) free the text */
    }
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, list;
    Arg    al[20];
    int    ac;
    int    i;
    XmString items[7];
    /* Prepare strings for list item */
    static char *weeks[] = {"Sunday", "Monday", "Tuesday", "Wednesday", 
                            "Thursday", "Friday", "Saturday"};

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Csgettext", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Create array of compound string */
    for (i = 0; i < 7; i++)
        items[i] = XmStringCreateLocalized(weeks[i]);

    /* Set list resources */
    ac = 0;
    XtSetArg(al[ac], XmNitems,            items); ac++; /* listed items */
    XtSetArg(al[ac], XmNitemCount,        7); ac++;     /* number of items */
    XtSetArg(al[ac], XmNvisibleItemCount, 7); ac++;     /* number to be shown */
    list = XmCreateList(panel, "list", al, ac);
    for (i = 0; i < 7; i++)
        XmStringFree(items[i]); /* free unnecessary compound string */
    XtManageChild(list);
    /* register a callback for doublie-click on list item */
    XtAddCallback(list, XmNdefaultActionCallback, SelectCB, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
