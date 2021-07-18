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

static Widget tgt_label, color_text, label_text;

/* [Apply] Button callback */
static ChangeCB(w, client_data, call_data)
    Widget    w;
    XtPointer client_data;
    XtPointer call_data;
{
    XrmValue  source, dest;           /* Resource value structure */
    char     *new_color, *new_label;  /* New color name and label (source) */
    Pixel     pixel;            /* Pixel value (destination data) */
    XmString  xmstr;            /* Compound String (destination data) */

    /* (A) Input new color name */
    new_color = XmTextFieldGetString(color_text);
    /* Without input, set focus to appropreate input area and ring a bell */
    if (new_color == NULL || *new_color == NULL) {
        XmProcessTraversal(color_text, XmTRAVERSE_CURRENT);
        XBell(XtDisplay(w), 0);
        return;
    }
    /* (B) Input new label string */
    new_label = XmTextFieldGetString(label_text);
    /* Without input, set focus to appropreate input area and ring a bell */
    if (new_label == NULL || *new_label == NULL) {
        XmProcessTraversal(label_text, XmTRAVERSE_CURRENT);
        XBell(XtDisplay(w), 0);
        return;
    }

    /* (C1) Prepare resource value structure (XrmValue) for color */
    source.size = strlen(new_color) + 1;  /* size of source data */
    source.addr = new_color;              /* source data */
    dest.size = sizeof(Pixel);     /* size of target data (integer) */
    dest.addr = &pixel;            /* storage for target data */
    /* (C2) Convert color name (string) to Pixel value */
    if (!XtConvertAndStore(w, XtRString, &source, XtRPixel, &dest)) {
        /* If conversion fail, set fucos to appropreate input area and bell */
        XmProcessTraversal(color_text, XmTRAVERSE_CURRENT);
        XBell(XtDisplay(w), 0);
        return;
    }

    /* (D1) Prepare resource value structure (XrmValue) for label string */
    source.size = strlen(new_label) + 1;  /* size of source data */
    source.addr = new_label;       /* source data */
    dest.size = sizeof(XmString);  /* size of target data (pointer) */
    dest.addr = &xmstr;             /* storage for target data */
    /* (D2) Convert label (string) to compound string (XmString) */
    if (!XtConvertAndStore(w, XtRString, &source, XmRXmString, &dest)) {
        XmProcessTraversal(label_text, XmTRAVERSE_CURRENT);
        XBell(XtDisplay(w), 0);
        return;
    }

    /* (E) Set color value (Pixel) and label string  (XmString) to the Label */
    XtVaSetValues(tgt_label, XmNbackground, pixel, NULL);
    XtVaSetValues(tgt_label, XmNlabelString, xmstr, NULL);

    /* (F) Free unnecessary data */
    XtFree(new_color);
    XtFree(new_label);
    XmStringFree(xmstr);
}

main(argc, argv)
    int  argc;
    char **argv;
{
    XtAppContext app_context;
    Widget toplevel, panel, field, button, color_title, label_title;
    Arg    al[20];
    int    ac;

    XtSetLanguageProc(NULL, NULL, NULL);

    ac = 0;
    toplevel = XtAppInitialize(&app_context, "Resconvert", NULL, 0,
                               &argc, argv, NULL, al, ac);

    /* Panel for background */
    ac = 0;
    panel = XmCreateRowColumn(toplevel, "panel", al, ac);
    XtManageChild(panel);

    /* Label at top (the color and string of this label will be changed) */
    ac = 0;
    tgt_label = XmCreateLabel(panel, "tgt_label", al, ac);
    XtManageChild(tgt_label);

    /* Center area */
    ac = 0;
    field = XmCreateRowColumn(panel, "field", al, ac);
    XtManageChild(field);

    /* "Background:" */
    ac = 0;
    color_title = XmCreateLabel(field, "color_title", al, ac);
    XtManageChild(color_title);
    /* Input area for new color name */
    ac = 0;
    color_text = XmCreateTextField(field, "color_text", al, ac);
    XtManageChild(color_text);

    /* "Label:" */
    ac = 0;
    label_title = XmCreateLabel(field, "label_title", al, ac);
    XtManageChild(label_title);
    /* Input area for new label string */
    ac = 0;
    label_text = XmCreateTextField(field, "label_text", al, ac);
    XtManageChild(label_text);

    /* [Apply] Button */
    ac = 0;
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);
    XtAddCallback(button, XmNactivateCallback, ChangeCB, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
