# BUG-10

## Category
- Temporal memory safety violation

## Description
- When the **checkerbox.c** program fails to allocate heap memory for pixels array,<br>
It frees the `img`, then goes to error_img and frees `img` again

## Affected Lines in the original program
- **checkerboard.c**:91

## Expected vs Observed
- Expected behavior: free memory once
- Observed behavior: free memory twice

## Steps to reproduce 
- Command
    ```bash
    ./checkerboard check.png 65534 65534 14 111111 222222
    ```

## Suggested fix
- Delete `free(img);` at **checkerboard.c**:91