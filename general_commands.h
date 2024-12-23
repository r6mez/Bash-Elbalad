#ifndef GENERAL_COMMANDS_H
#define GENERAL_COMMANDS_H
#include <climits>
#include <unistd.h>
#include "data.h"
#include "helper_functions.h"
using namespace std;

void fetchUser(){
    user = getlogin();
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

#endif GENERAL_COMMANDS_H // GENERAL_COMMANDS_H

