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
    Atom atom1, atom2, atom3, type;
    int format, stat;
    unsigned long nitems, left;
    char *name;
    pid_t *pid;
    Window root, *window;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Getprop", NULL, 0,
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

    /* Extract property data from root window */

    root = RootWindowOfScreen(XtScreen(toplevel));

    /* (2) Get application name from "_MY_NAME" property of root window */
    if (XGetWindowProperty(XtDisplay(toplevel), root, atom1, 0, 100, False,
                           XA_STRING, &type, &format, &nitems, &left, &name)
                                             == Success && type == XA_STRING) {
        printf("_MY_NAME: %s\n", name);
        XFree(name);  /* free if necessary */
    }

    /* (3) Get process ID from "_MY_PROC" property of root window */
    if (XGetWindowProperty(XtDisplay(toplevel), root, atom2, 0, 1, False,
                           XA_INTEGER, &type, &format, &nitems, &left, &pid)
                                            == Success && type == XA_INTEGER) {
        printf("_MY_PROC: %d\n", *pid);
        XFree(pid);  /* free if necessary */
    }

    /* (4) Get window ID from "_MY_WINDOW" property of root window */
    if (XGetWindowProperty(XtDisplay(toplevel), root, atom3, 0, 1, False,
                           XA_WINDOW, &type, &format, &nitems, &left, &window)
                                             == Success && type == XA_WINDOW) {
        printf("_MY_WINDOW: 0x%x\n", *window);
        XFree(window);  /* free if necessary */
    }

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
