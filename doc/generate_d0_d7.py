
s = \
"""d0: add-int/lit16
d1: rsub-int
d2: mul-int/lit16
d3: div-int/lit16
d4: rem-int/lit16
d5: and-int/lit16
d6: or-int/lit16
d7: xor-int/lit16"""

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
    print ("    void On%s(unsigned char reg1, \n" + \
          "            %sunsigned char reg2, \n" +
          "            %sint value);") % \
          (name, len(name) * " ", len(name) * " ")

index = 0
for name in name_str_list:
    print ("void BytecodeSegment::On%s(unsigned char reg1,\n" + \
          "                      %s   unsigned char reg2,\n" + \
           "                      %s   int value)") % \
          (name, " " * len(name), " " * len(name))
    print "{"
    print "    this->PrintLineNum();"
    print "    fprintf(this->out_file,"
    print "            \"%s v%%u, v%%u, %%d\\n\"," % (mem_str_list[index], )
    print "            reg1, "
    print "            reg2, "
    print "            value); "
    print "}\n"
    index += 1

for i in value_str_list:
    print "            case 0x%s:" % (i, )

print " " * 16 + "byte2 = this->GetNextByte();"
print " " * 16 + "byte3 = byte2 >> 4;"
print " " * 16 + "byte2 &= 0x0F;"
print " " * 16 + "word1 = this->GetNextShort();"
print " " * 16 + "int1 = ((int)(((unsigned int)word1) << 16)) >> 16;"
print " " * 16 + "\nswitch(byte1)"
print " " * 16 + "{"

index = 0
for i in value_str_list:
    print " " * 20 + "case 0x%s:" % (i, )
    print " " * 24 + "this->On%s(byte2, byte3, int1);" % (name_str_list[index],)
    print " " * 24 + "break;"
    index += 1
print " " * 16 + "}"
