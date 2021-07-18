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

/* (3) Input callback */
static void InputCB(client_data, source, id)
    XtPointer  client_data; /* Client data passed when this is registered */
    int       *source;      /* file descriptor */
    XtInputId *id;          /* ID of input callback */
{
    Widget scale = (Widget)client_data;
    char buff[BUFSIZ];
    int len, val;

    len = read(*source, buff, BUFSIZ); /* (4) input from the file descriptor */
    if (len < 1) {           /* If the read fails, */
        XtRemoveInput(*id);  /* delete the input callback */
        return;              /* so that this is not called again */
    }
    if (buff[len-1] == '\n') buff[len-1] = NULL;  /* ignore last newline */

    /* (5) Set the read value to scale */
    val = atoi(buff);
    XtVaSetValues(scale, XmNvalue, val, NULL);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget  toplevel, panel, scale;
    Arg     al[20];
    int     ac;
    FILE   *fp;
    int     fd;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Inputdata", NULL, 0,
                               &argc, argv, NULL, al, ac);
    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    scale = XmCreateScale(panel, "scale", al, ac);
    XtManageChild(scale);

    /* (1) Invoke other process by pipe */
    fp = popen("outputdata", "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot start sub program.\n");
        exit(1);
    }

    fd = fileno(fp);    /* file descriptor of the pipe (fp) */
    /* (2) register input callback to the application context */
    XtAppAddInput(app_context, fd, (XtPointer)XtInputReadMask, InputCB, scale);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
