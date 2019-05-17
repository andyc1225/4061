#include<stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <grp.h>
#include <stdbool.h>

//prints current working directory
void printCWD(){
    char s[100];
    printf("%s\n", getcwd(s, 100));
}
//also written when I was trying stuff, might be useful in the future
void cd(char* path){
    if(chdir(path) == 0){
        printCWD();
        return;
    }
    else{
        perror("Could not find directory");
        exit(1);
    }
}

//Takes in a path, the name of the file in the path, and the total size to increment
//returns the size after incrementation
int print_size(char* path, char *name, long size) {
    struct stat fileAttributes;
    //Written by John, used for relative traversal
    char curpath[1028];
    sprintf(curpath, "%s/%s", path, name);

    if(stat(curpath, &fileAttributes) < 0){
        perror("Failed to get stat");
        printf("Failed to get %s\n", name);
    }
    if(strcmp(name,".") == 0 || strcmp(name,"..") == 0){  //not counted parent directory or own directory
        return size;
    }
    else if(S_ISDIR (fileAttributes.st_mode) != 0){  //if it is a directory, need to recursively call it, basically same as du function
        DIR* dir;
        struct dirent *dent;

        if((dir = opendir(curpath)) == NULL) {  //opens directory
            perror("Cannot open directory");
            exit(2);
        }
        while ((dent = readdir(dir)) != NULL){  //reads directory
            size = print_size(curpath, dent->d_name, size);  //recursive call
        }
        if(closedir(dir) != 0) {  //closes directory to save memory
            perror("Cannot closes dir");
        }
        return size;

    }
    else{
        //printf("%ld  %30s\n", fileAttributes.st_size, name);  //printf function used in testing. If uncommented it works like -b (i think was the flag)
        size += fileAttributes.st_size;  //adds size to total size
        return size;
    }
}

//takes in a path, returns size of entire path
//works for things that are not too large
//i.e works for /mnt/c/Users on windows, does not work for / because that opens too many directories
void du(char* path){  //works like above
    long size = 0;
    DIR* dir;
    struct dirent *dent;

    if((dir = opendir(path)) == NULL) {
        perror("Cannot open directory");
        exit(2);

    }
    while ((dent = readdir(dir)) != NULL){
        size = print_size(path, dent->d_name, size);
    }
    if(closedir(dir) != 0) {
        perror("Cannot closes dir");
    }
    printf("%ld  ", size);  //final return size
    if(chdir(path) != 0){
        perror("failed to change directory, not found");
        exit(1);
    }
    printCWD();  //prints current directory after size so you know which directory was checked
}


int main(int argc , char *argv[])
{

  // Type your code here
  char* path;
  if (argc < 2) {  //assume that no argument means current directory
      path = ".";
  }
  else{
      path = argv[1];  //takes path only
  }
  du(path);

  return 0;
}
