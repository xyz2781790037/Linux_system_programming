#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int mydup(int oldfd)
{
    int flags;
    flags = fcntl(oldfd,F_DUPFD,0);
    if(flags == -1)
    {
        errExit("fcntl");
    }
    return flags;
}
int mydup2(int oldfd, int newfd)
{
    int flag, test, result;
    test = fcntl(newfd, F_GETFD);
    if(test >= 0)
    {
        close(newfd);
    }
    else if(test == -1 && errno != EBADF)
    {
        return -1;
    }
    if (oldfd == newfd)
    {
        flag = fcntl(oldfd, F_GETFL);
        if(flag == -1)
        {
            errno = EBADF;
            return -1;
        }
        return newfd;
    }
    result = fcntl(oldfd, F_DUPFD,newfd);
    if(result == -1)
    {
        perror("fcntl F_DUPFD");
    }
    return result;
}