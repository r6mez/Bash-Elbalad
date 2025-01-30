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
    printHeader();

    while (true) {
        printCommandPrompt();
        cout.flush();

        enableRawMode(); // Enable raw input mode
        string input = getInputWithArrows();
        disableRawMode();

        if(input == "\n" || input == "\r\n") continue; // Handles empty input

        vector<string> commands = split(input, ';');
        
        // Loop over each command and execute it
        for (const string& cmd_input : commands) {
            if (cmd_input.empty()) continue;  // Skip empty parts
            
            // If the input contains a mathematical expression
            if (isMathExpression(cmd_input)) {
                double result = evaluateExpression(cmd_input);
                if (!isnan(result)) {
                    cout << result << endl;
                }
                continue;
            }

            command newCommand(cmd_input);

        if (newCommand.name == "exit") {
            exit(0);
        }
        else if (newCommand.name == "clear") {
            clearScreen();
        }
        else if (newCommand.name == "pwd") {
            cout << currentDirectory << endl;
        }
        else if (newCommand.name == "cd") {
            changeDirectory(newCommand);
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
            readFileContent(newCommand);
        }
        else if (newCommand.name == "rm") {
            deleteFile(newCommand);
        }
        else if (newCommand.name == "mv") {
            moveFile(newCommand);
        }
        else if (newCommand.name == "cp") {
            copyFile(newCommand);
        }
        else if (newCommand.name == "find") {
            findFiles(newCommand);
        }
        else if (newCommand.name == "help") {
           displayDocumentation();
        }
        else {
            execute_command(newCommand);
        }

        saveCommandToFile(input);
    }
}
}