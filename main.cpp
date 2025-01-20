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

        cout << "Received input: " << input << endl;
        command newCommand(input);

        newCommand.printCommandDetails();

        if (newCommand.name == "exit") {
            return 0;
        } else if (newCommand.name == "clear") {
            clear();
        }
    }
}