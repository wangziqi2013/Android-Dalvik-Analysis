
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
#include <unordered_map>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

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
  STRING_POOL_INDEX_TOO_LARGE,
  ONLY_SUPPORT_ASCII,
  CORRUPT_RESOURCE_MAP = 25,
  NAME_SPACE_URI_NOT_FOUND,
  UNEXPECTED_RESOURCE_VALUE_LENGTH,
  UNSUPPORTED_RESOURCE_VALUE_TYPE,
  ILLEGAL_BOOLEAN_VALUE,
  UNEXPECTED_START_OF_RESOURCE_TABLE = 30,
  INVALID_UTF8_CONTINUIATION_BYTE,
  BASE_CLASS_NOT_IMPLEMENTED,
  INVALID_TYPE_ID,
  UNKNOWN_TYPE_TO_WRITE_XML,
  
  ERROR_CLOSE_FILE = 35,
  INVALID_ATTR_ENTRY,
  ERROR_MAP_FILE,
  ERROR_UNMAP_FILE,
  UNKNOWN_VALUE_TYPE_NULL,
  
  UNKNOWN_BOOLEAN_VALUE = 40,
  ONLY_SUPPORT_BASE_PACKAGE,
  PACKAGE_ID_NOT_FOUND,
  UNKNOWN_DIMENSION_UNIT,
  UNKNOWN_FRACTION_UNIT,
  
  INVALID_ENTRY_ID = 45,
  NO_TYPE_IN_TYPE_SPEC,
  PACKAGE_ALREADY_REGISTERED,
  INVALID_DRAWABLE_ENTRY,
  DEFAULT_CONFIG_TYPE_NOT_FOUND,
  
  ENTRY_NOT_PRESENT_IN_DEFAULT_TYPE = 50,
  INVALID_ARRAY_ENTRY,
  WRONG_ARRAY_INDEX,
  INVALID_BOOL_ENTRY,
  ATTR_NOT_ENUM_FLAGS,
  
  INVALID_ID_ENTRY = 55,
  INVALID_STYLE_ENTRY,
  RESOURCE_ID_NOT_AVAILABLE, 
  INVALID_STRING_ENTRY,
};

// Error string table
extern const char *error_str_table[];

// Report error on stderr and then throw exception
void ReportError(enum ErrorCode code, ...);

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/*
 * class FileUtility - Common file system functionalities
 */
class FileUtility {
 public:
  
  /*
   * OpenFile() - Opens a file using the given mode
   *
   * This function is simply a wrapper with error reporting
   */
  inline static FILE *OpenFile(const char *file_name, const char *mode) {
    FILE *fp = fopen(file_name, mode);
    if(fp == nullptr) {
      ReportError(ERROR_OPEN_FILE, file_name);
    }
    
    return fp;
  }
  
  /*
   * CloseFile() - Closes an opened file and reports error if necessary
   */
  inline static void CloseFile(FILE *fp) {
    int ret = fclose(fp);
    if(ret != 0) {
      ReportError(ERROR_CLOSE_FILE); 
    }
    
    return;
  }
  
  /*
   * WriteString() - Writes the content of the string to a file
   *
   * Thie function does not append new line after the string; This function
   * also has the optional argument of setting the identation of the line
   * which will be printed as space characters before the actual content of the
   * string
   */
  static void WriteString(FILE *fp, 
                          const char *s, 
                          int ident=0, 
                          int ident_length=4) {    
    int ret;
    if(ident != 0) {
      // This prints space characters of a given length
      ret = fprintf(fp, "%*s", ident * ident_length, "");
      if(ret < 0) {
        ReportError(ERROR_WRITE_FILE, "[Unknown]"); 
      }
    }
                                 
    // Use formatted print to save a few function calls
    ret = fprintf(fp, "%s", s);
    if(ret < 0) {
      ReportError(ERROR_WRITE_FILE, "[Unknown]"); 
    }
    
    return; 
  }
  
  /*
   * LoadFile() - Loads a file into memory
   *
   * This function optionally returns the file length through the second 
   * parameter. Howeevr if it is set to nullptr then we do not change it
   */
  static unsigned char *LoadFile(const char *file_name, 
                                 size_t *file_length_p=nullptr) {
    FILE *fp = OpenFile(file_name, "rb");
    
    // Get file length and then assign it back to the argument 
    // if it is not nullptr
    size_t file_length = GetFileLength(fp);
    if(file_length_p != nullptr) {
      *file_length_p = file_length; 
    }
    
    unsigned char *data_p = new unsigned char[file_length];
    if(data_p == nullptr) {
      ReportError(OUT_OF_MEMORY); 
    }
    
    size_t size_read = fread(data_p, 1, file_length, fp);
    if(size_read != file_length) {
      ReportError(ERROR_READ_FILE, size_read);
    }
    
    CloseFile(fp);
    
    return data_p;
  }
  
  /*
   * MapFileReadOnly() - Maps a file into memory using mmap()
   *
   * This function does not require a full load of the file content; instead
   * the file is mapped into vitual address and loaded on demand
   */
  static unsigned char *MapFileReadOnly(const char *file_name, 
                                        size_t *file_length_p=nullptr) {
    // Use stat() to retrieve file length
    size_t length = GetFileLength(file_name);
    if(file_length_p != nullptr) {
      *file_length_p = length; 
    }
  
    int fd = open(file_name, O_RDONLY, 0);
    if(fd == -1) {
      ReportError(ERROR_OPEN_FILE, file_name); 
    }
    
    // Map the file into private memory area
    void *ptr = mmap(NULL, 
                     length, 
                     PROT_READ, 
                     MAP_PRIVATE, 
                     fd, 
                     0);
    if(ptr == MAP_FAILED) {
      ReportError(ERROR_MAP_FILE, file_name); 
    }
    
    // It is safe to close the file after mapping because mmap() adds
    // an extra reference to the file
    int ret = close(fd);
    if(ret == -1) {
      ReportError(ERROR_CLOSE_FILE); 
    }
    
    return reinterpret_cast<unsigned char *>(ptr);
  }
  
  /*
   * UnmapFile() - Unmaps a mapped file
   *
   * This function requires the length of the file as an extra parameter since
   * munmap() requires the length to free the virtual address
   */
  static void UnmapFile(void *ptr, size_t length) {
    int ret = munmap(ptr, length);
    if(ret == -1) {
      ReportError(ERROR_UNMAP_FILE); 
    }
    
    return;
  }
  
  /*
   * GetFileLength() - Returns the length of the file
   *
   * This function does not open the file. Instead it uses stat() system
   * call to to retrieve basic information of the file
   */
  static size_t GetFileLength(const char *file_name) {
    struct stat st;
    
    // Get the file size and return
    stat(file_name, &st);
    return static_cast<size_t>(st.st_size);
  }
   
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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

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
  static T *Advance(T *ptr, size_t offset) {
    return reinterpret_cast<T *>((reinterpret_cast<uint8_t *>(ptr) + offset));
  }
  
  /*
   * Retract() - Moves a pointer backward by some bytes
   */
  template<typename T>
  static T *Retract(T *ptr, size_t offset) {
    return reinterpret_cast<T *>((reinterpret_cast<uint8_t *>(ptr) - offset));
  }
  
  /*
   * GetPtrDiff() - Returns the difference between two pointers
   *
   * ptr1 must be less than ptr2
   */
  static size_t GetPtrDiff(const void *ptr1, const void *ptr2) {
    assert(ptr1 <= ptr2);
    
    return static_cast<size_t>(static_cast<const unsigned char *>(ptr2) - \
                               static_cast<const unsigned char *>(ptr1));
  }
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

}
}

#include "buffer.h"
#include "utf.h"
 
#endif
