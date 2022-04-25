# BUG-1

## Category
- Unchecked system call returning code

## Description
- The program does not check for the zero width or zero height, thus providing <br>
such arguments will result in a cursed *png* file

## Affected Lines in the original program
- solid.c:48
- solid.c:53

## Expected vs Observed
- Expected behavior: We expect our program to create a filled rectangle of provided size.
- Observerd behavior: With 0 width or 0 height, we can't have a rectangle, and those arguments result<br>
in a broken *png* file

## Steps to reproduce 

1. Command 
    ```bash
    ./solid my_png.png 0 0
    ```
2. 

## Suggested fix
- solid.c:48
    ```c++
    if (height == 0 || height >= USHRT_MAX || *end_ptr)
    ```
- solid.c:53
    ```c++
    if (width == 0 || width >= USHRT_MAX || *end_ptr)
    ```