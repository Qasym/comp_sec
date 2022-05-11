# BUG-15

## Category
- String vulnerability

## Description
- In **filter.c** program when we copy the `argv[4]` to `arg`, <br>
we don't specify the copy size, this is troublesome due to `arg` <br>
having limited buffer

## Affected Lines in the original program
- **filter.c**:225

## Expected vs Observed
- Expected behavior: we don't copy beyond `ARG_SIZE` macro
- Observerd behavior: we copy beyond `ARG_SIZE` macro in certain cases

## Steps to reproduce 

1. Generate example image
    ```bash
    ./solid example.png 100 100 111111
    ```
2. Trigger the overflow
    ```bash
    ./filter example.png output.png grayscale 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
    ```

## Suggested fix
- Use `strncpy` instead of `strcpy`, specify `ARG_SIZE` as a third operator<br>
in `strncpy`
- In other words, replace the affected line with the following fix
    ```c++
    strncpy(arg, argv[4], ARG_SIZE);
    ```