#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#include <string>
#include <sstream>
#include <map>
#include <set>
#include "data.h"
using namespace std;

string shortenDirectory(string& path) {
    stringstream ss(path);
    string token;
    string result;
    vector<string> folders;

    // Split the path into folders
    while (getline(ss, token, '/')) {
        if (!token.empty()) {
            folders.push_back(token);
        }
    }

    // Build the shortened path
    for (size_t i = 0; i < folders.size(); ++i) {
        if (i < folders.size() - 2) {
            result += folders[i][0]; // Take only the first character
        }
        else {
            result += folders[i]; // Add the full folder name
        }
        if (i != folders.size() - 1) {
            result += '/'; // Add the path separator
        }
    }

    return result.empty() ? "/" : result;
}

void printCommandPrompt() {
    cout << colors::magenta << user << colors::yellow << " " << shortenDirectory(currentDirectory) << colors::magenta << " > " << colors::reset;
}

void printError(const string& message) {
    cout << colors::red << "Error: " << message << colors::reset << endl;
}

bool validOptions(command& cmd) {
    set<char>& validOptions = vaildCommands[cmd.name].first;
    bool allVaild = true;
    if (!cmd.options.empty()) {
        for (char& option : cmd.options) {
            if (!validOptions.count(option)) {
                string error = "Invalid option: ";
                error += option;
                printError(error);
                allVaild = false;
            }
        }
    }
    return allVaild;
}

bool validParameterNumber(command& cmd) {;
    if (vaildCommands[cmd.name].second.count(cmd.parameters.size()) == 0) {
        printError("Invalid number of parameters!");
        return false;
    }
    return true;
}
#endif //HELPER_FUNCTIONS_H
