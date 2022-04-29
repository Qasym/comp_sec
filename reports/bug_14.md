# BUG-14

## Category
- Wrong operators

## Description
- In the **resize.c** file when allocating the memory for the <br>
`new_img`, we have to multiply the number of pixels not add that number

## Affected Lines in the original program
- **resize.c**:48

## Expected vs Observed
- Expected behavior: proper memory allocation for the new image
- Observed behavior: improper memory allocation for the new image

## Steps to reproduce 

1. First generate image to test
    ```bash
    ./solid solid.png 100 100 111111
    ```
2. Run the following command to trigger the error
    ```bash
    ./resize solid.png resized.png 3
    ```

## Suggested fix
- Substitute the affected line by this
    ```c++
    new_img->px = malloc(n_pixels * sizeof(struct pixel));
    ```