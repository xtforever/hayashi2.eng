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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main()
{
    int i;
    for (i = 0; i <= 100; i += 5) {
        fprintf(stdout, "%d\n", i);  /* print value to standard out */
        fflush(stdout);              /* flush the output buffer */
        sleep(1);                    /* repeat after sleep */
    }
}
