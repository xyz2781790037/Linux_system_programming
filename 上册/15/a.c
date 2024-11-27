#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

// 自定义的权限检查函数
int my_access(const char *path, int mode)
{
    struct stat st;
    int result = stat(path, &st); // 获取文件状态
    if (result == -1)
    {
        if (errno == ENOENT && (mode & F_OK))
        {
            // 如果文件不存在并且请求了 F_OK，返回 -1
            errno = ENOENT;
            return -1;
        }
        perror("stat");
        return -1; // 如果文件不存在或其他错误，返回 -1
    }

    // 获取进程的有效用户ID和有效组ID
    uid_t euid = geteuid(); // 获取有效用户ID
    gid_t egid = getegid(); // 获取有效组ID

    // 获取文件的权限位
    mode_t file_mode = st.st_mode;
    mode_t file_uid_mode = file_mode & (S_IRUSR | S_IWUSR | S_IXUSR);   // 文件所有者的权限
    mode_t file_gid_mode = file_mode & (S_IRGRP | S_IWGRP | S_IXGRP);   // 文件所属组的权限
    mode_t file_other_mode = file_mode & (S_IROTH | S_IWOTH | S_IXOTH); // 其他用户的权限

    // 如果 mode 中包含 F_OK，检查文件是否存在
    if (mode & F_OK)
    {
        return 0; // 如果文件存在，直接返回 0
    }

    // 通过有效的用户和组ID来检查文件的权限
    if (mode & R_OK)
    { // 检查读权限
        if (euid == st.st_uid)
        {
            // 如果进程的有效用户ID与文件所有者相同
            if (!(file_uid_mode & S_IRUSR))
            {
                errno = EACCES;
                return -1;
            }
        }
        else if (egid == st.st_gid)
        {
            // 如果进程的有效组ID与文件所属组相同
            if (!(file_gid_mode & S_IRGRP))
            {
                errno = EACCES;
                return -1;
            }
        }
        else
        {
            // 其他用户
            if (!(file_other_mode & S_IROTH))
            {
                errno = EACCES;
                return -1;
            }
        }
    }

    if (mode & W_OK)
    { // 检查写权限
        if (euid == st.st_uid)
        {
            // 如果进程的有效用户ID与文件所有者相同
            if (!(file_uid_mode & S_IWUSR))
            {
                errno = EACCES;
                return -1;
            }
        }
        else if (egid == st.st_gid)
        {
            // 如果进程的有效组ID与文件所属组相同
            if (!(file_gid_mode & S_IWGRP))
            {
                errno = EACCES;
                return -1;
            }
        }
        else
        {
            // 其他用户
            if (!(file_other_mode & S_IWOTH))
            {
                errno = EACCES;
                return -1;
            }
        }
    }

    if (mode & X_OK)
    { // 检查执行权限
        if (euid == st.st_uid)
        {
            // 如果进程的有效用户ID与文件所有者相同
            if (!(file_uid_mode & S_IXUSR))
            {
                errno = EACCES;
                return -1;
            }
        }
        else if (egid == st.st_gid)
        {
            // 如果进程的有效组ID与文件所属组相同
            if (!(file_gid_mode & S_IXGRP))
            {
                errno = EACCES;
                return -1;
            }
        }
        else
        {
            // 其他用户
            if (!(file_other_mode & S_IXOTH))
            {
                errno = EACCES;
                return -1;
            }
        }
    }

    return 0; // 如果权限检查通过，返回0
}

int main()
{
    const char *file_path = "example.txt";

    // 检查文件是否存在
    if (my_access(file_path, F_OK) == 0)
    {
        printf("File '%s' exists.\n", file_path);
    }
    else
    {
        printf("File '%s' does not exist. Error: %s\n", file_path, strerror(errno));
    }

    // 检查文件是否可读
    if (my_access(file_path, R_OK) == 0)
    {
        printf("File '%s' is readable.\n", file_path);
    }
    else
    {
        printf("File '%s' is not readable. Error: %s\n", file_path, strerror(errno));
    }

    // 检查文件是否可写
    if (my_access(file_path, W_OK) == 0)
    {
        printf("File '%s' is writable.\n", file_path);
    }
    else
    {
        printf("File '%s' is not writable. Error: %s\n", file_path, strerror(errno));
    }

    // 检查文件是否可执行
    if (my_access(file_path, X_OK) == 0)
    {
        printf("File '%s' is executable.\n", file_path);
    }
    else
    {
        printf("File '%s' is not executable. Error: %s\n", file_path, strerror(errno));
    }

    return 0;
}
