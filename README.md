# Shell-Elbalad
A simple linux shell project.

# Todo:
### **1. Setup and Basics**
- [ ] **Create a new C++ project**: Set up your project and include necessary libraries (`<iostream>`, `<string>`, `<unistd.h>`, `<sys/wait.h>`).
- [ ] **Design the shell prompt**: Decide on a basic shell prompt.

### **2. Input Handling**
- [ ] **Read user input**: Use `std::getline` to capture input from the user.
- [ ] **Tokenize input**: Split the input string into command and arguments using a tokenizer like `std::istringstream` or `strtok`.

### **3. Command Execution**
- [ ] **Fork a child process**: Use `fork()` to create a new process for command execution.
- [ ] **Execute commands**: In the child process, use `execvp()` to execute the command.
- [ ] **Wait for the child process**: In the parent process, use `waitpid()` to wait for the child to complete.

### **4. Handle Built-in Commands**
- [ ] **Exit command**: Implement a way to exit the shell (e.g., if the user types `exit`).
- [ ] **Change directory (cd)**: Use `chdir()` to implement the `cd` command.
- [ ] **Print working directory (pwd)**: Use `getcwd()` to implement the `pwd` command.

### **5. Error Handling**
- [ ] **Command not found**: Handle cases where the command does not exist.
- [ ] **Invalid arguments**: Handle incorrect usage of commands.
- [ ] **Fork/exec errors**: Check for errors in `fork()` and `execvp()` and display appropriate messages.

### **6. Advanced Features (Optional)**
- [ ] **Handle background processes**: Allow commands to run in the background using `&`.
- [ ] **Pipes**: Support commands like `ls | grep cpp` using `pipe()` and `dup2()`.
- [ ] **Redirection**: Implement input/output redirection (`>`, `<`) using `freopen()` or `dup2()`.
- [ ] **Environment variables**: Allow access to environment variables using `getenv()` and `setenv()`.
- [ ] **Prompt customization**: Customize the prompt to display user or directory info (e.g., `user@host:~$`).
