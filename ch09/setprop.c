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
    Widget toplevel, panel, canvas;
    Arg al[20];
    int ac;
    Atom atom1, atom2, atom3;
    pid_t pid;
    Window window;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Setprop", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    XtSetArg(al[ac], XmNwidth, 200); ac++;
    XtSetArg(al[ac], XmNheight, 100); ac++;
    canvas = XmCreateDrawingArea(panel, "canvas", al, ac);
    XtManageChild(canvas);

    /* (1) Create atoms for property names */
    atom1 = XmInternAtom(XtDisplay(toplevel), "_MY_NAME", False);
    atom2 = XmInternAtom(XtDisplay(toplevel), "_MY_PROC", False);
    atom3 = XmInternAtom(XtDisplay(toplevel), "_MY_WINDOW", False);

    /* Store property data on root window */

    /* (2) Store application name in "_MY_NAME" of root window */
    XChangeProperty(XtDisplay(toplevel), RootWindowOfScreen(XtScreen(toplevel)),
                atom1, XA_STRING, 8, PropModeReplace, argv[0], strlen(argv[0]));

    /* (3) Store process ID in "_MY_PROC" of root window */
    pid = getpid();
    XChangeProperty(XtDisplay(toplevel), RootWindowOfScreen(XtScreen(toplevel)),
                    atom2, XA_INTEGER, 32, PropModeReplace, &pid, 1);

    XtRealizeWidget(toplevel); /* window of widget is created here */

    /* (4) Store window ID in "_MY_WINDOW" of root window */
    window = XtWindow(canvas);
    XChangeProperty(XtDisplay(toplevel), RootWindowOfScreen(XtScreen(toplevel)),
                    atom3, XA_WINDOW, 32, PropModeReplace, &window, 1);

    XtAppMainLoop(app_context);
}
