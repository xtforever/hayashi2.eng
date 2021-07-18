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

/* (1) Define custom options */
static XrmOptionDescRec opt[] = {
/* option arg      Matched resource      How to specify option   value */
    {"-label",     "*toggle.labelString",   XrmoptionSepArg,    NULL},
    {"-c",         "*toggle.background",    XrmoptionStickyArg, NULL},
    {"-on",        "*toggle.set",           XrmoptionNoArg,     "True"},
    {"-down",      "*arrow.arrowDirection", XrmoptionNoArg,     "ARROW_DOWN"},
};

/* (B) Show usage (valid options) */
static void 
Usage (char *name)
{
    fprintf(stderr, "Usage: %s [-label <string>] [-c<color>]\n", name);
    fprintf(stderr, "       [-on] [-down]\n");
    exit(1);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget  toplevel, panel, toggle, arrow;
    Arg     al[20];
    int     ac;

    XtSetLanguageProc(NULL, NULL, NULL);
                          /* (2) Pass definition and number of custom option */
    ac = 0;                                            /*  V        V     */
    toplevel = XtAppInitialize(&app_context, "Customopt", opt, XtNumber(opt),
                               &argc, argv, NULL, al, ac);
    if (argc > 1) Usage(argv[0]); /* (A) Undefined option is passed */

    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);
    ac = 0;
    toggle = XmCreateToggleButton(panel, "toggle", al, ac);
    XtManageChild(toggle);
    ac = 0;
    arrow = XmCreateArrowButton(panel, "arrow", al, ac);
    XtManageChild(arrow);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
