#include <climits>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h> // For directory handling
#include <sys/types.h> // For opendir, readdir, closedir
#include <sys/stat.h> // For stat, lstat
#include <cstring>    // For strerror
#include <cstdio>
#include <cstdlib>  // for getenv
#include <fstream>
#include <fcntl.h>    // For open, O_CREAT, O_WRONLY, O_RDONLY
#include <filesystem> // For recursive_directory_iterator
#include <sys/wait.h> // For waitpid()
#include <iomanip>
#include <set>
#include "data.h"
#include "helper_functions.h"
using namespace std;
using namespace filesystem;
namespace fs = std::filesystem;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void displayDocumentation() {
    ifstream file("Command_List_Documentation.txt");
    if (!file.is_open()) {
        printError("Could not open help file.");
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

void fetchUser() {
    struct passwd* pw = getpwuid(getuid());
    if (pw != nullptr) {
        user = pw->pw_name;
    }
    else {
        printError("Couldn't get user name - " + string(strerror(errno)));
        user = "unknown";
    }
}

void fetchCurrentDirectory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        currentDirectory = cwd;
    }
    else {
        printError("Couldn't get current directory - " + string(strerror(errno)));
    }
}


void diplayCurrentDirectoryContent(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;


    string path = cmd.parameters.empty() ? currentDirectory : cmd.parameters[0];

    bool longListing = false;
    for (char option : cmd.options) {
        if (option == 'l') {
            longListing = true;
            break;
        }
    }

    DIR* directory = opendir(path.c_str());

    struct dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
        string fullPath = path + "/" + entry->d_name;
        struct stat fileStat;
        if (stat(fullPath.c_str(), &fileStat) == 0) {
            if (longListing) {
                // Display file permissions
                cout << ((S_ISDIR(fileStat.st_mode)) ? "d" : "-")
                    << ((fileStat.st_mode & S_IRUSR) ? "r" : "-")
                    << ((fileStat.st_mode & S_IWUSR) ? "w" : "-")
                    << ((fileStat.st_mode & S_IXUSR) ? "x" : "-")
                    << ((fileStat.st_mode & S_IRGRP) ? "r" : "-")
                    << ((fileStat.st_mode & S_IWGRP) ? "w" : "-")
                    << ((fileStat.st_mode & S_IXGRP) ? "x" : "-")
                    << ((fileStat.st_mode & S_IROTH) ? "r" : "-")
                    << ((fileStat.st_mode & S_IWOTH) ? "w" : "-")
                    << ((fileStat.st_mode & S_IXOTH) ? "x" : "-") << " ";

                // Display number of links
                cout << setw(3) << fileStat.st_nlink << " ";

                // Display file size
                cout << setw(8) << fileStat.st_size << " ";

                // Display last modification time
                char timeBuf[80];
                strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_mtime));
                cout << timeBuf << " ";

                // Display file name
                cout << entry->d_name << endl;
            }
            else {
                cout << entry->d_name << " ";
            }
        }
        else {
            printError(("Unable to get file stats for " + string(entry->d_name) + " - " + string(strerror(errno))).c_str());
        }
    }
    cout << endl;

    closedir(directory);
}

void changeDirectory(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string targetDir;

    // Check if user provided a directory
    if (cmd.parameters.empty()) {
        // If no parameters, go to the Desktop
        const char* homeDir = getenv("HOME");  
        if (homeDir != nullptr) {
            targetDir = string(homeDir) + "/Desktop";  // Default to Desktop 
        } else {
            printError("Could not retrieve home directory.");
            return;
        }
    } else {
        // If the user provided a directory
        targetDir = cmd.parameters[0];
    }

    // Attempt to change the directory
    if (chdir(targetDir.c_str()) != 0) {
        printError("Couldn't change directory to " + targetDir + " - " + string(strerror(errno)));
    } 
}

void createFile(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string path = cmd.parameters[0];

    // create new file
    int fileDescriptor = open(path.c_str(), O_CREAT | O_WRONLY, 0644);
    if (fileDescriptor == -1) {
        printError("Couldn't creat file - " + string(strerror(errno)));
        return;
    }

    close(fileDescriptor);
}


void readFileContent(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string inputPath = "";  
    string outputPath = ""; 

    if (cmd.parameters.size() > 0 && cmd.parameters[0] != ">") {
        inputPath = cmd.parameters[0];  
    }

    // Check if output redirection (">") exists
    if (cmd.parameters.size() > 1 && cmd.parameters[1] == ">") {
        if (cmd.parameters.size() > 2) {
            outputPath = cmd.parameters[2];  
        }
    }

    if (inputPath.empty()) {
        printError("Invalid command: No input file specified.");
        return;
    }

    int inputFd = open(inputPath.c_str(), O_RDONLY);
    if (inputFd == -1) {
        printError("Couldn't open input file - " + string(strerror(errno)));
        return;
    }

    // If output redirection exists, open the output file for writing
    int outputFd = -1;
    if (!outputPath.empty()) {
        outputFd = open(outputPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outputFd == -1) {
            printError("Couldn't open or create output file - " + string(strerror(errno)));
            close(inputFd);
            return;
        }
    }

    // Read content from the input file and write to output 
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(inputFd, buffer, sizeof(buffer))) > 0) {
        if (outputFd != -1) {
            ssize_t bytesWritten = write(outputFd, buffer, bytesRead);
            if (bytesWritten == -1) {
                printError("Couldn't write to output file - " + string(strerror(errno)));
                close(inputFd);
                close(outputFd);
                return;
            }
        } else {
            cout.write(buffer, bytesRead);
        }
    }

    if (bytesRead == -1) {
        printError("Couldn't read input file - " + string(strerror(errno)));
    }

    close(inputFd);
    if (outputFd != -1) {
        close(outputFd);
    }
}



void deleteFile(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string path = cmd.parameters[0];

    if (fs::is_directory(path)) {
        // Delete the directory and its contents
        try {
            fs::remove_all(path);
        } catch (const fs::filesystem_error& e) {
            cout << "Error deleting directory: " << e.what() << endl;
        }
    } else {
        // Delete a single file
        if (unlink(path.c_str()) == -1) {
            cout << "Couldn't delete file: " << path << " - " << strerror(errno) << endl;
        } 
    }

}

void moveFile(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string sourcePath = cmd.parameters[0];
    string destinationPath = cmd.parameters[1];

   // checks if the destination path is a directory
   struct stat destStat;
    if (stat(destinationPath.c_str(), &destStat) == 0 && S_ISDIR(destStat.st_mode)) {
        // Append the filename to the destination path
        size_t pos = sourcePath.find_last_of("/\\");
        string fileName = (pos == string::npos) ? sourcePath : sourcePath.substr(pos + 1);
        destinationPath += "/" + fileName;
    }

    // renaming/moving the file
    if (rename(sourcePath.c_str(), destinationPath.c_str()) != 0) {
        if (errno == EXDEV) {
            // Handle case when moving across filesystems
            cout << "Moving across filesystems, copying instead..." << endl;
            ifstream source(sourcePath, ios::binary);
            ofstream destination(destinationPath, ios::binary);

            if (!source.is_open() || !destination.is_open()) {
                printError("couldn't open source or destination file");
                return;
            }

            // Copy the file
            destination << source.rdbuf();

            source.close();
            destination.close();

            // Delete the source file after successful copy
            if (remove(sourcePath.c_str()) != 0) {
                printError("couldn't delete source file after copying - " + string(strerror(errno)));
            } else {
                cout << "File moved successfully from " << sourcePath << " to " << destinationPath << endl;
            }
        } else {
            // Other errors
            printError("couldn't move file - " + string(strerror(errno)));
        }
    } 
}

void copyFile(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string sourcePath = cmd.parameters[0];
    string destinationPath = cmd.parameters[1];

    // checks if the destination path is a directory
    struct stat destStat;
    if (stat(destinationPath.c_str(), &destStat) == 0 && S_ISDIR(destStat.st_mode)) {
        // Append the filename to the destination path
        size_t pos = sourcePath.find_last_of("/\\");
        string fileName = (pos == string::npos) ? sourcePath : sourcePath.substr(pos + 1);
        destinationPath += "/" + fileName; 
    }

    // Open source file
    int sourceFd = open(sourcePath.c_str(), O_RDONLY);
    if (sourceFd == -1) {
        printError("Source file path not valid! - " + string(strerror(errno)));
        return;
    }

    // Open or create destination file
    int destFd = open(destinationPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) {
        printError("Destination file path not valid! - " + string(strerror(errno)));
        close(sourceFd);
        return;
    }

    // Copy content from source to destination
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(sourceFd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten == -1) {
            printError("Couldn't write. - " + string(strerror(errno)));
            close(sourceFd);
            close(destFd);
            return;
        }
    }

    if (bytesRead == -1) {
        printError("can't read from source file - " + string(strerror(errno)));
    }

    close(sourceFd);
    close(destFd);
}



void displayFileInfo(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string path = cmd.parameters[0];

    struct stat fileInfo;

    // Use lstat to avoid following symbolic links
    if (lstat(path.c_str(), &fileInfo) == -1) {
        printError("Cannot stat " + path + " - " + strerror(errno));
        return;
    }

    // Print file information
    cout << "File: " << path << endl;
    cout << "Size: " << fileInfo.st_size << " bytes" << endl;
    cout << "Inode: " << fileInfo.st_ino << endl;
    cout << "Links: " << fileInfo.st_nlink << endl;
    cout << "Permissions: " << oct << fileInfo.st_mode << dec << endl;
    cout << "UID: " << fileInfo.st_uid << endl;
    cout << "GID: " << fileInfo.st_gid << endl;
    cout << "Access: " << ctime(&fileInfo.st_atime);
    cout << "Modify: " << ctime(&fileInfo.st_mtime);
    cout << "Change: " << ctime(&fileInfo.st_ctime);

    // Check file type
    if (S_ISDIR(fileInfo.st_mode)) {
        cout << "Type: Directory" << endl;
    }
    else if (S_ISREG(fileInfo.st_mode)) {
        cout << "Type: Regular File" << endl;
    }
    else if (S_ISLNK(fileInfo.st_mode)) {
        cout << "Type: Symbolic Link" << endl;
    }
    else {
        cout << "Type: Other" << endl;
    }
}

void createDirectory(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string path = cmd.parameters[0];

    if (!cmd.options.empty() && cmd.options[0] == 'p') {
        stringstream ss(path);
        string dir;
        string currentPath;

        while (getline(ss, dir, '/')) {
            if (dir.empty()) continue; // Skip leading slash

            currentPath += dir + "/";

            if (mkdir(currentPath.c_str(), 0755) == -1) {
                if (errno != EEXIST) {
                    return;
                }
            }
        }

        cout << "Directories created: " << path << endl;
    }
    else if (mkdir(path.c_str(), 0755) == -1) {
        printError("Cannot create directory '" + path + "' - " + strerror(errno));
    }
    else {
        cout << "Directory created: " << path << endl;
    }
}

void removeDirectory(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string path = cmd.parameters[0];

    if (rmdir(path.c_str()) == -1) {
        printError("Cannot remove directory '" + path + "' - " + strerror(errno));
    }
    else {
        cout << "Directory created: " << path << endl;
    }
}

// find searchStartPath name
void findFiles(command& cmd) {
    if (validOptions(cmd) == false || validParameterNumber(cmd) == false) return;

    string searchStartPath, name;
    if (cmd.parameters.size() == 1) {
        searchStartPath = currentDirectory;
        name = cmd.parameters[0];
    }
    else if (cmd.parameters.size() == 2) {
        searchStartPath = cmd.parameters[0];
        name = cmd.parameters[1];
    }

    for (const auto& entry : recursive_directory_iterator(searchStartPath)) {
        if (entry.path().filename().string().find(name) != string::npos) {
            cout << entry.path().string() << endl;
        }
    }
}

void execute_command(const command& cmd) {
    vector<char*> argv;
    argv.push_back(const_cast<char*>(cmd.name.c_str()));

    if (!cmd.options.empty()) {
        string option_str = "-";
        for (const auto& opt : cmd.options) {
            option_str += opt;
        }
        argv.insert(argv.begin() + 1, const_cast<char*>(option_str.c_str()));
    }

    for (const auto& param : cmd.parameters) {
        argv.push_back(const_cast<char*>(param.c_str()));
    }

    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        printError("Fork failed!");
        exit(1);
    }
    else if (pid == 0) {
        execvp(argv[0], argv.data());
        printError("Command not found: " + string(argv[0]));
        exit(1);
    }
    else {
        int status;
        waitpid(pid, &status, 0); // Wait for the child process to complete

        if (WIFEXITED(status) == false) {
            printError("Child process terminated abnormally");
        }
    }
}

double evaluateExpression(const string& expression) {
    stringstream ss(expression);
    double num1, num2;
    char op;
    
    ss >> num1 >> op >> num2;
    
    switch(op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0) return num1 / num2;
            else {
                cout << "Error: Division by zero!" << endl;
                return NAN; // Return Not A Number for division by zero
            }
        default:
            cout << "Error: Invalid operator!" << endl;
            return NAN;
    }
}