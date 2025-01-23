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
        if(newCommand.name != "pwd"){
        newCommand.printCommandDetails();
        }

        if (newCommand.name == "exit") {
            return 0;
        } else if (newCommand.name == "clear") {
            clear();
        }
        else if (newCommand.name == "pwd"){
            cout<<currentDirectory<<endl;
        } else if(newCommand.name == "touch"){
            createFile(newCommand.parameters,newCommand.options);
        } else if(newCommand.name == "cat"){
            ReadFileContent(newCommand.parameters,newCommand.options);
        } else if(newCommand.name == "rm"){
            DeleteFile(newCommand.parameters,newCommand.options);
        } else if(newCommand.name == "mv"){
           MoveFile(newCommand.parameters,newCommand.options);
        } else if(newCommand.name == "cp"){
           CopyFile(newCommand.parameters,newCommand.options);
        }
    }
}