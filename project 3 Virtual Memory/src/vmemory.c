//Implement the API, modeling the translation of virtual page address to a
//physical frame address. We assume a 32 bit virtual memory and physical memory.
//Access to the page table is only via the CR3 register.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>

#include "vmemory.h"

#define OUT_TLB "../bin/tlb_out.txt"
#define Max 8

bool FIFO_policy = true;
int **cr3;

static int tlb[Max][2];
static int miss = 0;
static int totHit = 0;
static int tail = 0;
static int itemCount = 0;


//
// More static functions can be implemented here
//

//FIFO implementation of a queue with circular implementation or whatever the heck its called
static void enqueue(int vadd, int padd){
    tlb[tail][0] = vadd;
    tlb[tail][1] = padd;
    if(tail == Max - 1){
        tail = 0;
        //printf("full, next will be an eviction\n");
    }
    else{
        tail++;
    }
}

//sets all values of tlb to -1
static void initializeTLB(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 2; j++){
            tlb[i][j] = -1;
        }
    }
}

//takes a hex integer 0-15 and returns the binary representation as a string
static char* getBinaryFromHex(int hex){
    switch (hex)
    {
        case 0:
            return "0000";
        case 1:
            return "0001";
        case 2:
            return "0010";
        case 3:
            return "0011";
        case 4:
            return "0100";
        case 5:
            return "0101";
        case 6:
            return "0110";
        case 7:
            return "0111";
        case 8:
            return "1000";
        case 9:
            return "1001";
        case 10:
            return "1010";
        case 11:
            return "1011";
        case 12:
            return "1100";
        case 13:
            return "1101";
        case 14:
            return "1110";
        case 15:
            return "1111";
        default:
            printf("\n Invalid hex digit %c ", hex);
            return "invalid";
    }
}


// The implementation of get_vpage_cr3 is provided in 
// an object file, so no need to re-implement it
void initialize_vmanager(int policy) 
{
	// Set LRU policy when passsed as a parameter
	if (policy)
		FIFO_policy = false;
	cr3 = get_vpage_cr3();
	initializeTLB();
	//printf("cr3: %p\n", cr3);  just some extra testing
    //printf("cr3: %p\n", cr3[458][738]);
    /*
    if(*cr3 == NULL){
        printf("is null\n");
    }
    else{
        printf("wtf\n");
    }*/
}

//
// The implementation of following functions is required
//
//Virtual address translation

//takes a full sized virtual address and returns the physical value at that address
int fetch_physical_frame(unsigned int v_addr)
{
    unsigned int bits[8];
    unsigned int binary[20];
    for(int i = 0; i < 8; i++){  //first turn the whole address into a hex array
        bits[i] = (v_addr >> 4*i) & 0xF;
    }
    /*
    for(int i = 0; i < 8; i++){
        printf("%d\n", bits[i]);
    }
    */
    for(int i = 3; i < 8; i++){  //start at 3 to ignore the offset bits
        char* temp = getBinaryFromHex(bits[i]); //turn the 5 most significant hex addresses into 20 bits
        //printf("%s\n", temp);
        int count = 0;
        for(int j = (i-3)*4; j < ((i-3)*4)+4; j++){  //increments with -3 to account for i = 3 and *4 because the loop runs 4 times and so counter for binary should be in multiple of 4s
            if(strcmp(temp,"invalid") == 0){
                perror("something went wrong; invalid character");
                break;
            }
            else{
                //printf("%c\n", temp[count]);
                char array[2];
                array[0] = temp[count++];
                array[1] = '\0';
                //printf("j = %d\n",j);
                binary[j] = atoi(array);
            }
        }
    }
    /*
    for(int i = 0; i < 5; i++){
        for(int j = i*4; j < (i*4)+4; j++){
            printf("%d", binary[j]);
        }
        printf("\n");
    }*/

    //turns the binary numbers into integers, 10 binary numbers each for 20 msb
    int Switch = 0;  //switches at 10 to go to second part of the virtual address
    int power = 1;
    int indexNum1 = 0;
    int indexNum2 = 0;
    for(int i = 0; i < 5; i++){
        for(int j = (i*4)+3; j >= i*4; j--){
            if(Switch == 10){
                power = 1;
            }
            if(Switch < 10){
                indexNum1 += binary[j] * power;
                power = power * 2;
            }
            else{
                indexNum2 += binary[j] * power;
                power = power * 2;
            }
            Switch++;
        }
    }
    //printf("inum1 %d\n", indexNum1);
    //printf("inum2 %d\n", indexNum2);
    //Checks if it exists in the cr3
    if(&cr3[indexNum2][indexNum1] != NULL){
        //printf("cr3: %p\n", cr3[indexNum2][indexNum1]);
        return cr3[indexNum2][indexNum1];
    }
    return -1;
}

//prints the physical address
void print_physical_address(int frame, int offset)
{
    //doesn't exist print -1
    if(frame == -1){
        printf("-1\n");
        return;
    }
    //same thing as before but it does it in order of most significant to least
    unsigned int bits[8];
    int downcount = 4;
    for(int i = 0; i < 5; i++){
        bits[downcount--] = (frame >> 4*i) & 0xF;
    }
    //now for the offset
    downcount = 7;
    for(int i = 0; i < 3; i++){
        bits[downcount--] = (offset >> 4*i) & 0xF;
    }
    //prints the whole thing
    printf("0x");
    for(int i = 0; i < 8; i++){
        printf("%x", bits[i]);
    }
    printf("\n");
    return;
}

// TLB implementation
//Takes an address, doesn't matter if its only 20 msb or the whole one, checks if its in the TLB.
//If it is in the TLB it returns it, else it swaps it from the page table and returns -1
//This is where hits and misses are added to global counter
int get_tlb_entry(int n)
{
    bool hit = false;
    int index = 0;
    //checks if its in the tlb already
    for (int i = 0; i < 8; i++)
    {
        if (tlb[i][0] == n) {
            hit = true;
            index = i;
        }
    }

    if (hit)
    {
        totHit++;  //adding hits to global counter
        return tlb[index][1];  //return physical frame
    }

    //misses, need to add it to tlb and return -1

    //This part is if the number entered only contains the 20 MSB, otherwise get_tlb_entry works fine with full length addresses
    int temp = n;
    if(temp >> 20 == 0x0){
        //this part makes sure that n has a false offset because fetch_physical_frame takes an offset
        for(int i = 0; i < 3; i++){
            n = (n << 4);  //I could also just do n = (n << 12) but its already here
        }
    }

    //now add it to the tlb
    int phyFrame = fetch_physical_frame(n);
    if(phyFrame == -1){
        //perror("Page Fault, page does not exist");
    }
    else{
        //using enqueue for the FIFO
        enqueue(n,phyFrame);
    }

    //printf("miss\n");
    miss++;  //Adding misses to global counter
    return -1;

}

//Addes the virtual address and physical address into tlb based on FIFO
//Adding it in does not count hits or misses since this would be manual
void populate_tlb(int v_addr, int p_addr) 
{

    //printf("%d\n", tail);

    //No duplicates
    bool hit = false;
    int index = 0;

    for (int i = 0; i < 8; i++)
    {
        if (tlb[i][0] == v_addr) {
            hit = true;
            index = i;
        }
    }

    if (hit)
    {
        //printf("repeat\n");
        return;
    }

    //only add it if nothing the item is not a double
    enqueue(v_addr,p_addr);
    return;
}

//uses global counter to return hit:miss ratio
float get_hit_ratio()
{
	return totHit/miss;
}

//Write to the file in OUT_TLB
void print_tlb()
{
    //flushes to make sure buffer is clean
    fflush(stdout);
    int fd;
    int saved_stdout = dup(STDOUT_FILENO); //for restoring after the function

    //will write over last version and create
    if((fd = open(OUT_TLB, O_WRONLY | O_CREAT | O_TRUNC)) == -1){
        perror("failed to open");
    }//trunc writes over existing file
    if(dup2(fd, STDOUT_FILENO) == -1){
        perror("dup2 failed");
    }
    for(int i = 0; i < Max; i++){
        printf("%x %x\n", tlb[i][0], tlb[i][1]);  //writes out the hex addresses
    }
    printf("\n");
    fflush(stdout);  //flushes again to make sure all items are written to file
    close(fd);
    dup2(saved_stdout, STDOUT_FILENO); //restores output to terminal
    close(saved_stdout);
	return;
}
