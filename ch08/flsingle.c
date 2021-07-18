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
    Widget toplevel, label;
    Arg    al[20];
    int    ac;
    static char *text = "English Text";
    XmString string;
    static char *fontname = "*-14-*"; /* (1) the name of fontset */
    XmFontListEntry flentry;
    XmFontList fontlist;

    XtSetLanguageProc(NULL, NULL, NULL); /* necessary for fontset */
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Flsingle", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (2) Create entry of fontset */
    if ((flentry = XmFontListEntryLoad(XtDisplay(toplevel), fontname,
                         XmFONT_IS_FONTSET, XmFONTLIST_DEFAULT_TAG)) == NULL) {
        fprintf(stderr, "Cannot load fontlist entry from `%s'\n", fontname);
        exit(1);
    }

    /* (3) Create new fontlist from the entry */
    fontlist = XmFontListAppendEntry(NULL, flentry);
    XmFontListEntryFree(&flentry);  /* (4) free unnecessary memory */

    /* (5) Create compound string */
    string = XmStringCreateLocalized(text);

    ac = 0;
    XtSetArg(al[ac], XmNfontList, fontlist); ac++;  /* (6) set resource */
    XtSetArg(al[ac], XmNlabelString, string); ac++;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XmStringFree(string);
    XmFontListFree(fontlist);  /* (7) free unnecessary memory */
    XtManageChild(label);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
