#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2];
    int p2[2];
    char *buf = "a";

    // create two pipes for each direction
    // p1: parent -> child
    // p2: child -> paren
    pipe(p1);
    pipe(p2);

    // child
    if (fork() == 0)
    {
        if (read(p1[0], buf, 1))
        {
            printf("%d: received ping\n", getpid());

            write(p2[1], buf, 1);
            close(p2[1]);

            exit(0);
        }
        exit(1);
    }
    else
    {
        write(p1[1], buf, 1);
        close(p1[1]);
        if (read(p2[0], buf, 1))
        {
            printf("%d: received pong\n", getpid());
            exit(0);
        }
        exit(1);
    }
}