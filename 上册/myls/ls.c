#define _DEFAULT_SOURCE
#define S_ISREG
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
#include <ctype.h>
int filesort_a(const struct dirent **a, const struct dirent **b)
{
    char aw[256], bw[256];
    if ((*a)->d_name[0] == '.' && strcmp((*a)->d_name, ".") != 0 && strcmp((*a)->d_name, "..") != 0)
    {
        strncpy(aw, (*a)->d_name + 1, strlen((*a)->d_name) - 1);
        aw[strlen((*a)->d_name) - 1] = '\0';
    }
    else
    {
        strncpy(aw, (*a)->d_name, strlen((*a)->d_name));
        aw[strlen((*a)->d_name)] = '\0';
    }
    if ((*b)->d_name[0] == '.' && strcmp((*b)->d_name, ".") != 0 && strcmp((*b)->d_name, "..") != 0)
    {
        strncpy(bw, (*b)->d_name + 1, strlen((*b)->d_name) - 1);
        bw[strlen((*b)->d_name) - 1] = '\0';
    }
    else
    {
        strncpy(bw, (*b)->d_name, strlen((*b)->d_name));
        bw[strlen((*b)->d_name)] = '\0';
    }
    return strcasecmp(aw, bw);
}
int filesort(const struct dirent **a, const struct dirent **b)
{
    return strcasecmp((*a)->d_name, (*b)->d_name);
}
void mylsbase(struct dirent **fina, const char *c)
{
    int n;
    struct stat st;
    if (stat(c, &st) == -1)
    {
        perror("文件不存在或无法访问");
        return;
    }
    if (S_ISREG(st.st_mode))
    {
        printf("%s", c);
    }
    else
    {
        n = scandir(c, &fina, NULL, filesort);
        if (n == -1)
        {
            perror("scandir");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < n; i++)
        {
            if (strcmp(fina[i]->d_name, "..") != 0 && strncmp(fina[i]->d_name, ".", 1))
            {
                printf("%s\n", fina[i]->d_name);
            }
            free(fina[i]);
        }
        free(fina);
    }
}
void myls_a(struct dirent **fina)
{
    int n;
    n = scandir(".", &fina, NULL, filesort_a);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", fina[i]->d_name);
        free(fina[i]);
    }
    free(fina);
}
void myls_s(struct dirent **file) {}
void myls_l(struct dirent **file) {}
void myls_t(struct dirent **file) {}
void myls_r(struct dirent **file) {}
void myls_i(struct dirent **file) {}
int main(int argc, char *argv[])
{
    int c;
    char s[10];
    struct dirent **file;
    if (argc == 1)
    {
        mylsbase(file, ".");
    }
    else if (argc == 2 && argv[1][0] != '-')
    {
        mylsbase(file, argv[1]);
    }
    else
    {
        while ((c = getopt(argc, argv, "a::l::t::r::i::s::")) != -1)
        {
            switch (c)
            {
            case 'a':
                myls_a(file);
                break;
            case 'l':
                myls_l(file);
                break;
            case 't':
                myls_t(file);
                break;
            case 'r':
                myls_r(file);
                break;
            case 'i':
                myls_i(file);
                break;
            case 's':
                myls_s(file);
                break;
            }
        }
    }
    return 0;
}