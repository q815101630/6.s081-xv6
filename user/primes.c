#include "kernel/types.h"
#include "user/user.h"

int primes(int p1[2])
{

    int p;
    int n;

    int sub_id = fork();

    // if subprocess
    if (sub_id == 0)
    {
        // see if there is remaining number
        int p_size = read(p1[0], &p, sizeof(int));

        // No number left, base case
        if (p_size == 0)
        {
            close(p1[0]);
            close(p1[1]);
            exit(0);
        }

        // otherwise, first must be prime, pirnt
        printf("prime %d\n", p);

        // IMPORTANT: create new pipe only in the subprocess
        int p2[2];
        pipe(p2);

        while (read(p1[0], &n, sizeof(int)) != 0)
        {
            if (n % p != 0)
            {
                write(p2[1], &n, sizeof(int));
            }
        }
        close(p1[0]); // close the last pipe (read)
        close(p2[1]); // close the current pipe (write)

        primes(p2); // recursion
        exit(0);
    }
    else
    {

        while (1)
        {
            int wait_id = wait(0);
            if (sub_id == wait_id) // only exit the current process when all subprocess finishes
            {
                break;
            }
        }
        exit(0);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int p1[2];

    close(0); // close stdin, useless

    pipe(p1);

    for (int i = 2; i <= 35; ++i)
    {
        write(p1[1], &i, sizeof(int));
    }
    close(p1[1]); // close p1 write

    primes(p1); // enter recursion

    return 0;
}