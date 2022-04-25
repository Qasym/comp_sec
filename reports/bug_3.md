# BUG-3

## Category
- Heap overflow

## Description
- The identified bug tries to access memory beyond the end of the buffer.<br>
Resulting in a segmentation fault

## Affected Lines in the original program
- **circle.c**:29
- **circle.c**:30

## Expected vs Observed
- Expected behavior: program successfully takes the 7th argument
- Observer behavior: program actually takes the 8th argument and causes core dump

## Steps to reproduce

1. Command 
    ```bash
    ./circle input.png output.png 1 1 10 111111
    ```

## Suggested fix
- In the both affected lines, change `argv[7]` to `argv[6]`