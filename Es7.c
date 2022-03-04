#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {

        printf("Errore negli argomenti!");
        exit 0;
    }
    else
    {
        int p1p2[2], p2p3[2], p3p0[2]; // piope
        int p1, p2, p3;                // processi
        char buff[1024];

        pipe(p1p2);
        p1 = fork();
        if (p1 == 0)
        {
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            close(p1p2[0]);

            execl("/bin/cat", "cat", argv[1], NULL);
            return -1;
        }
        else
        {

            pipe(p2p3);
            p2 = fork();
            if (p2 == 0)
            {
                close(0);
                dup(p1p2[0]);
                close(p1p2[0]);
                close(p1p2[1]);

                close(1);
                dup(p2p3[1]);
                close(p2p3[1]);
                close(p2p3[0]);

                execl("/bin/awk", "awk", "{print $4}", NULL);
                return -1;
            }
            else
            {

                pipe(p3p0);
                p3 = fork();
                if (p3 == 0)
                {
                    close(p1p2[0]);
                    close(p1p2[1]);

                    close(0);
                    dup(p2p3[0]);
                    close(p2p3[0]);
                    close(p2p3[1]);

                    close(1);
                    dup(p3p0[1]);
                    close(p3p0[1]);
                    close(p3p0[0]);

                    execl("/bin/tail", "tail", "-n", "+2", NULL);
                    return -1;
                }
                else
                {
                    close(p1p2[0]);
                    close(p1p2[1]);
                    close(p2p3[0]);
                    close(p2p3[1]);
                    close(p3p0[1]);

                    write(p3p0[0], buff, sizeof(buff));
                    close(p3p0[0]);

                    return 0;
                }
            }
        }
    }
}