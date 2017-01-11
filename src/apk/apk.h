
#pragma once

#ifndef _APK_H
#define _APK_H

#include <cstdarg>
#include "common.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

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
   * The file header is located at the beginning of compressed file data
   */
  class FileRecord {
    uint32_t singature;
    
  };
 
 // Private data members
 private:
   
  // This could not be changed once assigned inside the constructor
  // because we do not support reusing of the class - always create a new 
  // instance everytime
  const std::string file_name;
  size_t file_length;
  
  unsigned char *raw_data_p;
 
 // Public member functions
 public:
  /*
   * Constructor
   *
   * This function loads the file into memory
   */
  ApkArchive(const std::string &p_filename) :
    file_name{p_filename} {
    // Open the file as read and binary
    FILE *fp = fopen(file_name.c_str(), "rb");
    if(fp == nullptr) {
      ReportError(ERROR_OPEN_FILE, file_name.c_str());
    }
    
    file_length = GetFileLength(fp);
    
    // Allocate a buffer to hold the entire file and then close it
    raw_data_p = new unsigned char[file_length];
    if(raw_data_p == nullptr) {
      ReportError(OUT_OF_MEMORY); 
    }
    
    size_t size_read = fread(raw_data_p, file_length, 1, fp);
    if(size_read != file_length) {
      ReportError(ERROR_READ_FILE, size_read); 
    }
    
    int close_ret = fclose(fp);
    assert(close_ret == 0);
    
    return;
  }
  
  /*
   * Destructor
   */
  ~ApkArchive() {
    delete[] raw_data_p;
    
    return;
  } 
 
 // Private member functions
 private:
  
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
  
  /*
   * GetFileLength() - Returns the size of an opened file
   *
   * This function does not stat(), and instead it moves the file pointer to
   * end end and then reports current offset
   */
  size_t GetFileLength(FILE *fp) {
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
   
 public: 
};

} // namespace android_dalvik_analysis {
} // namespace wangziqi2013

#endif
