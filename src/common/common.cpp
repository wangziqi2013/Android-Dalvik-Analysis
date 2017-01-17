
#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {

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
  "Error parsing resource chunk header: Unknown chunk type (%u)\n",
  "Error flushing a file\n",
  "Index to string pool too large: %lu (total count %lu)\n",
  "Only ASCII is supported in PrintAscii() (UTF-%s)\n",
  "Corrupt resource map: inconsistent header fields\n",
  "URI not found - no XML namespace available\n",
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
