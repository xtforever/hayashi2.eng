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
    int     size;    /* size of push button */
    String  label;   /* label string (String = char*) */
    Boolean margin;  /* whether margin is taken */
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* (2) Resource definition */
  {"size",   "Size",   XtRInt,     sizeof(int),     XtOffsetOf(CustomRec, size),
   XtRString,    "50"},
  {"label",  "Label",  XtRString,  sizeof(String),  XtOffsetOf(CustomRec, label),
   XtRString,    "default"},
  {"margin", "Margin", XtRBoolean, sizeof(Boolean), XtOffsetOf(CustomRec, margin),
   XtRImmediate, (XtPointer) False},
};

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, button;
    Arg    al[20];
    int    ac;
    CustomRec data;
    XmString xmstr;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
               /* resource database is created */
    toplevel = XtAppInitialize(&app_context, "Customres", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (3) Get resource values */
                        /* Array and number of resource definition */
                       /*                       V       V    */
    XtGetApplicationResources(toplevel, &data, res, XtNumber(res), NULL, 0);
                     /*                   A                */
                    /*      Resource value structure      */
    ac = 0;
    if (data.margin) {
        XtSetArg(al[ac], XmNmarginWidth, 50); ac++;   /* set margin   */
        XtSetArg(al[ac], XmNmarginHeight, 50); ac++;  /* around button */
    }
    panel = XmCreateBulletinBoard(toplevel, "panel", al, ac);
    XtManageChild(panel);

    xmstr = XmStringCreateLocalized(data.label); /* Compuond String */
    ac = 0;
    XtSetArg(al[ac], XmNwidth, data.size); ac++;   /* Set button size   */
    XtSetArg(al[ac], XmNheight, data.size); ac++;
    XtSetArg(al[ac], XmNlabelString, xmstr); ac++;   /* Set button label */
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);
    XmStringFree(xmstr);  /* Free unnecessary compound string */

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
