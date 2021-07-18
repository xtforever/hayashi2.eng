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

#define XhRXmStringTable "CustomXmStringTable"  /* (1) New resource type name */

extern Boolean CvtStrToXmStringTable(); /* Resource converter */

typedef struct {  /* Resource value structure */
    XmStringTable item_list;  /* Array of compound string */
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* Custom resource */
  {"itemList", "ItemList", XhRXmStringTable,
   sizeof(XmStringTable), XtOffsetOf(CustomRec, item_list),
   XtRImmediate, (XtPointer)NULL},
};

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, list;
    Arg    al[20];
    int    ac;
    CustomRec data;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Listres", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (2) Register resource converter */
    XtAppSetTypeConverter(app_context, XtRString, XhRXmStringTable,
                          CvtStrToXmStringTable, NULL, 0, XtCacheNone, NULL);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* (3) Get custom resource (Resouce converter is called) */
    XtGetApplicationResources(toplevel, &data, res, XtNumber(res), NULL, 0);

    ac = 0;
    /* (4) Set the items into the List */
    if (data.item_list != NULL)
        XtSetArg(al[ac], XmNitems, data.item_list); ac++;
    list = XmCreateScrolledList(panel, "list", al, ac);
    XtManageChild(list);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
