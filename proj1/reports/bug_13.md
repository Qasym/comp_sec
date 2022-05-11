# BUG-13

## Category
- Unchecked system call returning code

## Description
- **solid.c** program allocates memory for `pixel` variable,<br>
however it does not check if the allocation was successful

## Affected Lines in the original program
- **solid.c**:16

## Expected vs Observed
- Expected behavior: program handles the allocation error
- Observed behavior: program does not handle the allocation error and<br> 
may result in segmentation fault

## Steps to reproduce 
- Eye compiler

## Suggested fix
- Add check for the return value of `malloc` function
    ```c++
    if (!palette) { // if palette is not allocated
      goto error;
    }
    ```