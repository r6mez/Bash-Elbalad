# Shell-Elbalad
A simple linux shell project.

# Todo:
## **1. Project Setup**
- [x] **Set up the project.**

## **2. Core Functionality**
### **Shell Basics**
- [x] Design the shell prompt
- [x] `Exit` command
- [x] `pwd` command
- [ ] `cd` command
- [x] `clear` command
- [x] Prompt colors

### **Command Execution**
- [x] Command parsing
- [ ] Fork a child process
- [ ] Execute commands: use `execvp()` to execute the command in child process.
- [ ] Wait for the child process `waitpid()`

## **3. File Management Commands**
- [x] **cat**: Display file contents.
- [x] **touch**: Create new files.
- [ ] **rm**: Delete files.
    - [ ] Implement `-f` option (deletes non-empty dirs).
- [ ] **mv**
    - [x] Rename files.
    - [ ] Move file.
- [ ] **cp**: Copy files.
    - [ ] copy file into the same directory
    - [ ] Implement copy file into other directories
- [x] **ls**: List directory contents.
    - [x] Implement `-l` option (long list). 
- [x] **stat**: Display detailed file information.
- [ ] **chmod**: Change file permissions.
- [ ] **find**: Search for files.
- [ ] **mkdir**: Create directories.
    - [ ] Implement `-p` option.
- [x] **rmdir**: Remove directories.

## **5. Advanced Features**
### **Command Composition**
- [ ] **Pipes**: Support commands like `ls | grep cpp` using `pipe()` and `dup2()`.
- [ ] **Redirection**: Implement input/output redirection (`>`, `<`) using `freopen()` or `dup2()`.
- [ ] **Running multiple commands (;)**: Support executing multiple commands in sequence.

### **Environment and History**
- [ ] **History**: Store previously run commands and allow the user to view them using a command like `history`.
- [ ] **Handle Arrow Presses**: moving around the line and accessing history.
