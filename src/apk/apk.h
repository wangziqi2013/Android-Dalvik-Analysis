
#pragma once

#ifndef _APK_H
#define _APK_H

#include <cstdarg>
#include "common.h"

#include <zlib.h>

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

/*
 * class ApkArchive - In memory representation of an APK archive
 * 
 * We store the raw binary as well as decompressed files and directory structure
 * inside the class
 */
class ApkArchive {
  // Iterator needs to access its internal states
  friend class Iterator;
  
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
    RecordType signature;
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
    
    /*
     * IsValid() - Verifies the signature
     */
    inline bool IsValid() const {
      return signature == RecordType::LOCAL_FILE_HEADER;
    }
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
    
    /*
     * GetNext() - Returns the next central dir file header pointer
     *
     * Since the length of CentralDirFileHeader is not fixed length we need to
     * compute the next byte offset manually using the length of variable length 
     * fields recorded inside the object
     */
    inline CentralDirFileHeader *GetNext() {
      // Take the address of the current pointer
      // The compiler will use this to refer to the pointer on which we
      // call this function so it is totally legal
      unsigned char *byte_offset = reinterpret_cast<unsigned char *>(this);
      
      byte_offset += (file_name_length + \
                      extra_field_length + \
                      file_comment_length + \
                      sizeof(CentralDirFileHeader));
                      
      return reinterpret_cast<CentralDirFileHeader *>(byte_offset);
    }
    
    /*
     * IsValid() - Verifies the signature
     */
    inline bool IsValid() const {
      return signature == RecordType::CENTRAL_DIR_FILE_HEADER;
    }
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
  
  /*
   * class StringWrapper - Wrapper to a string that this class has no ownership
   *
   * This class contains a pointer to externally allocated string and a size_t
   * field to denote the size of the string.
   *
   * Ths string is not null-terminated and the handling of the string is 
   * automatically taken care of
   */
  class StringWrapper {
   private:
    char *p;
    size_t length;
    
   public:
    
    /*
     * constructor
     */
    StringWrapper(char *p_p, size_t p_length) :
      p{p_p},
      length{p_length}
    {}
    
    /*
     * PrintToFile() - Prints to a file specified in argument
     */
    void PrintToFile(FILE *fp) {
      // Print n - 1 characters first by putting a '\0' after it
      char ch = p[length - 1];
      p[length - 1] = '\0';
      
      fprintf(fp, "%s", p);
      fputc(ch, fp);
      
      p[length - 1] = ch;
      
      return;
    }
  };
  
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
    if(central_dir_p->IsValid() == false) {
      ReportError(CORRUPTED_ARCHIVE, 
                  "Invalid central directory file header signature");
    }
    
    return;
  }
 
 // Private iterator class (use Begin() to create one)
 private:
  
  /*
   * class Iterator - An encapsulation to archived files that allows pipelined
   *                  decompression/usage of these files
   */
  class Iterator {
   private:
    // Must keep it because the structure is not fixed length and therefore
    // we could not get the pointer using an offset
    CentralDirFileHeader *header_p;
    ApkArchive *archive_p;
    
   public:
     
    /*
     * Constructor
     */
    Iterator(ApkArchive *p_archive_p) :
      header_p{p_archive_p->central_dir_p},
      archive_p{p_archive_p}
    {}
    
    /*
     * IsEnd() - Whether we have reached the end of iteration
     */
    inline bool IsEnd() {
      return header_p->IsValid() == false;
    }
    
    /*
     * operator++ (prefix)
     */
    Iterator &operator++() {
      // Cannot do this on an invalid iterator
      if(IsEnd() == true) {
        archive_p->ReportError(END_OF_ITERATION);
      } else {
        header_p = header_p->GetNext();
      }
      
      return *this;
    }
    
    /*
     * operator++ (postfix)
     */
    Iterator operator++(int) {
      Iterator old_it = *this;
      
      // Call prefix version here to actually do the job
      ++(*this);
      
      return old_it;
    }
    
    /*
     * GetFileName() - Returns a StringWrapper as the file name
     */
    inline StringWrapper GetFileName() {
      return StringWrapper{header_p->file_name, header_p->file_name_length};
    }
    
    /*
     * operator* - Returns the string name
     */
    inline StringWrapper operator*() {
      return GetFileName();
    }
    
    /*
     * Decompress() - Use DEFLATE algorithm to decompress the data
     */
    void Decompress(void *dest, 
                    size_t dest_length, 
                    void *src, 
                    size_t src_length) {
      z_stream strm;
      int ret;
      
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      strm.avail_in = 0;
      strm.next_in = Z_NULL;
      
      ret = inflateInit(&strm);
      if (ret != Z_OK) {
        archive_p->ReportError(ERROR_INIT_ZLIB);
      }
      
      strm.avail_in = src_length;
      strm.next_in = src;
      
      strm.avail_out = dest_length;
      strm.next_out = dest;
      
      ret = inflate(&strm, Z_NO_FLUSH);
      if(ret != Z_STREAM_END) {
        archive_p->ReportError(ERROR_INFLATE); 
      }
      
      return;
    }
    
    /*
     * GetData() - Decompresses data and return it in an allocated array
     *
     * This function will allocate the array on the heap, and caller is
     * responsible for freeing the memory after using it 
     */
    void *GetData() {
      // Allocate as many that number of bytes 
      void *data = new unsigned char[header_p->uncompressed_size];
      if(data == nullptr) {
        archive_p->ReportError(OUT_OF_MEMORY);  
      } 
      
      Decompress(data, 
                 header_p->uncompressed_size, 
                 , 
                 header_p->compressed_size);
      
      return static_cast<void *>(data);
    }
  };
   
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
  
  /*
   * Begin() - Returns an iterator to the beginning of all file records
   */
  Iterator Begin() {
    return Iterator{this};
  }
  
  /*
   * DebugPrintAllFileName() - Prints out all files from the central dir as 
   *                           a debugging tool
   */
  void DebugPrintAllFileName() {
    Iterator it = Begin();
    
    while(it.IsEnd() == false) {
      it.GetFileName().PrintToFile(stderr);
      fputc('\n', stderr); 
      
      it++;
    }
    
    return;
  } 
};

} // namespace android_dalvik_analysis {
} // namespace wangziqi2013

#endif
