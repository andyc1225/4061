#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"  //parse_line function taken from util.h
typedef int bool;  //define boolean
#define true 1
#define false 0
#define BUFFERSIZE 100  //buffer_size taken from util.h

//grep takes in a line, a length, and a key to search for
//prints out a the entire line that contains a key
void grep(char *line, size_t len, char *key){
        int numToks;
        char *tokens[len];
        char delimit[]=" ";
        char fullLine[len];
        strcpy(fullLine,line);
        numToks = parse_line(line, tokens, delimit);  //puts all tokens of iString into tokens, numToks is number of tokens

        bool printed = false;  //checks to see if the line has been printed in case of repeat keys in a single sentence

        for (int i = 0; i < numToks; i++){
            //code to go word by word through curline, if found, print line
            if((strcmp(tokens[i],key)==0) && printed != true) {
                printf("%s", fullLine);
                printed = true;
            }
        }
        printed = false;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {

        printf("Wrong number of args, expected 1, given %d", argc - 1);
        exit(1);
    }

    char *key = argv[1]; //key is always the first item passed in
    if (argc >= 3)  //ignore any argument after argv[2]
    {
        char *filename = argv[2];

        int pos = 0;


        FILE *fp = fopen(filename, "r"); //opens file
        if (fp == NULL)
        {
            perror("could not open file");
        }

        char *line = NULL;
        size_t len = 0;

        while (getline(&line, &len, fp) != -1) {

            grep(line,len, key);

        }
        printf("\n");
        fclose(fp);
    }
    else {  //read from stdin
        char *line = NULL;
        size_t len = 0;
        while(getline(&line, &len, stdin) != -1){
            grep(line,len, key);
        }
    }
    return 0;
}