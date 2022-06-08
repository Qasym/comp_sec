INFO: Seed: 1109888979
INFO: Loaded 1 modules   (303 inline 8-bit counters): 303 [0x5b2330, 0x5b245f), 
INFO: Loaded 1 PC tables (303 PCs): 303 [0x573be0,0x574ed0), 
INFO:        2 files found in ../../seeds/
INFO: -max_len is not provided; libFuzzer will not generate inputs larger than 4096 bytes
AddressSanitizer:DEADLYSIGNAL
=================================================================
==60819==ERROR: AddressSanitizer: SEGV on unknown address (pc 0x0000004a6434 bp 0x000000000000 sp 0x7fffe90cf860 T0)
==60819==The signal is caused by a READ memory access.
==60819==Hint: this fault was caused by a dereference of a high value address (see register values below).  Dissassemble the provided pc to learn which register was used.
    #0 0x4a6434 in __asan::Allocator::Deallocate(void*, unsigned long, unsigned long, __sanitizer::BufferedStackTrace*, __asan::AllocType) (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x4a6434)
    #1 0x51f1b1 in free (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x51f1b1)
    #2 0x551b6c in load_png /home/kasym/coding/comp_sec/proj3/libfuzzer/src/pngparser.c:721:7
    #3 0x54f0d2 in LLVMFuzzerTestOneInput /home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png.c:15:7
    #4 0x4586f1 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x4586f1)
    #5 0x459b66 in fuzzer::Fuzzer::ReadAndExecuteSeedCorpora(std::__Fuzzer::vector<fuzzer::SizedFile, fuzzer::fuzzer_allocator<fuzzer::SizedFile> >&) (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x459b66)
    #6 0x459fd9 in fuzzer::Fuzzer::Loop(std::__Fuzzer::vector<fuzzer::SizedFile, fuzzer::fuzzer_allocator<fuzzer::SizedFile> >&) (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x459fd9)
    #7 0x449ce5 in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x449ce5)
    #8 0x471932 in main (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x471932)
    #9 0x7f82ee69e082 in __libc_start_main /build/glibc-SzIz7B/glibc-2.31/csu/../csu/libc-start.c:308:16
    #10 0x41e50d in _start (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x41e50d)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV (/home/kasym/coding/comp_sec/proj3/libfuzzer/src/fuzzer_load_png+0x4a6434) in __asan::Allocator::Deallocate(void*, unsigned long, unsigned long, __sanitizer::BufferedStackTrace*, __asan::AllocType)
==60819==ABORTING
MS: 0 ; base unit: 0000000000000000000000000000000000000000


artifact_prefix='./'; Test unit written to ./crash-da39a3ee5e6b4b0d3255bfef95601890afd80709
Base64: