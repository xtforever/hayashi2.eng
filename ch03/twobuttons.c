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

typedef struct {  /* (1) Resource value structure */
    int    size;
    int    offset;
    String label;   /* label string (String = char*) */
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* (2) Resource definition structure */
  {"size",   "Size",   XtRInt,    sizeof(int),    XtOffsetOf(CustomRec, size),
   XtRString, "50"},
  {"offset", "Offset", XtRInt,    sizeof(int),    XtOffsetOf(CustomRec, offset),
   XtRString, "20"},
  {"label",  "Label",  XtRString, sizeof(String), XtOffsetOf(CustomRec, label),
   XtRString, "default"},
};

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, button1, button2;
    Arg    al[20];
    int    ac;
    CustomRec data;
    XmString xmstr;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Twobuttons", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateBulletinBoard(toplevel, "panel", al, ac);
    /* (3) Get resource values */
    XtGetApplicationResources(panel, &data, res, XtNumber(res), NULL, 0);
    XtVaSetValues(panel, XmNwidth,  data.size, /* set size of panel */
                         XmNheight, data.size,
                         NULL);
    XtManageChild(panel);


    ac = 0;
    button1 = XmCreatePushButton(panel, "button1", al, ac);

    /* (3) Get resource values */
    XtGetApplicationResources(button1, &data, res, XtNumber(res), NULL, 0);

    xmstr = XmStringCreateLocalized(data.label); /* Compound string */
    XtVaSetValues(button1, XmNwidth,  data.size, /* Set button size */
                           XmNheight, data.size,
                           XmNx,      data.offset, /* Set button position */
                           XmNy,      data.offset,
                           XmNlabelString, xmstr, /* Set button label */
                           NULL);

    XtManageChild(button1);
    XmStringFree(xmstr);


    ac = 0;
    button2 = XmCreatePushButton(panel, "button2", al, ac);

    /* (3) Get resource values */
    XtGetApplicationResources(button2, &data, res, XtNumber(res), NULL, 0);

    xmstr = XmStringCreateLocalized(data.label); /* Compound string */
    XtVaSetValues(button2, XmNwidth,  data.size, /* set button size */
                           XmNheight, data.size,
                           XmNx,      data.offset, /* set button position */
                           XmNy,      data.offset,
                           XmNlabelString, xmstr, /* set button label */
                           NULL);

    XtManageChild(button2);
    XmStringFree(xmstr);


    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
