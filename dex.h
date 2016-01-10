
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

struct MethodDef
{
	method_id_t method;
	access_flag_t access_flag;
	unsigned int code_offset;
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
	unsigned int annotation_offset;
	unsigned int cls_data_offset;
	unsigned int static_value_offset;
	
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

// Helper functions, not "really" private
private:
	// Read count/offset pairs from the header
	void ReadCountOffsetPair(unsigned int *count, 
							 unsigned int *offset,
							 const char *category);
	void ReadTypeList(short_type_id_t **list_pp, 
					  unsigned int *type_count,
					  const char *category);
};

#endif
