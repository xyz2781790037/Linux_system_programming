#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main()
{
    // 保存当前 umask
    mode_t old_umask = umask(0); // 设置 umask 为 0 以读取当前 umask，但不会更改原值
    umask(old_umask);            // 恢复原来的 umask

    // 打印当前 umask
    printf("Old umask: %03o\n", old_umask);

    return 0;
}
