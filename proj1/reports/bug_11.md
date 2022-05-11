# BUG-11

## Category
- Arithmetic overflow

## Description
- The **resize.c** program does not check for the overflow when<br>
initializing the `new_height` and `new_width`

## Affected Lines in the original program
- **resize.c**:33
- **resize.c**:34

## Expected vs Observed
- Expected behavior: notify user of overflow condition
- Observed behavior: the program raises segmentation fault

## Steps to reproduce 

1. Command 
    ```bash
    ./solid img.png 100 100 111111
    ```
2. Command
    ```bash
    ./resize img.png resized.png 100000
    ```

## Suggested fix
- Add the following check before initializing `new_height` and `new_width`
    ```c++
    if (USHRT_MAX < (unsigned)(height * factor) ||
        USHRT_MAX < (unsigned)(width * factor)) {
      printf("Overflow detected!\n");
      goto error;
    }
    ```