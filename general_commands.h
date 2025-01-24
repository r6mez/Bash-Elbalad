#include <climits>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h> // For directory handling
#include <sys/types.h> // For opendir, readdir, closedir
#include <sys/stat.h> // For stat, lstat
#include <cstring>    // For strerror
#include <fcntl.h>    // For open, O_CREAT, O_WRONLY, O_RDONLY
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
        set<char> validOptions = { 'l' };
        for (char option : options) {
            if (!validOptions.count(option)) {
                cout << "Invalid option: " << option << endl;
                return;
            }
        }
    }

    cout << "Invalid options: ";
    for (char option : options) {
        cout << option << ", ";
    }
    cout << endl;

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

void createFile(vector<string>& parameters, vector<char>& options) {
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
        set<char> validOptions = { 'l' };
        for (char option : options) {
            if (!validOptions.count(option)) {
                cout << "Invalid option: " << option << endl;
                return;
            }
        }
    }

    cout << "Invalid options: ";
    for (char option : options) {
        cout << option << ", ";
    }
    cout << endl;


    // create new file
    int fileDescriptor = open(path.c_str(), O_CREAT | O_WRONLY, 0644);
    if (fileDescriptor == -1) {
        perror("Error creating file");
        return;
    }

    close(fileDescriptor);
}


void ReadFileContent(vector<string>& parameters, vector<char>& options) {
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

    // Open the file in read-only mode
    int fileDescriptor = open(path.c_str(), O_RDONLY);
    if (fileDescriptor == -1) {
        perror("Error opening file");
        return;
    }

    // Read and display the file content
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        cout << buffer;
    }

    if (bytesRead == -1) {
        perror("Error reading file");
    }

    // Close the file
    close(fileDescriptor);


}

void DeleteFile(vector<string>& parameters, vector<char>& options) {
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

    // delete file with unlink    
    if (unlink(path.c_str()) == -1) {
        perror("Error deleting file");
    }
    else {
        cout << "File deleted successfully: " << path << endl;
    }
}

void MoveFile(vector<string>& parameters, vector<char>& options) {
    string sourcePath, destinationPath;


    if (parameters.empty()) {
        cout << "Didn't provide a file path!" << endl;
        return;
    }
    else if (parameters.size() == 1) {
        cout << "Too few parameters! You must specify a source and destination path." << endl;
        return;
    }
    else if (parameters.size() == 2) {
        sourcePath = parameters[0];
        destinationPath = parameters[1];
    }
    else {
        cout << "Too many parameters! Only enter 2 paths (source and destination)." << endl;
        return;
    }

    // Check if there's any invalid option.
    if (!options.empty()) {
        cout << "Invalid options: ";
        for (char option : options) {
            cout << option << ", ";
        }
        cout << endl;
        return;
    }

    // Rename or Move file
    if (rename(sourcePath.c_str(), destinationPath.c_str()) != 0) {
        perror("Error moving file");
    }

}

void CopyFile(vector<string>& parameters, vector<char>& options) {
    string sourcePath, destinationPath;


    if (parameters.empty()) {
        cout << "Didn't provide a file path!" << endl;
        return;
    }
    else if (parameters.size() == 1) {
        cout << "Too few parameters! You must specify a source and destination path." << endl;
        return;
    }
    else if (parameters.size() == 2) {
        sourcePath = parameters[0];
        destinationPath = parameters[1];
    }
    else {
        cout << "Too many parameters! Only enter 2 paths (source and destination)." << endl;
        return;
    }

    // Check if there's any invalid option.
    if (!options.empty()) {
        cout << "Invalid options: ";
        for (char option : options) {
            cout << option << ", ";
        }
        cout << endl;
        return;
    }

    // Open source file
    int sourceFd = open(sourcePath.c_str(), O_RDONLY);
    if (sourceFd == -1) {
        perror("Error opening source file");
        return;
    }

    // Open or create destination file
    int destFd = open(destinationPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) {
        perror("Error opening destination file");
        close(sourceFd);
        return;
    }

    // Copy content from source to destination
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(sourceFd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten == -1) {
            perror("Error writing to destination file");
            close(sourceFd);
            close(destFd);
            return;
        }
    }

    if (bytesRead == -1) {
        perror("Error reading from source file");
    }

    close(sourceFd);
    close(destFd);

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