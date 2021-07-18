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
    static char *t1 = "First line ", *t2 = "label", *t3 = "Second line", *t4 = "Reverse";
    XmStringContext    context;
    char              *text, *tag;
    XmStringDirection  dir;
    Boolean            sep;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cssegments", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Create segments with tag, direction, and separator */
    /* Create segment 1 */
    s1 = XmStringSegmentCreate(t1, "SMALL", XmSTRING_DIRECTION_L_TO_R, False);

    /* Create and append segment 2 */
    s2 = XmStringSegmentCreate(t2, "LARGE", XmSTRING_DIRECTION_L_TO_R, True);
    string = XmStringConcat(s1, s2);
    XmStringFree(s1);
    XmStringFree(s2);

    /* Create and append segment 3 */
    s1 = string;
    s2 = XmStringSegmentCreate(t3, "SMALL", XmSTRING_DIRECTION_L_TO_R, True);
    string = XmStringConcat(s1, s2);
    XmStringFree(s1);
    XmStringFree(s2);

    /* Create and append segment 4 */
    s1 = string;
    s2 = XmStringSegmentCreate(t4, "LARGE", XmSTRING_DIRECTION_R_TO_L, False);
    string = XmStringConcat(s1, s2);
    XmStringFree(s1);
    XmStringFree(s2);

    ac = 0;
    XtSetArg(al[ac], XmNlabelString, string); ac++;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XtManageChild(label);

    /* (1) Initialize context of compound string */
    if (XmStringInitContext(&context, string)) {
        /* (2) Extract next segment */
        while (XmStringGetNextSegment(context, &text, &tag, &dir, &sep)) {
            /* (3) Inquire each component of the segment */
            printf("Text: %s\t", text);
            printf("Tag: %s\t", tag);
            if (dir == XmSTRING_DIRECTION_L_TO_R) printf("Direction: LtoR\t");
            else                                  printf("Direction: RtoL\t");
            if (sep == True) printf("Separator: Yes\n");
            else             printf("Separator: No\n");
            /* (4) Free text and tag because they were copied */
            XtFree(text);
            XtFree(tag);
        }
        /* (5) Free context of the compound string */
        XmStringFreeContext(context);
    }

    XmStringFree(string);  /* free unnecessary compound string */

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
