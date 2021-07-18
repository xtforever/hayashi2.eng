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

static Widget CreateMenu(parent)
    Widget parent;
{
    Widget  menubar, filemenu, editmenu, secondhelp, helpbutton, helpmenu;
    Arg     al[20];
    int     ac;
    int     n = 0;

    menubar = XmVaCreateSimpleMenuBar(parent, "menubar",
                XmVaCASCADEBUTTON, NULL, NULL,
                XmVaCASCADEBUTTON, NULL, NULL,
                XmVaCASCADEBUTTON, NULL, NULL,
                NULL);
    XtManageChild(menubar);

    filemenu = XmVaCreateSimplePulldownMenu(menubar, "filemenu",
                  n++, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaSEPARATOR,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  NULL);

    editmenu = XmVaCreateSimplePulldownMenu(menubar, "editmenu",
                  n++, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaSEPARATOR,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaSEPARATOR,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaSEPARATOR,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  NULL);

    secondhelp = XmVaCreateSimplePulldownMenu(menubar, "secondhelp",
                  n++, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  NULL);

    ac = 0;
    helpbutton = XmCreateCascadeButtonGadget(menubar, "helpbutton", al, ac);
    XtManageChild(helpbutton);

    XtVaSetValues(menubar, XmNmenuHelpWidget, helpbutton, NULL);

    helpmenu = XmVaCreateSimplePulldownMenu(menubar, "helpmenu",
                  n++, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,
                  NULL);

    return(menubar);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, mainwin, menubar, work, text, command, copy, cut, paste;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Menulabels", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    XtSetArg(al[ac], XmNshowSeparator, True); ac++;
    XtSetArg(al[ac], XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE); ac++;
    mainwin = XmCreateMainWindow(toplevel, "mainwin", al, ac);
    XtManageChild(mainwin);

    menubar = CreateMenu(mainwin);

    ac = 0;
    work = XmCreateRowColumn(mainwin, "work", al, ac);
    XtManageChild(work);

    ac = 0;
    command = XmCreateRowColumn(mainwin, "command", al, ac);
    XtManageChild(command);

    XmMainWindowSetAreas(mainwin, menubar, command, NULL, NULL, work);

    ac = 0;
    text = XmCreateText(work, "text", al, ac);
    XtManageChild(text);

    ac = 0;
    copy = XmCreatePushButton(command, "copy", al, ac);
    XtManageChild(copy);
    ac = 0;
    cut = XmCreatePushButton(command, "cut", al, ac);
    XtManageChild(cut);
    ac = 0;
    paste = XmCreatePushButton(command, "paste", al, ac);
    XtManageChild(paste);

    XtRealizeWidget(toplevel);

    XtAppMainLoop(app_context);
}
