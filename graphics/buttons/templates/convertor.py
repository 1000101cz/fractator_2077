import sys

f_in = open(sys.argv[1], "r")
f_out = open(sys.argv[2], "w")

lines = f_in.readlines()


count = 0
f_out.write("uint8_t buttonX[] = {")
for line in lines:
    count += 1
    f_out.write(line.strip("\n"))
    f_out.write(", ")
    if (count % 50 == 0):
        f_out.write("\n")
f_out.write("};")
f_in.close()
f_out.close()
