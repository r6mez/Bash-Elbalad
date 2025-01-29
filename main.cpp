#include <iostream>
#include "command.h"
#include "general_commands.h"
#include "data.h"
#include "input_and_history.h"
using namespace std;

int main() {
    fetchUser();
    fetchCurrentDirectory();
    loadAllCommandsFromFile();
    clearScreen();

    while (true) {
        printCommandPrompt();
        cout.flush();

        enableRawMode(); // Enable raw input mode
        string input = getInputWithArrows();
        disableRawMode();

        if(input == "\n" || input == "\r\n") continue; // Handles empty input

        command newCommand(input);

        if (newCommand.name == "exit") {
            break;
        }
        else if (newCommand.name == "clear") {
            clearScreen();
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
        else if (newCommand.name == "find") {
            findFiles(newCommand);
        }
        else {
            execute_command(newCommand);
        }

        saveCommandToFile(input);
    }
}