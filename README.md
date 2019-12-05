
### 1. Purpose of 'mapreduce' Program ###
Given a folder with multi-level hierarchy, this program can count the number of words, per letter of the alphabet in the entire folder hierarchy. The result will be reported in the alphabetical order and redirected to a file.

---

### 2. How to Compile the Program ###
'mymake' can be compiled by a given Makefile inside the directory src
User can compile the program by ’make':
```
    $ make
```
Then an executable named 'mapreduce' will be generated. To remove the executable and the generated files, user can simply type in 'make clean'.
```
    $ make clean
```
---

### 3. What exactly the program does ###
This program can be executed via
- $ ./mapreduce [folderName] [#mappers]

- folderName would be the directory the user want to traverse
- #mappers would be the number of mapper processes that executes the map function on the partitioned data.

1. Data Partitioning Phase

    First, the program will check if the given directory exists. If not, nothing will happen. Otherwise, the program will traverse the directory and create a folder called "MapperInput" in the current dirctory and create mapper files that contains path of the files inside the given directory. The number of mapper files will depend on the number of mappers that user inputs. 

2. Map Function

    In this phase, the program will spawn all the mapper processes and each one will count the number of words of files inside it and stored them in alphabetical order. Multiple pipes will be created and each mapper will write its result in its corresponding pipe.

3. Reduce Function

    In this phase, the program will read all the data of pipes written by the previous process. And sum up all the data. The result will stored in a file called "ReducerResult.txt". 

4. Final Result

    The result generated from the previous phase will redircet to a file called "FinalResult.txt"

---

### 4. Additional Assumptions ###
-   After running the program, multiple files will be created. Users need to clean up these files on ther own, if they need to "mapreduce" another folder. Otherwise, the result would be incorrect since the existence of previous mappers and result files can result in unexpected effects.

---


