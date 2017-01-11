
#include "common.h"

// This is used to reduce duplicated and scattered error messages
const char *error_str_table[] = {
  "Cannot seek file\n",
  "Error acquiring file size\n",
  "Error opening file: %s\n",
  "Out of memory\n",
  "Error reading file. Size actually read: %lu\n",
  "The archive is too small. Size: %lu\n",
};
