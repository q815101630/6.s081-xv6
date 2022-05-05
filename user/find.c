#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *getname(char *path)
{
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    return p;
}

void find(char *path, char *name)
{
    int fd;
    struct stat st;
    char buf[512], *p;
    struct dirent de;

    // read the current path and retrieve stat
    fd = open(path, 0);
    fstat(fd, &st);

    switch (st.type)
    {
    case T_FILE:
        // base case: it is a file
        if (strcmp(getname(path), name) == 0)
        {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        // recurse if it is directory
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            // ignore . ..
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, name);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        fprintf(2, "You must enter only two arguments!\n");
        exit(1);
    }
    else
    {
        int fd;
        struct stat st;

        // 1) the file (directory) cannot be opened
        // Read 0 byte
        if ((fd = open(argv[1], 0)) < 0)
        {
            fprintf(2, "find: cannot open %s\n", argv[1]);
            exit(1);
        }
        // 2) the file (directory) cannot be stat
        if (fstat(fd, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", argv[1]);
            close(fd);
            exit(1);
        }
        // 3) it must be a directory
        if (st.type != T_DIR)
        {
            fprintf(2, "find: %s must be a directory\n", argv[1]);
            exit(1);
        }

        // Enter recursion
        find(argv[1], argv[2]);
        exit(0);
    }
}