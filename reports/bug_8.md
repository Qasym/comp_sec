# BUG-8

## Category
- Arithmetic overflow

## Description
- In the **checkerbox.c** file, we don't check the square width <br>
to be less than the height of the total picture

## Affected Lines in the original program
- **checkerbox.c**:102 to **checkerbox.c**:103
- Of course the rest of the loop will also be affected

## Expected vs Observed
- Expected behavior: notify user when bad input is given
- Observed behavior: continues to use bad input which results in segmentation fault

## Steps to reproduce 

1. Command 
    ```bash
    ./checkerboard checker_board.png 100 100 1000 112233 332211
    ```
## Suggested fix
- Add additional if statement to check for bad input
- Specifically, add this check after successfully initializing all the variables
    ```c++
    if (square_width > height) {
        goto error;
    }
    ```