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
        else if(mode & st.st_gid)
    }
}