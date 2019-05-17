#include<stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <grp.h>
#include <stdbool.h>
#include "util.h"
#define BUFFERSIZE 100 //same buffersize as grep
#define MAX_CMD_LEN 256  //from util.h
#define MAXTOKENS 100  //defined in util.h

int status = 1;  //global status for main

void printCWD(){  //taken from du
    char s[100];
    printf("%s", getcwd(s, 100));
}

//cd inside main
//exit inside main
char *read_line(void) {
    char *line = NULL;
    size_t len = 0;
    if(getline(&line, &len, stdin) == -1){
        perror("failed to read line");
    } //taken from grep
    return line;
}

int cdCMD(char *path) {
    if (chdir(path) != 0) {  //change directory function
        perror("failed to change directory, not found");
    }
}

void exitCMD(){
    status = 0;  //ends while loop in main
}

//takes in 2 commands, pipes in to out
//executes command inside, writes to stdout_fileno
void pipeCMD(char** commandin, char** commandout){

    int p[2];  //p[0] = read, p[1] = write
    pipe(p);
    if(fork() == 0){   //fork child
        if(dup2(p[1], 1) == -1){
            perror("dup2 failed");
        }  //from stdout to pipe
        if(close(p[0]) == -1){
            perror("close failed");
        }   //close read side of pipe because we will be writing here
        execv(commandin[0], commandin);  //calls commandin which the output will be piped to parent
        perror("execv broke\n");
    }
    else{
        if(dup2(p[0], 0) == -1){
            perror("dup2 failed");
        } //stdin to pipe
        if(close(p[1]) == -1){
            perror("close failed");
        }  //close write side of pipe because we will be reading here
        char buffer[BUFFERSIZE];
        int nbytes;
        execv(commandout[0], commandout);  //calls commandout which will execute while the child pipes information
        perror("execv broke\n");
        while (fgets(buffer, BUFFERSIZE, stdin)) {  //calls fgets so that each newline is the cutoff for read
            write(STDOUT_FILENO, buffer, nbytes);
        }
    }
    wait(NULL);

}

//takes a filename as a destination and ">" or ">>" for redirect
//changes the output of stout_fileno to destination
void redirect(char* destination, char* redirectType)
{
    int fd;

    if (strcmp(redirectType, ">") == 0)
    {
        if((fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC)) == -1){
            perror("failed to open");
        }//trunc writes over existing file
        if(dup2(fd, STDOUT_FILENO) == -1){
            perror("dup2 failed");
        }
    }
    else if (strcmp(redirectType, ">>") == 0)
    {
        if((fd = open(destination, O_APPEND | O_WRONLY | O_CREAT)) == -1){
            perror("failed to open");
        } //append writes to end of file
        if(dup2(fd, STDOUT_FILENO) == -1){
            perror("dup2 failed");
        }
    }
    else{
        perror("unknown redirect type");
    }
    close(fd);
}

//make sure to add 1 to size of pargs for null char
//takes in a list of tokens that have been parsed by parse_line
//takes in a starting index and an ending index
//the ending index is 1 greater than where you actually want to end i.e. 4 items in array ending index is 4
//takes in a pointer to a string array and returns the same array
char** createArgs(char**tokens, int start, int stop, char** pargs){

    int countPargs = 0;
    //for the command name because all commands need a location
    if(strcmp(tokens[start], "du") == 0 || strcmp(tokens[start], "grep") == 0){  //checks if commands are written by us
        char slash[] = "./";
        char *lCommand = (char*)malloc(sizeof(char) * (2 + strlen(tokens[start]) + 1));  //I don't know how to free this but I know we have to malloc
        sprintf(lCommand, "%s%s", slash, tokens[start]);  //append a file location and the actual command
        pargs[countPargs++] = lCommand;  //add the rewritten command to pargs
    }
    else if(strcmp(tokens[start], "du") != 0 || strcmp(tokens[start], "grep") != 0){  //checks to make sure it is not the implemented du or grep command
        char bin[] = "/bin/";
        char *lCommand = (char*)malloc(sizeof(char) * (5 + strlen(tokens[start]) + 1));
        sprintf(lCommand, "%s%s", bin, tokens[start]);
        pargs[countPargs++] = lCommand;
    }
    //adds the rest of the items to the array
    for(int i = start + 1; i < stop; i++){  //starts at + 1 because we added the command above
        pargs[countPargs++] = tokens[i];
    }
    //adds a null to the end of the argument array to be passed to execv
    pargs[countPargs] = NULL;
    return pargs;
}

//The chunk of the code that actually does stuff
//takes in tokens from parse_line and numToks also from parse_line
void parse(char **tokens, int numtoks)
{
    //declaration of things to pass and check
    bool isPipe = false;
    bool redir = false;
    bool isCd = false;
    char* pathcd;
    int pipeIndex;
    char* redirchar;  //assume there is only one redirect
    char* redirectDest;
    int redirectIndex;

    char **input;
    char **output;

    //first parse out the location of pipes and redirects because thats what this project was about
    for (int i = 0; i < numtoks; i++)
    {
        if (strcmp(tokens[i], "|") == 0)  //assume that the first command for the pipe is the first command of the command line
        {
            isPipe = true;
            pipeIndex = i;
        }
        else if (strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], ">>") == 0)
        {
            redirectIndex = i;
            redir = true;
            redirchar = tokens[i];  //redirect type to pass to redirect
            if(i+1 < numtoks){   //checks to make sure redirect destination exists if destination is at end
                redirectDest = tokens[i+1];  //redirect file to pass to redirect
            }
        }
    }

    //Assumed that cd and exit would be run by themselves as single execute
    if (redir && isPipe)
    {
        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            redirect(redirectDest, redirchar);
            char *pargs1[pipeIndex + 1];
            char *pargs2[redirectIndex - (pipeIndex + 1)];  //using numtoks automatically adds one to total
            pipeCMD(createArgs(tokens, 0, pipeIndex, pargs1), createArgs(tokens, pipeIndex + 1, redirectIndex, pargs2));
            perror("pipe child execute failed");
        }
        wait(NULL);
    }
        //Only a pipe command, no redirect command
    else if (isPipe)
    {
        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            char *pargs1[pipeIndex + 1];
            char *pargs2[numtoks - (pipeIndex + 1)];  //using numtoks automatically adds one to total
            pipeCMD(createArgs(tokens, 0, pipeIndex, pargs1), createArgs(tokens, pipeIndex + 1, numtoks, pargs2));
            perror("pipe child execute failed");
        }
        wait(NULL);
    }
    //just redirection
    else if (redir)
    {
        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            char *pargs[redirectIndex + 1];
            redirect(redirectDest, redirchar);
            execv(pargs[0], createArgs(tokens, 0, redirectIndex, pargs));
            perror("child execute failed");
        }
        wait(NULL);
    }
    //single execute, again assumed that cd and exit would be single execute
    else
    {
        if(strcmp(tokens[0], "exit") == 0){
            exitCMD();
        }
        else if (strcmp(tokens[0], "cd") == 0){
            if(numtoks == 2){
                cdCMD(tokens[1]);
            }
        }
        else {
            pid_t pid;
            if ((pid = fork()) < 0) {
                perror("fork failed");
                exit(1);
            } else if (pid == 0) {
                char *pargs[numtoks + 1];
                execv(pargs[0], createArgs(tokens, 0, numtoks, pargs));
                perror("child execute failed");
            }
            wait(NULL);
        }
    }
}

int main() {

    char *iString;
    // Type your code here

    while (status) {
        // Read from STDIN using file operations. Must not use scanf based functions.
        printf("[4061-shell]");
        printCWD();
        printf(" $ ");

        iString = read_line();  //calls read_line declared above

        iString = trimwhitespace(iString);  //trims whitespace on iString

        int numToks;
        char *tokens[MAXTOKENS];  //maxtokens declared in util
        char delimit[] = " ";  //separate by white space
        numToks = parse_line(iString, tokens, delimit);  //puts all tokens of iString into tokens, numToks is number of tokens
        parse(tokens, numToks); //calls parse
    }
    return 0;
}
