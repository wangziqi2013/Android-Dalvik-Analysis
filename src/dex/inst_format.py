
from lxml import html

HTML_FILE_NAME = "./html/inst_format.html"
CPP_FILE_NAME = "./inst_format.cpp"

DISCLAIMER = \
    """
//
// This file is auto-generated using Python script %s and
// HTML documentation %s, in order to automate coding process.
//
// Target file(s): %s
//
// Author: Ziqi Wang (wangziqi2013)
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

def write_cpp_file():
    fp = open(CPP_FILE_NAME, "w")

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
    desc = ""
    for tr in tr_list:
        td_list = tr.xpath("td")

        # This is the name we are using as the human readable name
        name = "i" + td_list[-3].text

        fp.write("    %s = %d, /* %s */\n" % (name, num, hex(num)))
        num += 1

    fp.write("};\n")

    fp.close()
    return;


if __name__ == "__main__":
    write_cpp_file()