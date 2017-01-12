
#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {

// This is used to reduce duplicated and scattered error messages
const char *error_str_table[] = {
  "Cannot seek file\n",
  "Error acquiring file size\n",
  "Error opening file: %s\n",
  "Out of memory\n",
  "Error reading file. Size actually read: %lu\n",
  "The archive is too small. Size: %lu\n",
  "Did not find end of central directory record (invalid format?)\n",
  "Multi-part archive file not supported (# of disks: %u)\n",
  // This is general error message and carries a more specific reason string
  "Corrupted archive: %s\n", 
  "Iterator has passed the end of iteration\n",
  "Failed to initialize zlib\n",
  "Error decompressing data using zlib (errno: %d; reason: %s)\n",
  "Unknown compression method: %u\n",
  "Error executing chdir() on path: %s\n",
  "Error executing stat() on path: %s\n",
  "Error executing mkdir() on path: %s\n",
  "Error creating new file for write: %s\n",
  "Error writing to file: %s\n",
  "Error unlinking a regular file: %s\n",
};

/*
 * ReportError() - Reports error on stderr and then throw exception
 *
 * The exception thrown is always integer 0 and this function does not return
 */
void ReportError(const char *format, ...) {
  va_list args;
  
  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);
  
  throw 0;
  
  return;
}

}
}
