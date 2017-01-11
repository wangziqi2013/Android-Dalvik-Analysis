
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
   * enum class RecordType - Denotes the type of the record, because we cannot
   *                         decide the type before looking at its signature
   */
  enum class RecordType : uint32_t {
    LOCAL_FILE_HEADER = 0x04034b50,
    DATA_DESCRIPTOR = 0x08074b50,
    CENTRAL_DIR_FILE_HEADER = 0x02014b50,
    END_OF_CENTRAL_DIR = 0x06054b50,
  };
  
  /*
   * class LocalFileHeader - This corresponds to the file record inside the APK 
   *                         archive
   *
   * The file header is located at the beginning of compressed file data
   */
  class LocalFileHeader {
   public: 
    RecordType singature;
    uint16_t version_to_extract;
    uint16_t general_purpose_flags;
    uint16_t compression_method;
    uint16_t modification_time;
    uint16_t modification_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_length;
    uint16_t extra_field_length;
    
    // This is just a pointer to the next byte and does not 
    // constitute the actual length
    char file_name[0];
  } BYTE_ALIGNED;
  
  // Make sure we aligned it properly
  static_assert(sizeof(LocalFileHeader) == 30UL, 
                "class LocalFileHeader does not have correct length");
  
  /*
   * class CentralDirFileHeader - This represents the central file header record
   *                              located at the end of the archive
   */
  class CentralDirFileHeader {
   public:
    RecordType signature;
    uint16_t version_made_by;
    uint16_t version_to_extract; 
    
    // The following are the same with those in local file header
    uint16_t general_purpose_flags;
    uint16_t compression_method;
    uint16_t modification_time;
    uint16_t modification_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_length;
    uint16_t extra_field_length;
    
    uint16_t file_comment_length;
    // Disk number where file starts; we always assume it is 0
    uint16_t disk_num;
    uint16_t internal_file_attr;
    uint32_t external_file_attr;
    
    // This is the relative offset on the disk it first appears
    // for a single part file this is the offset from the beginning
    uint32_t local_header_offset;
    
    char file_name[0];
  } BYTE_ALIGNED;
  
  static_assert(sizeof(CentralDirFileHeader) == 46UL, 
                "class CentralDirFileHeader does not have correct length");
  
  /*
   * class EndOfCentralDir - This is served as the header of the entire archive 
   *                         and is located at the end of the file
   */
  class EndOfCentralDir {
   public:
    RecordType singature;
    uint16_t this_disk;
    uint16_t total_disk;
    uint16_t this_disk_file_count;
    uint16_t total_file_count;
    uint32_t size_of_central_dir;
    uint32_t central_dir_offset;
    uint16_t comment_length;
    
    // This does not constitute the actual length and is only
    // used as the pointer to the next byte
    char comment[0];
  } BYTE_ALIGNED;
  
  // Make sure we aligned it properly
  static_assert(sizeof(EndOfCentralDir) == 22UL, 
                "class EndOfCentralDir does not have correct length");
  
  // All archive must be at least that long
  static constexpr size_t MINIMUM_ARCHIVE_LENGTH = sizeof(EndOfCentralDir);
  
 // Private data members
 private:
   
  // This could not be changed once assigned inside the constructor
  // because we do not support reusing of the class - always create a new 
  // instance everytime
  const std::string file_name;
  size_t file_length;
  
  // This one is kept as the current read position
  size_t read_offset;
  
  // This points to the last structure in the archive
  EndOfCentralDir *eof_central_dir_p;
  
  // This points to the first entry of central dir file header
  // and it is an array
  CentralDirFileHeader *central_dir_p;
  // This is the number of central dirs
  size_t central_dir_count;
  
  unsigned char *raw_data_p;
 
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
  
  /*
   * ScanForEndOfCentralDir() - Scan in the byte stream from the end to the 
   *                            beginning for finding the end of central dir
   *
   * Since we know the minimum size of the central record we could start
   * scanning from the last byte - minimum length of the EOF central record
   *
   * This will set the member once it is found. Otherwise the data member will
   * be set to nullptr
   */
  void ScanForEndOfCentralDir() {
    unsigned char *scan_begin = \
      raw_data_p + file_length - MINIMUM_ARCHIVE_LENGTH;
    eof_central_dir_p = reinterpret_cast<EndOfCentralDir *>(scan_begin);
    
    // Do a byte-wise scan until the beginning
    while(scan_begin >= raw_data_p) {
      if(eof_central_dir_p->singature == RecordType::END_OF_CENTRAL_DIR) {
        // If the comment length matches the actual length we have jumped then
        // we presume it is a valid EOF central record and return
        if(scan_begin + \
            eof_central_dir_p->comment_length + \
            MINIMUM_ARCHIVE_LENGTH == \
           raw_data_p + file_length) {
          return;
        }
      }
      
      // Otherwise move one byte backward and retry
      scan_begin--;
      eof_central_dir_p = reinterpret_cast<EndOfCentralDir *>(scan_begin);
    }
    
    // If we have ever reached here just return because nothing was found
    eof_central_dir_p = nullptr;
    
    return;
  }
  
  /*
   * FindCentralDir() - Find the central directory and validate its contents
   */
  void FindCentralDir() {
    // Do not support multi-part file
    if(eof_central_dir_p->this_disk != 0 || \
       eof_central_dir_p->total_disk != 0) {
      ReportError(MULTI_PART_NOT_SUPPORTED, eof_central_dir_p->total_disk);
    } else {
      dbg_printf("    # of disks: %u\n", 
                 eof_central_dir_p->total_disk);
    }
    
    // Both file counts must be the same otherwise we have inconsistency
    if(eof_central_dir_p->this_disk_file_count != \
       eof_central_dir_p->total_file_count) {
      ReportError(CORRUPTED_ARCHIVE, "Inconsistent file count");
    } else {
      dbg_printf("    # of records: %u\n", 
                 eof_central_dir_p->total_file_count); 
    }
    
    // Fill in the central dir count and pointer
    central_dir_count = eof_central_dir_p->total_file_count;
    central_dir_p = \
      reinterpret_cast<CentralDirFileHeader *>(
        raw_data_p + eof_central_dir_p->central_dir_offset);
    
    // Verify that both the first and last one has a valid header
    if((central_dir_p->signature != RecordType::CENTRAL_DIR_FILE_HEADER)) {
      ReportError(CORRUPTED_ARCHIVE, 
                  "Invalid central directory file header signature");
    }
    
    return;
  }
  
  /*
   * GetNextHeader() - Returns the next central dir file header pointer
   *
   * Since the length of CentralDirFileHeader is not fixed length we need to
   * compute the next byte offset manually using the length of variable length 
   * fields recorded inside the object
   */
  inline CentralDirFileHeader *GetNextHeader(CentralDirFileHeader *header_p) {
    unsigned char *byte_offset = reinterpret_cast<unsigned char *>(header_p);
    
    byte_offset += (header_p->file_name_length + \
                    header_p->extra_field_length + \
                    header_p->file_comment_length);
                    
    return reinterpret_cast<CentralDirFileHeader *>(byte_offset);
  }
   
 // Public member functions
 public:
  /*
   * Constructor
   *
   * This function loads the file into memory
   */
  ApkArchive(const std::string &p_filename) :
    file_name{p_filename},
    file_length{0UL},
    read_offset{0UL},
    eof_central_dir_p{nullptr},
    central_dir_p{nullptr},
    central_dir_count{0UL} {
    // Open the file as read and binary
    FILE *fp = fopen(file_name.c_str(), "rb");
    if(fp == nullptr) {
      ReportError(ERROR_OPEN_FILE, file_name.c_str());
    }
    
    // File length must be at least longer to hold the file header
    file_length = GetFileLength(fp);
    if(file_length <= MINIMUM_ARCHIVE_LENGTH) {
      ReportError(FILE_TOO_SMALL, file_length);
    }
    
    // Allocate a buffer to hold the entire file and then close it
    raw_data_p = new unsigned char[file_length];
    if(raw_data_p == nullptr) {
      ReportError(OUT_OF_MEMORY); 
    }
    
    size_t size_read = fread(raw_data_p, 1, file_length, fp);
    if(size_read != file_length) {
      ReportError(ERROR_READ_FILE, size_read);
    }
    
    int close_ret = fclose(fp);
    assert(close_ret == 0);
    
    // Find end of central dir record first
    ScanForEndOfCentralDir();
    if(eof_central_dir_p == nullptr) {
      ReportError(NO_EOF_CENTRAL_DIR); 
    } else {
      dbg_printf("Found end of central directory record @ %p\n", 
                 eof_central_dir_p);
    }
    
    FindCentralDir();
    dbg_printf("Found central directory record beginning @ %p\n",
               central_dir_p);
    
    return;
  }
  
  /*
   * Destructor
   */
  ~ApkArchive() {
    delete[] raw_data_p;
    
    return;
  }  
  
  void DebugPrintAllFileName() {
    
  } 
};

} // namespace android_dalvik_analysis {
} // namespace wangziqi2013

#endif
