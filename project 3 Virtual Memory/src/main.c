//main.c program to translate the virtual addresses from the virtual.txt
//into the physical frame address. Project 3 - CSci 4061

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "vmemory.h"

#define INPUT_FILE "../bin/virtual.txt"

int main(int argc, char* argv[])
{
	if (argc > 2) {
		printf("Too many arguments, enter up to one argument\n");
		exit(-1);
	}

	int policy = (argc == 2) ? 1:0;
	initialize_vmanager(policy);
	/* Tests used to make sure all functions work properly
	printf("%d\n", fetch_physical_frame((unsigned int) 0x72ae2247));
	printf("%d\n", fetch_physical_frame((unsigned int) 0x00000000));
	print_physical_address(464680, 583);
	print_physical_address(0x9ea, 0x628);
	print_tlb();
	populate_tlb(0xd82c0, 0x7e6b0);
	populate_tlb(0x6baa9, 0x7fc3d);
	populate_tlb(0x82e2e, 0x7b6f8);
	populate_tlb(0x72ae2, 0x71728);
	populate_tlb(0x7a024, 0x72a2c);
	populate_tlb(0xe87a1, 0x7a60d);
	populate_tlb(0x81332, 0x73b0f);
	populate_tlb(0x48268, 0x7f90f);
	populate_tlb(0x72ae2, 0x71728);
	printf("This one should return %d\n", get_tlb_entry(0x72ae2));
	print_physical_address(0x72ae2, 0x247);
	printf("This one should page fault %d\n", get_tlb_entry(0x00000));
	print_physical_address(0x00000, 0x000);
	printf("This one should return %d\n", get_tlb_entry(0xd82c0));
	print_physical_address(0xd82c0, 0x333);
	populate_tlb(0xC17C6, 0x7aba8);
	printf("This one should miss %d\n", get_tlb_entry(0xd82c0));
	print_physical_address(0xd82c0, 0x333);
	printf("Should be 1, %f\n",get_hit_ratio());
	print_tlb();
    printf("This one should miss %d\n", get_tlb_entry(0x82e2e665));
    printf("This one should return %d\n", get_tlb_entry(0x82e2e665));*/

	//TODO: Fill the algorithm by reading from INPUT_FILE
	//Takes input file and it calls get_tlb_entry on every address in the file, calls print_tlb at end
	//fetch_physical_frame also works but is commented out, uncomment to use
	//Main was pretty ambiguous in what to do, thats why the tests above are so long
    FILE *fp = fopen(INPUT_FILE, "r");
    if (fp == NULL)
    {
        perror("cannot open file");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        //printf("%s", line);
        //printf("%d\n", fetch_physical_frame(strtoll(line, NULL, 0)));
        get_tlb_entry(strtoll(line, NULL, 0));
    }
    print_tlb();
    //printf("%f\n", get_hit_ratio()); it should be zero because they all miss
    fclose(fp);
	
	//Free the page table
	free_resources();
	return 0;
}
