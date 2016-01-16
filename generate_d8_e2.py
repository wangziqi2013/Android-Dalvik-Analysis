
s = \
"""d8: add-int/lit8
d9: rsub-int/lit8
da: mul-int/lit8
db: div-int/lit8
dc: rem-int/lit8
dd: and-int/lit8
de: or-int/lit8
df: xor-int/lit8
e0: shl-int/lit8
e1: shr-int/lit8
e2: ushr-int/lit8"""

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
    print ("    void On%s(unsigned char dest, \n" + \
          "            %sunsigned char src, \n" +
          "            %sint value);") % \
          (name, len(name) * " ", len(name) * " ")

index = 0
for name in name_str_list:
    print ("void BytecodeSegment::On%s(unsigned char dest,\n" + \
          "                      %s   unsigned char src,\n" + \
           "                      %s   int value)") % \
          (name, " " * len(name), " " * len(name))
    print "{"
    print "    this->PrintLineNum();"
    print "    fprintf(this->out_file,"
    print "            \"%s v%%u, v%%u, %%d\\n\"," % (mem_str_list[index], )
    print "            dest, "
    print "            src, "
    print "            value); "
    print "}\n"
    index += 1

for i in value_str_list:
    print "            case 0x%s:" % (i, )

print " " * 16 + "byte2 = this->GetNextByte();"
print " " * 16 + "byte3 = this->GetNextByte();"
print " " * 16 + "byte4 = this->GetNextByte();"
print " " * 16 + "int1 = ((int)(((unsigned int)word1) << 24)) >> 24;"
print " " * 16 + "\nswitch(byte1)"
print " " * 16 + "{"

index = 0
for i in value_str_list:
    print " " * 20 + "case 0x%s:" % (i, )
    print " " * 24 + "this->On%s(byte2, byte3, int1);" % (name_str_list[index],)
    print " " * 24 + "break;"
    index += 1
print " " * 16 + "}"



