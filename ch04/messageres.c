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

#define XhRMultiLineXmString "MultiLineXmString" /* (1) New resource type name */

extern Boolean CvtStrToMultiLineXmString(); /* Resource converter */

static Widget dialog;

typedef struct {  /* Resource value structure */
    XmString message;  /* Array of compound string */
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* Custom resource */
  {"message", "Message", XhRMultiLineXmString,
   sizeof(XmString), XtOffsetOf(CustomRec, message),
   XtRImmediate, (XtPointer)NULL},
};

static void 
ShowMessageCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    XtManageChild(dialog);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, button;
    Arg    al[20];
    int    ac;
    CustomRec data;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Messageres", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (2) Register resource converter */
    XtAppSetTypeConverter(app_context, XtRString, XhRMultiLineXmString,
                         CvtStrToMultiLineXmString, NULL, 0, XtCacheNone, NULL);

    /* (3) Get custom resource (Resouce converter is called) */
    XtGetApplicationResources(toplevel, &data, res, XtNumber(res), NULL, 0);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);
    XtAddCallback(button, XmNactivateCallback, ShowMessageCB, NULL);

    ac = 0;
    if (data.message != NULL)
        XtSetArg(al[ac], XmNmessageString, data.message); ac++;
    dialog = XmCreateMessageDialog(toplevel, "dialog", al, ac);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
