#include <unistd.h>
#include <stdio.h>

int main (void)
{
    int i;
    for (i = 0; i <= 100; i += 5) {
        fprintf(stdout, "%d\n", i);  /* print value to standard out */
        fflush(stdout);              /* flush the output buffer */
        sleep(1);                    /* repeat after sleep */
    }
}
