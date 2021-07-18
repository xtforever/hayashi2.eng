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

static void OpenCB(), SaveCB();
static Widget text;
static Atom xa_wm_delete;

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, field, label, fname, open, save;
    Arg al[20];
    int ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Sessionsave", NULL, 0,
                               &argc, argv, NULL, al, ac);

    ac = 0;
    panel = XmCreateForm(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Input file name and open the file */
    ac = 0;
    field = XmCreateRowColumn(panel, "field", al, ac);
    XtManageChild(field);
    /* "File Name: */
    ac = 0;
    label = XmCreateLabel(field, "label", al, ac);
    XtManageChild(label);
    /* Input area */
    ac = 0;
    fname = XmCreateTextField(field, "fname", al, ac);
    XtManageChild(fname);
    /* [Open] */
    ac = 0;
    open = XmCreatePushButton(field, "open", al, ac);
    XtAddCallback(open, XmNactivateCallback, OpenCB, fname);
    XtManageChild(open);
    /* [Save] */
    ac = 0;
    save = XmCreatePushButton(field, "save", al, ac);
    XtAddCallback(save, XmNactivateCallback, SaveCB, fname);
    XtManageChild(save);

    /* text area to show the content of the file */
    ac = 0;
    text = XmCreateScrolledText(panel, "text", al, ac);
    XtManageChild(text);
    XtVaSetValues(XtParent(text), XmNtopWidget, field, NULL);

    XtRealizeWidget(toplevel);

    /* Do not restart by session manager */
    XDeleteProperty(XtDisplay(toplevel), XtWindow(toplevel), XA_WM_COMMAND);

    XtAppMainLoop(app_context);
}

/* [Open] button callback */
static void OpenCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget textf = (Widget)client_data;
    char *filename;
    FILE *fp;
    char buff[BUFSIZ];

    /* Retrieve the entered file name */
    XtVaGetValues(textf, XmNvalue, &filename, NULL);

    /* Show the content of the file on the text widget */
    if ((fp = fopen(filename, "r")) != NULL) {  /* open the file */
        XmTextSetString(text, "");  /* once clear the text */
        while (fgets(buff, BUFSIZ, fp) != NULL)
            XmTextInsert(text, XmTextGetLastPosition(text), buff); /* append */
        fclose(fp);
    }

    XtFree(filename);
}

/* [Save] button callback */
/* (3) this is also called when "Close" button is pressed */
static void SaveCB(w, client_data, call_data)
    Widget w;
    XtPointer client_data;
    XtPointer call_data;
{
    Widget textf = (Widget)client_data;
    char *filename;
    char *string;
    FILE *fp;

    /* retrieve the entered file name */
    XtVaGetValues(textf, XmNvalue, &filename, NULL);

    /* get text content */
    string = XmTextGetString(text);

    /* write the text contents into the file */
    if ((fp = fopen(filename, "w")) != NULL) {
        fputs(string, fp);
        fclose(fp);
    }

    XtFree(filename);
    XtFree(string);
}
