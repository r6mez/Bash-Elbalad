#ifndef DATA_H
#define DATA_H
#include <string>
#include <unordered_map>
#include <set>
using namespace std;

string user;
string currentDirectory;

/*
They have to be here and initialized when the program starts
and only when user or directory changed.
 */

 // command name + vaild options + number of vaild parameters
unordered_map<string, pair<set<char>, set<int>>> vaildCommands = {
    {"clear", { {}, {0}}},
    {"pwd", { {}, {0}}},
    {"cd", { {}, {0,1}}},
    {"ls", { {'l'}, {0, 1}}},
    {"stat", { {}, {1}}},
    {"touch", { {}, {1}}},
    {"cat", { {}, {1,2,3}}},
    {"rm", { {'f'}, {1}}},
    {"mv", { {}, {2}}},
    {"cp", { {}, {2}}},
    {"mkdir", { {'p'}, {1}}},
    {"rmdir", { {}, {1}}},
    {"find", { {}, {1, 2}}},
    {"exit", { {}, {0}}},
    {"help", { {}, {0}}}
};

class colors {
public:
    inline static const std::string reset = "\033[0m";
    inline static const std::string red = "\033[31m";
    inline static const std::string green = "\033[32m";
    inline static const std::string yellow = "\033[33m";
    inline static const std::string blue = "\033[34m";
    inline static const std::string magenta = "\033[35m";
    inline static const std::string cyan = "\033[36m";
};


#endif //DATA_H
