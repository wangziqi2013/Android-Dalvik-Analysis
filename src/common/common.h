
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

namespace wangziqi2013 {
namespace android_dalvik_analysis {
  
enum ErrorCode : uint64_t {
  ERROR_SEEK_FILE = 0, 
  ERROR_ACQUIRE_FILE_SIZE,
  ERROR_OPEN_FILE,
  OUT_OF_MEMORY,
  ERROR_READ_FILE,
  FILE_TOO_SMALL = 5,
  NO_EOF_CENTRAL_DIR,
  MULTI_PART_NOT_SUPPORTED,
  CORRUPTED_ARCHIVE,
  END_OF_ITERATION,
  ERROR_INIT_ZLIB = 10,
  ERROR_INFLATE,
  UNKNOWN_COMPRESSION_METHOD,
  ERROR_CHDIR,
  ERROR_STAT,
  ERROR_MKDIR = 15,
  ERROR_CREATE_FILE,
  ERROR_WRITE_FILE,
  ERROR_UNLINK,
  ERROR_GETCWD,
  
  // The following are related to parsing binary XML
  UNEXPECTED_START_OF_XML = 20,
  UNKNOWN_CHUNK_TYPE,
  ERROR_FLUSH_FILE,
};

// Error string table
extern const char *error_str_table[];

// Report error on stderr and then throw exception
void ReportError(enum ErrorCode code, ...);

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
   * CreateDir() - Creates a new dir under cwd
   *
   * If there is already a regular file with the same name dir creation would
   * fail. Caller should guarantee the regular file is removed
   */
  inline static void CreateDir(const char *dir) {
    int ret = mkdir(dir, S_IRUSR | S_IWUSR | S_IXUSR);
    if(ret == -1) {
      ReportError(ERROR_MKDIR, dir); 
    }
    
    return;
  }
  
  /*
   * DeleteFile() - Deletes a file 
   */
  inline static void DeleteFile(const char *file_name) {
    int ret = unlink(file_name);
    if(ret == -1) {
      ReportError(ERROR_UNLINK, file_name); 
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
    struct stat buf;
    int ret = stat(dir, &buf);
    
    // If this happens either we make the dir or error and exit
    if(ret == -1) {
      // The entry does not exist, so just create one
      if(errno == ENOENT) {
        CreateDir(dir);
        EnterDir(dir);
      } else {
        ReportError(ERROR_STAT, dir); 
      }
    } else if(S_ISDIR(buf.st_mode)) {
      EnterDir(dir);
    } else {
      dbg_printf("Deleting regular file %s to create directory\n", dir);
      
      DeleteFile(dir);
      CreateDir(dir);
      EnterDir(dir);
    }
    
    return;
  }
  
  /*
   * GetCwd() - Returns the current working directory in a heap allocated
   *            chunk of memory
   *
   * The caller is responsible for freeing the memory
   */
  static char *GetCwd() {
    static constexpr int DEFAULT_CWD_SIZE = 1024;
    
    // We double this everytime it is not large enough 
    int current_size = DEFAULT_CWD_SIZE;
    
    do {
      char *p = new char[current_size];
      if(p == nullptr) {
        ReportError(OUT_OF_MEMORY); 
      }
      
      // Try using current size first
      // Note that here the interface changes slightly and there is an option
      // for it to allocate from heap by the system, but this way we have really
      // messy code with operator new/malloc() mixed together
      char *ret = getcwd(p, current_size);
      if(ret == nullptr) {
        // If the array is too small just double the size
        // and try again
        if(errno == ERANGE) {
          delete[] p;
          current_size *= 2;
          
          continue;
        } else {
          ReportError(ERROR_GETCWD, current_size); 
        }
      } else {
        return p; 
      }
    } while(1);
    
    assert(false);
    return nullptr;
  }
};

/*
 * class TypeCast - Helper class to cast types without writing too much code
 *
 * This class is just a syntactic sugar in a sense that it does not generate 
 * any real code. It only interacts with the type system during compilation
 */
class TypeUtility {
 public: 
 
  /*
   * Advance() - Moves a pointer either forward by some bytes
   */
  template<typename T>
  T *Advance(T *ptr, size_t offset) {
    return reinterpret_cast<T *>((reinterpret_cast<uint8_t *>(ptr) + offset));
  }
  
  /*
   * Retract() - Moves a pointer backward by some bytes
   */
  template<typename T>
  T *Retract(T *ptr, size_t offset) {
    return reinterpret_cast<T *>((reinterpret_cast<uint8_t *>(ptr) - offset));
  }
};

/*
 * class Buffer - In memory buffer for holding data for buffering them to be
 *                written into file 
 */
class Buffer {
 private: 
  unsigned char *data_p;
  size_t length;
  
  size_t current_length;
  
  // Use 64K as default buffer size if none is given
  static constexpr size_t DEFAULT_SIZE = 0x1 << 16;
  
  /*
   * Expand() - Adjust the size of the buffer for holding more data
   *
   * Do not use this to shrink the buffer
   */
  void Expand(size_t resize_length) {
    assert(resize_length > length);
    
    unsigned char *temp = new unsigned char[resize_length];
    if(temp == nullptr) {
      ReportError(OUT_OF_MEMORY);
    }
    
    memcpy(temp, data_p, current_length);
    delete[] data_p;
    
    data_p = temp;
    length = resize_length;
    
    return;
  }
  
 public: 
  /*
   * Constructor
   */
  Buffer(size_t p_length) :
    data_p{new unsigned char[p_length]},
    length{p_length},
    current_length{0UL}
  {}
  
  /*
   * Constructor - Use default size
   */
  Buffer() :
    Buffer{DEFAULT_SIZE}
  {}
  
  /*
   * Destructor
   */
  ~Buffer() {
    assert(data_p != nullptr);
    
    delete[] data_p;
    
    return; 
  }
  
  /*
   * Append() - Append bytes after the current location
   *
   * This function might trigger Oversize() operation if requested size
   * is too large
   */
  void Append(void *p, size_t request_length) {
    size_t resize_length = length;
    while(current_length + request_length > resize_length) {
      resize_length <<= 1;
    }
    
    // If there is a resize then just do it here
    if(resize_length != length) {
      Expand(resize_length); 
    }
    
    // New buffer must be large enough
    assert(current_length + request_length <= length);
    
    // Copy data
    memcpy(data_p + current_length, p, request_length);
    
    // Should not change this before the potential resize() because 
    // then resize might copy more data then expected
    current_length += request_length;
    
    return;
  }
  
  /*
   * WriteToFile() - Flushes all contents to a file and clear buffer
   *
   * This function will flush the buffer, so it is relatively slow
   */
  void WriteToFile(FILE *fp) {
    assert(fp != nullptr);
    
    size_t ret = fwrite(data_p, 1, current_length, fp);
    if(ret != current_length) {
      ReportError(ERROR_WRITE_FILE); 
    }
    
    // Make sure we could see the content immediately
    ret = fflush(fp);
    if(ret != 0) {
      ReportError(ERROR_FLUSH_FILE, "(unknown)"); 
    }
    
    // Reset the pointer to cleared buffer
    current_length = 0;
    
    return;
  }
};

}
}
 
#endif
