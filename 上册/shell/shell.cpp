#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;
vector<string> orders;
string order;
int main()
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
    cin >> order;
    orders.push_back(order);
    orders.clear();
    return 0;
}