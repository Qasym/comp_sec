# Bug 0

## Name

Temporal memory safety violation

## Description

*pngparser.c* program allocates memory for a chunk and frees it twice due to `free()` not<br>
giving any guarantees that the pointer is `NULL`

## Affected Lines
In *pngparser.c*:275 and *pngparser.c*:690

## Expected vs Observed
Expected behavior: The program successfully deallocates memory
Observed behavior: Program tries to deallocates memory twice in the same address

## Steps to Reproduce / Command
1. We need to have **alf-gcc** compiled binary *size*
2. We need to have files from afl-fuzz (the one necessary for this replication will be in the same directory with this file)
3. Then we need to take the following file among afl_out/crashes:<br>
*id:000014,sig:06,src:000056,op:havoc,rep:2*
4. Run the following command: (we assume that the files from steps 1 and 2 are in the same directory)<br>
`./size ./id:000014,sig:06,src:000056,op:havoc,rep:2`

## Suggested Fix Description
The program does a good job at cheching if pointers are null or not, <br>
we simply need to make the pointer `NULL` after we free it<br>
Substitute this at 275th line
```c++
if (chunk->chunk_data)
    free(chunk->chunk_data);
```
with
```c++
if (chunk->chunk_data) {
    free(chunk->chunk_data);
    chunk->chunk_data = NULL;
}
```