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
    XmFontList fontlist;
    XmFontContext context;
    XmFontListEntry entry;
    XtPointer font;
    XmFontType type;
    XFontSet fontset;
    XFontStruct **fst_list;
    char **fontname_list;
    XFontStruct *fontstruct;
    unsigned long value;
    char *fontname;
    char *tag;
    int n, i;

    XtSetLanguageProc(NULL, NULL, NULL); /* necessary for fontset */
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Cslabel", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    label = XmCreateLabel(toplevel, "label", al, ac);
    XtManageChild(label);

    /* (1) Retrieve fontlist from widget's resource */
    XtVaGetValues(label, XmNfontList, &fontlist, NULL);

    /* (2) Initialize font context */
    if (XmFontListInitFontContext(&context, fontlist)) {
        /* (3) Retrieve fontlist entry from fontlist */
        while ((entry = XmFontListNextEntry(context)) != NULL) {
            /* (4) Extract font information from the entry */
            font = XmFontListEntryGetFont(entry, &type);
            /* (5) the type is fontset */
            if (type == XmFONT_IS_FONTSET) {
                printf("Fontset\n");
                fontset = (XFontSet)font; /* cast to fontset type */
                /* print original font name and locale name */
                printf("\tBase name: %s\n",XBaseFontNameListOfFontSet(fontset));
                printf("\tLocale name: %s\n", XLocaleOfFontSet(fontset));
                /* inquire font structure in the fontset */
                n = XFontsOfFontSet(fontset, &fst_list, &fontname_list);
                for (i = 0; i < n; i++)  /* print each font name */
                    printf("\tFont names [%d]: %s\n", i, fontname_list[i]);
            }
            /* (6) the type is font */
            else if (type == XmFONT_IS_FONT) {
                printf("Font\n");
                fontstruct = (XFontStruct *)font; /* cast to font type */
                /* inquire font name from font property */
                if (XGetFontProperty(fontstruct, XA_FONT, &value)) {
                    fontname = XGetAtomName(XtDisplay(label), value);
                    printf("\tFont name: %s\n", fontname);
                    XFree(fontname);
                }
            }
            /* (7) extract tag name from the entry */
            tag = XmFontListEntryGetTag(entry);
            printf("\tTag name: %s\n", tag);
            XtFree(tag);
        }
        /* (8) free font context */
        XmFontListFreeFontContext(context);
    }

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
