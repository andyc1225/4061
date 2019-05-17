Group Members:
Andy Chang - 
2/15 - met with groups for the first time, added lines 26 - 39
2/23 - added for lines 70 - 109
2/24 - fixed bugs, added lines 117 - 122
2/25 - fixed bugs, added lines 111 - 113, added lines 41 - 69, changed typedef boolean declaration, ran tests, added comments, added error codes (lines 111 - 113 extended to 115, lines 122 extended to 128), created Makefile

I had 2 other group member, they did not do anything in this project so I have removed them from the readme

Known Bugs - During the first and following runs of ./myBashProj -p /bin/cat file1.txt file2.txt "&" /bin/echo "Hello!", the text of file2.txt will appear on the user's typing line.
Ctrl+C to escape and then re-typing it will work once, then the same error will occur again.
Also seems to happen sporadically.
No other known bugs.
Example:
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ ./myBashProj -p /bin/cat file1.txt file2.txt "&" /bin/echo
"Hello!"
Hello!
test file 1
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ test file 2


Test cases used - 
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ gcc myBashProj.c -o myBashProj
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ ./myBashProj -p /bin/cat file1.txt file2.txt "&" /bin/echo "Hello!" "&" /bin/cat file1.txt file2.txt
Hello!
test file 1
test file 1
test file 2
test file 2
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ ./myBashProj -p /bin/cat file1.txt file2.txt "&" /bin/echo "Hello!" "&" /bin/cat file1.txt file2.txt
Hello!
test file 1
test file 1
test file 2
test file 2
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ ./myBashProj -p /bin/cat file1.txt file2.txt "&" /bin/echo "Hello!"
Hello!
test file 1
test file 2
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ ./myBashProj /bin/cat file1.txt file2.txt "&" /bin/echo "Hello!" "&" /bin/cat file1.txt file2.txt
test file 1
test file 2
Hello!
test file 1
test file 2
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ ./myBashProj /bin/cat file1.txt file2.txt "&" /bin/echo "Hello!"
test file 1
test file 2
Hello!
chan1467@csel-kh1250-01:/home/chan1467/Desktop/4061Project $ ./myBashProj /bin/cat file1.txt file2.txt
test file 1
test file 2

Extra credit has not been attempted.

Extra instructions:
It is assumed that the p Flag (-p) will always be the first input after the ./ExecutableName
e.g. ./ExecutableName -p command parameters "&" command parameters