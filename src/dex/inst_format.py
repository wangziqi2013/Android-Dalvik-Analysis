
#
# Auto C++ code generator for Dalvik bytecode parser
#

from lxml import html

HTML_FILE_NAME = "./html/inst_format.html"
CPP_FILE_NAME = "./inst_format.cpp"
HEADER_FILE_NAME = "./inst_format.h"

DISCLAIMER = \
    """
//
// This file is auto-generated using Python script %s and
// HTML documentation %s, in order to automate coding process.
//
// Target file: %s
//
    """

def write_disclaimer(fp, target_name):
    """
    Write the machine generated disclaimer

    :param fp: An opened file object
    :return: None
    """
    fp.write(DISCLAIMER % (__file__, HTML_FILE_NAME, target_name))

    return

def transform_format_string(desc):
    """
    This function does necessary transformation to the format string
    extracted from the HTML file and then make it suitable for parsing

    :param desc:
    :return:
    """
    desc2 = ""
    for c in desc:
        if ord(c) == 0xD8:
            desc2 += '?'
        else:
            desc2 += c

    desc = desc2
    desc = desc.replace("lo", "")
    desc = desc.replace("hi", "")
    desc = desc.replace("|", " ")

    token_list = desc.split()
    desc = ""
    prev = ""
    for i in token_list:
        if i == prev:
            desc += i
        else:
            prev = i
            desc += (" " + i)

    return desc

def write_header_file():
    fp = open(HEADER_FILE_NAME, "w")

    write_disclaimer(fp, CPP_FILE_NAME)

    fp.write("\n")
    fp.write("#include \"common.h\"\n\n")
    fp.write("using namespace wangziqi2013;\n")
    fp.write("using namespace android_dalvik_analysis;\n")
    fp.write("using namespace dex;\n")

    fp.write(
        """
/*
 * enum class InstFormat - Human readable form of instruction format
 */
"""
    )

    fp.write("enum class InstFormat {\n")

    fp2 = open(HTML_FILE_NAME)
    s = fp2.read()
    fp2.close()

    tree = html.fromstring(s)
    tr_list = tree.xpath("//table[@class='format']/tbody/tr")

    num = 0
    name_desc_list = []
    for tr in tr_list:
        td_list = tr.xpath("td")

        if len(td_list) == 4:
            desc = transform_format_string(td_list[0].text_content())

            print desc

            # Uncomment this to print the hex representation
            #print ":".join("{:02x}".format(ord(c)) for c in desc)

        # This is the name we are using as the human readable name
        name = "i" + td_list[-3].text

        fp.write("  %s = %d, /* %s */\n" % (name, num, desc))

        # Associate the name and desc and its index in the table
        # and store it into a list and return
        name_desc_list.append((name, desc, num))

        num += 1

    fp.write("};\n")

    fp.write("""
/*
 * class ArgumentPack - This is used to parse the byte code arguments
 *                      according to different bytecode types
 *
 * All arguments remain in the same format as they were in the compact form
 * of the byte code. In the case where the member is larger than the actual
 * number, only zero-extension is performed
 */
class ArgumentPack {
 public:
  uint64_t B;
  uint32_t A;
  uing16_t C;

  // This is to main the correct alignment
  uint16_t padding;

  uint8_t D;
  uint8_t E;
  uint8_t F;
  uint8_t G;
};
    """)

    fp.close()
    return name_desc_list


def write_cpp_file(name_desc_list):
    """
    Writes the CPP file that contains routines to generate the routine for
    parsing different instruction formats as well as the jumping table

    :param name_desc_list:
    :return:
    """

if __name__ == "__main__":
    name_desc_list = write_header_file()
    write_cpp_file(name_desc_list)
