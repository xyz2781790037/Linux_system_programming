#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void tee(const char *dest, int argc)
{
    int destination;
    char buffer[1024];
    ssize_t n;
    n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (n < 0)
    {
        perror("Failed to read input");
        exit(1);
    }
    if(n > 0 && buffer[n - 1] == '\n')
    {
        n--;
    }
    buffer[n] = '\0';
    if (argc == 2)
    {
        destination = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644); // 0644: 用户读写权限，其他用户只读
    }
    else
    {
        destination = open(dest, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }
    if (destination < 0)
    {
        perror("Failed to open file");
        exit(1);
    }
    if (write(destination, buffer, n) < 0)
    {
        perror("Failed to write to file");
        close(destination);
        exit(1);
    }
    close(destination);
    printf("Data written to %s\n", dest);
}
int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        printf("%s is error\n", argv[0]);
        exit(1);
    }
    if (argc == 2)
    {
        tee(argv[1], argc);
    }
    else if (argc == 3)
    {
        tee(argv[2], argc);
    }
    return 0;
}