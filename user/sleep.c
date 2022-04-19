#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(2,"Sleep failed: No argument provided!\n");
        exit(1);
    }
    else if (argc >= 3)
    {
        fprintf(2,"Sleep failed: Too many arguments provided!\n");
        exit(1);
    }

    sleep(atoi(argv[1]));
    exit(0);
}