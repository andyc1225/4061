Andy: 
-3/5 Started working on shell, created readline, created cd, created iString, created status, called read_line, called trimwhitespace, created parse_line section, wrote psudocode for exit
-3/6 - Started working on du, created printcwd for testing, created print_size, created cd, created du, wrote out main, added section to check directories and recursively call to check size
-3/6 - Started working on grep, wrote out a comparison function for each word in a file. Have not implemented stdin or printing the line.
-3/7 - John implemented a method to recursively access the directories, I took his sprintf in print_size and implemented it. Removed the recursive method because I didn't use it. Checked to make sure it worked with .. Added closedir.
-3/9 - fixed print statements in du. Implemented grep to work on files. Implemented grep to work on stdin. Added grep function in grep for clarity and shortening the code. Added print current working directory into shell, created exit method for clarity, changed read_line to match the readline in grep, checked to make sure the [4061-shell]cwd $ was working and reading correctly.
-3/10 - Wrote pipe function. Tested pipe function with /cat/bin file.txt as commandin and grep word as commandout. Fixed and completed redirect function. Tested redirect with /cat/bin file.txt | grep word > out.txt and >> out.txt
-3/11 - Changed how to redirect function works. Now it can be run seperately from the pipe function. Run another command after redirect sets the destination of the second command to redirect destination. Set status to be global so exitCMD now works from anywhere. Completed parse function, wrote parse for redir && isPipe, wrote parse for isPipe, wrote parse for redir, wrote parse for single execution. Wrote out all comments in shell, du, and grep. Wrote createArgs function, changed lCommand so that it would be malloced and not destroy stackframe. Took lCommand from previous parser prototype, ran tests that were provided in pdf. Wrote out known bugs, test cases, and assumptions in the Readme.
John: 
-3/6 Started working on du, solved method to correctly recursively access each nested directory. Encountered issue with printing file size. Yet to implement base case.
-3/7 - Changed and shortened fileopen and read line.
-3/10 - Wrote and tested redirect prototype. Was able to create files but not redirect the outputs.
-3/11 - Rewrote parser and redirect. Created prototype that was a viable solution to roadblock.

Last group member did not do any work for any projects in this class and therefore has been redacted


Assumptions - We assumed that:

There would only be one redirect in each command. The redirect function takes a file as a destination and therefore a redirect leading to another redirect would not make sense.

The first command in the command line would lead into a pipe that leads into a second command when pipes are used. As we only used redirects and pipes, and redirect takes a file redirecting into a pipe would not make sense.

The redirect commands only include ">" and ">>".

From the second assumption we continued to assume that a pipe would always be before a redirect.

Assumed that exit and cd would always be single commands since exit and cd do not contain return values or print anything.

The DU command works like in the solution provided where it only prints the final total size of the path, the directory of the path, and nothing else.

CreateArgs calls malloc without freeing the value. However, the value of malloc depends on the command entered which is either 2 + size of command or 5 + size of command. Unless the string for the command was abnormally large or the shell was ran for an abnormally long time, this should not cause issues.

Known Bugs - Du will not work on directories that are too large. Du will work on directories such as User but will not work on root(/) because the recursive function to traverse the tree from root will open too many directories. If root is sufficiently small enough it will work.

Test cases - 
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ du
173428  /mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ du ..
239266  /mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ du /mnt/c/Users/andyc/Desktop
503772292  /mnt/c/Users/andyc/Desktop

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ ls -l
total 192
-rwxrwxrwx 1 chan1467 chan1467    301 Mar 12 02:31 Makefile
-rwxrwxrwx 1 chan1467 chan1467   2608 Mar 12 02:38 README.txt
-rwxrwxrwx 1 chan1467 chan1467  13088 Mar 12 02:49 du
-rwxrwxrwx 1 chan1467 chan1467   3095 Mar 12 02:19 du.c
-rwxrwxrwx 1 chan1467 chan1467  13240 Mar 12 02:49 grep
-rwxrwxrwx 1 chan1467 chan1467   2065 Mar 12 02:32 grep.c
-rwxrwxrwx 1 chan1467 chan1467 107782 Feb 20 22:23 project2.pdf
-rwxrwxrwx 1 chan1467 chan1467  17960 Mar 12 02:49 shell
-rwxrwxrwx 1 chan1467 chan1467   9033 Mar 12 02:29 shell.c
-rwxrwxrwx 1 chan1467 chan1467    988 Feb 20 19:09 util.c
-rwxrwxrwx 1 chan1467 chan1467    564 Feb 20 19:09 util.h
-rwxrwxrwx 1 chan1467 chan1467   2704 Mar 12 02:49 util.o

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ ls -l > out.txt
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ cat out.txt
total 192
-rwxrwxrwx 1 chan1467 chan1467    301 Mar 12 02:31 Makefile
-rwxrwxrwx 1 chan1467 chan1467   2608 Mar 12 02:38 README.txt
-rwxrwxrwx 1 chan1467 chan1467  13088 Mar 12 02:49 du
-rwxrwxrwx 1 chan1467 chan1467   3095 Mar 12 02:19 du.c
-rwxrwxrwx 1 chan1467 chan1467  13240 Mar 12 02:49 grep
-rwxrwxrwx 1 chan1467 chan1467   2065 Mar 12 02:32 grep.c
-rwxrwxrwx 1 chan1467 chan1467      0 Mar 12 02:52 out.txt
-rwxrwxrwx 1 chan1467 chan1467 107782 Feb 20 22:23 project2.pdf
-rwxrwxrwx 1 chan1467 chan1467  17960 Mar 12 02:49 shell
-rwxrwxrwx 1 chan1467 chan1467   9033 Mar 12 02:29 shell.c
-rwxrwxrwx 1 chan1467 chan1467    988 Feb 20 19:09 util.c
-rwxrwxrwx 1 chan1467 chan1467    564 Feb 20 19:09 util.h
-rwxrwxrwx 1 chan1467 chan1467   2704 Mar 12 02:49 util.o

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ grep 12 out.txt
-rwxrwxrwx 1 chan1467 chan1467    301 Mar 12 02:31 Makefile
-rwxrwxrwx 1 chan1467 chan1467   2608 Mar 12 02:38 README.txt
-rwxrwxrwx 1 chan1467 chan1467  13088 Mar 12 02:49 du
-rwxrwxrwx 1 chan1467 chan1467   3095 Mar 12 02:19 du.c
-rwxrwxrwx 1 chan1467 chan1467  13240 Mar 12 02:49 grep
-rwxrwxrwx 1 chan1467 chan1467   2065 Mar 12 02:32 grep.c
-rwxrwxrwx 1 chan1467 chan1467      0 Mar 12 02:52 out.txt
-rwxrwxrwx 1 chan1467 chan1467  17960 Mar 12 02:49 shell
-rwxrwxrwx 1 chan1467 chan1467   9033 Mar 12 02:29 shell.c
-rwxrwxrwx 1 chan1467 chan1467   2704 Mar 12 02:49 util.o

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ grep 1000 out.txt

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ ls >> out.txt
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ cat out.txt
total 192
-rwxrwxrwx 1 chan1467 chan1467    301 Mar 12 02:31 Makefile
-rwxrwxrwx 1 chan1467 chan1467   2608 Mar 12 02:38 README.txt
-rwxrwxrwx 1 chan1467 chan1467  13088 Mar 12 02:49 du
-rwxrwxrwx 1 chan1467 chan1467   3095 Mar 12 02:19 du.c
-rwxrwxrwx 1 chan1467 chan1467  13240 Mar 12 02:49 grep
-rwxrwxrwx 1 chan1467 chan1467   2065 Mar 12 02:32 grep.c
-rwxrwxrwx 1 chan1467 chan1467      0 Mar 12 02:52 out.txt
-rwxrwxrwx 1 chan1467 chan1467 107782 Feb 20 22:23 project2.pdf
-rwxrwxrwx 1 chan1467 chan1467  17960 Mar 12 02:49 shell
-rwxrwxrwx 1 chan1467 chan1467   9033 Mar 12 02:29 shell.c
-rwxrwxrwx 1 chan1467 chan1467    988 Feb 20 19:09 util.c
-rwxrwxrwx 1 chan1467 chan1467    564 Feb 20 19:09 util.h
-rwxrwxrwx 1 chan1467 chan1467   2704 Mar 12 02:49 util.o
Makefile
README.txt
du
du.c
grep
grep.c
out.txt
project2.pdf
shell
shell.c
util.c
util.h
util.o

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ cat out.txt | grep 12
-rwxrwxrwx 1 chan1467 chan1467    301 Mar 12 02:31 Makefile
-rwxrwxrwx 1 chan1467 chan1467   2608 Mar 12 02:38 README.txt
-rwxrwxrwx 1 chan1467 chan1467  13088 Mar 12 02:49 du
-rwxrwxrwx 1 chan1467 chan1467   3095 Mar 12 02:19 du.c
-rwxrwxrwx 1 chan1467 chan1467  13240 Mar 12 02:49 grep
-rwxrwxrwx 1 chan1467 chan1467   2065 Mar 12 02:32 grep.c
-rwxrwxrwx 1 chan1467 chan1467      0 Mar 12 02:52 out.txt
-rwxrwxrwx 1 chan1467 chan1467  17960 Mar 12 02:49 shell
-rwxrwxrwx 1 chan1467 chan1467   9033 Mar 12 02:29 shell.c
-rwxrwxrwx 1 chan1467 chan1467   2704 Mar 12 02:49 util.o

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ cat out.txt | grep 12 > newfile.txt
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ ls
Makefile    du    grep    newfile.txt  project2.pdf  shell.c  util.h
README.txt  du.c  grep.c  out.txt      shell         util.c   util.o
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ cat newfile.txt
-rwxrwxrwx 1 chan1467 chan1467    301 Mar 12 02:31 Makefile
-rwxrwxrwx 1 chan1467 chan1467   2608 Mar 12 02:38 README.txt
-rwxrwxrwx 1 chan1467 chan1467  13088 Mar 12 02:49 du
-rwxrwxrwx 1 chan1467 chan1467   3095 Mar 12 02:19 du.c
-rwxrwxrwx 1 chan1467 chan1467  13240 Mar 12 02:49 grep
-rwxrwxrwx 1 chan1467 chan1467   2065 Mar 12 02:32 grep.c
-rwxrwxrwx 1 chan1467 chan1467      0 Mar 12 02:52 out.txt
-rwxrwxrwx 1 chan1467 chan1467  17960 Mar 12 02:49 shell
-rwxrwxrwx 1 chan1467 chan1467   9033 Mar 12 02:29 shell.c
-rwxrwxrwx 1 chan1467 chan1467   2704 Mar 12 02:49 util.o

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test $ cd ..
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting $ cd ..
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing $ cd subtesting
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting $ cd ..
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing $ cd subtesting/
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting $ cd ..
[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing $ cd /subtesting
failed to change directory, not found: No such file or directory

[4061-shell]/mnt/c/Users/andyc/Desktop/4061_project_2/testing $ exit
chan1467@DESKTOP-T9RE16S:/mnt/c/Users/andyc/Desktop/4061_project_2/testing/subtesting/final test$