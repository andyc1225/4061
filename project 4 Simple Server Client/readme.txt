Andy - Reorganized all the shared memory code outside of the while loops
made accept non-blocking by adding fctl and EWOULDBLOCK
added message typing
added getline and strcpy code in case statements
added "clear input buffer while" loop
added msgrcv in case 1 and 2
moved shmdt outside of loop
added send for the exit in clients
added other msgrcv for broadcast and changed flags to nowait
added msgsnd in clients
added some comments
Finalized and tested all the code.
Wrote readme

John
Created server, client1, client2 and set up the socket connections
and the menu/input/msg prompts and switches in them
set up working message queue and shared mem, and all the key stuff with it
and the initial sending recieving stuff for them
wrote makefile

Bugs - no known bugs.

no extra credit attempted

Test cases -

tested client 1 and 2 for displayed message and return message

server menu -> 1 -> some message
client1 -> r -> server menu

server menu -> 1 -> some message
client2 -> r -> server menu

server menu -> 3 -> some message
server menu -> 3 -> some message

server menu -> 4 -> option does not exist

server menu -> 0 -> closes all server and client
