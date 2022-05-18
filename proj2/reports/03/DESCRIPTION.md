# Bug 1

## Name

Wrong operator

## Description

*size.c* program incorrectly interpretes the exit code of `load_png` function

## Affected Lines
*size.c*:16

## Expected vs Observed
Expected behavior: The program enters the if statement when `load_png` fails
Observed behavior: The program enters the if statement when `load_png` succeeds

## Steps to Reproduce / Command
1. We need to have **alf-gcc** compiled binary *size*
2. We need to have files from afl-fuzz (the one necessary for this replication will be in the same directory with this file)
3. Then we need to take the following file inside 03/ directory:<br>
*id:000001,sig:06,src:000000,op:flip1,pos:8*
4. Run the following command: (we assume that the files from steps above are in the same directory)<br>
```bash
./size ./id:000001,sig:06,src:000000,op:flip1,pos:8
```

## Suggested Fix Description
Simply remove `!` from the affected line
```c++
if (load_png(input_name, &img)) {
    return 1;
}
```