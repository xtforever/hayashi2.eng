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

typedef struct {  /* (Res1) Resource value structure */
    Boolean  input_by_mm;  /* input unit: True => mm, False => inch */
    XmString mm_label;     /* "Millimeter" */
    XmString inch_label;   /* "Inch" */
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* (Res2) Resource definition structure */
  {"inputByMm", "InputByMm", XtRBoolean,
   sizeof(Boolean), XtOffsetOf(CustomRec, input_by_mm), XtRString, "True"},
  {"mmLabel", "MmLabel", XmRXmString,
   sizeof(XmString), XtOffsetOf(CustomRec, mm_label), XtRString, "mm"},
  {"inchLabel", "InchLabel", XmRXmString,
   sizeof(XmString), XtOffsetOf(CustomRec, inch_label), XtRString, "inch"},
};

static XrmOptionDescRec opt[] = {  /* (Opt1) Custom option */
    {"-mm",   "*inputByMm", XrmoptionNoArg, "True"},  /* mm */
    {"-inch", "*inputByMm", XrmoptionNoArg, "False"}, /* inch */
};

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, text, label;
    Arg    al[20];
    int    ac;
    CustomRec data;

    XtSetLanguageProc(NULL, NULL, NULL);
                                                 /* (Opt2) Custom options */
    ac = 0;                                            /*  V        V    */
    toplevel = XtAppInitialize(&app_context, "Inputunit", opt, XtNumber(opt),
                               &argc, argv, NULL, al, ac);
    /* (Res3) Get resource values */
    XtGetApplicationResources(toplevel, &data, res, XtNumber(res), NULL, 0);

    ac = 0;
    XtSetArg(al[ac], XmNorientation, XmHORIZONTAL); ac++;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);
    ac = 0;
    text = XmCreateTextField(panel, "text", al, ac);
    XtManageChild(text);

    ac = 0;
    if (data.input_by_mm) {
        XtSetArg(al[ac], XmNlabelString, data.mm_label); ac++;
    } else {
        XtSetArg(al[ac], XmNlabelString, data.inch_label); ac++;
    }
    label = XmCreateLabel(panel, "label", al, ac);
    XtManageChild(label);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
