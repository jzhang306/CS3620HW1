### How to run:
#### Task1: ./shell<br>
#### Task2: ./Monitor<br>
#### Task3: <br>
GUI.py and Monitor.c must stay at the same path<br>
In the terminal, run with python3 GUI.py<br>
### What should be learned?<br>
1. By building a UNIX shell, I learned how to use the system calls, especially: exit(), exec(), and fork().<br>
The built-in shell takes in the command argument by standart I/O, and then parse it into two parts: commands, and arguments. After storing them in an array, we then need to determine if the command is a system call. Since cd, history, help, exit, etc. belong to the built-in commands, which means they run directly in the shell program, they are fast, but we have to implement them. For external commands, since they are all executables in the /bin folder, we can invoke system calls by creating children process to run them. The special function is execvp() in exec() family.<br>
2. The process monitor is a program that visits the files in /proc to extract the information from the process file. /proc folder contains all the useful information about processes, after going into one process folder, we can find files and use cat command to open exe(if permitted) and status which contains the context switches, user id, etc. We have to iterate over all processes, then in every process, we iterate over the status file and get wanted texts. After doing that, we only keep those matching our user id, and then print out the exe path stored in proc/.../exe.<br>
3. A GUI for the monitor is implemented by python tkinter, a very useful module for drawing and GUI. As required by the task3, the GUI should base on task2, so I imported subprocess to run the executable file in task2. After running task2, I capture the output of the process, decode them,  and then create a tkinter gadget using the scrolled text to store those output words.<br>

### Credit:<br>
1. This website clearly explains how to construct a shell, and its basic structure, but it uses double pointers and function pointers, which are harder and unnecessary for this assignment. I also learned the codes that parse the input string.<br>
https://brennan.io/2015/01/16/write-a-shell-in-c/
https://github.com/brenns10/lsh

2. This video implements these functions in a very different way, it uses buffer and scanf to store the input data, and parse the input differently. Only reference to give me a simpler structure for the shell codes.<br>
https://www.bilibili.com/video/BV1bT4y1E76h/

3. For the process monitor:<br>
https://stackoverflow.com/questions/33266678/

https://stackoverflow.com/questions/48778826/c-extracting-info-from-file-in-proc

https://stackoverflow.com/questions/9241538/dt-dir-undefined

https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-readlink-read-value-symbolic-link

https://docs.python.org/3/library/tkinter.scrolledtext.html
