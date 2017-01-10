
#pragma once

#ifndef _APK_H
#define _APK_H

namespace wangziqi2013 {
namespace android_dalvik_analysis {

#include <cstdio>

/*
 * class ApkArchive - In memory representation of an APK archive
 * 
 * We store the raw binary as well as decompressed files and directory structure
 * inside the class
 */
class ApkArchive {
 // class definitions
 private:
  
  /*
   * class FileRecord - This corresponds to the file record inside the APK 
   *                    archive
   *
   * Each file has a compressed representation and a decompressed representation
   */
  class FileRecord {
  
  };
 
 // Private data members
 private:
   
 
  // Name of the file
  // This could not be changed once assigned inside the constructor
  // because we do not support reusing of the class - always create a new 
  // instance everytime
  const std::string filename;
 
 // Private member functions
 private:
  /*
   * Constructor
   */
  ApkArchive(const std::string filename) {
    FILE *fp = 
  }
  
  /*
   * ReportError() - Reports error on stderr and then throw exception
   *
   * The exception thrown is always integer 0
   */
  void ReportError(const char *format, ...) {
    va_list args;
    
    va_start (args, format);
    vfprintf (stderr, format, args);
    va_end (args);
    
    throw 0;
    
    return;
  }
  
  /*
   * GetFileSize() - Returns the size of an opened file
   *
   * This function does not stat(), and instead it moves the file pointer to
   * end end and then reports current offset
   */
  size_t GetFileSize(FILE *fp) {
    assert(fp != nullptr);
    
    int ret = fseek(fp, 0, SEEK_END);
    if(ret != 0) {
      ReportError(CANNOT_SEEK_FILE); 
    }
    
    long file_size = ftell(fp);
    if(file_size == -1L) {
      ReportError(ERROR_ACQUIRE_FILE_SIZE); 
    }
    
    int ret = fseek(fp, 0, SEEK_SET);
    if(ret != 0) {
      ReportError(CANNOT_SEEK_FILE); 
    }
    
    return static_cast<size_t>(file_size);
  }
   
 public: 
};

} // namespace android_dalvik_analysis {
} // namespace wangziqi2013

#endif
