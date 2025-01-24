#include <climits>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>   
#include <unistd.h>  
#include <sys/stat.h> 
#include <cstdio>
#include "data.h"
#include "helper_functions.h"
using namespace std;

void fetchUser(){
    struct passwd *pw = getpwuid(getuid());
    if (pw != nullptr) {
        user = pw->pw_name;
    } else {
        perror("getpwuid() error");
        user = "unknown";
    }
}

void fetchCurrentDirectory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        currentDirectory = cwd;
    } else {
        perror("getcwd() error");
    }
}

void clear() {
    cout << "\033[2J\033[1;1H";
}

void printCommandPrompt() {
    cout << user << " " << shortenDirectory(currentDirectory) << " > ";
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
        cout << "Invalid options: ";
        for (char option : options) {
            cout << option << ", ";
        }
        cout << endl;
    }

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
    } else {
        cout << "File deleted successfully: " << path << endl;
    }
}

void MoveFile(vector<string>& parameters, vector<char>& options) {
    string sourcePath, destinationPath;

    
    if (parameters.empty()) {
        cout << "Didn't provide a file path!" << endl;
        return;
    } else if (parameters.size() == 1) {
        cout << "Too few parameters! You must specify a source and destination path." << endl;
        return;
    } else if (parameters.size() == 2) {
        sourcePath = parameters[0];
        destinationPath = parameters[1];
    } else {
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
    } else if (parameters.size() == 1) {
        cout << "Too few parameters! You must specify a source and destination path." << endl;
        return;
    } else if (parameters.size() == 2) {
        sourcePath = parameters[0];
        destinationPath = parameters[1];
    } else {
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





