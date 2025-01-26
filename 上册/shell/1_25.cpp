#include <iostream>
#include <unistd.h>
#include <deque>
#include <sys/wait.h>
#include <cstring>
using namespace std;
string order, orders;
char *args[1024];
void getcurrentdir()
{
    char path[1024];
    if (getcwd(path, sizeof(path)) != nullptr)
    {
        cout << "\033[32m➜  \033[0m";
        string cwd;
        cwd.assign(path);
        size_t pathpos = cwd.find_last_of('/');
        string m_Name = cwd.substr(pathpos + 1);
        cout << "\033[1;36m" << m_Name << "\033[0m" << " ";
    }
    else
    {
        cerr << "Error getting current directory!" << endl;
    }
}
int main()
{
    char filename[1024];
    while (1)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            cerr << "Fork failed!" << endl;
            return 1;
        }
        else if (pid >= 0)
        {
            getcurrentdir();
            getline(cin, orders);
            order = "/usr/bin/";
            order += orders;
            int index = 0;
            for (char s : order)
            {
                if (s == ' ')
                {
                    break;
                }
                filename[index++] = s;
            }
            filename[index] = '\0';
            size_t orderpos = order.find_first_of(' ');
            order = order.substr(orderpos + 1);
            int i = 0, j = 0;
            for (char c : order)
            {
                if (c == ' ')
                {
                    i++, j = 0;
                }
                else
                {
                    args[i][j++] = c;
                }
            }
            args[i] = nullptr;
            if(execv(filename, args) == -1)
            {
                perror("execvp");
            }
            order.clear();
            memset(filename, '\0', strlen(filename));
        }
        // else if(pid > 0)
        // {
        //     wait(nullptr);
        //     cout << "wait" << endl;
        // }
    }
    return 0;
}