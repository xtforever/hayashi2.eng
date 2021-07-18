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

/* (1) Define representation type resource name */
#define XhRLocation "Location"

/* (2) Define string values for representation types (specified in resource file) */
static String loc_names[] = {"left", "center", "right"};

/* (3) Define resource values (used in program) */
enum {LEFT, CENTER, RIGHT};
static unsigned char loc_vals[] = {LEFT, CENTER, RIGHT};

typedef struct {  /* (5) Resource value structure */
    unsigned char  location;
} CustomRec, *CustomPtr;

static XtResource res[] = {  /* (5) Resource structure */
  {"location", "Location", XhRLocation,
   sizeof(unsigned char), XtOffsetOf(CustomRec, location),
   XtRImmediate, (XtPointer) CENTER}, /* Default is center */
};

static XrmOptionDescRec opt[] = {  /* (6) Custom options */
    {"-left",   "*location", XrmoptionNoArg, "left"},
    {"-center", "*location", XrmoptionNoArg, "center"},
    {"-right",  "*location", XrmoptionNoArg, "right"},
};

int 
main (int argc, char **argv)
{
    XtAppContext app_context;
    Widget toplevel, panel, button;
    Arg    al[20];
    int    ac;
    XmRepTypeId reptype;
    CustomRec data;

    XtSetLanguageProc(NULL, NULL, NULL);
    ac = 0;                        /* (6) Custom option list and number */
    toplevel = XtAppInitialize(&app_context, "Reptype", opt, XtNumber(opt),
                               &argc, argv, NULL, al, ac);
    /* (4) Register representation type resource */
    reptype = XmRepTypeRegister(XhRLocation, loc_names, loc_vals,
                                XtNumber(loc_names));
    /* (5) Get resource values */
    XtGetApplicationResources(toplevel, &data, res, XtNumber(res), NULL, 0);

    ac = 0;
    panel = XmCreateBulletinBoard(toplevel, "panel", al, ac);
    XtManageChild(panel);

    ac = 0;
    switch (data.location) {
        case LEFT:   XtSetArg(al[ac], XmNx,  50); ac++; break;
        case CENTER: XtSetArg(al[ac], XmNx, 150); ac++; break;
        case RIGHT:  XtSetArg(al[ac], XmNx, 250); ac++; break;
    }
    button = XmCreatePushButton(panel, "button", al, ac);
    XtManageChild(button);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
}
