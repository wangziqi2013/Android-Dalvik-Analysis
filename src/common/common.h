
#pragma once

#ifndef _COMMON_H
#define _COMMON_H

#include <cassert>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <cstdint> 
#include <cstring>
#include <cstdarg>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEBUG_PRINT

#ifdef DEBUG_PRINT

#define dbg_printf(fmt, ...)                              \
  do {                                                    \
    fprintf(stderr, "%-24s: " fmt, __FUNCTION__, ##__VA_ARGS__); \
    fflush(stdout);                                       \
  } while (0);

#else

static void dummy(const char*, ...) {}

#define dbg_printf(fmt, ...)   \
  do {                         \
    dummy(fmt, ##__VA_ARGS__); \
  } while (0);

#endif

// I copied this from Linux kernel code to favor branch prediction unit on CPU
// if there is one
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

// This is used to cancel alignment for structures
#define BYTE_ALIGNED __attribute__((packed, aligned(1)))

#define ERROR_SEEK_FILE (error_str_table[0])
#define ERROR_ACQUIRE_FILE_SIZE (error_str_table[1])
#define ERROR_OPEN_FILE (error_str_table[2])
#define OUT_OF_MEMORY (error_str_table[3])
#define ERROR_READ_FILE (error_str_table[4])
#define FILE_TOO_SMALL (error_str_table[5])
#define NO_EOF_CENTRAL_DIR (error_str_table[6])
#define MULTI_PART_NOT_SUPPORTED (error_str_table[7])
#define CORRUPTED_ARCHIVE (error_str_table[8])
#define END_OF_ITERATION (error_str_table[9])
#define ERROR_INIT_ZLIB (error_str_table[10])
#define ERROR_INFLATE (error_str_table[11])
#define UNKNOWN_COMPRESSION_METHOD (error_str_table[12])
#define ERROR_CHDIR (error_str_table[13])
#define ERROR_STAT (error_str_table[14])
#define ERROR_MKDIR (error_str_table[15])
#define ERROR_CREATE_FILE (error_str_table[16])
#define ERROR_WRITE_FILE (error_str_table[17])

namespace wangziqi2013 {
namespace android_dalvik_analysis {
  
// Error string table
extern const char *error_str_table[];

// Report error on stderr and then throw exception
void ReportError(const char *format, ...);

/*
 * class FileUtility - Common file system functionalities
 */
class FileUtility {
 public:
   
  /*
   * GetFileLength() - Returns the size of an opened file
   *
   * This function does not stat(), and instead it moves the file pointer to
   * end end and then reports current offset
   */
  static size_t GetFileLength(FILE *fp) {
    assert(fp != nullptr);
    
    int seek_ret;
    
    seek_ret = fseek(fp, 0, SEEK_END);
    if(seek_ret != 0) {
      ReportError(ERROR_SEEK_FILE); 
    }
    
    long file_size = ftell(fp);
    if(file_size == -1L) {
      ReportError(ERROR_ACQUIRE_FILE_SIZE); 
    }
    
    // Do not forget to set it back to the beginning of file
    seek_ret = fseek(fp, 0, SEEK_SET);
    if(seek_ret != 0) {
      ReportError(ERROR_SEEK_FILE); 
    }
    
    return static_cast<size_t>(file_size);
  }
  
  /*
   * EnterDir() - Changes current working directory to the specified dir
   */
  inline static void EnterDir(const char *dir) {
    int ret = chdir(dir);
    if(ret == -1) {
      ReportError(ERROR_CHDIR, dir); 
    }
    
    return;
  }
  
  /*
   * CreateOrEnterDir() - Creates a new or enters an existing dir
   *
   * If a regular file with the same name is there then we just unlink the file
   * and then create new dir
   *
   * This function always work under cwd. After new entry is created it enters
   * the new dir
   */
  static void CreateOrEnterDir(const char *dir) {
    
  }
};

}
}
 
#endif
