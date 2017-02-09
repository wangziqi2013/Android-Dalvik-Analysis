
//
// This file is auto-generated using Python script inst_format.py and
// HTML documentation ./html/inst_format.html, in order to automate coding process.
//
// Target file: ./inst_format.cpp
//
    
#include "common.h"
#include "inst_format.h"

using namespace wangziqi2013;
using namespace android_dalvik_analysis;
using namespace dex;

/*
 * AA_op_BBBBBBBBBBBBBBBB() - Parses using format AA op BBBBBBBBBBBBBBBB
 */
inline uint8_t *AA_op_BBBBBBBBBBBBBBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->B = EncodingUtility::Get64Bit(data_p);
  data_p += 8;

  return data_p;
}

/*
 * xx_op() - Parses using format ?? op
 */
inline uint8_t *xx_op(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  data_p++;

  return data_p;
}

/*
 * B_A_op_CCCC() - Parses using format B A op CCCC
 */
inline uint8_t *B_A_op_CCCC(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::GetLow4Bit(data_p);
  arg_pack_p->B = EncodingUtility::GetHigh4Bit(data_p);
  data_p++;
  arg_pack_p->C = EncodingUtility::Get16Bit(data_p);
  data_p += 2;

  return data_p;
}

/*
 * AA_op_BBBBBBBB() - Parses using format AA op BBBBBBBB
 */
inline uint8_t *AA_op_BBBBBBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->B = EncodingUtility::Get32Bit(data_p);
  data_p += 4;

  return data_p;
}

/*
 * AA_op() - Parses using format AA op
 */
inline uint8_t *AA_op(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get8Bit(data_p);
  data_p++;

  return data_p;
}

/*
 * AA_op_CC_BB() - Parses using format AA op CC BB
 */
inline uint8_t *AA_op_CC_BB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->B = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->C = EncodingUtility::Get8Bit(data_p);
  data_p++;

  return data_p;
}

/*
 * xx_op_AAAAAAAA() - Parses using format ?? op AAAAAAAA
 */
inline uint8_t *xx_op_AAAAAAAA(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  data_p++;
  arg_pack_p->A = EncodingUtility::Get32Bit(data_p);
  data_p += 4;

  return data_p;
}

/*
 * AA_op_BBBB_CCCC() - Parses using format AA op BBBB CCCC
 */
inline uint8_t *AA_op_BBBB_CCCC(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->B = EncodingUtility::Get16Bit(data_p);
  data_p += 2;
  arg_pack_p->C = EncodingUtility::Get16Bit(data_p);
  data_p += 2;

  return data_p;
}

/*
 * B_A_op() - Parses using format B A op
 */
inline uint8_t *B_A_op(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::GetLow4Bit(data_p);
  arg_pack_p->B = EncodingUtility::GetHigh4Bit(data_p);
  data_p++;

  return data_p;
}

/*
 * xx_op_AAAA() - Parses using format ?? op AAAA
 */
inline uint8_t *xx_op_AAAA(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  data_p++;
  arg_pack_p->A = EncodingUtility::Get16Bit(data_p);
  data_p += 2;

  return data_p;
}

/*
 * xx_op_AAAA_BBBB() - Parses using format ?? op AAAA BBBB
 */
inline uint8_t *xx_op_AAAA_BBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  data_p++;
  arg_pack_p->A = EncodingUtility::Get16Bit(data_p);
  data_p += 2;
  arg_pack_p->B = EncodingUtility::Get16Bit(data_p);
  data_p += 2;

  return data_p;
}

/*
 * A_G_op_BBBB_F_E_D_C() - Parses using format A G op BBBB F E D C
 */
inline uint8_t *A_G_op_BBBB_F_E_D_C(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->G = EncodingUtility::GetLow4Bit(data_p);
  arg_pack_p->A = EncodingUtility::GetHigh4Bit(data_p);
  data_p++;
  arg_pack_p->B = EncodingUtility::Get16Bit(data_p);
  data_p += 2;
  arg_pack_p->C = EncodingUtility::GetLow4Bit(data_p);
  arg_pack_p->D = EncodingUtility::GetHigh4Bit(data_p);
  data_p++;
  arg_pack_p->E = EncodingUtility::GetLow4Bit(data_p);
  arg_pack_p->F = EncodingUtility::GetHigh4Bit(data_p);
  data_p++;

  return data_p;
}

/*
 * AA_op_BBBB() - Parses using format AA op BBBB
 */
inline uint8_t *AA_op_BBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->B = EncodingUtility::Get16Bit(data_p);
  data_p += 2;

  return data_p;
}


// This is the jump table that defines parsing functions
uint8_t *(*op_jump_table[])(ArgumentPack *, uint8_t *) = {
  [InstFormat::i00x] = nullptr,
  [InstFormat::i10x] = xx_op,
  [InstFormat::i12x] = B_A_op,
  [InstFormat::i11n] = B_A_op,
  [InstFormat::i11x] = AA_op,
  [InstFormat::i10t] = AA_op,
  [InstFormat::i20t] = xx_op_AAAA,
  [InstFormat::i20bc] = AA_op_BBBB,
  [InstFormat::i22x] = AA_op_BBBB,
  [InstFormat::i21t] = AA_op_BBBB,
  [InstFormat::i21s] = AA_op_BBBB,
  [InstFormat::i21h] = AA_op_BBBB,
  [InstFormat::i21c] = AA_op_BBBB,
  [InstFormat::i23x] = AA_op_CC_BB,
  [InstFormat::i22b] = AA_op_CC_BB,
  [InstFormat::i22t] = B_A_op_CCCC,
  [InstFormat::i22s] = B_A_op_CCCC,
  [InstFormat::i22c] = B_A_op_CCCC,
  [InstFormat::i22cs] = B_A_op_CCCC,
  [InstFormat::i30t] = xx_op_AAAAAAAA,
  [InstFormat::i32x] = xx_op_AAAA_BBBB,
  [InstFormat::i31i] = AA_op_BBBBBBBB,
  [InstFormat::i31t] = AA_op_BBBBBBBB,
  [InstFormat::i31c] = AA_op_BBBBBBBB,
  [InstFormat::i35c] = A_G_op_BBBB_F_E_D_C,
  [InstFormat::i35ms] = A_G_op_BBBB_F_E_D_C,
  [InstFormat::i35mi] = A_G_op_BBBB_F_E_D_C,
  [InstFormat::i3rc] = AA_op_BBBB_CCCC,
  [InstFormat::i3rms] = AA_op_BBBB_CCCC,
  [InstFormat::i3rmi] = AA_op_BBBB_CCCC,
  [InstFormat::i51l] = AA_op_BBBBBBBBBBBBBBBB,
};
