File I/0 Utility Programs:

Jared Wilson (wil00352)

Matthew J


Design of our program:

We used our base shell program from the last lab to start our project. It uses the same parse and read line functions along with
other initializtion so we did not have to rewrite that code. It has different executable commands though which we developed using
the Utility programs and libraries. We designed our commands using a switch statement that tests for the specific builtins
that we created. With this, it calls upon the specific functions corresponding with the keyword used. Using the "help" command
will show all the built-ins that we created and how to use them. Also in this README, we will describe how to use these as well.
We used new libaries to implement functions and variables that we did not in the first one. An example of this is using DIR and FILE 
as variable types. We also used pointers a lot to step through directories and files when needed. 


List of Commands and how they are used:

Once compiling and running our shell, it will come up with a prompt. Here you can enter any of the system commands or our builtin
commands. The built-in commands are as follows - 

ls:
Using the "ls" command, you can see all of the different folders and files in the current directory you are in with your program.
This uses directory functions to list out what filenames are in the directory. It will iterate trhough the directory as well until
you hit the end which displays everything at once.

find:
Using the "find" command, it will help you find a file by starting at a directory. This starting directory then can iterate through
your system until it finds the file you are looking for. This process is shown as the command is running. It uses the directory dirent
function calls which helps keep track of what directory you are in.

grep:
The "grep" command takes a exact string and exact filename as two arguments and will search through the file for that exact string. 
This then displays the line it is in and what line number it is as it goes through the code. It can be as intricat as one word or an
entire sentence that it can find. As long as the file is in the same folder as your code, it will work. This uses system and standard
I/O libraries to serach through the file for the string.

more:
Instead of sending an output directly to the screen, this output will go to the "more" command that will ouput a screen at at time for
output. This also uses ioctl() as it has operating characteristics of character special files, including terminals. This allows to display
output in one terminal page at a time.
 

help:
Using the "help" command, it produces a list of all the built-in commands that we have implemented. This is helpful to new users
as they can see what they can do while running our shell.

exit:
The "exit" command turns off our shell. This completely stops the current process you are in. If you are in a cloned, child
process then it will take you back to the first parent. If you are in the parent, it will close the program entirely. 


Pros and Cons of our Shell:

Pros: Easy to use, compiles, runs forever unless exited, does the simple tasks as assigned to us, different I/0 functions

Cons: More was the most difficult to write and confusing. It was not explained very well in the assignment.

Intructions on using our Shell:
1. Put the file into a specific place on your computer
2. Open up the terminal and cd into that folder
3. Run "gcc myshell.c -o foo"
4. Run "sudo ./a.out"
5. This should show the command line for the shell, you can now enter any of the above commands shown.
6. Type help to see the list of builtin commands
7. The system commands can also be used such as mkdir, echo, and ls.
8. To exit, type exit into the shell
