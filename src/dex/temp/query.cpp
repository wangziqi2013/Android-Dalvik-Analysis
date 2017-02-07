
#include "query.h"

/*
 * Constructor - Initialize the query object
 */
DexQuery::DexQuery(DalvikExecutable *pde_p)
{
    // Major data object
    this->de_p = pde_p;
    
    return;
}


///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

/*
 * GetString() - Return a read-only string pointer given a string index
 *
 * Dependng on encoding strategy of the DEX file, string returned by
 * this method might or might not be an ASCII string. But no matter which
 * encoding is used, the last byte of the string is always a '\0'
 */
const char *DexQuery::GetString(unsigned int index)
{
    return this->de_p->string_item_list[index].data;
}

/*
 * GetTypeString() - Return a string representing the type
 *
 * Type is actually another level of indirection on string index
 * type itself it encoded in the string, but there is no standalone
 * type struct
 */
const char *DexQuery::GetTypeString(unsigned int index)
{
    // Type is an alias to string
    return this->GetString(this->de_p->type_item_list[index]);   
}
