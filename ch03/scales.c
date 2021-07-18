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

typedef struct {  /* Resource value structure */
    int    num_labels;
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* Resource definition structure */
  {"numLabels", "NumLabels", XtRInt,
   sizeof(int), XtOffsetOf(CustomRec, num_labels),
   XtRImmediate, (XtPointer) 2},
};

static Widget CreateScaleWithMarks(parent, name)
    Widget parent;
    String name;
{
    Widget scale, label;
    CustomRec data;
    char mark[20];
    int i, min, max;

    scale = XmCreateScale(parent, name, NULL, 0); /* Create scale */
    XtVaGetValues(scale, XmNminimum, &min, /* Check max and min of scale */
                         XmNmaximum, &max, NULL);
    /* Get custom resource for the scale */
    XtGetApplicationResources(scale, &data, res, XtNumber(res), NULL, 0);
    /* Add the specified numbers of marks */
    for (i = 0; i < data.num_labels; i++) {
        sprintf(mark, "%d", min + i * (max - min) / (data.num_labels - 1));
        label = XmCreateLabelGadget(scale, mark, NULL, 0);
        XtManageChild(label);
    }
    return scale;
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, scale1, scale2;
    Arg    al[20];
    int    ac;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Scales", NULL, 0,
                               &argc, argv, NULL, al, ac);
    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Create two scales with marks */
    scale1 = CreateScaleWithMarks(panel, "scale1");
    XtManageChild(scale1);
    scale2 = CreateScaleWithMarks(panel, "scale2");
    XtManageChild(scale2);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
