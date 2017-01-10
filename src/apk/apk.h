
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
 
 // Private member functions
 private:
   
 public: 
};

} // namespace android_dalvik_analysis {
} // namespace wangziqi2013

#endif
