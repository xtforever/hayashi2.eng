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

/* Resource Converter: Convert a string into multi line compound string */
/*                       Insert new line at "\n" */
Boolean CvtStrToMultiLineXmString(display, args, num_args, from, to, data)
    Display   *display;
    XrmValue  *args;
    Cardinal  *num_args;
    XrmValue  *from;
    XrmValue  *to;
    XtPointer *data;
{
    char     *string = from->addr;  /* string containing "\n" */
    wchar_t   wcs_str[BUFSIZ + 1], *wcs_ptr, *wcs_token, wcs_newline[BUFSIZ];
    size_t    length;
    char      token[BUFSIZ + 1];
    XmString  whole = NULL, oneline, work, newline;

    to->size = sizeof(XmString);
    *(XmString *)(to->addr) = NULL;

    /* (1) Convert original multi byte string to wide character string */
    length = mbstowcs(wcs_str, string, BUFSIZ);
    if (length == (size_t)-1 || length == 0)
        return False;

    /* (2) Convert "\n" to wide character */
    length = mbstowcs(wcs_newline, "\n", BUFSIZ-1);
    if (length == (size_t)-1 || length == 0)
        return False;

    wcs_ptr = wcs_str;
    newline = XmStringSeparatorCreate();

    /* (3) Repeat at "\n" */
    while ((wcs_token = wcstok(wcs_ptr, wcs_newline)) != NULL) {
        wcs_ptr = NULL;
        /* (4) Convert back the wide character string to multi byte string */
        length = wcstombs(token, wcs_token, BUFSIZ);
        if (length == (size_t)-1 || length == 0)
            return False;

        /* Separator is not necessary before first line */
        if (whole == NULL) {
            whole = XmStringCreateLocalized(token);
            continue;
        }

        /* (5) Insert a separator before the second and following lines */
        work = whole;
        whole = XmStringConcat(work, newline);
        XmStringFree(work);

        /* (6) Append current line to the whole compound string */
        work = whole;
        oneline = XmStringCreateLocalized(token);
        whole = XmStringConcat(work, oneline);
        XmStringFree(work);
        XmStringFree(oneline);
    }
    XmStringFree(newline);

    *(XmString *)(to->addr) = whole;
    return True;
}
