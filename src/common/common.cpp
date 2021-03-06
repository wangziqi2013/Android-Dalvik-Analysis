
#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {

// This is the static variable that saves stderr
int FileUtility::saved_stderr = -1;

// This is used to reduce duplicated and scattered error messages
const char *error_str_table[] = {
  // 0
  "Cannot seek file\n",
  "Error acquiring file size\n",
  "Error opening file: %s\n",
  "Out of memory\n",
  "Error reading file. Size actually read: %lu\n",
  // 5
  "The archive is too small. Size: %lu\n",
  "Did not find end of central directory record (invalid format?)\n",
  "Multi-part archive file not supported (# of disks: %u)\n",
  // This is general error message and carries a more specific reason string
  "Corrupted archive: %s\n", 
  "Iterator has passed the end of iteration\n",
  
  // 10
  "Failed to initialize zlib\n",
  "Error decompressing data using zlib (errno: %d; reason: %s)\n",
  "Unknown compression method: %u\n",
  "Error executing chdir() on path: %s\n",
  "Error executing stat() on path: %s\n",
  // 15
  "Error executing mkdir() on path: %s\n",
  "Error creating new file for write: %s\n",
  "Error writing to file: %s\n",
  "Error unlinking a regular file: %s\n",
  "Error reading current working directory (buffer size %d)\n",
  
  // 20
  "Error parsing XML: Unexpected start of a new XML document\n",
  "Error parsing resource chunk header: "
    "Unknown chunk type (0x%04X) @ offset 0x%lX\n",
  "Error flushing a file\n",
  "Index to string pool too large: %lu (total count %lu)\n",
  "Only ASCII is supported in PrintAscii() (UTF-%s)\n",
  
  // 25
  "Corrupt resource map: inconsistent header fields\n",
  "URI not found - no XML namespace available\n",
  "Unexpected length of struct ResourceValue: %u\n",
  "Unsupported resource value type: 0x%.4X (data: 0x%X)\n",
  "Illegal value for boolean type: %u\n",
  
  // 30
  "Unexpected start of resource table\n",
  "Unecpected UTF-8 continuiation byte at offset %ld", 
  "Base class function not implemented (internal error)\n",
  "Invalid type ID: 0x%08X\n",
  "Unknown resource type for writing the XML file\n",
  
  // 35
  "Error closing the file\n",
  "Invalid type attr entry: %s\n",
  "Error mapping file into memory: %s\n",
  "Error unmapping file from virtual memory\n",
  "Unknown value for type NULL_TYPE: %u\n",
  
  // 40
  "Unknown resource value of boolean type: 0x%X\n",
  "Package ID is zero - non-base package not supported\n",
  "Package ID not found in the current resource table; Package ID: 0x%02X\n",
  "Unknown unit for dimension: 0x%X\n",
  "Unknown unit for fraction: 0x%X\n",
  
  // 45
  "Invalid entry ID: 0x%08X\n",
  "TypeSpec structure contains no Type object (RES ID: 0x%08X)\n",
  "Package (ID = 0x%02X) has already been registered\n",
  "Invalid drawable entry (index: %lu)\n",
  "Default configuration type is not found (type ID: 0x%02X)\n",
  
  // 50
  "Entry not present in default type (type ID: 0x%02X; entry ID: 0x%04X)\n",
  "Invalid array entry (index: %lu)\n",
  "Wrong array index in index type: expected %lu, actual %lu\n",
  "Invalid boolean type entry (index: %lu)\n",
  "Attribute is not enum or flags - unknown type (index: %lu)\n",
  
  //55
  "Invalid id entry (index: %lu)\n",
  "Invalid style entry (index: %lu)\n",
  "Resource ID (0x%08X) is not availale\n",
  "Invalid string entry (index: %lu)\n",
  "Invalid dimension entry (index: %lu)\n",
  
  //60
  "Invalid color entry (index: %lu)\n",
  "Invalid integer entry (index: %lu)\n",
  "Invalid fraction entry (index: %lu)\n",
  "Invalid plurals entry (index: %lu)\n",
  "Name space (URI ID = %u) already exists!\n",
  
  // 65
  "Unexpected end of XML element node (stack empty)\n",
  "Element end tag and start tag does not match\n",
  "Unexpected end of a name space that does not exist\n",
  "There is at least one unclosed element after parsing\n",
  "There is at least one unclosed name space after parsing\n",
  
  // 70
  "Invaid path for writing XML file\n",
  "Multiple XML root node encountered\n",
  "Missing XML root node\n",
  "The header of DEX file is incorrect: %s\n",
  "Error duplicating a file descriptor: %d\n",
  
  // 75
  "Erorr obtaining the file handler\n",
  "There is no stderr file handler saved while restoring it\n",
  "Validation fails: Incorrect field data\n",
  "Validation fails: Incorrect method data\n",
  "Corrupt bytecode - unexpected end of code data section\n",
};

/*
 * ReportError() - Reports error on stderr and then throw exception
 *
 * The exception thrown is always integer 0 and this function does not return
 */
void ReportError(enum ErrorCode code, ...) {
  va_list args;
  
  va_start (args, code);
  vfprintf (stderr, error_str_table[code], args);
  va_end (args);
  
  // Under debug mode this allows us to inspect the stack to locate 
  // places this is called
  // Under release mode this is removed and a real exception will be thrown
  assert(false && "ReportError() throws an exception; "
                  "under debug mode use gdb to inspect the execution stack.");
  
  throw code;
  
  return;
}

}
}
