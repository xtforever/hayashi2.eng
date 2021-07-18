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
    Widget toplevel, panel, label;
    Arg al[20];
    int ac;
    Pixmap bitmap;
    unsigned int width, height;
    int xh, yh;
    XIconSize *iconsizes;
    Boolean width_match, height_match, size_match;
    int count, i, j, min, max, inc;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Iconbm", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    label = XmCreateLabel(panel, "label", al, ac);
    XtManageChild(label);

    XtRealizeWidget(toplevel);

    if (argc == 2) {
        /* (1) Read bitmap file for icon */
        if (XReadBitmapFile(XtDisplay(toplevel), XtWindow(toplevel), argv[1],
                        &width, &height, &bitmap, &xh, &yh) != BitmapSuccess) {
            fprintf(stderr, "Cannot create bitmap for '%s'\n", argv[1]);
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Usage: %s bitmap_file_name\n", argv[0]);
        exit(1);
    }

    /* (2) Query the desired icon sizes */
    if (XGetIconSizes(XtDisplay(toplevel),
             RootWindowOfScreen(XtScreen(toplevel)), &iconsizes, &count) == 0) {
        fprintf(stderr, "Cannot get icon size information.\n");
        exit(1);
    }

    /* (3) Check if the bitmap matches to the desired icon size */
    size_match = False;
    for (i = 0; i < count; i++) {
        width_match = False;
        height_match = False;

        /* (4) width matches ? */
        min = iconsizes[i].min_width;
        max = iconsizes[i].max_width;
        inc = iconsizes[i].width_inc;
        for (j = min; j <= max; j += inc) {
            if (j == width) {
                width_match = True;
                break;
            }
        }

        /* (5) height matches ? */
        min = iconsizes[i].min_height;
        max = iconsizes[i].max_height;
        inc = iconsizes[i].height_inc;
        for (j = min; j <= max; j += inc) {
            if (j == height) {
                height_match = True;
                break;
            }
        }

        /* (6) OK if width and height matches */
        if (width_match && height_match) {
            size_match = True;
            break;
        }
    }

    XFree(iconsizes);

    /* (7) If the bitmap is desired size, show the bitmap on icon */
    if (size_match)
        XtVaSetValues(toplevel, XmNiconPixmap, bitmap, NULL);
    else {
        fprintf(stderr, "The size of icon bitmap is not good !\n");
        exit(1);
    }

    XtAppMainLoop(app_context);
}
