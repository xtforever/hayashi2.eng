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
    XmString string, s1, s2;
    /* prepare strings for each font */
    static char *t1 = "First line ", *t2 = "label", *t3 = "Second line", *t4 = "Reverse";

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cssegments", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Create segments with tag, direction, and separato */
    /* (1) Create segment 1 */
    s1 = XmStringSegmentCreate(t1, "SMALL", XmSTRING_DIRECTION_L_TO_R, False);

    /* (2) Create and append segment 2 */
    s2 = XmStringSegmentCreate(t2, "LARGE", XmSTRING_DIRECTION_L_TO_R, True);
    string = XmStringConcat(s1, s2);
    XmStringFree(s1); /* (5) Free original compound string */
    XmStringFree(s2); /* (5) Free original compound string */

    /* (3) Create and append segment 3 */
    s1 = string;
    s2 = XmStringSegmentCreate(t3, "SMALL", XmSTRING_DIRECTION_L_TO_R, True);
    string = XmStringConcat(s1, s2);
    XmStringFree(s1); /* (5) Free original compound string */
    XmStringFree(s2); /* (5) Free original compound string */

    /* (4) Create and append segment 4 */
    s1 = string;
    s2 = XmStringSegmentCreate(t4, "LARGE", XmSTRING_DIRECTION_R_TO_L, False);
    string = XmStringConcat(s1, s2);
    XmStringFree(s1); /* (5) Free original compound string */
    XmStringFree(s2); /* (5) Free original compound string */

    ac = 0;
    XtSetArg(al[ac], XmNlabelString, string); ac++;  /* (6) set resource */
    label = XmCreateLabel(toplevel, "label", al, ac);
    XmStringFree(string);  /* (7) free unnecessary compound string */
    XtManageChild(label);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
