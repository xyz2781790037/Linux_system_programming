// #include<sys/stat.h>
// #include<fcntl.h>
// #include<unistd.h>
// #include<stdio.h>
// #include<stdlib.h>
// void tee(const char* dest)
// {
//     int destination;
//     char buffer[1024];
//     fgets(buffer, 1024, stdin);
//     destination = open(dest, O_WRONLY | O_CREAT | O_APPEND,0644);//0644文件所有者具有读写权利，其他只读
//     size_t n;
//     write(destination, buffer, 1024);

//     close(destination);
//     printf("File write to %s\n", dest);
// }
// int main(int argc,char*argv[])
// {
//     if(argc != 2)
//     {
//         printf("%s is error.\n", argv[0]);
//         exit(1);
//     }
//     tee(argv[1]);
//     return 0;
// }
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void tee(const char *dest)
{
    int destination;
    char buffer[1024];
    ssize_t bytes_read;

    // 读取用户输入，避免多余的空白字符
    bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1); // 读入标准输入

    if (bytes_read < 0)
    {
        perror("Failed to read input");
        exit(1);
    }

    // 确保读取到的字符串以 '\0' 结束
    buffer[bytes_read] = '\0';

    // 打开文件进行写操作（如果文件不存在，则创建并写入；如果存在，则追加内容）
    destination = open(dest, O_WRONLY | O_CREAT | O_APPEND, 0644); // 0644: 用户读写权限，其他用户只读

    if (destination < 0)
    {
        perror("Failed to open file");
        exit(1);
    }

    // 写入读取到的内容
    if (write(destination, buffer, bytes_read) < 0)
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
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    tee(argv[1]);
    return 0;
}
