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
#include <stdlib.h>
#include <Xm/XmAll.h>

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, label;
    Arg    al[20];
    int    ac;
    static char *text1 = "Normal Character ", *text2 = "Bold Character";
    XmString string, s1, s2;
    /* (1) Prepare multiple font names */
    static char *fontname1 = "*-medium-*-16-*", *fontname2 = "*-bold-*-16-*";
    XmFontListEntry flentry;
    XmFontList fontlist;

    XtSetLanguageProc(NULL, NULL, NULL); /* need for fontset */
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Flmulti", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (2) Create the first fontlist entry */
    if ((flentry = XmFontListEntryLoad(XtDisplay(toplevel), fontname1,
                                       XmFONT_IS_FONTSET, "MEDIUM")) == NULL) {
        fprintf(stderr, "Cannot load fontlist entry from `%s'\n", fontname1);
        exit(1);
    }
    /* (3) Create new fontlist from the entry */
    fontlist = XmFontListAppendEntry(NULL, flentry);
    XmFontListEntryFree(&flentry);  /* free unnecessary memory */

    /* (4) Create the next fontlist entry */
    if ((flentry = XmFontListEntryLoad(XtDisplay(toplevel), fontname2,
                                       XmFONT_IS_FONTSET, "BOLD")) == NULL) {
        fprintf(stderr, "Cannot load fontlist entry from `%s'\n", fontname2);
        exit(1);
    }
    /* (5) Append the entry to the existing fontlist */
    fontlist = XmFontListAppendEntry(fontlist, flentry);
    XmFontListEntryFree(&flentry);  /* free unnecessary memory */

    /* (6) Create compound strings */
    s1 = XmStringCreate(text1, "MEDIUM");  /* medium thickness */
    s2 = XmStringCreate(text2, "BOLD");    /* bold thickness */
    string = XmStringConcat(s1, s2);  /* concatenate the two strings */
    XmStringFree(s1);  /* free original strings */
    XmStringFree(s2);

    ac = 0;
    XtSetArg(al[ac], XmNfontList, fontlist); ac++;  /* set resource */
    XtSetArg(al[ac], XmNlabelString, string); ac++;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XmStringFree(string);
    XmFontListFree(fontlist); /* free unnecessary memory */
    XtManageChild(label);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
