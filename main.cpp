#include <iostream>
#include "command.h"
#include "general_commands.h"
#include "data.h"
using namespace std;

int main() {
    fetchUser();
    fetchCurrentDirectory();
    clear();

    while (true) {
        printCommandPrompt();

        // wait for input
        string input;
        getline(cin, input);

        command newCommand(input);
        if(validOptions(newCommand) == false) continue;
        if(validParameterNumber(newCommand) == false) continue;

        if (newCommand.name == "exit") {
            exit(0);
        }
        else if (newCommand.name == "clear") {
            clear();
        }
        else if (newCommand.name == "pwd") {
            cout << currentDirectory << endl;
        }
        else if (newCommand.name == "ls") {
            diplayCurrentDirectoryContent(newCommand);
        }
        else if (newCommand.name == "stat") {
            displayFileInfo(newCommand);
        }
        else if (newCommand.name == "mkdir") {
            createDirectory(newCommand);
        }
        else if (newCommand.name == "rmdir") {
            removeDirectory(newCommand);
        }
        else if (newCommand.name == "touch") {
            createFile(newCommand);
        }
        else if (newCommand.name == "cat") {
            ReadFileContent(newCommand);
        }
        else if (newCommand.name == "rm") {
            DeleteFile(newCommand);
        }
        else if (newCommand.name == "mv") {
            MoveFile(newCommand);
        }
        else if (newCommand.name == "cp") {
            CopyFile(newCommand);
        }
    }
}