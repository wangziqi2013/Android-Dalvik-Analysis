
#pragma once

#ifndef _BUFFER_H
#define _BUFFER_H

namespace wangziqi2013 {
namespace android_dalvik_analysis {

/*
 * class Buffer - In memory buffer for holding data for buffering them to be
 *                written into file 
 */
class Buffer {
 private: 
  unsigned char *data_p;
  size_t length;
  
  size_t current_length;
  
#ifndef BUFFER_TEST_MODE
  // Use 64K as default buffer size if none is given
  static constexpr size_t DEFAULT_SIZE = 0x1UL << 16;
  // 1 KB for stack buffer. Anything longer than this will be 
  // allocated on the heap and then combined into existing data
  static constexpr size_t STACK_BUFFER_SIZE = 0x1UL << 10;
#else
  // Under debug mode make is as small as possible to test whether 
  // bufer expansion works
  static constexpr size_t DEFAULT_SIZE = 4UL;
  // For buffer test we want to cover all branches so make it smaller 
  // such that we have a change to test stack and heap allocation
  static constexpr size_t STACK_BUFFER_SIZE = 4UL;
#endif
  
  /*
   * Expand() - Adjust the size of the buffer for holding more data
   *
   * Do not use this to shrink the buffer
   */
  void Expand(size_t resize_length) {
#ifdef BUFFER_TEST_MODE
    dbg_printf("Expand() called: from %lu to %lu\n", length, resize_length);
#endif
    
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
  
  // Deleted functions to avoid copy construction and assignment
  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer &operator=(Buffer &&) = delete;
  
  /*
   * Move constructor - This is called be vector::emplace_back()
   */
  Buffer(Buffer &&other) :
    data_p{other.data_p},
    length{other.length},
    current_length{other.current_length} {
    // Set it to null to avoid destruction
    other.data_p = nullptr;
    
    return;    
  }
  
  /*
   * Destructor
   */
  ~Buffer() {
    if(data_p != nullptr) {
      delete[] data_p;
    }
    
    return; 
  }
  
  /*
   * GetLength() - Returns the current length of data
   */
  inline size_t GetLength() const {
    return current_length; 
  }
  
  /*
   * GetData() - Returns a const pointer to the data field
   */
  inline const void *GetData() const {
    return data_p; 
  }
  
  /*
   * Rewind() - Move the tail pointer towards the beginning of the buffer
   *
   * If the rewinded size is too large (i.e. larger than the current content)
   * then assertion would fail
   */
  inline void Rewind(size_t size) {
    assert(size <= current_length);
    
    current_length -= size;
    
    return;
  }
  
  /*
   * Reset() - Resets the current length to 0
   */
  inline void Reset() {
    current_length = 0;
  }
  
  /*
   * AppendByte() - Appends only a byte to the buffer
   *
   * This is more efficient since it takes less effort to determine whether a
   * expand is needed and also to copy the char data
   */
  void AppendByte(unsigned char byte) {
    // If the buffer is full just double the size
    if(current_length == length) {
      Expand(length << 1);
    }
    
    assert(current_length < length);
    
    data_p[current_length] = byte;
    current_length++;
    
    return;
  }
  
  /*
   * Append() - Append bytes after the current location
   *
   * This function might trigger Oversize() operation if requested size
   * is too large
   */
  void Append(const void *p, size_t request_length) {
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
   * Append() - Appends a C srting (i.e. null-terminated string) to the buffer
   *
   * This function overloads the most common form of Append()
   */
  inline void Append(const char *p) {
    Append(p, strlen(p));
  }
  
  /*
   * Append() - Appends a character
   *
   * Since this could be made more efficient because we know the storage
   * grows by 1 at most so just call AppendByte() inside
   */
  inline void Append(char ch) {
    AppendByte(ch); 
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
  
  /*
   * Printf() - Formatted output into the buffer object
   *
   * This function operates by calling vsnprintf() twice: The first time we 
   * do a stack allocation and count how many characters are required. And
   * if the stack buffer is not sufficient we open a heap buffer with length
   * indicated by the first call and then do a second try to print the content
   * into the heap buffer. Fianlly the buffer is combined with existing data
   * inside the buffer 
   */
  void Printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    // The return value is the number of characters excluding the '\0'
    // that would have been written if the buffer is large enough
    char stack_buffer[STACK_BUFFER_SIZE];
    int print_length = \
      vsnprintf(stack_buffer, STACK_BUFFER_SIZE, format, args);
    
    size_t expected_length = static_cast<size_t>(print_length) + 1;
    
    // Need do heap allocation to hold data and then combine
    if(expected_length > STACK_BUFFER_SIZE) {
      char *heap_buffer = new char[expected_length];
      if(heap_buffer == nullptr) {
        ReportError(OUT_OF_MEMORY); 
      }
      
      // Must use a new set of states since the old one has been traversed
      va_list args_2;
      va_start(args_2, format);
      
      // We already know the return value
      vsnprintf(heap_buffer, expected_length, format, args_2);
      
      va_end (args_2);
      
      // Do not append the terminating 0 character
      Append(heap_buffer, static_cast<size_t>(print_length));
      
      // Release heap buffer here
      delete[] heap_buffer;
    } else {
      Append(stack_buffer, print_length);
    }
    
    va_end (args);
    return;
  }
};

}
}

#endif
