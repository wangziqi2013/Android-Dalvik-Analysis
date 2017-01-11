
/*
 * test_suite.cpp - Test helper function
 *
 * This file is always compiled under debug mode to simplify error handling 
 * and some other tasks. So do not put performance critical function here
 * and instead put those function that will only be run by a limited time,
 * i.e. set up and tear down functions
 */

#include "test_suite.h"

/*
 * PrintTestName() - As name suggests 
 */
void PrintTestName(const char *name) {
  dbg_printf("=\n");
  dbg_printf("========== %s ==========\n", name);
  dbg_printf("=\n");
  
  return;
}
