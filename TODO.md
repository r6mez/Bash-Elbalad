# Todo:
---

## **1. Project Setup**
- [x] **Set up the project.**

## **2. Core Functionality**
### **Shell Basics**
- [x] Design the shell prompt
- [x] `Exit` command
- [x] `pwd` command
- [x] `cd` command
- [x] `clear` command
- [x] Prompt colors

### **Command Execution**
- [x] Command parsing
- [x] Fork a child process using `fork()` and`execvp()`.

## **3. File Management Commands**
- [x] **cat**: Display file contents.
- [x] **touch**: Create new files.
- [x] **rm**: 
    - [x] remove files. 
    - [x] Implement `-f` option (deletes non-empty dirs).
- [x] **mv**
    - [x] Rename files.
    - [x]  Move file.
- [x] **cp**: Copy files.
    - [x] copy file into the same directory
    - [x] Implement copy file into other directories
- [x] **ls**: List directory contents.
    - [x] Implement `-l` option (long list). 
- [x] **stat**: Display detailed file information.
- [x] **find**: Search for files.
- [x] **mkdir**: Create directories.
    - [x] Implement `-p` option.
- [x] **rmdir**: Remove directories.

## **5. Advanced Features**
### **Command Composition**
- [ ] **Pipes**: Support commands like `ls | grep cpp` using `pipe()` and `dup2()`.
- [x] **Redirection**: Implement input/output redirection (`>`, `<`) using `freopen()` or `dup2()`.
- [x] **Running multiple commands (;)**: Support executing multiple commands in sequence.

### **Environment and History**
- [x] **History**: Store previously run commands and allow the user to view them using a command like `history`.
- [x] **Handle Arrow Presses**: moving around the line and accessing history.
