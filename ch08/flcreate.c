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
    Widget toplevel, label;
    Arg    al[20];
    int    ac;
    static char *text = "English Text";
    XmString string;
    static char *fontname = "*-14-*"; /* default font set name */
    XFontSet fontset;
    char **miss, *def;
    int n_miss, i;
    XmFontListEntry flentry;
    XmFontList fontlist;

    XtSetLanguageProc(NULL, NULL, NULL); /* need for fontset */
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Flcreate", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (1) The font set name is specified on command line */
    if (argc > 1)
        fontname = argv[1];

    /* (2) Create font set */
    fontset = XCreateFontSet(XtDisplay(toplevel), fontname,
                             &miss, &n_miss, &def);
    if (n_miss > 0) {  /* (3) There is missing character set */
        for (i = 0; i < n_miss; i++)  /* (4) print the missing charset */
            fprintf(stderr, "missing charsets: %s\n", miss[i]);
        XFreeStringList(miss);  /* (5) free the char list */
    }

    /* (6) Create fontlist entry from font set */
    flentry = XmFontListEntryCreate(XmFONTLIST_DEFAULT_TAG,
                                    XmFONT_IS_FONTSET, fontset);
    /* (7) Create new fontlist from the entry */
    fontlist = XmFontListAppendEntry(NULL, flentry);
    XmFontListEntryFree(&flentry);  /* free unnecessary memory */

    /* Create compound string */
    string = XmStringCreateLocalized(text);

    ac = 0;
    XtSetArg(al[ac], XmNfontList, fontlist); ac++;
    XtSetArg(al[ac], XmNlabelString, string); ac++;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XmStringFree(string);
    XmFontListFree(fontlist);
    XtManageChild(label);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
