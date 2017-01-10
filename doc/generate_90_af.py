
s = \
"""90: add-int
91: sub-int
92: mul-int
93: div-int
94: rem-int
95: and-int
96: or-int
97: xor-int
98: shl-int
99: shr-int
9a: ushr-int
9b: add-long
9c: sub-long
9d: mul-long
9e: div-long
9f: rem-long
a0: and-long
a1: or-long
a2: xor-long
a3: shl-long
a4: shr-long
a5: ushr-long
a6: add-float
a7: sub-float
a8: mul-float
a9: div-float
aa: rem-float
ab: add-double
ac: sub-double
ad: mul-double
ae: div-double
af: rem-double"""

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
    print ("    void On%s(unsigned char dest, \n" + \
          "            %sunsigned char reg1, \n" +
          "            %sunsigned char reg2);") % \
          (name, len(name) * " ", len(name) * " ")

index = 0
for name in name_str_list:
    print ("void BytecodeSegment::On%s(unsigned char dest,\n" + \
          "                      %s   unsigned char reg1,\n" + \
           "                      %s   unsigned char reg2)") % \
          (name, " " * len(name), " " * len(name))
    print "{"
    print "    this->PrintLineNum();"
    print "    fprintf(this->out_file,"
    print "            \"%s v%%u, v%%u, v%%u\\n\"," % (mem_str_list[index], )
    print "            dest, "
    print "            reg1, "
    print "            reg2); "
    print "}\n"
    index += 1

for i in value_str_list:
    print "            case 0x%s:" % (i, )

print " " * 16 + "byte2 = this->GetNextByte();"
print " " * 16 + "byte3 = this->GetNextByte();"
print " " * 16 + "byte4 = this->GetNextByte();"
print " " * 16 + "\nswitch(byte1)"
print " " * 16 + "{"

index = 0
for i in value_str_list:
    print " " * 20 + "case 0x%s:" % (i, )
    print " " * 24 + "this->On%s(byte2, byte3, byte4);" % (name_str_list[index],)
    print " " * 24 + "break;"
    index += 1
print " " * 16 + "}"
