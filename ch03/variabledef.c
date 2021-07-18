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

static CustomRec data;  /* Prepare a resource value structure */

/* Called when resource is not specified in resource file */
static void
DefaultMargin (Widget w, int offset, XrmValue *value)
{
    Boolean margin;

    if (data.size > 80) margin = True;    /* if size is larger than 80, */
    else                margin = False;  /* take margin arond  */

    value->addr = &margin;  /* return the determined default value */
}

static XtResource res[] = {  /* (2) Resource definition structure */
  {"size",   "Size",   XtRInt,     sizeof(int),     XtOffsetOf(CustomRec, size),
   XtRString,    "50"},
  {"label",  "Label",  XtRString,  sizeof(String),  XtOffsetOf(CustomRec, label),
   XtRString,    "default"},
  {"margin", "Margin", XtRBoolean, sizeof(Boolean), XtOffsetOf(CustomRec, margin),
   XtRCallProc, (XtPointer) DefaultMargin},
};

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, button;
    Arg    al[20];
    int    ac;
    XmString xmstr;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Variabledef", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (3) Get resource value */
    XtGetApplicationResources(toplevel, &data, res, XtNumber(res), NULL, 0);
    ac = 0;
    if (data.margin) {
        XtSetArg(al[ac], XmNmarginWidth, 50); ac++;
        XtSetArg(al[ac], XmNmarginHeight, 50); ac++;
    }
    panel = XmCreateBulletinBoard(toplevel, "panel", al, ac);
    XtManageChild(panel);

    xmstr = XmStringCreateLocalized(data.label);
    ac = 0;
    XtSetArg(al[ac], XmNwidth, data.size); ac++;
    XtSetArg(al[ac], XmNheight, data.size); ac++;
    XtSetArg(al[ac], XmNlabelString, xmstr); ac++;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);
    XmStringFree(xmstr);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
