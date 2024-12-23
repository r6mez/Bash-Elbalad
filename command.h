#ifndef COMMAND_H
#define COMMAND_H
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct command {
    string name;
    vector<char> options;
    vector<string> parameters;

    command() {

    }

    command(string input) {
        parseCommand(input);
    }

    void parseCommand(string input) {
        stringstream ss(input);
        ss >> name;

        string token;
        while (ss >> token) {
            if (token[0] == '-') { // take options first if exist, format: -abcd
                for (size_t i = 1; i < token.length(); ++i) {
                    options.push_back(token[i]);
                }
            } else {
                // input first parameter
                parameters.push_back(token);

                // take new pararmeters until end of them
                while (ss >> token) {
                    parameters.push_back(token);
                }
            }
        }
    }

    void printCommandDetails() {
        cout << "Name: " << name << endl;

        cout << "Options: ";
        for (size_t i = 0; i < options.size(); ++i) {
            cout << options[i] << " ";
        }
        cout << endl;

        cout << "Parameters: ";
        for (size_t i = 0; i < parameters.size(); ++i) {
            cout << parameters[i] << " ";
        }
        cout << endl;
    }
};


#endif //COMMAND_H
