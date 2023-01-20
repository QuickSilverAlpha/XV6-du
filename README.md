# XV6-du
Implemented du command with some modifications in XV6

## Part 1 - Implementing du program
If you are familiar with this command already, please note that for Part 3 and Part 4 of this assignment, we are changing the default behavior of du to better reflect points/difficulty.

### Objective: 
Create a user program named “du” that prints a list of the files in the current directory (where the program was run), with their on-disk size in bytes.  The final line prints the sum of all of the sizes of the files within the directory.

`User call:`
`du`
`(no arguments or flags)`

`Expected Output: (Correction on output – added “./” prefix)`
`34 ./file1`  
`234 ./file2`  
`1 ./file3`  
`34577 ./file4`  
`34846`  



(you must include a single newline character after your final line)
Each line is formatted as “%d %s” – “{size in bytes}{space}{filename}”
The final line is the sum of the sizes of all files in the directory where du was called.  

**Expected Behaviors:**
Size is considered to be the byte count stored in the xv6 stat structure.
Only T_FILE types should be included in the output and in the calculation of the directory size.
This implementation (with no arguments or flags) should not recurse into subdirectories.
Your program should be able to be run from within any directory within xv6, and to report the files within that directory correctly.  

**Hints:**
Analyze the existing ls program to understand how its output relates to this new program’s requirements, as well as for ideas about how to structure your own program. 


## Part 4 - Flags/Args for your  du program
Just as in regular C, the main() function for an xv6 user program can accept the following signature:

`int main(int argc, char *argv[]){ … }`  

The first variable indicates the number of space delimited arguments that were invoked by the user, and the second is an array of those arguments, already split by spaces.

### Objective:
Add functionality to your prior du program so that it can handle various combinations of the following flags/arguments:

**-k** 
Report the number of blocks each entry is allocated in the filesystem (instead of bytes)  

**-t [threshold]**
Filter files based on a lower-bound threshold of > [threshold] bytes.
[file | directory]
