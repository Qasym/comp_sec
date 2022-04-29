# BUG-16

## Category
- Itearation error

## Description
- In **filter.c** file, whenever we call the grayscale filter <br>
it iterates up to the edge including the edge (`<=` not `<`)

## Affected Lines in the original program
- **filter.c**:118
- **filter.c**:119
- **filter.c**:64
- **filter.c**:65

## Expected vs Observed
- Expected behavior: program iterates without any problems
- Observed behavior: program fails to iterate successfully (segmentation fault)

## Steps to reproduce 
- **Note:** Use the original **solid.c** file
1. Generate corrupted image with width and height zero
    ```bash
    ./solid corrupted.png 0 0 111111
    ```
2. Pass the image to the filter
    ```bash
    ./filter corrupted.png output.png negative
    ```

## Suggested fix
- A suggested fix is to substitute the `<=` sign by `<` in the affected lines