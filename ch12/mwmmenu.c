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

static Widget button;
static void MenuCB();

/* (1) Items appended to window menu */
static char *mwm_menu = "\
no-label                       f.separator    \n\
menu1     _1    Ctrl<Key>F1    f.send_msg  1  \n\
menu2     _2    Ctrl<Key>F2    f.send_msg  2  \n\
menu3     _3    Ctrl<Key>F3    f.send_msg  3  ";

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel;
    Arg al[20];
    int ac;
    Atom xa_mwm_msg;
    int menu_no;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    XtSetArg(al[ac], XmNmwmMenu, mwm_menu); ac++;
    toplevel = XtAppInitialize(&app_context, "Mwmmenu", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (2) receive client message from f.send_msg function of mwm */
    xa_mwm_msg = XmInternAtom(XtDisplay(toplevel), "_MOTIF_WM_MESSAGES", False);
    XmAddWMProtocols(toplevel, &xa_mwm_msg, 1);

    /* (3) activate protocol and register callback for each menu number */
    for (menu_no = 1; menu_no <= 3; menu_no++) {
        XmActivateProtocol(toplevel, xa_mwm_msg, menu_no);
        XmAddProtocolCallback(toplevel, xa_mwm_msg, menu_no, MenuCB, menu_no);
    }

    ac = 0;
    button = XmCreatePushButton(toplevel, "button", al, ac);
    XtManageChild(button);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}

/* (4) Called when additional menu is selected */
static void MenuCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    int menu_no = (int)client_data;  /* selected menu number */
    char buff[40];
    XmString string;

    /* (5) Create label from selected number and show it on push button */
    sprintf(buff, "Selected Menu: %d", menu_no);
    string = XmStringCreateLocalized(buff);
    XtVaSetValues(button, XmNlabelString, string, NULL);
    XmStringFree(string);
}
