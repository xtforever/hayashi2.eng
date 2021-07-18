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
/* Convert array of strings to multi line compound string */
XmString StringListToXmString(text, n)
    char **text;  /* array of strings */
    int n;        /* number of strings (number of lines) */
{
    int i;
    XmString  whole, oneline, work, separator;

    if (n < 1) return NULL;
    /* (1) separator is not necessary before the first line */
    whole = XmStringCreateLocalized(text[0]);
    if (n == 1) return whole;

    /* (2) Following codes are done after second lines */
    separator = XmStringSeparatorCreate(); /* (3) Create a separator */
    for (i = 1; i < n; i++) {
        /* (4) Insert separator before second and following line */
        work = whole;
        whole = XmStringConcat(work, separator);
        XmStringFree(work); /* (6) free original compound string */

        /* (5) Append current line aftre the while compound string */
        work = whole;
        oneline = XmStringCreateLocalized(text[i]);
        whole = XmStringConcat(work, oneline);
        XmStringFree(work); /* (6) free original compound string */
        XmStringFree(oneline); /* (6) free original compound string */
    }
    XmStringFree(separator);
    return whole;
}
