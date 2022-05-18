# Bug 1

## Name

Heap overflow

## Description

*pngparser.c* program does not check if accessing index is smaller than the size of the buffer.

## Affected Lines
From *pngparser.c*:480 to *pngparser.c*:483

## Expected vs Observed
Expected behavior: The program carefully accesses elements in buffer
Observed behavior: The program accesses elements outside buffer

## Steps to Reproduce / Command
1. We need to have **alf-gcc** compiled binary *size*
2. We need to have files from afl-fuzz (the one necessary for this replication will be in the same directory with this file)
3. Then we need to take the following file among afl_out/crashes:<br>
*id:000015,sig:06,src:000028,op:flip4,pos:18*
4. Run the following command: (we assume that the files from steps above are in the same directory)<br>
`./size ./id:000015,sig:06,src:000028,op:flip4,pos:18`

## Suggested Fix Description
We simply need to check if we have overflow before trying to access elements in buffer<br>
At *pngparser.c*:464, we need to substitute
```c++
if (inflated_buf[idy * (1 + 4 * width)]) { //! error overflow
    goto error;
}
```
with
```c++
if ((idy * (1 + 4 * width)) >= (sizeof(inflated_buf) / sizeof(uint32_t)) || inflated_buf[idy * (1 + 4 * width)]) { //! error overflow
    goto error;
}
```
And we need another check at *pngparser.c*:471
```c++
if (pixel_idx >= (sizeof(inflated_buf) / sizeof(uint32_t))) {
    goto error;
}
```


## Note
**All the affected lines are written with respect to the fixed pngparser.c, please keep that in mind!**