#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <cstring>
using namespace std;

string orders;
vector<char *> args;

void getcurrentdir()
{
    char path[1024];
    if (getcwd(path, sizeof(path)) != nullptr)
    {
        cout << "\033[32m➜  \033[0m";
        string cwd = path;
        size_t pathpos = cwd.find_last_of('/');
        string m_Name = (pathpos != string::npos) ? cwd.substr(pathpos + 1) : cwd;
        cout << "\033[1;36m" << m_Name << "\033[0m ";
    }
    else
    {
        cerr << "Error getting current directory!" << endl;
    }
}

void parseCommand()
{
    args.clear();
    size_t start = 0;
    size_t end = 0;
    bool inArg = false;

    while (end <= orders.size())
    {
        if (end == orders.size() || orders[end] == ' ')
        {
            if (inArg)
            {
                string token = orders.substr(start, end - start);
                char *arg = new char[token.size() + 1];
                strcpy(arg, token.c_str());
                args.push_back(arg);
                inArg = false;
            }
            if (end != orders.size())
                start = end + 1;
        }
        else if (!inArg)
        {
            start = end;
            inArg = true;
        }
        end++;
    }
    args.push_back(nullptr);
}

int main()
{
    while (1)
    {
        getcurrentdir();
        getline(cin, orders);
        if (orders.empty())
            continue;

        parseCommand();

        // 构造完整路径（这里简化处理，实际应搜索PATH）
        string full_path = "/usr/bin/" + string(args[0]);
        char *filename = new char[full_path.size() + 1];
        strcpy(filename, full_path.c_str());

        pid_t pid = fork();
        if (pid < 0)
        {
            cerr << "Fork failed!" << endl;
            return 1;
        }
        else if (pid == 0)
        {
            if (execv(filename, args.data()) == -1)
            {
                perror("execv");
            }
            exit(1);
        }
        else
        {
            wait(nullptr);
        }

        // 清理内存
        for (char *arg : args)
        {
            if (arg)
                delete[] arg;
        }
        delete[] filename;
    }
    return 0;
}