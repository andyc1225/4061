Andy - 3/27 - Finished fetch_physical_frame. Added getBinaryFromHex.
3/28 - Finished print_physical_address.
3/29 - Finished get_tlb_entry. Added initializetlb.
3/30 - Finished and fixed get_tlb_entry, populate_tlb, get_hit_ratio, and print_tlb. Added enqueue. Added static variables and defined max
4/1 - Finished testing and main. Flushed out all the comments. Added check in get_tlb_entry to work with full length addresses

John
3/28 - worked on get_tlb_entry. Instantiated queue struct with internal array
3/29 - changed queue struct to 2d array that overwrote old elements
3/30 - wrote open file and read loop in main

Bugs - no known bugs.
Test cases are in main. Large comment block. You can't miss it.

in case you want it here
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

464680
-1
0x71728247
0x009ea628
This one should return 464680
0x72ae2247
This one should page fault -1
0x00000000
This one should return 517808
0xd82c0333
This one should miss -1
0xd82c0333
Should be 1, 1.000000
This one should miss -1
This one should return 505592

No extra credit attempted

The tlb does not deal with offset because print_physical_address takes offset as a parameter. Because of this it is assumed that the offset will be dealt with by the user.
All other assumptions for how code should work are written just before each function declaration.
