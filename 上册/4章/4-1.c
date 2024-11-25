#include <stdio.h>
#include <stdlib.h>
void copy_file(const char *sour, const char *dest)
{
    FILE *source, *destination;
    char buffer[1024];
    source = fopen(sour, "rb");
    if (source == NULL)
    {
        perror("Error opening source file");
        exit(1);
    }
    destination = fopen(dest, "wb");
    if (destination == NULL)
    {
        perror("Error opening destination file");
        fclose(source);
        exit(1);
    }
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), source)) > 0)//>优先级大于=
    {
        if(n < sizeof(buffer))
        {
            printf("read %zu bytes\n", n);
        }
        fwrite(buffer, 1, n, destination);
    }
    fclose(source);
    fclose(destination);
    printf("File copied from %s to %s\n", sour, dest);
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("%s is error", argv[0]);
        exit(1);
    }
    copy_file(argv[1], argv[2]);
    return 0;
}