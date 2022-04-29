# BUG-9

## Category
- Arithmetic overflow
- Heap overflow

## Description
- When passing a `square_width` that does not divide the height or width,<br>
it results in an access of the not-supposed-to access fields

## Affected Lines in the original program
- **checkerbox.c**:102 to **checkerbox.c**:127

## Expected vs Observed
- Expected behavior: notify user about bad input
- Observed behavior: segmentation fault

## Steps to reproduce 

- One way of doing it is through this command, in this example<br>
it tries to access pixels at the beginning of the next row<br> (results in a weird picture)
    ```bash
    ./checkerboard check.png 1000 460 400 111111 ffffff
    ```
- Another way of doing this is through this command, in this example<br>
it tries to access the next row, which is impossible for the<br>
last row, thus segmentation fault
    ```bash
    ./checkerboard check.png 1000 460 230 111111 ffffff
    ```

## Suggested fix
- Add additional check for invalid input
- Specifically add this after successfully initializing variables
    ```c++
    if (height % square_width != 0 || width % square_width != 0) {
        goto error;
    }
    ```