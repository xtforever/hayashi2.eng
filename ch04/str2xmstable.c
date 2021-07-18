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

/* Resource Converter: Convert a string to array of compound strings */
/*                       Divide at "\n" */
Boolean CvtStrToXmStringTable(display, args, num_args, from, to, data)
    Display   *display;
    XrmValue  *args;
    Cardinal  *num_args;
    XrmValue  *from;
    XrmValue  *to;
    XtPointer *data;
{
    char    *string = from->addr;  /* string containing "\n" */
    wchar_t  wcs_str[BUFSIZ * 5], *wcs_ptr, *wcs_token, wcs_newline[BUFSIZ];
    size_t   length;
    char     token[BUFSIZ * 5];
    int      count = 0;
    XmStringTable table = NULL;

    to->size = sizeof(XmStringTable);
    *(XmStringTable *)(to->addr) = NULL;

    /* (1) Convert original multi byte string to wide character string */
    length = mbstowcs(wcs_str, string, BUFSIZ * 5 - 1);
    if (length == (size_t)-1 || length == 0)
        return False;

    /* (2) Convert "\n" to wide character */
    length = mbstowcs(wcs_newline, "\n", BUFSIZ - 1);
    if (length == (size_t)-1 || length == 0)
        return False;

    wcs_ptr = wcs_str;
    /* (3) Repeat at "\n" */
    while ((wcs_token = wcstok(wcs_ptr, wcs_newline)) != NULL) {
        wcs_ptr = NULL;
        count++;
        /* (4) Convert back the wide character string to multi byte string */
        length = wcstombs(token, wcs_token, BUFSIZ * 5 - 1);
        if (length == (size_t)-1 || length == 0)
            return False;
        /* (5) Enlarge the array */
        table = (XmStringTable)XtRealloc((char *)table, sizeof(XmString)*count);
        if (table == NULL)
            return False;
        /* (6) Create compound string and store it into the array */
        table[count - 1] = XmStringCreateLocalized(token);
    }

    *(XmStringTable *)(to->addr) = table;
    return True;
}
