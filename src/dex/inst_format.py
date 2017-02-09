
#
# Auto C++ code generator for Dalvik bytecode parser
#

from lxml import html

HTML_FILE_NAME = "./html/inst_format.html"
CPP_FILE_NAME = "./inst_format.cpp"
HEADER_FILE_NAME = "./inst_format.h"

ARG_PACK_CLASS_NAME = "ArgumentPack"

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

    desc = desc.strip()

    return desc

def transform_desc_to_function_name(s):
    """
    Transforms a desc into valid C function name

    :param s: Input desc parsed from HTML
    :return: str
    """
    s = s.replace(' ', '_')
    s = s.replace('?', 'x')

    return s



def write_header_file():
    fp = open(HEADER_FILE_NAME, "w")

    write_disclaimer(fp, HEADER_FILE_NAME)

    fp.write("""
#pragma once

#ifndef _INST_FORMAT_H
#define _INST_FORMAT_H

#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {
namespace dex {
""")

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
class %s {
 public:
  uint64_t B;
  uint32_t A;
  uing16_t C;

  // This is the opcode
  uint8_t op;
  // This is to main the correct alignment
  uint8_t padding;

  uint8_t D;
  uint8_t E;
  uint8_t F;
  uint8_t G;
};
    """ % (ARG_PACK_CLASS_NAME, ))

    fp.write("""
} // namespace dex
} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
    """)

    fp.close()
    return name_desc_list

def generate_function_body(token_list, fp):
    """
    This function generates the function body for the specified format

    :param token_list:
    :param fp:
    :return:
    """
    print "old: " + str(token_list)
    # Reverse the byte prder first
    new_token_list = []
    total_length = 0
    buffer = []
    for token in token_list:
        length = len(token)
        buffer.append(token)

        total_length += length
        if total_length >= 4:
            buffer.reverse()
            new_token_list += buffer
            buffer = []
            total_length = 0

    token_list = new_token_list
    print "new: " + str(token_list)

    half_byte = False
    for token in token_list:
        length = len(token)
        dest = token[0]

        # Deal with special cases
        if token == "op":
            dest = token
        elif token == "??":
            fp.write("  data_p++;\n")
            continue

        if length == 1:
            if half_byte is False:
                fp.write("  arg_pack_p->%s = EncodingUtility::GetLow4Bit(data_p);\n" % (dest, ))
                half_byte = True
            else:
                fp.write("  arg_pack_p->%s = EncodingUtility::GetHigh4Bit(data_p);\n" % (dest, ))
                fp.write("  data_p++;\n")
                half_byte = False
        elif length == 2:
            assert(half_byte is False)
            fp.write("  arg_pack_p->%s = EncodingUtility::Get8Bit(data_p);\n" % (dest, ))
            fp.write("  data_p++;\n")
        elif length == 4:
            assert (half_byte is False)
            fp.write("  arg_pack_p->%s = EncodingUtility::Get16Bit(data_p);\n" % (dest, ))
            fp.write("  data_p += 2;\n")
        elif length == 8:
            assert (half_byte is False)
            fp.write("  arg_pack_p->%s = EncodingUtility::Get32Bit(data_p);\n" % (dest, ))
            fp.write("  data_p += 4;\n")
        elif length == 16:
            assert (half_byte is False)
            fp.write("  arg_pack_p->%s = EncodingUtility::Get64Bit(data_p);\n" % (dest, ))
            fp.write("  data_p += 8;\n")
        else:
            raise ValueError("Invalid length = %d" % (length, ))

    return

def write_cpp_file(name_desc_list):
    """
    Writes the CPP file that contains routines to generate the routine for
    parsing different instruction formats as well as the jumping table

    :param name_desc_list:
    :return:
    """
    fp = open(CPP_FILE_NAME, "w")

    write_disclaimer(fp, CPP_FILE_NAME)

    fp.write("\n")
    fp.write("#include \"common.h\"\n")
    fp.write("#include \"inst_format.h\"\n\n")
    fp.write("using namespace wangziqi2013;\n")
    fp.write("using namespace android_dalvik_analysis;\n")
    fp.write("using namespace dex;\n")

    desc_set = set()
    for i in name_desc_list:
        desc_set.add(i[1])

    for i in desc_set:
        # This is the only invalid one
        if i == "N/A":
            continue

        # This is the name of the function
        func_name = transform_desc_to_function_name(i)
        # This is the internal stucture we use to analyze the
        # argument structure
        token_list = i.split()

        fp.write("""
/*
 * %s() - Parses using format %s
 */
""" % (func_name, i))
        fp.write("inline uint8_t *%s(%s *arg_pack_p, uint8_t *data_p) {\n" %
                 (func_name, ARG_PACK_CLASS_NAME))

        # Here we generate function body
        generate_function_body(token_list, fp)

        fp.write("\n")
        fp.write("  return data_p;\n}\n")

    fp.close()

    return

if __name__ == "__main__":
    name_desc_list = write_header_file()
    write_cpp_file(name_desc_list)
