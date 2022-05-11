# BUG-7

## Category
- Local persisting pointers

## Description
- In `struct pixel* get_pixel()` function in **filter.c**<br>
the original implementation returns address of a local variable<br>
which results in a core dump

## Affected Lines in the original program
- **filter.c**:107 to **filter.c**:110

## Expected vs Observed
- Expected behavior: the function returns pointer to an allocated memory address
- Observer behavior: the funciton returns pointer to a local variable address, <br>
which results in a core dump when accessed outside the scope of the function 

## Steps to reproduce 

1. Command 
    ```bash

    ```

## Proof of concept


## Suggested fix
- Suggested fix is to allocate a pixel in heap, and return pointer to it<br>
- As follows
    ```c++
    struct pixel *get_pixel() {
      struct pixel *px;
      px = malloc(sizeof(struct pixel));
      return px;
    }
    ```