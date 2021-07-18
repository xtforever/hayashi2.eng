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
    XtAppContext app;
    Display *dpy1, *dpy2;
    Widget  top1, button1, top2, button2;
    Arg     al[20];
    int     ac;
    int     argc2;
    char  **argv2;

    /* (1) copy command line arguments */
    argc2 = argc;
    argv2 = (char **)malloc(sizeof(char *) * argc);
    memcpy(argv2, argv, sizeof(char *) * argc);

    /* (2) Initialize Xt, create shell, set language proc (common for both display) */
    XtToolkitInitialize();
    app = XtCreateApplicationContext();
    XtSetLanguageProc(NULL, NULL, NULL);


    /* (3) Open DISPLAY (class name is "Dpy1") */
    dpy1 = XtOpenDisplay(app, NULL, NULL, "Dpy1", NULL, 0, &argc, argv);

    /* (4) Create widget hierarchy on the DISPLAY */
    ac = 0;
    top1 = XtAppCreateShell(NULL, "Dpy1", applicationShellWidgetClass,
                            dpy1, al, ac);
    ac = 0;
    button1 = XmCreatePushButton(top1, "button1", al, ac);
    XtManageChild(button1);


    /* (5) Open DISPLAY2 (class name is "Dpy2") */
    dpy2 = XtOpenDisplay(app, getenv("DISPLAY2"),
                         NULL, "Dpy2", NULL, 0, &argc2, argv2);

    /* (6) Create widget hierarchy on the DISPLAY2 */
    ac = 0;
    top2 = XtAppCreateShell(NULL, "Dpy2", applicationShellWidgetClass,
                            dpy2, al, ac);
    ac = 0;
    button2 = XmCreatePushButton(top2, "button2", al, ac);
    XtManageChild(button2);


    /* (7) Realize each top level shell */
    XtRealizeWidget(top1);
    XtRealizeWidget(top2);

    /* (8) Event loop is only one */
    XtAppMainLoop(app);
}
