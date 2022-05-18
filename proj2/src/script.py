import sys, os

allfiles = [f for f in os.listdir("./afl_out/crashes/") if os.path.isfile(os.path.join("./afl_out/crashes", f))]

for file_str in allfiles:
    if not ".txt" in file_str:
        os.system("echo +============+ executing ./size ./afl_out/crashes/" + file_str + " +============+")
        os.system("./size ./afl_out/crashes/" + file_str)