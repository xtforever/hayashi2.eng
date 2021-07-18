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

static XrmOptionDescRec opt[] = {
    {"-label", "*button.labelString", XrmoptionSepArg, NULL},
};

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget  toplevel, panel, button;
    Arg     al[20];
    int     ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Simplebtn", opt, XtNumber(opt),
                               &argc, argv, NULL, al, ac);

    if (argc > 1) {
        fprintf(stderr, "Usage: %s [-label <string>]\n", argv[0]);
        exit(1);
    }

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
