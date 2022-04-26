# BUG-9

## Category
- Arithmetic overflow

## Description
- When passing a `square_width` that does not divide the height,<br>
it results in a segmentation fault

## Affected Lines in the original program
- **checkerbox.c**:102 to **checkerbox.c**:127

## Expected vs Observed
- Expected behavior: notify user about bad input
- Observed behavior: segmentation fault

## Steps to reproduce 

1. Command 
    ```bash
    ./checkerboard check.png 1000 360 400 111111 ffffff
    ```

## Suggested fix
- Add additional check for invalid input
- Specifically add this after successfully initializing variables
    ```c++
    if (height % square_width != 0) {
        goto error;
    }
    ```