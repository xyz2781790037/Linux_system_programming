#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include <sstream>
#include <fcntl.h>
using namespace std;

string order;
vector<char *> args[100];
char path[1024];
char last_time_path[1024];
int pipecount, pipes[100][2], argscount;
pid_t pids[100];

void getcurrentdir()
{
    order.clear();
    if (getcwd(path, sizeof(path)) != nullptr)
    {
        cout << "\033[32m➜  \033[0m";
        string cwd(path);
        size_t pathpos = cwd.find_last_of('/');
        string m_Name = (pathpos != string::npos) ? cwd.substr(pathpos + 1) : cwd;
        cout << "\033[1;36m" << m_Name << "\033[0m ";
    }
    else
    {
        perror("getcwd");
    }
}
// 修改后的删除函数（通过索引直接删除）
void erase_args(int count, int index)
{
    delete[] args[count][index]; // 释放内存
    args[count].erase(args[count].begin() + index);
}

// 改进后的重定向处理
void redirect(int count)
{
    for (int i = 0; (int)i < args[count].size(); i++)
    {
        if (args[count][i] == nullptr)
        {
            i++;
            continue;
        }
        if (strcmp(args[count][i], "<") == 0)
        {
            int fd = open(args[count][i + 1], O_RDONLY, 0644);
            dup2(fd, STDIN_FILENO);
            close(fd);
            erase_args(count, i + 1);
            erase_args(count, i);
            continue;
        }
        else if (strcmp(args[count][i], ">") == 0)
        {
            int fd = open(args[count][i + 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            erase_args(count, i + 1);
            erase_args(count, i);
            continue;
        }
        else if (strcmp(args[count][i], "2>") == 0)
        {
            int fd = open(args[count][i + 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDERR_FILENO);
            close(fd);
            erase_args(count, i + 1);
            erase_args(count, i);
            continue;
        }
        else if (strcmp(args[count][i], ">>") == 0)
        {
            int fd = open(args[count][i + 1], O_RDWR | O_APPEND | O_CREAT, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            erase_args(count, i + 1);
            erase_args(count, i);
            continue;
        }
    }
}
vector<string> splitCommands(const string &input)
{
    vector<string> commands;
    size_t start = 0;
    size_t end = 0;
    while ((end = input.find('|', start)) != string::npos)
    {
        commands.push_back(input.substr(start, end - start));
        start = end + 1;
        while (start < input.size() && input[start] == ' ')
            start++;
    }
    commands.push_back(input.substr(start));
    return commands;
}
void segstr(int count, const string &cmd)
{
    args[count].clear();
    istringstream iss(cmd);
    string token;
    while (iss >> token)
    {
        char *arg = new char[token.size() + 1];
        strcpy(arg, token.c_str());
        args[count].push_back(arg);
    }
    args[count].push_back(nullptr);
}
void cdcommit()
{
    string target = order.substr(2);
    size_t start = target.find_first_not_of(' ');
    if (start == string::npos)
        target = "";
    else
        target = target.substr(start);

    char current[1024];
    getcwd(current, sizeof(current));

    if (target.empty() || target == "~")
    {
        target = getenv("HOME");
    }
    else if (target == "-")
    {
        if (chdir(last_time_path) != 0)
            perror("cd");
        return;
    }

    if (chdir(target.c_str()) == -1)
    {
        perror("cd");
    }
    else
    {
        strcpy(last_time_path, current);
    }
}

void space_kg()
{
    string cleaned;
    bool inSpace = false;
    for (char c : order)
    {
        if (c == ' ')
        {
            if (!inSpace && !cleaned.empty())
            {
                cleaned += ' ';
                inSpace = true;
            }
        }
        else
        {
            cleaned += c;
            inSpace = false;
        }
    }
    if (!cleaned.empty() && cleaned.back() == ' ')
        cleaned.pop_back();
    order = cleaned;
}

void lscolor()
{
    if (order.find("--color=auto") == string::npos)
    {
        order += " --color=auto";
    }
}

int main()
{
    while (1)
    {
        // Reset state
        memset(pipes, 0, sizeof(pipes));
        for (auto &v : args)
            v.clear();

        getcurrentdir();
        getline(cin, order);
        space_kg();
        if (order.empty())
            continue;

        // Handle built-in commands
        if (order == "exit")
            break;
        if (order.find("clear") != string::npos)
        {
            system("clear");
            continue;
        }
        if (order.find("cd") != string::npos)
        {
            cdcommit();
            continue;
        }

        // Handle ls color
        if (order.find("ls") != string::npos)
            lscolor();

        // Split commands
        vector<string> commands = splitCommands(order);
        argscount = commands.size();
        pipecount = argscount - 1;

        // Create pipes
        for (int i = 0; i < pipecount; ++i)
        {
            if (pipe(pipes[i]) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // Parse arguments
        for (int i = 0; i < argscount; ++i)
        {
            segstr(i, commands[i]);
        }

        // Create processes
        for (int i = 0; i < argscount; ++i)
        {
            pids[i] = fork();
            if (pids[i] == 0)
            { // Child
                // Input redirection
                if (i > 0)
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }

                // Output redirection
                if (i < pipecount)
                {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }
                redirect(i);
                // Close all pipes
                for (int j = 0; j < pipecount; ++j)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // Execute
                execvp(args[i][0], args[i].data());
                cerr << "zgsh: command not found: " << args[i][0] << endl;
                exit(EXIT_FAILURE);
            }
        }

        // Parent close all pipes
        for (int i = 0; i < pipecount; ++i)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        // Wait for children
        for (int i = 0; i < argscount; ++i)
        {
            waitpid(pids[i], NULL, 0);
        }

        // Free memory
        for (int i = 0; i < argscount; ++i)
        {
            for (char *ptr : args[i])
            {
                delete[] ptr;
            }
            args[i].clear();
        }
    }
    return 0;
}