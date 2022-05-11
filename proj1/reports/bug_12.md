# BUG-12

## Category
- Stack buffer overflow 

## Description
- If we provide a name longer than 500 characters, we create a stack buffer<br>
overflow in **solid.c** program

## Affected Lines in the original program
- **solid.c**:33

## Expected vs Observed
- Expected behavior: program works/halts without causing any fault
- Observed behavior: program terminates due to buffer overflow

## Steps to reproduce 
- Simply providing an argument with length more than 500<br>
will result in a stack overflow
- You can run this command
    ```bash
    ./solid 11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111.png 100 100 111111
    ```

## Suggested fix
- Substitute the affected `strcpy` with `strncpy`
    ```c++
    strncpy(output_name, argv[1], OUTPUT_NAME_SIZE);
    ```