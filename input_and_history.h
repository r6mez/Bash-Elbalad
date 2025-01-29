#ifndef HISTORY_H
#define HISTORY_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>
using namespace std;

vector<string> history;
int historyIndex;

void saveCommandToFile(string &command){
    command.pop_back(); // Remove the newline character
    ofstream file("history.txt", std::ios::app);
    history.push_back(command);
    file << command << endl;
    file.close();
    historyIndex = history.size() - 1;
}

void loadAllCommandsFromFile(){
    ifstream file("history.txt");
    string command;
    while(getline(file, command)){
        history.push_back(command);
    }
    historyIndex = history.size() - 1;
    file.close();
}

// Function to configure terminal for raw input
void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable line buffering and echo
    term.c_iflag &= ~(ICRNL); // Prevent '\r' → '\n' translation
    term.c_cc[VMIN] = 1; // Read 1 char at a time
    term.c_cc[VTIME] = 0; // No timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Restore defaults
    term.c_iflag |= (ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

string getInputWithArrows() {
    string input;
    char ch;
    size_t cursorPos = 0; 

    while (true) {
        if (read(STDIN_FILENO, &ch, 1) <= 0) continue; // Read 1 character

        if (ch == '\x1b') { // Escape sequence (arrow keys)
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) <= 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) <= 0) continue;

            if (seq[0] == '[') {
                switch (seq[1]) {
                case 'A': // Up arrow
                    if (!history.empty() && historyIndex >= 0) {
                        for (size_t i = 0; i < input.length(); i++) {
                            cout << "\b \b";
                        }
                        input = history[historyIndex];
                        if(historyIndex) historyIndex--;
                        cursorPos = input.length();
                        cout << input;
                        cout.flush();
                    }
                    break;
                case 'B': // Down arrow
                    if (!history.empty() && historyIndex <= history.size() - 1) {
                        for (size_t i = 0; i < input.length(); i++) {
                            cout << "\b \b"; 
                        }
                        input = history[historyIndex];
                        if(historyIndex < history.size() - 1) historyIndex++;
                        cursorPos = input.length();
                        cout << input; 
                        cout.flush();
                    }
                    break;
                case 'C': // Right arrow
                    if (cursorPos < input.length()) {
                        cursorPos++;
                        cout << "\033[C"; // Move cursor right
                        cout.flush();
                    }
                    break;
                case 'D': // Left arrow
                    if (cursorPos > 0) {
                        cursorPos--;
                        cout << "\033[D"; // Move cursor left
                        cout.flush();
                    }
                    break;
                }
            }
        }
        else if (ch == '\r' || ch == '\n') { // Enter key
            cout << "\r\n"; // Move to the next line
            input += '\n';
            break;
        }
        else if (ch == 127 || ch == '\b') { // Backspace
            if (cursorPos > 0) {
                input.erase(cursorPos - 1, 1);
                cursorPos--;
                cout << "\b \b"; // Remove last character from display
                // Redraw the input after the cursor
                for (size_t i = cursorPos; i < input.length(); i++) {
                    cout << input[i];
                }
                cout << ' '; // Clear the last character
                // Move the cursor back to its position
                for (size_t i = cursorPos; i <= input.length(); i++) {
                    cout << "\b";
                }
                cout.flush();
            }
        }
        else { // Regular character
            input.insert(cursorPos, 1, ch); // Insert character at cursor position
            cursorPos++;
            // Redraw the input after the cursor
            for (size_t i = cursorPos - 1; i < input.length(); i++) {
                cout << input[i];
            }
            // Move the cursor back to its position
            for (size_t i = cursorPos; i < input.length(); i++) {
                cout << "\b";
            }
            cout.flush();
        }
    }
    return input;
}


#endif // HISTORY_H