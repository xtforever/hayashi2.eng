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

/* Resource converter: Convert bitmap file name to Pixmap */
Boolean 
CvtStrToBitmap (Display *display, XrmValue *args, Cardinal *num_args, XrmValue *from, XrmValue *to, XtPointer *converter_data)
{
    Screen *screen = *((Screen**)args[0].addr); /* (1) Screen */
    Widget widget = *((Widget*)args[1].addr);   /* (2) Widget ID */
    char *bitmap_file = from->addr;  /* (3) Bitmap file name */
    Pixel fg, bg;
    Pixmap bitmap; /* pixmap created from bitmap file */

    /* (4) Inquire color structure of the widget */
    XtVaGetValues(widget, XmNforeground, &fg,
                          XmNbackground, &bg, NULL);

    /* (5) Create pixmap in the same color structure as widget */
    bitmap = XmGetPixmap(screen, bitmap_file, fg, bg);

    to->size = sizeof(Pixmap);       /* Return size of pixmap */

    if (bitmap == XmUNSPECIFIED_PIXMAP) { /* (6) failed to create pixmap */
        *(Pixmap *)(to->addr) = XtUnspecifiedPixmap;
        return False;  /* Failed to convert */
    }
    else { /* (7) Success to convert pixmap */
        *(Pixmap *)(to->addr) = bitmap;  /* Return the created pixmap */
        return True;  /* Success to convert */
    }
}
