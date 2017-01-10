
s = \
"""b0: add-int/2addr
b1: sub-int/2addr
b2: mul-int/2addr
b3: div-int/2addr
b4: rem-int/2addr
b5: and-int/2addr
b6: or-int/2addr
b7: xor-int/2addr
b8: shl-int/2addr
b9: shr-int/2addr
ba: ushr-int/2addr
bb: add-long/2addr
bc: sub-long/2addr
bd: mul-long/2addr
be: div-long/2addr
bf: rem-long/2addr
c0: and-long/2addr
c1: or-long/2addr
c2: xor-long/2addr
c3: shl-long/2addr
c4: shr-long/2addr
c5: ushr-long/2addr
c6: add-float/2addr
c7: sub-float/2addr
c8: mul-float/2addr
c9: div-float/2addr
ca: rem-float/2addr
cb: add-double/2addr
cc: sub-double/2addr
cd: mul-double/2addr
ce: div-double/2addr
cf: rem-double/2addr"""

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
        j = j.replace("/", "")
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
