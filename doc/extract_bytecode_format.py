
fp = open("bytecode_format.txt", "r")

t = fp.read()

prev_index = 0
while True:
    next_index = t.find("<tr>\n  <td>", prev_index)
    prev_index = next_index + 1

    if next_index == -1:
        break

    close_index = t.find("</td>", next_index + 1)
    format_str = t[next_index + 11:close_index]

    inst, ty = format_str.split(" ")
    if inst.find("..") != -1:
        insts = inst.split("..")
        start = int(insts[0], 16)
        end = int(insts[1], 16)
        insts = range(start, end + 1)
    else:
        insts = [int(inst, 16)]
        
    for i in insts:
        print "[0x%.2x] = _%s," % (i, ty)
