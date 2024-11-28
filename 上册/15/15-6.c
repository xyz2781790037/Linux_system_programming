#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
int main(int argc, char *argv[])
{
    struct stat st;
    for (int i = 1; i < argc; i++)
    {
        if (stat(argv[i], &st) == 0)
        {
            if (S_ISREG(st.st_mode))
            {
                chmod(argv[i], st.st_mode | S_IRGRP | S_IRUSR | S_IROTH);
            }
            else if (S_ISDIR(st.st_mode))
            {
                chmod(argv[i], st.st_mode | S_IRGRP | S_IRUSR | S_IROTH | S_IXGRP | S_IXUSR | S_IXOTH);
            }
        }
    }
    return 0;
}