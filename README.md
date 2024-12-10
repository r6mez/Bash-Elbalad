# Shell-Elbalad
A simple linux shell project.

# Todo:
## **1. Project Setup**
- [ ] **Create a new C++ project**: Set up your project and include necessary libraries (`<iostream>`, `<string>`, `<unistd.h>`, `<sys/wait.h>`).

## **2. Core Functionality**
### **Shell Basics**
- [ ] **Design the shell prompt**: Decide on a basic shell prompt.
- [ ] **Exit command**: Implement a way to exit the shell (e.g., if the user types `exit`).
- [ ] **Print working directory (pwd)**: Use `getcwd()` to implement the `pwd` command.
- [ ] **Change directory (cd)**: Use `chdir()` to implement the `cd` command.
- [ ] **Clear screen**: Implement a command to clear the terminal.
- [ ] **Prompt customization**: Customize the prompt to display user or directory info (e.g., `user@host:~$`).

### **Command Execution**
- [ ] **Read user input**: Use `std::getline` to capture input from the user.
- [ ] **Tokenize input**: Split the input string into commands and arguments using a tokenizer like `std::istringstream` or `strtok`.
- [ ] **Fork a child process**: Use `fork()` to create a new process for command execution.
- [ ] **Execute commands**: In the child process, use `execvp()` to execute the command.
- [ ] **Wait for the child process**: In the parent process, use `waitpid()` to wait for the child to complete.

## **3. File Management Commands**
- [ ] **cat**: Display file contents.
- [ ] **touch**: Create new files.
- [ ] **rm**: Delete files.
- [ ] **mv**: Move or rename files.
- [ ] **cp**: Copy files.
- [ ] **ls**: List directory contents.
- [ ] **stat**: Display detailed file information.
- [ ] **chmod**: Change file permissions.
- [ ] **find**: Search for files.
- [ ] **mkdir**: Create directories.
- [ ] **rmdir**: Remove directories.
- [ ] **du**: Display disk usage of files or directories.
- [ ] **ln**: Create symbolic or hard links.
- [ ] **df**: Display available disk space.

## **4. Error Handling**
- [ ] **Command not found**: Handle cases where the command does not exist.
- [ ] **Invalid arguments**: Handle incorrect usage of commands.
- [ ] **Fork/exec errors**: Check for errors in `fork()` and `execvp()` and display appropriate messages.

## **5. Advanced Features**
### **Process Management**
- [ ] **Handle background processes**: Allow commands to run in the background using `&`.

### **Command Composition**
- [ ] **Pipes**: Support commands like `ls | grep cpp` using `pipe()` and `dup2()`.
- [ ] **Redirection**: Implement input/output redirection (`>`, `<`) using `freopen()` or `dup2()`.
- [ ] **Running multiple commands (;)**: Support executing multiple commands in sequence.

### **Environment and History**
- [ ] **History**: Store previously run commands and allow the user to view them using a command like `history`.
