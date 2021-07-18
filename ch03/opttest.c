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

/* (1) Define custom options */
static XrmOptionDescRec opt[] = {
/* Option arg       Matched resource      How to specify option   Value   */
    {"-label",      "*toggle.labelString",   XrmoptionSepArg,    NULL},

    {"-c",          "*toggle.background",    XrmoptionStickyArg, NULL},

    {"-on",         "*toggle.set",           XrmoptionNoArg,     "True"},
    {"-off",        "*toggle.set",           XrmoptionNoArg,     "False"},
    {"-up",         "*arrow.arrowDirection", XrmoptionNoArg,     "arrow_up"},
    {"-down",       "*arrow.arrowDirection", XrmoptionNoArg,     "arrow_down"},
    {"-left",       "*arrow.arrowDirection", XrmoptionNoArg,     "arrow_left"},
    {"-right",      "*arrow.arrowDirection", XrmoptionNoArg,     "arrow_right"},

    {"-ah30",       "*arrow.height",         XrmoptionNoArg,     "30"},
    {"-ah40",       "*arrow.height",         XrmoptionNoArg,     "40"},
    {"-ah50",       "*arrow.height",         XrmoptionNoArg,     "50"},
    {"-red",        "*arrow.background",     XrmoptionNoArg,     "red"},
    {"-green",      "*arrow.background",     XrmoptionNoArg,     "green"},
    {"-blue",       "*arrow.background",     XrmoptionNoArg,     "blue"},
    {"-small",      "*fontList",             XrmoptionNoArg,     "*-14-*:"},
    {"-medium",     "*fontList",             XrmoptionNoArg,     "*-16-*:"},
    {"-large",      "*fontList",             XrmoptionNoArg,     "*-24-*:"},

    {"arrow_left",  "*arrow.arrowDirection", XrmoptionIsArg,     NULL},
    {"arrow_right", "*arrow.arrowDirection", XrmoptionIsArg,     NULL},
    {"arrow_up",    "*arrow.arrowDirection", XrmoptionIsArg,     NULL},
    {"arrow_down",  "*arrow.arrowDirection", XrmoptionIsArg,     NULL},

    {"-skip",     NULL, XrmoptionSkipArg,  NULL}, /* ignore next arg */
    {"-skipline", NULL, XrmoptionSkipLine, NULL}, /* ignore all remaining args */
    {"-skip0", NULL, XrmoptionSkipNArgs, (XtPointer)0}, /* ignore this option */
    {"-skip1", NULL, XrmoptionSkipNArgs, (XtPointer)1}, /* same as XrmoptionSkipArg */
    {"-skip2", NULL, XrmoptionSkipNArgs, (XtPointer)2}, /* ignore next 2 args */
    {"-skip3", NULL, XrmoptionSkipNArgs, (XtPointer)3}, /* ignore next 3 args */
};

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget  toplevel, panel, toggle, arrow;
    Arg     al[20];
    int     ac;

    XtSetLanguageProc(NULL, NULL, NULL);
                           /* (2) Definition and number of Custom options */
    ac = 0;                                          /*  V        V     */
    toplevel = XtAppInitialize(&app_context, "Opttest", opt, XtNumber(opt),
                               &argc, argv, NULL, al, ac);
    /* Print undefined or ignored options */
    if (argc > 1) {
        int i;
        printf("%d options are remained after initializing Xt.\n", argc - 1);
        for (i = 1; i < argc; i++)
            printf("    %s\n", argv[i]);
    }

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
