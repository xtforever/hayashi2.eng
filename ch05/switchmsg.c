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

#include <nl_types.h>
#include <Xm/XmAll.h>

#define DEFAULT_MSG_TEXT "Message text is not available."

static Widget question;
static nl_catd catd;

/* "File" menu callback */
static void 
FileMenuCB (Widget w, XtPointer client_data, XtPointer call_data)
{
    int menu_item = (int)client_data;  /* selected menu number */
    char *text;
    XmString string;

    /* (2) Read message text for selected item */
    switch (menu_item) {
    case 0:  /* 0th menu item:  "New" */
    case 1:  /* 1st menu item:  "Open" */
            text = catgets(catd, 1, 1, DEFAULT_MSG_TEXT); /* message number 1 */
            break;
    case 2:  /* 2nd menu item:  "Save" */
            text = catgets(catd, 1, 2, DEFAULT_MSG_TEXT); /* message number 2 */
            break;
    case 3:  /* 3rd menu item:  "Exit" */
            text = catgets(catd, 1, 3, DEFAULT_MSG_TEXT); /* message number 3 */
            break;
    }

    /* (3) Create a compound string from the message text */
    string = XmStringCreateLocalized(text);
    /* (4) Show the message on the question dialog */
    XtVaSetValues(question, XmNmessageString, string, NULL);
    XmStringFree(string);
    XtManageChild(question);
}

/* Create menu bar and pulldown menu */
static Widget 
CreateMenu (Widget parent)
{
    Widget  menubar, filemenu;

    /* Create menu bar */
    menubar = XmVaCreateSimpleMenuBar(parent, "menubar",
                  XmVaCASCADEBUTTON, NULL, NULL,  /* "File" menu */
                  NULL);
    XtManageChild(menubar);

    /* Create "File" pulldown menu */
    filemenu = XmVaCreateSimplePulldownMenu(menubar, "filemenu", 0, FileMenuCB,
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,  /* "New" */
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,  /* "Open" */
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,  /* "Save" */
                  XmVaSEPARATOR,  /* Separator */
                  XmVaPUSHBUTTON, NULL, NULL, NULL, NULL,  /* "Exit" */
                  NULL);

    return(menubar);
}

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, mainwin, menubar, work, text;
    Arg    al[20];
    int    ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Switchmsg", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* (1) Open message catalog */
    catd = catopen("switchmsg", NL_CAT_LOCALE);

    ac = 0;
    XtSetArg(al[ac], XmNshowSeparator, True); ac++;
    mainwin = XmCreateMainWindow(toplevel, "mainwin", al, ac);
    XtManageChild(mainwin);

    menubar = CreateMenu(mainwin); /* Create menu */

    /* Create work area */
    ac = 0;
    work = XmCreateRowColumn(mainwin, "work", al, ac);
    XtManageChild(work);
    ac = 0;
    text = XmCreateText(work, "text", al, ac);
    XtManageChild(text);

    XmMainWindowSetAreas(mainwin, menubar, NULL, NULL, NULL, work);

    /* Create question dialog */
    ac = 0;
    question = XmCreateQuestionDialog(toplevel, "question", al, ac);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
