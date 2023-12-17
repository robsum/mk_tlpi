# mk_tlpi
Excercises from the book Michael Kerrisk - The Linux Programming Interface

# usage
To compile programs, download mk_tlpi repository from book's website: https://man7.org/tlpi/code/
Then, extract it and rename folder to mk_linux_programming_interface.

Before compilation, set the CPATH variable to mk_linux_programming_interface directory location, ex:
export CPATH=/home/robert/Workspace/
Then, the files should compile successfully.
For some files it may be necessary to compile it with .o files like this:
gcc $CPATH/mk_linux_programming_interface/tlpi-dist/lib/error_functions.o $CPATH/mk_linux_programming_interface/tlpi-dist/lib/get_num.o free_and_sbrk.c
