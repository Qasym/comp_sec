#### Name
Temporal memory safety violation

#### Description

An attempt to free memory location that hadn't been previously allocated

#### Affected Lines
**original_pngparser.c**:701

#### Expected vs Observed
Expected behavior: If statements correctly prevent double-free from happening
Observed behavior: If statement does not prevent double free since condition is not set properly

#### Steps to Reproduce

- **original_pngparser.c** must be used to reproduce this bug,<br>
since **pngparser.c** contains fixes
- We also obtain **fuzzer_load_png** binary from the **original_pngparser.c**

##### Command

```
./fuzzer_load_png poc_00.bin
```

#### Suggested Fix Description
When we allocate memory for chunk, we have to set chunk_data to NULL, so that error that happens before<br>
memory allocation for data_chunk does not trigger its deallocation