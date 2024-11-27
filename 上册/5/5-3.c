#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void tee(const char *dest, int num_byte, int argc)
{
    int destination;
    char buffer = 'a';
    ssize_t n;
    if (argc == 3)
    {
        destination = open(dest, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    }
    else if (argc == 4)
    {
        destination = open(dest, O_WRONLY | O_CREAT, 0644);
        
    }
    if (destination < 0)
    {
        perror("Failed to open file");
        exit(1);
    }
    for (int i = 0; i < num_byte;i++)
    {
        if(argc == 4)
        {
            if(lseek(destination, 0, SEEK_END) < 0)
            {
                perror("Fail to seek the file");
            }
        }
        if (write(destination, &buffer, 1) < 0)
        {
            perror("Failed to write to file");
            close(destination);
            exit(1);
        }
    }
    close(destination);
    printf("Data written to %s\n", dest);
}
int main(int argc, char *argv[])
{
    int num;
    sscanf(argv[2], "%d", &num);
    tee(argv[1], num, argc);
    return 0;
}