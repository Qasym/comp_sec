# BUG-2

## Category
- Command injection

## Description
- We can pass a filename that contains flags, arguments for `stat`.<br>
That will result in a command injection (for example we can get a filesystem info, instead of the output file)

## Affected Lines in the original program
- **solid.c**:123 to **solid.c**:125

## Expected vs Observed
- Expected behavior: simply output the size of the file created by the program.
- Observer behavior: execution of the unexpected commands available through `stat`.

## Steps to reproduce 

1. Command 
    ```bash
    ./solid "-f circle.c" 1 1 000000
    ```

## Suggested fix
- Use `stat()` function instead of executing the `stat -c %s` command