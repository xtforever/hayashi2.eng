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

#include <Xm/XmAll.h>  /* Motif header file */
static void ButtonCB();

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget  toplevel, panel, control, label, text, button;
    Arg     al[20];
    int     ac;
                /* (1) Declare the language */
    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;     /* (2) Initialize X Toolkit */
    toplevel = XtAppInitialize(&app_context, "Simple", NULL, 0,
                               &argc, argv, NULL, al, ac);

                /* (3) Create Widgets at the order of widget tree */
    ac = 0;         /* (3) Create a rowcolomn of background */
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);       /* (4) Manage */

    ac = 0;         /* (3) Create another rowcolumn of upper area */
    control = XmCreateRowColumn(panel, "control", al, ac);
    XtManageChild(control);       /* (4) Manage */

    ac = 0;             /* (3) Create label */
    label = XmCreateLabel(control, "label", al, ac);
    XtManageChild(label);        /* (4) Manage */

    ac = 0;             /* (3) Create text field */
    text = XmCreateTextField(control, "text", al, ac);
    XtManageChild(text);         /* (4) Manage */

    ac = 0;         /* (3) Create push button */
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);       /* (4) Manage */

                /* (5) Event Processing: Adding callback function */
    XtAddCallback(button, XmNactivateCallback, ButtonCB, text);

    XtRealizeWidget(toplevel);   /* (6) Realize */
    XtAppMainLoop(app_context);  /* (7) Event loop */
}

/* (5) Event Processing: Create callback function */
static void 
ButtonCB (
    Widget w,            /* Widget on which this callback was registered */
    XtPointer client_data,  /* Client data passed on registration */
    XtPointer call_data    /* Callback Struct */
)
{
    Widget text = (Widget) client_data;
    char *filename;

    filename = XmTextFieldGetString(text);  /* Retrieve the inputted text */
    printf("Entered file name: %s\n", filename);  /* Print the text */
    XtFree(filename);    /* Free unnecessary data */
}
