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
        newCommand.printCommandDetails();

        if (newCommand.name == "exit") {
            return 0;
        }
        else if (newCommand.name == "clear") {
            clear();
        }
        else if (newCommand.name == "pwd") {
            cout << currentDirectory << endl;
        }
        else if (newCommand.name == "ls") {
            diplayCurrentDirectoryContent(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "stat") {
            displayFileInfo(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "mkdir") {
            createDirectory(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "rmdir") {
            removeDirectory(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "touch") {
            createFile(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "cat") {
            ReadFileContent(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "rm") {
            DeleteFile(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "mv") {
            MoveFile(newCommand.parameters, newCommand.options);
        }
        else if (newCommand.name == "cp") {
            CopyFile(newCommand.parameters, newCommand.options);
        }
    }
}