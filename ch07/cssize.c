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

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget   toplevel, label;
    Arg      al[20];
    int      ac;
    static char *text = "Text";
    XmString     string;
    XmFontList   fontlist;
    Dimension    width, height, baseline;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cssize", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Create compound string */
    string = XmStringCreateLocalized(text);

    ac = 0;
    XtSetArg(al[ac], XmNlabelString, string); ac++;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XtManageChild(label);

    /* (1) Get fontlist resource from widget's resource */
    XtVaGetValues(label, XmNfontList, &fontlist, NULL);

    /* (2) Query width and height of the text */
    width = XmStringWidth(fontlist, string);
    height = XmStringHeight(fontlist, string);
    printf("Text width: %d, height: %d\n", width, height);

    /* (3) Query width and height of the text at one time */
    XmStringExtent(fontlist, string, &width, &height);
    printf("Text extent: %d, %d\n", width, height);

    /* (4) Get the distance from top of the text to baseline */
    baseline = XmStringBaseline(fontlist, string);
    printf("Baseline position: %d\n", baseline);

    XmStringFree(string);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
