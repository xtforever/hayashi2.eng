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
    Widget toplevel;
    Arg al[20];
    int ac;
    Atom atom;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Atom", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Get atom when it is already allocated */
    atom = XmInternAtom(XtDisplay(toplevel), "MY_DATA", True);
    printf("Atom is %d\n", atom);

    /* Create new atom if it is not allocated */
    atom = XmInternAtom(XtDisplay(toplevel), "MY_DATA", False);
    printf("Atom is %d\n", atom);
}
