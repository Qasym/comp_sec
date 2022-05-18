# Bug 1

## Name

Arithmetic overflow

## Description

*pngparser.c* program assigns uint32 value to uint16 variable, which results in an arithmetic overflow

## Affected Lines
From *pngparser.c*:438 to *pngparser.c*:439

## Expected vs Observed
Expected behavior: The program checks for the overflow
Observed behavior: The program allows overflow to happen

## Steps to Reproduce / Command
1. We need to have **alf-gcc** compiled binary *size*
2. We need to have files from afl-fuzz (the one necessary for this replication will be in the same directory with this file)
3. Then we need to take the following file inside 01/ directory:<br>
*id:000026,sig:06,src:000001,op:flip1,pos:16*
4. Run the following command: (we assume that the files from steps above are in the same directory)<br>
```bash
./size ./id:000026,sig:06,src:000001,op:flip1,pos:16
```

## Suggested Fix Description
We need to check if the assigned value is equal to the assigning value<br>
A simple if check is enough for this fix, put this right after assigning to `img->size_x`<br>
`img->size_y`
```c++
if (img->size_x != width || img->size_y != height) {
    goto error;
}
```