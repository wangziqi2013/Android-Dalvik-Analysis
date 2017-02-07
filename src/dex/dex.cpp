
#include "dex.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;

const char DexFile::FILE_SIGNATURE_037[8] = "dex\n037";
const char DexFile::FILE_SIGNATURE_035[8] = "dex\n035";

const char *DexFile::FILE_SIGNATURE_LIST[2] = {
  DexFile::FILE_SIGNATURE_035,
  DexFile::FILE_SIGNATURE_037,
};
