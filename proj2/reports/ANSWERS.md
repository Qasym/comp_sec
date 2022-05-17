1. Why did you need to change `is_png_chunk_valid`?
    Because it checks if the chunk valid or not before running the 
2. Why did we give you exactly TWO seeds for fuzzing?
3. Why did you have to use `afl-gcc` to compile the source (and not e.g. ordinary gcc)?
4. How many crashes in total did AFL produce? How many unique crashes?
5. Why are hangs counted as bugs in AFL? Which type of attack can they be used for?
6. Which interface of `libpngparser` remains untested by AFL (take a look at `pngparser.h`)?
7. How long did you run AFL for? If you run it for twice as long, do you expect to find twice as many bugs? Why?