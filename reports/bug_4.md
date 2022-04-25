# BUG-4

## Category
- Wrong operators

## Description
- Because some operators were not assigning but were comparing values,<br>
resulting program didn't give the desired output

## Affected Lines in the original program
- **circle.c**:61
- **circle.c**:84

## Expected vs Observed
- Expected behavior: fully drawn circle
- Observer behavior: partially drawn circle

## Steps to reproduce 

1. Command 
    ```bash
    ./solid input.png 100 100 123456
    ```
2. Command
    ```bash
    ./circle input.png output.png 10 10 10 111222
    ```

## Proof of concept
- Observed<br>
    ![Observed](circle_wrong_op.png)
- Expected<br>
    ![Expected](circle_right_op.png)

## Suggested fix
- Substitute the comparison operator `==` to the assigning operator `=`