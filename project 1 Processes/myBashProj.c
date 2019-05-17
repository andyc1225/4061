#ifndef __cplusplus
typedef int bool;  //define boolean
#define true 1
#define false 0
#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char **argv)
{
    bool pFlag; //Zach said in lab that we can assume -p will be the second argument

    if (argc < 2)
    {

        printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
        exit(1);
    }

    int *ands = (int *)malloc(((argc / 2) - 1) * sizeof(int)); //divide by 2 because there can't be more ands than commands, subtract 1 to remove the name of the program
    int andAmount = 0;

    //loop to find pFlag and place "&" symbols into ands list
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "&") == 0)
        {
            ands[andAmount++] = i;
        }
        if (strcmp(argv[i], "-p") == 0)
        {
            pFlag = true;
        }
    }
    //If pFlag is not true then set it to false
    if (pFlag != true)
    {
        pFlag = false;
    }


    if (andAmount == 0)  //if the command line only contains one command
    {
        int start;
        if (pFlag == true)  //in case the pFlag is not removed
        {
            start = 2;
        }
        else
        {
            start = 1;
        }
        int count = start;
        char *pargs[argc - start];  //amount of arguments is size of entire line minus the name of this program and pFlag if included
        for (int i = 0; i < argc; i++)
        {
            pargs[i] = argv[count++];  //items in pargs start from after name of this program or after pFlag
        }
        execv(argv[start], pargs);
        perror("single execute failed");
        exit(0);
    }
    else
    {
        pid_t pidA[(argc / 2) - 1]; //at least half the string has to be commands and parameters, removes the count for the name of the program as well
        int start, stop, stopMinusStart;
        if (pFlag == true)  //if the pFlag is true
        {
            start = 2, stop = ands[0]; //index three will be the start of the command assuming the p flag is there
            stopMinusStart = stop - start;
        }
        else
        {
            start = 1, stop = ands[0]; //starts after the name of this program
            stopMinusStart = stop - start;
        }
        for (int i = 0; i < andAmount + 1; i++) {  //+1 because andAmount is the total, not counting 0

            int startCount = start;
            char *pargs[stopMinusStart + 1];  //creates a list of arguments for the process, +1 for null
            for (int j = 0; j < stopMinusStart; j++) {
                pargs[j] = argv[startCount++];
            }
            pargs[stopMinusStart] = NULL;


            if ((pidA[i] = fork()) < 0) {
                perror("fork failed");
                exit(1);
            } else if (pidA[i] == 0) {
                execv(argv[start], pargs);
                perror("child execute failed");
                exit(0);
            }

            if (i + 1 < andAmount) { //sets stop and start values based on location of ands
                start = ands[i] + 1;
                stop = ands[i + 1];
                stopMinusStart = stop - start;
            } else {                                   //no more ands, final command
                start = ands[andAmount - 1] + 1;
                stop = argc;
                stopMinusStart = stop - start;
            }

            if (pFlag == false) {  //sequential wait
                wait(NULL);
            }
        }
    }

    if(pFlag == true) {  //parallel wait
        while (andAmount > 0) {
            wait(NULL);
            andAmount--;
        }
    }
}
