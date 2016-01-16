
#ifndef _DEX_H
#define _DEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <map>
#include <vector>

using namespace std;

enum file_map_item_t
{
    DEX_HEADER = 0x0,
    STRING_ID,
    TYPE_ID,
    PROTOTYPE_ID,
    FIELD_ID,
    METHOD_ID,
    CLASS_DEF,
    MAP_LIST = 0x1000,
    TYPE_LIST,
    ANNOTATION_LIST,
    ANNOTATION_REF,
    CLASS_DATA_ITEM = 0x2000,
    CODE,
    STRING_DATA,
    DEBUG_INFO,
    ANNOTATION,
    ENCODED_ARRAY,
    ANNOTATION_DIR,
};

typedef unsigned short file_map_item_type_t;
typedef unsigned int type_id_t;
typedef unsigned short short_type_id_t;
typedef unsigned int str_id_t;
typedef unsigned int proto_id_t;
typedef unsigned short short_proto_id_t;
typedef unsigned int field_id_t;
typedef unsigned int method_id_t;

// Used as access flags
typedef unsigned int access_flag_t;

struct FileMapItem
{
    file_map_item_type_t type;
    unsigned int count;
    unsigned int offset;
    
    // Trivial constructor
    FileMapItem() {}
    
    // Normal constructor
    FileMapItem(file_map_item_type_t ptype, 
                unsigned int pcount,
                unsigned int poffset)
    {
        this->type = ptype;
        this->count = pcount;
        this->offset = poffset;
        
        return;
    }
    
    // Copy constructor
    FileMapItem(const FileMapItem &fmi)
    {
        this->type = fmi.type;
        this->count = fmi.count;
        this->offset = fmi.offset;
        
        return;    
    }
};

/*
 * struct StringItem - Hold string data ptr and ASCII flag
 */
struct StringItem
{
    char *data;
    bool is_ascii;
};

/*
 * struct ProtoItem - Hold function prototype information
 */
struct ProtoItem
{
    str_id_t description;
    type_id_t rtype;
    // A list of indexed into the type id list as argument type
    short_type_id_t *param_type_list;
    // Number of parameters
    unsigned int param_count;
};

/*
 * struct FieldItem - Holds the class, type and identifier of a field
 */
struct FieldItem
{
    short_type_id_t cls;
    short_type_id_t type;
    str_id_t name;
};

/*
 * struct MethodItem - Holds the class, prototype and name of a method
 */
struct MethodItem
{
    short_type_id_t cls;
    short_proto_id_t proto;
    str_id_t name;
};

/*
 * struct FieldDef - Field definition inside class definition
 *
 * Used as both static field and instance field
 */
struct FieldDef
{
    // An index into field item list
    field_id_t field;
    access_flag_t access_flag;
};

/*
 * struct TryItem - Try block item definition, inside one method
 *
 * A try structure is described by the start address and end address
 * (i.e. covered length), and also its handler offset is included
 * for exception handling
 */
struct TryItem
{
    // Start address inside method code, in 16 bit word
    unsigned int start_word_address;
    // Covered length in 16 bit word
    unsigned short word_length;
    // The handler of this item in handler list
    // Since handlers are not equi-lengthed this offset
    // is necessary instead of an index
    unsigned short handler_byte_offset;
};

/*
 * struct TypedHandlerItem
 *
 * Typed (or, all typed) handler
 */
struct TypedHandlerItem
{
    unsigned int type;
    // The offset in byte code
    unsigned int word_offset;
};

/*
 * struct HandlerItem
 *
 * Handler items for try block inside a method
 */
struct HandlerItem
{
    // If this is set to true then the last handler in
    // the array is all type handler
    bool all_type_handler_flag;
    // The offset into byte code of all typed handler
    unsigned int all_type_handler_word_offset;
    
    unsigned int pair_count;
    TypedHandlerItem *typed_handler_list;
    
    // Offset in the array, used in try block to identify
    // handler item
    unsigned int array_offset;
    
    ~HandlerItem()
    {
        delete[] typed_handler_list;
    }
};

/*
 * struct MethodDef
 *
 * Defines a method's input, output, bytecode, try, exception handler
 * and other parameters
 */
struct MethodDef
{
    method_id_t method;
    access_flag_t access_flag;
    unsigned int code_offset;
    
    // Next is code object - We again make it more compact by merging
    // code item into method def - 1 to 1 correspondence
    // Number of registered used in this method
    unsigned short register_count;
    // Size of input variables in WORD size
    unsigned short input_word_count;
    unsigned short output_word_count;
    unsigned short try_item_count;
    
    unsigned int debug_info_offset;
    // Size of instructions in 16 bits (i.e. * 2 to get byte size)
    unsigned int instruction_size_16bit;
    // Actual byte code
    unsigned char *bytecode;
    
    unsigned int try_offset;
    
    // A list of try item. Also need to be released
    TryItem *tries;
    
    // Encoded as the first ULEB128 in handler list
    // NOTE: Handler and try may not have the same count
    // because one try could have multiple handlers for different
    // types. But number of handler is always greater than or equal to
    // the number of types, which implies that if try number is more than 0
    // then handler is also more than 0
    // If try handler is 0 then handler is also zero
    unsigned int handler_count;
    
    HandlerItem *handler_list;
    
    // That's the few places where we define destructor
    // This is done to prevent deep loops in destructor of the 
    // container class
    // However in order to prevent delete <-> delete[] confusion
    // we do this in a restricted manner
    ~MethodDef()
    {
        // Only one suffices, but we want stronger condition
        // to make it less error-prone
        if(this->bytecode && this->code_offset == 0)
        {
            delete[] this->bytecode;
            delete[] this->tries;
            delete[] this->handler_list;
        }
        
        return;
    }
    
};

/*
 * struct ClassdefItem - Merges class def, class data and interface list
 */
struct ClassdefItem
{
    // Original fields in the raw file
    type_id_t cls;
    type_id_t super;
    access_flag_t access_flag;
    // Points to a type list structure
    unsigned int intf_offset;
    str_id_t source_file;
    unsigned int annotation_offset;   // TODO: READ ANNOTATION
    unsigned int cls_data_offset;
    unsigned int static_value_offset; // TODO: READ STATIC VALUE INTO MEMORY
    
    // Derived fields in the raw file
    unsigned int intf_count;
    short_type_id_t *intf_list;
    
    // These are actually another structre called ClassData
    // but we merge it with ClassdefItem since it is always
    // a 1-to-1 correspondence - no need to add another level
    // of indirection
    unsigned int static_field_count;
    unsigned int instance_field_count;
    unsigned int direct_method_count;
    unsigned int virtual_method_count;
    
    FieldDef *static_field_list;
    FieldDef *instance_field_list;
    
    MethodDef *direct_method_list;
    MethodDef *virtual_method_list;
};

/*
 * struct DalvikExecutable - Hold every piece of metadata and code
 */
struct DalvikExecutable
{
    static const char valid_signature[8];
    static const unsigned int valid_header_size = 0x00000070;
    static const unsigned int valid_endian_indicator = 0x12345678;
     
    FILE *fp;
    // Offset of the file map
    unsigned int file_map_offset;
    // The offset into DEX file after the padding
    // i.e. the first byte of FILE MAP PTR
    unsigned int after_padding_offset;
    // Read ID size/offset from this offset into DEX file
    unsigned int after_file_map_offset;
    
    map<file_map_item_type_t, FileMapItem> file_map;
    StringItem *string_item_list;
    type_id_t *type_item_list;
    ProtoItem *proto_item_list;
    FieldItem *field_item_list;
    MethodItem *method_item_list;
    ClassdefItem *classdef_item_list;
    
    // Count and offset, except for data which is bytes
    unsigned int string_id_count, string_id_offset;
    unsigned int type_id_count, type_id_offset;
    unsigned int proto_id_count, proto_id_offset;
    unsigned int field_id_count, field_id_offset;
    unsigned int method_id_count, method_id_offset;
    unsigned int classdef_id_count, classdef_id_offset;
    unsigned int data_size, data_offset;
    
    DalvikExecutable(const char *filename);
    ~DalvikExecutable();
    
    void OpenFile(const char *filename);
    
    // This is only defined for non-short version
    // It should work with short indices since they
    // can be simply 0 extended without any problem
    char *GetTypeString(type_id_t t) 
    { 
        return this->string_item_list[this->type_item_list[t]].data; 
    }
    
    char *GetString(str_id_t s)
    {
        return this->string_item_list[s].data;    
    }
    
    void VerifyDexHeader();
    void ReadFileMapPtrAndItem();
    void ReadCountOffsets();
    // Read a 32-bit value encoded as ULEB128
    unsigned int ReadULEB128(bool sign_extension);
    void ReadStringIdTable();
    void ReadTypeIdTable();
    void ReadProtoIdTable();
    void ReadFieldIdTable();
    void ReadMethodIdTable();
    void ReadClassdefTable();
    void ReadEncodedFieldTable(FieldDef **fd_pp, 
                               unsigned int field_count,
                               const char *category);
    void ReadEncodedMethodTable(MethodDef **md_pp, 
                                unsigned int method_count,
                                const char *category);
    void ReadBytecodeForAll();

// Helper functions, not "really" private
private:
    // Read count/offset pairs from the header
    void ReadCountOffsetPair(unsigned int *count, 
                             unsigned int *offset,
                             const char *category);
    void ReadTypeList(short_type_id_t **list_pp, 
                      unsigned int *type_count,
                      const char *category);
    void ReadBytecodeForMethod(MethodDef *md_p);
    void ReadTryAndHandlerForMethod(MethodDef *md_p);
};

#endif
