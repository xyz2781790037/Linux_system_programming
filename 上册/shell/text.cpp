#include "shell.cpp"
#include <fcntl.h> // 增加头文件用于文件操作

// 修改1：增强参数分割逻辑
void segstr(int count)
{
    args[count].clear();
    int start = 0, end = 0;
    bool in_quote = false; // 支持引号包含的空格

    while (end <= segcmd.size())
    {
        if (segcmd[end] == '"' && (end == 0 || segcmd[end - 1] != '\\'))
        {
            in_quote = !in_quote;
        }

        if (!in_quote && (segcmd[end] == '|' || end == segcmd.size()))
        {
            // 处理管道分割
            string segment = segcmd.substr(start, end - start);
            // 移除管道符号后的空格
            if (!segment.empty() && segment.back() == ' ')
            {
                segment.pop_back();
            }
            segcmd = segcmd.substr(end + 1);
            end = 0;
            start = 0;

            // 分割参数
            istringstream iss(segment);
            string token;
            while (iss >> quoted(token))
            { // 使用quoted支持带空格参数
                char *arg = new char[token.size() + 1];
                strcpy(arg, token.c_str());
                args[count].push_back(arg);
            }
            args[count].push_back(nullptr);
            return;
        }
        end++;
    }
}

// 修改2：添加重定向处理
void redirect(int count)
{
    vector<char *> new_args;
    int flags;

    for (size_t i = 0; i < args[count].size(); ++i)
    {
        if (!args[count][i])
            continue;

        if (strcmp(args[count][i], ">") == 0)
        {
            // 输出重定向
            flags = O_WRONLY | O_CREAT | O_TRUNC;
            if (i + 1 >= args[count].size())
            {
                cerr << "zgsh: syntax error near >" << endl;
                exit(EXIT_FAILURE);
            }
            int fd = open(args[count][i + 1], flags, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            i++; // 跳过文件名
        }
        else if (strcmp(args[count][i], "<") == 0)
        {
            // 输入重定向
            flags = O_RDONLY;
            if (i + 1 >= args[count].size())
            {
                cerr << "zgsh: syntax error near <" << endl;
                exit(EXIT_FAILURE);
            }
            int fd = open(args[count][i + 1], flags);
            dup2(fd, STDIN_FILENO);
            close(fd);
            i++;
        }
        else
        {
            new_args.push_back(args[count][i]);
        }
    }
    new_args.push_back(nullptr);

    // 清理旧参数
    args[count] = new_args;
}

// 修改3：完善错误处理
void pidfork(pid_t pid, int count)
{
    // ... [其他代码不变] ...
    else if (pids[count] == 0)
    {
        // 添加重定向处理
        redirect(count);

        // 管道处理
        if (count > 0)
        {
            close(pipes[count - 1][1]); // 关闭不需要的写端
            dup2(pipes[count - 1][0], STDIN_FILENO);
            close(pipes[count - 1][0]);
        }
        if (count < pipecount)
        {
            close(pipes[count][0]); // 关闭不需要的读端
            dup2(pipes[count][1], STDOUT_FILENO);
            close(pipes[count][1]);
        }

        // 执行命令
        if (execvp(args[count][0], args[count].data()) == -1)
        {
            if (errno == ENOENT)
            {
                cerr << "zgsh: command not found: " << args[count][0] << endl;
            }
            else
            {
                perror("zgsh");
            }
            exit(EXIT_FAILURE);
        }
    }
}

// 修改4：内存管理优化
int main()
{
    while (1)
    {
        // ... [其他代码不变] ...
        for (int i = 0; i < argscount; ++i)
        {
            // 先释放原有内存
            for (char *ptr : args[i])
            {
                if (ptr)
                    delete[] ptr;
            }
            args[i].clear();
        }
    }
}