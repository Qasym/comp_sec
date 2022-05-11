# BUG-5

## Category
- Type error

## Description
- `strtol()` function expects to receive a pointer to pointer,<br>
but in our original program we only pass a pointer

## Affected Lines in the original program
- **rect.c**:34

## Expected vs Observed
- Expected behavior: we successfully parse the hex value
- Observed behavior: segmentation fault(core dump)

## Steps to reproduce 

1. Command 
    ```bash
    ./solid solid.png 100 100 123456
    ```
2. Command
    ```bash
    ./rect solid.png rect_out.png 0 0 10 10 111222
    ```

## Suggested fix
- In the affected line, put the ampersand before `end_ptr` <br>
(or pass the address of `end_ptr`, not the pointer itself)