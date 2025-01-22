#include <climits>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h> // For directory handling
#include <sys/types.h> // For opendir, readdir, closedir
#include <sys/stat.h> // For stat, lstat
#include <cstring>    // For strerror
#include <iomanip>
#include <set>
#include "data.h"
#include "helper_functions.h"
using namespace std;

void fetchUser() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        user = pw->pw_name;
    }
    else {
        perror("getpwuid() error");
        user = "unknown";
    }
}

void fetchCurrentDirectory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        currentDirectory = cwd;
    }
    else {
        perror("getcwd() error");
    }
}

void clear() {
    cout << "\033[2J\033[1;1H";
}

void printCommandPrompt() {
    cout << user << " " << shortenDirectory(currentDirectory) << " > ";
}

void diplayCurrentDirectoryContent(vector<string>& parameters, vector<char>& options) {
    string path;

    if (parameters.empty()) {
        path = currentDirectory;
    }
    else if (parameters.size() == 1) {
        path = parameters.front();
    }
    else {
        perror("Too many parameters! only enter 1 path or 0.");
        return;
    }

    // Check if there's any invalid option.
    if (!options.empty()) {
        set<char> validOptions = { 'l' };
        for (char option : options) {
            if (!validOptions.count(option)) {
                cout << "Invalid option: " << option << endl;
                return;
            }
        }
    }

    DIR* directory = opendir(path.c_str());

    struct dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
        if (options.empty()) {
            cout << entry->d_name << " ";
        }
        else if (options.front() == 'l') {
            int width = 40;
            cout << left << setw(width) << entry->d_name << setw(width) << entry->d_reclen << endl;
        }
    }
    cout << endl;
}

void displayFileInfo(vector<string>& parameters, vector<char>& options) {
    string path;

    if (parameters.empty()) {
        cout << "Didn't provide a file path!" << endl;
    }
    else if (parameters.size() == 1) {
        path = parameters.front();
    }
    else {
        cout << "Too many parameters! only enter 1 path or 0." << endl;
        return;
    }

    // Check if there's any invalid option.
    if (!options.empty()) {
        cout << "Invalid options: ";
        for (char option : options) {
            cout << option << ", ";
        }
        cout << endl;
    }


    struct stat fileInfo;

    // Use lstat to avoid following symbolic links
    if (lstat(path.c_str(), &fileInfo) == -1) {
        cout << "Error: Cannot stat " << path << "' - " << strerror(errno) << endl;
        return;
    }

    // Print file information
    cout << "\tFile: " << path << endl;
    cout << "\tSize: " << fileInfo.st_size << " bytes" << endl;
    cout << "\tInode: " << fileInfo.st_ino << endl;
    cout << "\tLinks: " << fileInfo.st_nlink << endl;
    cout << "\tPermissions: " << oct << fileInfo.st_mode << dec << endl;
    cout << "\tUID: " << fileInfo.st_uid << endl;
    cout << "\tGID: " << fileInfo.st_gid << endl;
    cout << "\tAccess: " << ctime(&fileInfo.st_atime);
    cout << "\tModify: " << ctime(&fileInfo.st_mtime);
    cout << "\tChange: " << ctime(&fileInfo.st_ctime);

    // Check file type
    if (S_ISDIR(fileInfo.st_mode)) {
        cout << "\tType: Directory" << endl;
    }
    else if (S_ISREG(fileInfo.st_mode)) {
        cout << "\tType: Regular File" << endl;
    }
    else if (S_ISLNK(fileInfo.st_mode)) {
        cout << "\tType: Symbolic Link" << endl;
    }
    else {
        cout << "\tType: Other" << endl;
    }
}

void createDirectory(vector<string>& parameters, vector<char>& options) {
    string path;

    if (parameters.empty()) {
        cout << "Didn't provide a file path!" << endl;
    }
    else if (parameters.size() == 1) {
        path = parameters.front();
    }
    else {
        cout << "Too many parameters! only enter 1 path." << endl;
        return;
    }

    // Check if there's any invalid option.
    if (!options.empty()) {
        cout << "Invalid options: ";
        for (char option : options) {
            cout << option << ", ";
        }
        cout << endl;
    }

    if (mkdir(path.c_str(), 0755) == -1) {
        cout << "Error: Cannot create directory '" << path << "' - " << strerror(errno) << endl;
    }
    else {
        cout << "Directory created: " << path << endl;
    }
}

void removeDirectory(vector<string>& parameters, vector<char>& options) {
    string path;

    if (parameters.empty()) {
        cout << "Didn't provide a file path!" << endl;
    }
    else if (parameters.size() == 1) {
        path = parameters.front();
    }
    else {
        cout << "Too many parameters! only enter 1 path." << endl;
        return;
    }

    // Check if there's any invalid option.
    if (!options.empty()) {
        cout << "Invalid options: ";
        for (char option : options) {
            cout << option << ", ";
        }
        cout << endl;
    }

    if (rmdir(path.c_str()) == -1) {
        cout << "Error: Cannot remove directory '" << path << "' - " << strerror(errno) << endl;
    }
    else {
        cout << "Directory created: " << path << endl;
    }
}