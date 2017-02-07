
#ifndef _QUERY_H
#define _QUERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include "dex.h"

using namespace std;

/*
 * struct DexIterator - Template class for iterator
 *
 * Defines basic functionality that an iterator must follow:
 * - operator overloading for it++, ++it, *it
 * - Method Next() and Get()
 */
template<class T1, class T2> struct DexIterator
{
    // Single step to next
    virtual void Next() = 0;
    virtual T2 Get() = 0;
    // it++ - need to save the instance first
    virtual T1 operator++(int) = 0;
    // ++it
    virtual T1 &operator++() = 0;
    // *it
    virtual T2 operator*() = 0;
};

/*
 * struct StringIterator - Defines interface for interating through 
 * dex strings
 *
 * The only context we need to maintain the current index in the 
 * string table
 */
struct StringIterator : public DexIterator<StringIterator, const char *>
{
    unsigned int current_index;
    unsigned int end_index;
    
    virtual const char *Get();
    virtual void Next();
    virtual StringIterator operator++(int);
    virtual StringIterator &operator++();
    virtual const char *operator*();
};

/*
 * struct DexQuery - A query object for a specific dex object
 */
struct DexQuery
{
    DalvikExecutable *de_p;
    
    DexQuery(DalvikExecutable *pde_p);
    const char *GetString(unsigned int index);
    const char *GetTypeString(unsigned int index);
};

#endif
