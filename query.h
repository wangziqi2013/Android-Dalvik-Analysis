
#ifndef _QUERY_H
#define _QUERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include "dex.h"

using namespace std;

struct ApkQuery
{
    DalvikExecutable *de_p;
    
    ApkQuery(DalvikExecutable *pde_p);
};

#endif
