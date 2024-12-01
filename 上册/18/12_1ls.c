// #define _GNU_SOURCE /* 使用versionsort需要加上该宏定义 */
#define _DEFAULT_SOURCE
#define MAX_FILENAME_LENGTH 20
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>

struct dirent
{
#ifndef __USE_FILE_OFFSET64
    __ino_t d_ino; /* inode number 索引节点号 */
    __off_t d_off; /* offset to this dirent 在目录文件中的偏移 */
#else
    __ino64_t d_ino;
    __off64_t d_off;
#endif
    unsigned short d_reclen; /* length of this d_name 文件名长 */
    unsigned char d_type;    /* the type of d_name 文件类型 */
    char d_name[256];        /* file name (null-terminated) 文件名，最长255字符 */
};

char mylsmk[7];
int filesort(const struct dirent **a, const struct dirent **b)
{
    if (strcoll((*a)->d_type, (*a)->d_type) == 0)
    {
        
    }
}
void mylsbase(struct dirent **fina)
{
    int n;
    n = scandir(".", &fina, NULL, alphasort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        if (strcmp(fina[i]->d_name, "..") != 0 && strncmp(fina[i]->d_name, ".", 1))
        {
            printf("%s  ", fina[i]->d_name);
            if ((i + 1) % 4 == 0)
            {
                printf("\n");
            }
        }
        free(fina[i]);
    }
    free(fina);
}
void myls_a(struct dirent **fina)
{
    int n;
    n = scandir(".", &fina, NULL, alphasort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        printf("%s  ", fina[i]->d_name);
        free(fina[i]);
    }

    free(fina);
}
void myls_l()
{
    struct stat st;
    if (S_ISDIR(st.st_mode))
        printf("d");
    else if (S_ISLNK(st.st_mode))
        printf("l");
    else
        printf("-");
    printf((st.st_mode & S_IRUSR) ? "r" : "-");
    printf((st.st_mode & S_IWUSR) ? "w" : "-");
    printf((st.st_mode & S_IXUSR) ? "x" : "-");
    printf((st.st_mode & S_IRGRP) ? "r" : "-");
    printf((st.st_mode & S_IWGRP) ? "w" : "-");
    printf((st.st_mode & S_IXGRP) ? "x" : "-");
    printf((st.st_mode & S_IROTH) ? "r" : "-");
    printf((st.st_mode & S_IWOTH) ? "w" : "-");
    printf((st.st_mode & S_IXOTH) ? "x" : "-");

    printf("%d", st.st_size);
}
int main(int argc, char *argv[])
{
    int mark_ls;
    if (argc > 1)
    {
        if (strcmp(argv[1], "-l") == 0)
        {
            mark_ls = 1;
        }
    }
    struct dirent **fina; // filename
    if (argc == 1)
    {
        mylsbase(fina);
    }
    else
    {
        for (int i = 0; i < mark_ls; i++)
        {
            if (mark_ls)
            {
                myls_a(fina);
            }
        }
    }
    return 0;
}