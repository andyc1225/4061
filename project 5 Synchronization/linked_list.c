#define _DEFAULT_SOURCE
#define NUM_ARGS 2

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#include "linked_list.h"

//given from header
node *list;

//Create mutex
pthread_mutex_t lock;


//int threadno = 0;
int seq_no = 0;
int line_no = 0;
bool end = false;
char* fileName;

//Read a particular line from file
//param 1: file name, param 2: line_no
char* read_line(char * s, int l){
    //printf("in read_line\n");

    FILE *fp;
    char *str;
    str = (char*) malloc(sizeof(char)*LINE_SIZE);

    fp = fopen(s , "r");
    if(fp == NULL) {
        perror("Error opening file");
        exit(-1);
    }

    //move to line number
    for(int i = 0; i < l; i++){
        if(fgets (str, LINE_SIZE, fp)==NULL){
            //printf("reached the end\n");
            end = true;
            return NULL;
        }
        //puts(str);
    }

    return str;

}

//Reverses all the pointers in a linked list
//takes the head node
node* reverse(node * head)
{
    node* prev = NULL;
    node* current = head;
    node* next = NULL;
    while (current != NULL)
    {
        next  = current->next;
        // Reverse current node's pointer
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
}

//Traverse the linked list with the head as the argument
void traversal(node * arg){
    node *head = arg;
    head = reverse(head);
    FILE *file = fopen("copy.txt", "w");
    if(file == NULL){
        perror("Error opening file");
        exit(-1);
    }
    char buf[256];
    while(head->next != NULL) {
        //printf("%d %d %s", head->seq_no, head->line_no, head->content);
        sprintf(buf, "seq %d, line %d, content %s", head->seq_no, head->line_no, head->content);
        if(fputs(buf, file) == EOF){
            perror("failed to write");
            exit(-1);
        }
        head = head->next;
    }
}

//Insert a node created using create_node into the linked list
//param 1: head address, param 2: node
void insert(node * h, node * n){
    node *head = h;
    //insert first node after head node
    if(head->next == NULL){
        head->next = n;
    }
    else {
        //places it in order or at end if n->line_no is greatest
        while (head->next != NULL && n->line_no < head->next->line_no) {
            head = head->next;
        }
        if(head->next != NULL) {
            n->next = head->next;
        }
        head->next = n;
    }
}

//Create a new node to be inserted into the linked list
//param 1: line_no; param 2: line content
node* create_node(int i, char * s){

    node *temp = (node *)malloc(sizeof(node));

    //fill the node
    temp->seq_no = seq_no++;
    temp->line_no = i;
    temp->content = s;
    temp->ll_lock = *(pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

    //initialize the mutex
    pthread_mutex_init(&temp->ll_lock, NULL);

    temp->next = NULL;
    return temp;
}

void readInsert(void* arg) {

    node *head = arg;

    //int thisthread = threadno;
    //threadno++;

    //int internalcounter = 0;

    int templine;


    while (end != true) {
        //printf("threadno read = %d\n", thisthread);

        //lock to increment line_no
        pthread_mutex_lock(&lock);
        //if reached the end just break the loop, unlock for next thread
        if (end == true) {
            pthread_mutex_unlock(&lock);
            break;
        }
        //increment line number
        line_no++;
        templine = line_no;
        pthread_mutex_unlock(&lock);

        //read a line
        char *line = read_line(fileName, line_no);

        //if line is null then its reached EOF
        if(line == NULL){
            break;
        }

        //printf("line read in threadno %d = %s", thisthread, line);


        //use head node to lock for insert
        pthread_mutex_lock(&head->ll_lock);
        //create a node
        node *temp = create_node(templine, line);

        //insert
        insert(head, temp);
        //printf("threadno insert %s at %d\n", line, thisthread);
        pthread_mutex_unlock(&head->ll_lock);

        //internalcounter++;
    }
    //printf("internal counter %d\n", internalcounter);
}

int main(int argc, char *argv[])
{
    if (argc != NUM_ARGS + 1) {

        printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
        exit(1);
    }

    fileName = argv[1];
    int threadInt = atoi(argv[2]);

    if (threadInt < 1 || threadInt > 16)
    {

        printf("Thread # must be between 1-16\n");
        exit(1);
    }
    pthread_t threads[threadInt];

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

    head = create_node(line_no, read_line(fileName, line_no));


    //changed readline to just read
    for (int i=0; i < threadInt; ++i) {
        pthread_create(&threads[i], NULL, readInsert, (void*) head);
    }

    for (int i=0; i < threadInt; ++i) pthread_join(threads[i], NULL);

    //printf("%d %d %s",head->seq_no, head->line_no, head->content);

    traversal(head);

}