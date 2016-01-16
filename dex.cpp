
#include "dex.h"
#include "bytecode.h"

static bool dex_stdout_print_flag = false;
static bool dex_log_print_flag = true;

// Maintain a unified log file across instances
// Reference counts are used to makre sure the log will be 
// created and closed on the right time and place
static FILE *log_fp = NULL;
static int log_ref_count = 0;
// Logs are named by a timestamp to avoid confusion
static char log_filename[sizeof("DexReader_xxxxxxxx.log")];

#define dex_assert(cond) \
do { \
    if(!(cond)) \
    { \
        fprintf(stdout, "Assertion Fail @ Line %d File %s\n", \
                        __LINE__,  \
                        __FILE__); \
        if(log_fp) \
        { \
            fprintf(log_fp, "Assertion Fail @ Line %d File %s\n", \
                            __LINE__,  \
                            __FILE__); \
        } \
        exit(1); \
    } \
} while(0);

#define dex_printf(fmt, args...) \
do { \
    if(dex_stdout_print_flag) \
    { \
        fprintf(stderr, fmt "\n", args); \
    } \
    \
    if(dex_log_print_flag && log_fp) \
    { \
        fprintf(log_fp, fmt "\n", args); \
    } \
} while(0);

// If cond evaluates to false, then print fmt with arguments, and then fail
#define dex_debug(cond, fmt, args...) \
do { \
    if(!(cond)) \
    { \
        fprintf(stdout, "Debug Fail @ Line %d File %s:\n*** " fmt "\n", \
                        __LINE__, \
                        __FILE__, \
                        args); \
        if(log_fp) \
        { \
            fprintf(log_fp, "Debug Fail @ Line %d File %s:\n*** " fmt "\n", \
                            __LINE__, \
                            __FILE__, \
                            args); \
        } \
        exit(1); \
    } \
} while(0);

// The valid dex file signature at the beginning 8 bytes
const char DalvikExecutable::valid_signature[8] = "dex\n035";

/*
 * Constructor
 *
 * Open the file specified in argument. If open fails then assertion fail
 * The file pointer is held open throughout the lifecycle of object instance
 */
DalvikExecutable::DalvikExecutable(const char *filename)
{
    // Prevent from using integer length other than 4
    dex_debug(sizeof(int) == 4,
              "sizeof(int) == %u is not supported",
              sizeof(int));
    // Short type must be 2 bytes (this seems to be always true)
    dex_debug(sizeof(short) == 2,
              "sizeof(short) == %u is not supported",
              sizeof(short));
    dex_debug(sizeof(char) == 1,
              "sizeof(char) == %u is not supported",
              sizeof(char));
    
    FILE *fp = fopen(filename, "rb");
    dex_debug(fp, "File %s open fails", filename);
    
    this->fp = fp;
    this->string_item_list = NULL;
    this->proto_item_list = NULL;
    
    // Open log file if it has not been opened
    if(log_ref_count == 0)
    {
        // Use timestamps to distinguish between logs
        sprintf(log_filename, "DexReader_%.8X.log", (unsigned int)time(NULL));
        log_fp = fopen(log_filename, "w");
        dex_debug(log_fp,
                  "Cannot open log file: %s",
                  log_filename);
                  
        dex_printf("Log file created: %s", log_filename);
    }
    
    log_ref_count++;
    
    return;
}

/*
 * Destructor
 *
 * - Close DEX file pointer
 * - If no other instances are referencing the log, close log file as well
 * - Cleanup nested structures 
 */
DalvikExecutable::~DalvikExecutable()
{
    dex_assert(fclose(this->fp) == 0);
    
    this->file_map.clear();
    
    // Clean every string instance in the list
    for(unsigned int i = 0;
        i < this->string_id_count;
        i++)
    {
        StringItem *si_p = this->string_item_list + i;
        delete[] si_p->data;
    }
    // Free the list itself
    delete[] this->string_item_list;
    delete[] this->type_item_list;
    
    for(unsigned int i = 0;
        i < this->proto_id_count;
        i++)
    {
        ProtoItem *pi_p    = this->proto_item_list + i;
        delete[] pi_p->param_type_list;
    }
    
    delete[] this->proto_item_list;
    delete[] this->field_item_list;
    delete[] this->method_item_list;
    
    // Cleanup classdef item list
    for(unsigned int i = 0;
        i < this->classdef_id_count;
        i++)
    {
        ClassdefItem *ci_p = this->classdef_item_list + i;
        
        delete[] ci_p->direct_method_list;
        delete[] ci_p->instance_field_list;
        delete[] ci_p->static_field_list;
        delete[] ci_p->virtual_method_list;    
        // This is a type list
        delete[] ci_p->intf_list;
    }
    
    delete[] this->classdef_item_list;
    
    // If no one is using the log, just close the file
    log_ref_count--;
    if(log_ref_count == 0)
    {
        dex_printf("Log file %s closed", log_filename);
        dex_assert(fclose(log_fp) == 0);
    }
    
    return;
}

/*
 * ReadULEB128() - Read a 32-bit unsigned value encoded in ULEB128
 *
 * Read 1 byte, check whether highest bit is 0, if not then continue
 * the LSB 7 bits are treated as payload
 *
 * Argument sign_extension is used to instruct whether sign extension
 * should be enforced. The second MSB of the last byte is sign bit
 * and should be propagated to the remaining bits
 */
unsigned int DalvikExecutable::ReadULEB128(bool sign_extension)
{
    unsigned char bytes[5];
    unsigned char *ptr = bytes;
    unsigned char byte;
    unsigned int value = 0x00000000;
    
    // Read 5 bytes into memory
    dex_assert(fread(bytes, sizeof(bytes), 1, this->fp) == 1);
    
    do
    {
        byte = *ptr;

        // Mask off MSB, shift 7N bits, and ORed into the value
        value |= ((byte & 0x7F) << ((ptr - bytes) * 7));
        ptr++;
    } while(byte & 0x80);
    
    // When we are here, byte is the last byte we have read
    // whose MSB must be 0, LSB must be sign bit
    // and ptr is the next byte potentially to be read
    if(sign_extension && (byte & 0x40))
    {
        // Number of bytes we have read
        // ptr has already been increased by 1 so no need to +1
        int byte_num = ptr - bytes;
        // The index of the most significant bit
        int msb_index = byte_num * 7 - 1;
        // Number of "1"s we need to fill to the high bits
        int shift_num = msb_index > 31 ? 0 : 31 - msb_index;
        // Cast to signed int for arithmetic shift, and then cast back
        // for bitwise operation
        unsigned int mask = (unsigned int)((signed int)0x80000000 >> \
                                           shift_num);
        value |= mask;
    }
    
    // Adjust the pointer since there might be data we have not read
    dex_assert(fseek(this->fp, 
                     (int)(ptr - bytes) - (int)5, 
                     SEEK_CUR) == 0);
    
    return value;
}

/*
 * ReadCountOffsetPair() - Read two unsigned int into given arguments
 *
 * This is repeatedly called multiple times for reading DEX header
 * Argument category is used for print statement which provides a 
 * flexible way to report what have been read
 */
void DalvikExecutable::ReadCountOffsetPair(unsigned int *count,
                                              unsigned int *offset,
                                           const char *category)
{
    dex_assert(fread(count, sizeof(*count), 1, this->fp) == 1 && \
               fread(offset, sizeof(*offset), 1, this->fp) == 1);
    dex_printf("%s count: %u (0x%.8X), offset 0x%.8X",
               category,
               *count,
               *count,
               *offset);
    
    return;
}

/*
 * ReadTypeList() - Read a list of type_id_t into array
 *
 * Read starts at whereever fp points to (no seek). It is assumed
 * count does not equal to 0
 *
 * Assume all list elements are stored as unsigned short rather than
 * unsigned int. Type count is first read into the pointer
 */
void DalvikExecutable::ReadTypeList(short_type_id_t **list_pp, 
                                    unsigned int *count,
                                    const char *category)
{
    dex_assert(fread(count, sizeof(*count), 1, this->fp) == 1);
    
    *list_pp = new(short_type_id_t[*count]);
    dex_assert(*list_pp);
    
    // Read *count consecutively stored members using one call
    dex_assert(fread(*list_pp, 
                     sizeof(**list_pp), 
                     *count, 
                     this->fp) == *count);
    
    dex_printf("Type list for %s", category);
    for(unsigned int i = 0;i < *count;i++)
    {
        dex_printf("\tType id: %s",
                   this->GetTypeString((type_id_t)(*list_pp)[i]));    
    }
    
    return;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/*
 * ReadBytecodeForMethod() - Read byte code given the method instance
 *
 * Byte code is associated with methods in a 1-to-1 correspondence
 * and are stored in an array inside the method instance
 *
 * We assume all other necessary information has been read into the method
 * instance by previous function calls
 */
void DalvikExecutable::ReadBytecodeForMethod(MethodDef *md_p)
{
    // It is possible for a method to have not code
    // maybe a native or abstract method
    if(md_p->code_offset == 0)
    {
        // This is important - the destructor would check this
        md_p->bytecode = NULL;
        md_p->debug_info_offset = 0;
        md_p->input_word_count = 0;
        md_p->instruction_size_16bit = 0;
        md_p->output_word_count = 0;
        md_p->register_count = 0;
        md_p->try_item_count = 0;
        
        return;    
    }
    
    dex_assert(fseek(this->fp, md_p->code_offset, SEEK_SET) == 0);
    
    // Number of registers used by this method
    dex_assert(fread(&md_p->register_count,
                        sizeof(md_p->register_count),
                     1,
                     this->fp) == 1);
    // Number of words taken by input variables
    dex_assert(fread(&md_p->input_word_count,
                     sizeof(md_p->input_word_count),
                     1,
                     this->fp) == 1);
    // Number of words taken by output variables
    dex_assert(fread(&md_p->output_word_count,
                     sizeof(md_p->output_word_count),
                     1,
                     this->fp) == 1);
    // Number of try items
    dex_assert(fread(&md_p->try_item_count,
                     sizeof(md_p->try_item_count),
                     1,
                     this->fp) == 1);
    // Location of debug information
    dex_assert(fread(&md_p->debug_info_offset,
                     sizeof(md_p->debug_info_offset),
                     1,
                     this->fp) == 1);
                     
    // Number of 16 bit words taken by instruction byte code
    dex_assert(fread(&md_p->instruction_size_16bit,
                     sizeof(md_p->instruction_size_16bit),
                     1,
                     this->fp) == 1);
    
    // Next we allocate space for byte code
    md_p->bytecode = new(unsigned char [md_p->instruction_size_16bit * 2]);
    dex_assert(md_p->bytecode);
    
    // Read byte code into memory
    dex_assert(fread(md_p->bytecode,
                     md_p->instruction_size_16bit * 2,
                     1,
                     this->fp) == 1);
                     
    // If there is try block, and instruction size in 16 bit 
    // is an odd number
    unsigned short padding;
    if(md_p->try_item_count != 0 && (md_p->instruction_size_16bit % 2))
    {
        dex_assert(fread(&padding,
                         sizeof(padding),
                         1,
                         this->fp) == 1);
        dex_assert(padding == 0x0000);    
    }
    
    MethodItem *mi_p = this->method_item_list + md_p->method;
    dex_printf("Bytecode for cls: %s, name: %s, size: %u bytes",
               this->GetTypeString(mi_p->cls),
               this->GetString(mi_p->name),
               md_p->instruction_size_16bit * 2);
    
    //TODO: READ TRY BLOCK (as stated in header file)
    
    return;
}

/*
 * ReadBytecode() - Read bytecode for all classes and all methods
 *
 * This is a wrapper function to traverse all classes and all methods,
 * and read byte code for each method by invoking separate functions
 */
void DalvikExecutable::ReadBytecode()
{
    // Traverse on classes
    for(unsigned int i = 0;
        i < this->classdef_id_count; 
        i++)
    {
        ClassdefItem *ci_p = this->classdef_item_list + i;
        
        // Traverse on direct methods
        // If there is no class data area then the count is 0
        for(unsigned int j = 0;j < ci_p->direct_method_count;j++)
        {
            MethodDef *md_p = ci_p->direct_method_list + j; 
            this->ReadBytecodeForMethod(md_p);
        }
        
        // Traverse on virtual methods
        for(unsigned int j = 0;j < ci_p->virtual_method_count;j++)
        {
            MethodDef *md_p = ci_p->virtual_method_list + j; 
            this->ReadBytecodeForMethod(md_p);
        }
    }
    return;    
}

/*
 * ReadEncodedMEthodTable() - Encoded ULEB128 elements
 *
 * Similar to ReadEncodedFieldTable()
 */
void DalvikExecutable::ReadEncodedMethodTable(MethodDef **md_pp,
                                              unsigned int method_count,
                                              const char *category)
{
    // If there is not any method table just set it to NULL and
    // we will jump that array when traversing
    if(method_count == 0)
    {
        *md_pp = NULL;
        return;
    }
    
    *md_pp = new(MethodDef[method_count]);
    dex_assert(*md_pp);
    
    method_id_t last_index = 0;
    for(unsigned int i = 0;i < method_count;i++)
    {
        MethodDef *md_p = (*md_pp) + i;
        md_p->method = last_index + this->ReadULEB128(false);
        last_index = md_p->method;
        
        md_p->access_flag = this->ReadULEB128(false);
        md_p->code_offset = this->ReadULEB128(false);
        
        dex_printf("Method (%s) rtype: %s, id: %s, cls: %s",
                   category,
                   this->GetTypeString( \
                           this->proto_item_list[this->method_item_list[md_p->method].proto].rtype
                    ),
                   this->GetString(this->method_item_list[md_p->method].name),
                   this->GetTypeString(this->method_item_list[md_p->method].cls));
    }
    
    return;    
}

/*
 * ReadEncodedFieldTable() - Read encoded definitions of fields in a classdef
 *
 * It reads from the current position of this->fp, for field_count 
 * elements. The index in each field def is in differencial
 * encoding which means we need to record the last index
 */
void DalvikExecutable::ReadEncodedFieldTable(FieldDef **fd_pp, 
                                             unsigned int field_count,
                                             const char *category)
{
    // If there is not any static field, then set pointer to NULL and return
    if(field_count == 0)
    {
        *fd_pp = NULL;
        return;    
    }
    
    *fd_pp = new(FieldDef[field_count]);
    dex_assert(*fd_pp);
    
    // In order to decode differencial-encoded index values we
    // use an extra variable to record the last index
    field_id_t last_id = 0;
    for(unsigned int i = 0;i < field_count;i++)
    {
        // This is the current field definition pointer
        FieldDef *fd_p = (*fd_pp) + i;
        
        fd_p->field = this->ReadULEB128(false) + last_id;
        last_id = fd_p->field;
        fd_p->access_flag = this->ReadULEB128(false);
        
        dex_printf("Field (%s) type: %s, id: %s, cls: %s",
                   category,
                   this->GetTypeString(this->field_item_list[fd_p->field].type),
                   this->GetString(this->field_item_list[fd_p->field].name),
                   this->GetTypeString(this->field_item_list[fd_p->field].cls));
    }
    
    return;    
}

/*
 * ReadClassdefTable() - Read class definition table
 *
 * - Static field count
 * - Instance field count
 * - Direct method count
 * - Virtual method count
 */
void DalvikExecutable::ReadClassdefTable()
{
    unsigned int prev_offset;
    
    this->classdef_item_list = new(ClassdefItem[this->classdef_id_count]);
    dex_assert(this->classdef_item_list);
    
    dex_assert(fseek(this->fp, this->classdef_id_offset, SEEK_SET) == 0);
    
    for(unsigned int i = 0;i < this->classdef_id_count;i++)
    {
        ClassdefItem *ci_p = this->classdef_item_list + i;
        
        // Read class type specification
        dex_assert(fread(&ci_p->cls, 
                         sizeof(ci_p->cls), 
                         1, 
                         this->fp) == 1);
        // Read access flag
        dex_assert(fread(&ci_p->access_flag, 
                         sizeof(ci_p->access_flag), 
                         1, 
                         this->fp) == 1);
        // Read super class
        dex_assert(fread(&ci_p->super,
                         sizeof(ci_p->super),
                         1,
                         this->fp) == 1);
        // Offset to interface type_id_t list (could be 0)
        dex_assert(fread(&ci_p->intf_offset,
                         sizeof(ci_p->intf_offset),
                         1,
                         this->fp) == 1);
        // Read source file index (string index)
        dex_assert(fread(&ci_p->source_file,
                         sizeof(ci_p->source_file),
                         1,
                         this->fp) == 1);
        // Read annotation offset (I don;t knwow what is that...)
        dex_assert(fread(&ci_p->annotation_offset,
                         sizeof(ci_p->annotation_offset),
                         1,
                         this->fp) == 1);
        // Read class data offset. We use this value to seek
        dex_assert(fread(&ci_p->cls_data_offset, 
                         sizeof(ci_p->cls_data_offset),
                         1,
                         this->fp) == 1);
        // Read static value's intialization values
        dex_assert(fread(&ci_p->static_value_offset,
                         sizeof(ci_p->static_value_offset),
                         1,
                         this->fp) == 1);
        
        dex_printf("Classdef id: %s, super id: %s",
                   this->GetTypeString(ci_p->cls),
                   this->GetTypeString(ci_p->super)); 
        
        // Now save current position before any optional seeks
        prev_offset = ftell(this->fp);
        
        // There is no interface defined for this class
        if(ci_p->intf_offset == 0)
        {
            ci_p->intf_count = 0;
            ci_p->intf_list = NULL;
            goto read_classdef_data;    
        }
        
        // First seek to where type list is stored
        dex_assert(fseek(this->fp, ci_p->intf_offset, SEEK_SET) == 0)
        // Read interface list into both count and list variables
        // If count is 0 then list is set to NULL
        this->ReadTypeList(&ci_p->intf_list,
                           &ci_p->intf_count,
                           "class");

    read_classdef_data:
        // This class has no associated data
        if(ci_p->cls_data_offset == 0)
        {
            ci_p->direct_method_count = 0;
            ci_p->direct_method_list = NULL;
            ci_p->virtual_method_count = 0;
            ci_p->virtual_method_list = NULL;
            ci_p->static_field_count = 0;
            ci_p->static_field_list = NULL;
            ci_p->instance_field_count = 0;
            ci_p->instance_field_list = NULL;
            
            goto read_classdef_restore_offset;
        }
        
        dex_assert(fseek(this->fp, ci_p->cls_data_offset, SEEK_SET) == 0);
        
        ci_p->static_field_count = this->ReadULEB128(false);
        ci_p->instance_field_count = this->ReadULEB128(false);
        ci_p->direct_method_count = this->ReadULEB128(false);
        ci_p->virtual_method_count = this->ReadULEB128(false);
        
        this->ReadEncodedFieldTable(&ci_p->static_field_list, 
                                ci_p->static_field_count,
                                "static");
        this->ReadEncodedFieldTable(&ci_p->instance_field_list,
                                ci_p->instance_field_count,
                                "instance");
        this->ReadEncodedMethodTable(&ci_p->direct_method_list,
                                      ci_p->direct_method_count,
                                     "direct");
        this->ReadEncodedMethodTable(&ci_p->virtual_method_list,
                                     ci_p->virtual_method_count,
                                     "virtual");
                                     
    // No matter what happens we always restore the offset
    read_classdef_restore_offset:
        // Seek back to the place where we have finished reading
        // the classdef body
        dex_assert(fseek(this->fp, prev_offset, SEEK_SET) == 0);
    }
    
    return;
}

/*
 * ReadMethodIdTable() - Read method descriptors
 *
 * - Method class type (short)
 * - Method prototype (short)
 * - Method identifier
 */
void DalvikExecutable::ReadMethodIdTable()
{
    this->method_item_list = new(MethodItem[this->method_id_count]);
    dex_assert(this->method_item_list);
    
    dex_assert(fseek(this->fp, this->method_id_offset, SEEK_SET) == 0);
    
    for(unsigned int i = 0;i < this->method_id_count;i++)
    {
        MethodItem *mi_p = this->method_item_list + i;
        
        // First two are 16 bit short
        dex_assert(fread(&mi_p->cls, sizeof(mi_p->cls), 1, this->fp) == 1);    
        dex_assert(fread(&mi_p->proto, sizeof(mi_p->proto), 1, this->fp) == 1);
        // The last one is 32 bit unsigned int
        dex_assert(fread(&mi_p->name, sizeof(mi_p->name), 1, this->fp) == 1);
        
        dex_printf("%u Method: %s, Class: %s ProtoType: %s",
                   i,
                   this->GetString(mi_p->name),
                   this->GetTypeString(mi_p->cls),
                   this->GetString(
                           this->proto_item_list[mi_p->proto].description
                    ));
    }
    
    return;
}

/*
 * ReadFieldIdTable() - Read field descriptors
 *
 * - Field class type (short)
 * - Field type (short)
 * - Field identifier
 */
void DalvikExecutable::ReadFieldIdTable()
{
    this->field_item_list = new(FieldItem[this->field_id_count]);
    dex_assert(this->field_item_list);
    
    dex_assert(fseek(this->fp, this->field_id_offset, SEEK_SET) == 0);
    
    for(unsigned int i = 0;i < this->field_id_count;i++)
    {
        FieldItem *fi_p = this->field_item_list + i;
        
        // First two are 16 bit short
        dex_assert(fread(&fi_p->cls, sizeof(fi_p->cls), 1, this->fp) == 1);    
        dex_assert(fread(&fi_p->type, sizeof(fi_p->type), 1, this->fp) == 1);
        // The last one is 32 bit unsigned int
        dex_assert(fread(&fi_p->name, sizeof(fi_p->name), 1, this->fp) == 1);
        
        dex_printf("%u Field: %s, Class: %s Type: %s",
                   i,
                   this->GetString(fi_p->name),
                   this->GetTypeString(fi_p->cls),
                   this->GetTypeString(fi_p->type));
    }
    
    return;
}

/*
 * ReadProtoIdTable() - Read proto information
 *
 * Proto item are stored as three consecutive unsigned integers
 * 1. Prototype body (a short description)
 * 2. Return type
 * 3. An offset to parameter list
 *
 * We unpack this structure for 3, and stores directly the list of parameter
 * into ProtoItem instances. 
 *
 * For faster I/O, we first read offset into a temporary place, and only
 * start building param list after all ProtoItems have been read. This brings
 * a more regular I/O pattern which also benefits from disk caching, etc.
 */
void DalvikExecutable::ReadProtoIdTable()
{
    // Use this buffer to hold 
    unsigned int *offset_list = new(unsigned int[this->proto_id_count]);
    dex_assert(offset_list);
    
    this->proto_item_list = new(ProtoItem[this->proto_id_count]);
    dex_assert(proto_item_list);
    
    dex_assert(fseek(this->fp, this->proto_id_offset, SEEK_SET) == 0);
    
    for(unsigned int i = 0;
        i < this->proto_id_count;
        i++)
    {
        ProtoItem *pi_p = this->proto_item_list + i;
        
        dex_assert(fread(&pi_p->description,
                           sizeof(pi_p->description),
                           1,
                           this->fp) == 1);
        dex_assert(fread(&pi_p->rtype,
                         sizeof(pi_p->rtype),
                         1,
                         this->fp) == 1);
                         
        // NOTE: This is read into offset list
        dex_assert(fread(offset_list + i,
                         sizeof(*offset_list),
                         1,
                         this->fp) == 1);
    }
    
    for(unsigned int i = 0;
        i < this->proto_id_count;
        i++)
    {
        ProtoItem *pi_p = this->proto_item_list + i;
        
        // If the offset to paramter list is 0 then there
        // is no parameters, and we simply set them to 0
        if(offset_list[i] == 0)
        {
            pi_p->param_count = 0;
            pi_p->param_type_list = NULL;
            
            goto after_read_param_list;
        }
        
        // First seek to the location where type list is stored
        dex_assert(fseek(this->fp, offset_list[i], SEEK_SET) == 0);
        // Then read type list
        this->ReadTypeList(&pi_p->param_type_list, 
                           &pi_p->param_count,
                           "prototype");
                         
after_read_param_list:
        dex_printf("%u %s %s",
                   i, 
                   this->GetTypeString(pi_p->rtype),
                   this->GetString(pi_p->description));
    }
    
    delete[] offset_list;
    return;    
}

/*
 * ReadTypeIdTable() - Read an array of indexed into the string table
 * as type ID table
 *
 * All indexed are read into memory in one large fread() call
 * memory is allocated as a big array and should be deallocated on
 * destruction
 */
void DalvikExecutable::ReadTypeIdTable()
{
    this->type_item_list = new(type_id_t[this->type_id_count]);
    dex_assert(this->type_item_list);
    
    dex_assert(fseek(this->fp, this->type_id_offset, SEEK_SET) == 0);
    
    dex_assert(fread(this->type_item_list, 
                     sizeof(*this->type_item_list), 
                     this->type_id_count,
                     this->fp) == this->type_id_count);
    
    for(unsigned int i = 0;i < this->type_id_count;i++)
    {
        dex_assert(this->string_item_list[this->type_item_list[i]].is_ascii);
        dex_printf("%u Type: %s",
                   i,
                   this->GetTypeString(i));
    }
                     
    return;
}

/*
 * ReadStringIdTable() - Read string ID table into the object
 *
 * String ID table is specified by this->string_id_offset which is
 * an array of 32 bit offset values which points to the actual string item
 *
 * String ID index is implied by its location relative to the start of
 * the table.
 *
 * If the string is not a pure ASCII string (i.e. UTF-8 encoded) then we
 * cannot process it (to be improved later), and just mark the structure 
 * as is_ascii = false
 */
void DalvikExecutable::ReadStringIdTable()
{
    // First seek to the start of the table
    dex_assert(fseek(this->fp, this->string_id_offset, SEEK_SET) == 0);
    
    unsigned int *item_offset_array = \
        new(unsigned int [this->string_id_count]);
    dex_assert(item_offset_array);
    
    this->string_item_list = new(StringItem [this->string_id_count]);
    dex_assert(this->string_item_list);
    
    // Read the entire array in one function call
    dex_assert(fread(item_offset_array,
                     sizeof(*item_offset_array), 
                     this->string_id_count, 
                     this->fp) == this->string_id_count);
    
    // Variable i is also the index of the string
    for(unsigned int i = 0;i < this->string_id_count;i++)
    {
        unsigned int item_offset = item_offset_array[i];
        dex_assert(fseek(this->fp, item_offset, SEEK_SET) == 0);
        
        // Read the size which is encoded as ULEB128 without sign extension
        // Variable len is the number of UTF8 "unit" which may not be the
        // number of ASCII codes, also not including the trailing 0
        unsigned int len = this->ReadULEB128(false);
        
        StringItem *si_p = this->string_item_list + i;
        
        si_p->data = new(char[len + 1]);
        si_p->is_ascii = true;
        
        dex_assert(fread(si_p->data, len + 1, 1, this->fp) == 1);
        
        // If not a valid ASCII string then ignore
        // because we did not read the whole content
        char *string_data = si_p->data;
        for(int j = 0;j < len;j++)
        {
            if((unsigned int)string_data[j] >= (unsigned int)0x80)
            {
                si_p->is_ascii = false;
            }
        }
        
        dex_printf("%u String len: %u, content: [%s]",
                   i,
                   len,
                   si_p->is_ascii ? si_p->data : "*NOT ASCII*");
        
        // Make sure it consists of ASCII character
        // If not then we are reading a substring which could not have
        // trailing '\0'.
        //dex_assert(si_p->data[len] == '\0');
    }
    
    delete[] item_offset_array;
}

/*
 * ReadCountOffsets() - Read remaining part of DEX header
 *
 * - String ID
 * - Type ID
 * - Proto ID
 * - Field ID
 * - Method ID
 * - Classdef ID
 * - Data area
 */
void DalvikExecutable::ReadCountOffsets()
{
    // First seek to the position where we start reading 
    // all count/offset pairs
    dex_assert(fseek(this->fp, this->after_file_map_offset, SEEK_SET) == 0);
    
    this->ReadCountOffsetPair(&this->string_id_count,
                              &this->string_id_offset,
                              "String id");
                              
    this->ReadCountOffsetPair(&this->type_id_count,
                              &this->type_id_offset,
                              "Type id");
                              
    this->ReadCountOffsetPair(&this->proto_id_count,
                              &this->proto_id_offset,
                              "Proto id");
                              
    this->ReadCountOffsetPair(&this->field_id_count,
                              &this->field_id_offset,
                              "Field id");
                              
    this->ReadCountOffsetPair(&this->method_id_count,
                              &this->method_id_offset,
                              "Method id");
                              
    this->ReadCountOffsetPair(&this->classdef_id_count,
                              &this->classdef_id_offset,
                              "Classdef id");
                              
    this->ReadCountOffsetPair(&this->data_size,
                              &this->data_offset,
                              "Data bytes");
    
    return;
}

/*
 * ReadFileMapPtrAndItem() - Read file map offset ptr first, and 
 * then read all map items
 */
void DalvikExecutable::ReadFileMapPtrAndItem()
{
    int count;
    unsigned int prev_offset;
    
    // First seek to the place where first byte of map ptr is stored
    dex_assert(fseek(this->fp, this->after_padding_offset, SEEK_SET) == 0);
    count = fread(&this->file_map_offset, 
                    sizeof(this->file_map_offset), 
                    1, 
                    this->fp);
    dex_assert(count == 1);
    dex_printf("File map offset: %.8X", this->file_map_offset);
    
    // Save previous offset value. We will restore that later before return
    prev_offset = ftell(this->fp);
    dex_assert(prev_offset != -1);
    // This is actually the starting point of later reading process
    this->after_file_map_offset = prev_offset;
    dex_assert(fseek(this->fp, this->file_map_offset, SEEK_SET) == 0);
    
    int item_count;
    count = fread(&item_count, sizeof(item_count), 1, this->fp);
    dex_assert(count == 1);
    dex_printf("File map item count: %d", item_count);
    
    for(int i = 0;i < item_count;i++)
    {
        unsigned int item_count_and_offset[2];
        unsigned short type_and_unused[2];
        
        // Type 2bytes, unused 2 bytes
        count = fread(type_and_unused, sizeof(short), 2, this->fp);
        dex_assert(count == 2 && type_and_unused[1] == 0);
        
        count = fread(item_count_and_offset, 
                      sizeof(unsigned int), 
                      2,  
                      this->fp);
        dex_assert(count == 2); 
        
        dex_printf("\tItem type: 0x%X, count: %u, offset 0x%.8X",
                   (unsigned int)type_and_unused[0],
                   item_count_and_offset[0],
                   item_count_and_offset[1]);
        
        this->file_map[(file_map_item_type_t)type_and_unused[0]] = \
            FileMapItem(type_and_unused[0],
                        item_count_and_offset[0],
                        item_count_and_offset[1]);
    }
    
    // Return to where we need to proceed next
    dex_assert(fseek(this->fp, prev_offset, SEEK_SET) == 0);
    return;
}

/*
 * VerifyDexHeader() - Check metadata correctness in file header
 *
 * - Signature on first 8 bytes
 * - Full size (claimed size vs actual size)
 * - Header size (Should be a constant)
 * - Endian indicator
 * - Padding bits
 */
void DalvikExecutable::VerifyDexHeader()
{
    char signature[8];
    int count, file_len;
    
    // Move to file end, check length, and then move back
    dex_assert(fseek(this->fp, 0, SEEK_END) == 0);
    file_len = ftell(this->fp);
    dex_assert(file_len != -1);
    dex_assert(fseek(this->fp, 0, SEEK_SET) == 0);
    
    count = fread(signature, sizeof(signature), 1, this->fp);    
    dex_assert(count == 1);
    
    // Prevent stack overflow
    signature[sizeof(signature) - 1] = '\0';
    
    dex_debug(strcmp(signature, DalvikExecutable::valid_signature) == 0,
              "Invalid signature: %s",
              signature);
    dex_printf("Signature: OK (%s)", "dex\\n035\'\\0\'");
    
    // Jump over checksum and SHA-1, 24 bytes in total
    count = fseek(this->fp, 24, SEEK_CUR);
    dex_assert(count == 0);
    
    int claimed_len;
    count = fread(&claimed_len, sizeof(claimed_len), 1, this->fp);
    dex_assert(count == 1);
    dex_debug(claimed_len == file_len,
              "File len %d and claimed len %d not equal",
              file_len, 
              claimed_len);
    dex_printf("Claimed file size: %d", claimed_len);
    
    unsigned int header_size;
    count = fread(&header_size, sizeof(header_size), 1, this->fp);
    dex_assert(count == 1);
    dex_debug(header_size == DalvikExecutable::valid_header_size,
              "Header size invalid: %u, should be %u",
              header_size,
              DalvikExecutable::valid_header_size);
    dex_printf("Header size: %u", header_size);
              
    unsigned int endian_indicator;
    count = fread(&endian_indicator, sizeof(endian_indicator), 1, this->fp);
    dex_assert(count == 1);
    dex_debug(endian_indicator == DalvikExecutable::valid_endian_indicator,
               "Endian indicator invalid: %u, should be %u",
               endian_indicator,
               DalvikExecutable::valid_endian_indicator);
    dex_printf("Endian indicator: 0x%.8X", endian_indicator);
    
    unsigned int padding1, padding2;
    count = fread(&padding1, sizeof(padding1), 1, this->fp);
    dex_assert(count == 1);
    count = fread(&padding2, sizeof(padding2), 1, this->fp);
    dex_assert(count == 1);
    dex_debug((padding1 | padding2) == 0x00000000U,
              "Padding not zero: %.8X%.8X",
              padding2, 
              padding1);
    dex_printf("Padding: 0x%.8X%.8X", padding2, padding1);
    
    // Record the first byte of FILE MAP PTR
    this->after_padding_offset = ftell(this->fp);
    dex_assert(this->after_padding_offset != -1);
    
    return;
}

int main()
{
    DalvikExecutable de = DalvikExecutable("classes.dex");
    de.VerifyDexHeader();
    de.ReadFileMapPtrAndItem();
    de.ReadCountOffsets();
    de.ReadStringIdTable();
    de.ReadTypeIdTable();
    de.ReadProtoIdTable();
    de.ReadFieldIdTable();
    de.ReadMethodIdTable();
    de.ReadClassdefTable();
    de.ReadBytecode();
    
    FILE *out_file = fopen("dump.txt", "wb");
    dex_assert(out_file);
    
    int counter = 0;
    for(unsigned int i = 0;
        i < de.classdef_id_count; 
        i++)
    {
        ClassdefItem *ci_p = de.classdef_item_list + i;
        
        // Traverse on direct methods
        // If there is no class data area then the count is 0
        for(unsigned int j = 0;j < ci_p->direct_method_count;j++)
        {
            MethodDef *md_p = ci_p->direct_method_list + j; 
            if(md_p->instruction_size_16bit == 0) continue;
            BytecodeSegment bs(md_p->bytecode,
                               md_p->instruction_size_16bit * 2,
                               out_file,
                               stderr);
            bs.Dispatch();
            counter++;
        }
        
        // Traverse on virtual methods
        for(unsigned int j = 0;j < ci_p->virtual_method_count;j++)
        {
            MethodDef *md_p = ci_p->virtual_method_list + j; 
            if(md_p->instruction_size_16bit == 0) continue;
            BytecodeSegment bs(md_p->bytecode,
                               md_p->instruction_size_16bit * 2,
                               out_file,
                               stderr);
            bs.Dispatch();
            counter++;
        }
    }
    
    printf("Finished counter = %d\n", counter);
    
    return 0;
}
