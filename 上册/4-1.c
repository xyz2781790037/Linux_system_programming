#include <sys/stat.h>
#include<fcntl.h>
#include "tlpi_hdr.h"

//防止头文件重复包含
#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc,char*argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    __ssize_t numRead;
}