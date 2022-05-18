1. Why did you need to change `is_png_chunk_valid`?
    Because it checks if the chunk valid or not before running the fuzzer,<br>
    thus some of the fuzzer inputs are not going to pass which reduces the test space
2. Why did we give you exactly TWO seeds for fuzzing?
    That is because we have two functions that interprete images
    `convert_color_palette_to_image` and `convert_rgb_alpha_to_image`, adding less than two seeds<br>
    means that we will not cover every case<br>
3. Why did you have to use `afl-gcc` to compile the source (and not e.g. ordinary gcc)?
    afl-gcc is a wrapper around gcc and for that reason is optimized better<br>
    for AFL fuzzing operations
4. How many crashes in total did AFL produce? How many unique crashes?
    There were 30 unique crashes, I don't remember how many crashes in total I had<br>
    Possibly around hundred of thousands
5. Why are hangs counted as bugs in AFL? Which type of attack can they be used for?
    Hangs happen when the program stays at one point without giving any useful result<br>
    which may result in a timeout. It can be used for denial of service attacks, in which<br>
    attacker simply sends input that invokes hang over and over again
6. Which interface of `libpngparser` remains untested by AFL (take a look at `pngparser.h`)?
    `store_png` function
7. How long did you run AFL for? If you run it for twice as long, do you expect to find twice as many bugs? Why?
    I run AFL multiple times using many source files (such as I fixed bug and fuzzed again), for <br>
    that reason I can't say exactly how much my AFL worked, but it was more than 30 minutes.<br>    
    I don't expect AFL to find more bugs, since the time since last unique bug has been found <br>
    increases each time for each unique bug.