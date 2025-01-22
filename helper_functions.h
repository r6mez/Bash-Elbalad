#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#include <string>
#include <sstream>
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
        } else {
            result += folders[i]; // Add the full folder name
        }
        if (i != folders.size() - 1) {
            result += '/'; // Add the path separator
        }
    }

    return result.empty() ? "/" : result;
}


#endif //HELPER_FUNCTIONS_H
