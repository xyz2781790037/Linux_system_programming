#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
int myaccess(const char *path, int mode)
{
    struct stat st;
    int result = stat(path, &st);
    if (result == -1)
    {
        perror("stat");
        return -1;
    }
    // 获取进程的有效用户ID和有效组ID
    uid_t muid = getuid();
    gid_t mgid = getgid();
    // 获取文件的权限位
    mode_t filemode = st.st_mode;
    mode_t file_uid = filemode & (S_IRUSR | S_IWUSR | S_IXUSR);
    mode_t file_gid = filemode & (S_IRGRP | S_IWGRP | S_IRGRP);
    mode_t file_other = filemode & (S_IROTH | S_IWOTH | S_IXOTH);
    if (!(mode & F_OK))
    {
        return -1;
    }
    if (mode & R_OK)
    {
        if (muid == st.st_uid)
        {
            if (!(file_uid & S_IRUSR))
            {
                errno = EACCES;
                return -1;
            }
        }
        else if (mgid == st.st_gid)
        {
            if (!(file_gid & S_IRGRP))
            {
                errno = EACCES;
                return -1;
            }
        }
        else
        {
            if(!(file_other & S_IROTH))
            {
                errno = EACCES;
                return -1;
            }
        }
    }
    if (mode & W_OK)
    {
        if (muid == st.st_uid)
        {
            if (!(file_uid & S_IWUSR))
            {
                errno = EACCES;
                return -1;
            }
        }
        else if (mgid == st.st_gid)
        {
            if (!(file_gid & S_IWGRP))
            {
                errno = EACCES;
                return -1;
            }
        }
        else
        {
            if (!(file_other & S_IWOTH))
            {
                errno = EACCES;
                return -1;
            }
        }
    }
    if (mode & X_OK)
    {
        if (muid == st.st_uid)
        {
            if (!(file_uid & S_IXUSR))
            {
                errno = EACCES;
                return -1;
            }
        }
        else if (mgid == st.st_gid)
        {
            if (!(file_gid & S_IXGRP))
            {
                errno = EACCES;
                return -1;
            }
        }
        else
        {
            if (!(file_other & S_IXOTH))
            {
                errno = EACCES;
                return -1;
            }
        }
    }
    return 0;
}