#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(char *buf, int argc, char *argv[])
{
    // append the extra string arg into argv
    // argv[0]: xargs
    // argv[1]: echo, eg.
    // argv[argc]: \0
    char *nargv[argc + 1];
    memcpy(nargv, argv + 1, (argc - 1) * sizeof(char *));
    nargv[argc - 1] = buf;
    nargv[argc] = '\0'; // null terminate the nargv

    int pid = fork();

    if (pid == 0)
    {
        exec(nargv[0], nargv);
    }
    else
    {
        wait((int *)0);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(2, "xargs: need more argument!\n");
        exit(1);
    }
    if (argc > MAXARG - 1)
    {
        fprintf(2, "xargs: too many argument!\n");
        exit(1);
    }

    char buf[512];
    char *p = buf;

    // continuing read the data until a deliminator
    while (read(0, p, sizeof(char)) == sizeof(char))
    {
        if (*p == '\n')
        {
            // null terminate the string!
            *p = '\0';
            xargs(buf, argc, argv);

            // after complete the current exec, back to the start
            p = buf;
        }
        else
        {
            p++;
        }
    }
    exit(0);
}
