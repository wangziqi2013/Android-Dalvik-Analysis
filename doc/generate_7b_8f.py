
s = \
"""7b: neg-int
7c: not-int
7d: neg-long
7e: not-long
7f: neg-float
80: neg-double
81: int-to-long
82: int-to-float
83: int-to-double
84: long-to-int
85: long-to-float
86: long-to-double
87: float-to-int
88: float-to-long
89: float-to-double
8a: double-to-int
8b: double-to-long
8c: double-to-float
8d: int-to-byte
8e: int-to-char
8f: int-to-short"""

l = s.splitlines()
name_str_list = []
mem_str_list = []
value_str_list = []
for i in l:
    ii = i.split(": ")
    name = ii[1]
    value_str_list.append(ii[0].upper())
    mem_str_list.append(name)
    name_list = name.split("-")
    name_str = ""
    for j in name_list:
        name_str += (j[0].upper() + j[1:])

    name_str_list.append(name_str)

print name_str_list

for name in name_str_list:
    print "    void On%s(unsigned char reg1, unsigned char reg2);" % name

index = 0
for name in name_str_list:
    print ("void BytecodeSegment::On%s(unsigned char reg1,\n" + \
          "                      %s   unsigned char reg2)") % \
          (name, " " * len(name))
    print "{"
    print "    this->PrintLineNum();"
    print "    fprintf(this->out_file,"
    print "            \"%s v%%u, v%%u\\n\"," % (mem_str_list[index], )
    print "            reg1, "
    print "            reg2); "
    print "}\n"
    index += 1

for i in value_str_list:
    print "            case 0x%s:" % (i, )

print " " * 16 + "byte2 = this->GetNextByte();"
print " " * 16 + "byte3 = byte2 >> 4;"
print " " * 16 + "byte2 &= 0x0F;"
print " " * 16 + "switch(byte1)"
print " " * 16 + "{"

index = 0
for i in value_str_list:
    print " " * 20 + "case 0x%s:" % (i, )
    print " " * 24 + "this->On%s(byte2, byte3);" % (name_str_list[index],)
    print " " * 24 + "break;"
    index += 1
