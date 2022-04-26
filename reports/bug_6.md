# BUG-1

## Category
- Iteration error

## Description
- Poor implementation of nested loops resulted in a program<br>
that does not follow the intented specification

## Affected Lines in the original program
- **rect.c**:61 to **rect.c**:83

## Expected vs Observed
- Expected behavior: the program draws a rectangle in the provided image
- Observed behavior: program sometimes fails to draw anything, draws diagonals,<br>
anything except the rectangle

## Steps to reproduce 

- **Note**: we consider that the fix from bug_5.md is applied

1. Command 
    ```bash
    ./solid out.png 100 100 123456
    ```
2. Command
    ```bash
    ./rect out.png out.png 0 0 20 20 111222
    ```

## Proof of concept
- Observed<br>
    ![Observed](rect_bad.png)
- Expected<br>
    ![Expected](rect_good.png)

## Suggested fix
- Delete all affected lines
- Use for loops that iterate over the exact rectangle region
- Code here
    ```c++
    for (unsigned i = top_left_y; i <= bottom_right_y; i++) {
      for (unsigned j = top_left_x; j <= bottom_right_x; j++) {
        image_data[i][j].red = (hex_color & 0xff0000) >> 16;
        image_data[i][j].green = (hex_color & 0x00ff00) >> 8;
        image_data[i][j].blue = (hex_color & 0x0000ff);
        image_data[i][j].alpha = 0xff;
      }
    }
    ```