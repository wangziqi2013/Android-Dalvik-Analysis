
#pragma once

#ifndef _PACKAGE_GROUP_H
#define _PACKAGE_GROUP_H

#include <unordered_map>
#include "common.h"
#include <mutex>

namespace wangziqi2013 {
namespace android_dalvik_analysis { 

// Declare them here first and then 
class ResourceTable;

/*
 * class PackageGroup - A group of packages for retrieving resources
 */
class PackageGroup {
 private: 
  // This maps the package ID to its containing resource table
  // We do not directly map to package because the member function is written
  // inside the ResourceTable class
  std::unordered_map<uint8_t, ResourceTable *> package_map;
  
  // Use this to protect the map under concurrent workload
  std::mutex map_lock;
  
 public:
  
  /*
   * RegisterPackage() - Associates package ID with resource table
   */
  void RegisterPackage(uint8_t package_id, ResourceTable *table_p) {
    map_lock.lock();
    
    // This returns pair<iterator, bool> with the second element denoting 
    // whether the insertion really happened
    auto insert_ret = package_map.insert(std::make_pair(package_id, table_p));
    
    map_lock.unlock();
    
    if(insert_ret.second == false) {
      ReportError(PACKAGE_ALREADY_REGISTERED, 
                  static_cast<uint32_t>(package_id));
    }
    
    return;
  }
  
  /*
   * UnregisterPackage() - Unregister a package from the package group
   *
   * This has to be called everytime a resource table is destroyed
   */
  void UnregisterPackage(uint8_t package_id) {
    map_lock.lock();
    
    // Return value indicates the number of packages removed
    // If it is 0 then we know the ID even does not exist
    size_t erased_count = package_map.erase(package_id);
    
    map_lock.unlock();
    
    // If the package is not registered then throw error
    if(erased_count == 0UL) {
      ReportError(PACKAGE_ID_NOT_FOUND, static_cast<uint32_t>(package_id)); 
    }
    
    return;
  }
  
  /*
   * GetResourceTable() - Given a package group, return the resource table
   *                      that contains the package group
   *
   * If resource table is not found just report error
   */
  ResourceTable *GetResourceTable(uint8_t package_id) {
    map_lock.lock();
    
    auto it = package_map.find(package_id);
    if(it == package_map.end()) {
      ReportError(PACKAGE_ID_NOT_FOUND, static_cast<uint32_t>(package_id)); 
    }
    
    map_lock.unlock();
    
    return it->second;
  }
};

// This is the global object
extern PackageGroup package_group;

} // namespace android_dalvik_analysis
} // namespace wangziqi2013

#endif
