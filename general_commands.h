#include <climits>
#include <unistd.h>
#include <pwd.h>
#include "data.h"
#include "helper_functions.h"
using namespace std;

void fetchUser(){
    struct passwd *pw = getpwuid(getuid());
    if (pw != nullptr) {
        user = pw->pw_name;
    } else {
        perror("getpwuid() error");
        user = "unknown";
    }
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



