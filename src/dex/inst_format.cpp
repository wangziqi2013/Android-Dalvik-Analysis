
//
// This file is auto-generated using Python script inst_format.py and
// HTML documentation ./html/inst_format.html and ./html/bytecode.html, in order to automate coding process.
//
// Target file: ./inst_format.cpp
//
    
#include "common.h"
#include "inst_format.h"

namespace wangziqi2013 {
namespace android_dalvik_analysis {
namespace dex {

/*
 * AA_op_BBBBBBBBBBBBBBBB() - Parses using format AA op BBBBBBBBBBBBBBBB
 */
uint8_t *AA_op_BBBBBBBBBBBBBBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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
uint8_t *xx_op(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->xx = EncodingUtility::Get8Bit(data_p);
  data_p++;

  return data_p;
}

/*
 * B_A_op_CCCC() - Parses using format B A op CCCC
 */
uint8_t *B_A_op_CCCC(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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
uint8_t *AA_op_BBBBBBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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
uint8_t *AA_op(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get8Bit(data_p);
  data_p++;

  return data_p;
}

/*
 * AA_op_CC_BB() - Parses using format AA op CC BB
 */
uint8_t *AA_op_CC_BB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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
uint8_t *xx_op_AAAAAAAA(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->xx = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get32Bit(data_p);
  data_p += 4;

  return data_p;
}

/*
 * AA_op_BBBB_CCCC() - Parses using format AA op BBBB CCCC
 */
uint8_t *AA_op_BBBB_CCCC(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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
uint8_t *B_A_op(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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
uint8_t *xx_op_AAAA(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->xx = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->A = EncodingUtility::Get16Bit(data_p);
  data_p += 2;

  return data_p;
}

/*
 * xx_op_AAAA_BBBB() - Parses using format ?? op AAAA BBBB
 */
uint8_t *xx_op_AAAA_BBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
  arg_pack_p->op = EncodingUtility::Get8Bit(data_p);
  data_p++;
  arg_pack_p->xx = EncodingUtility::Get8Bit(data_p);
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
uint8_t *A_G_op_BBBB_F_E_D_C(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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
uint8_t *AA_op_BBBB(ArgumentPack *arg_pack_p, uint8_t *data_p) {
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

// Byte code value to format identifier mapping
InstFormat bytecode_type_list[] = {
  [0] = i10x, /* 0x0 */
  [1] = i12x, /* 0x1 */
  [2] = i22x, /* 0x2 */
  [3] = i32x, /* 0x3 */
  [4] = i12x, /* 0x4 */
  [5] = i22x, /* 0x5 */
  [6] = i32x, /* 0x6 */
  [7] = i12x, /* 0x7 */
  [8] = i22x, /* 0x8 */
  [9] = i32x, /* 0x9 */
  [10] = i11x, /* 0xa */
  [11] = i11x, /* 0xb */
  [12] = i11x, /* 0xc */
  [13] = i11x, /* 0xd */
  [14] = i10x, /* 0xe */
  [15] = i11x, /* 0xf */
  [16] = i11x, /* 0x10 */
  [17] = i11x, /* 0x11 */
  [18] = i11n, /* 0x12 */
  [19] = i21s, /* 0x13 */
  [20] = i31i, /* 0x14 */
  [21] = i21h, /* 0x15 */
  [22] = i21s, /* 0x16 */
  [23] = i31i, /* 0x17 */
  [24] = i51l, /* 0x18 */
  [25] = i21h, /* 0x19 */
  [26] = i21c, /* 0x1a */
  [27] = i31c, /* 0x1b */
  [28] = i21c, /* 0x1c */
  [29] = i11x, /* 0x1d */
  [30] = i11x, /* 0x1e */
  [31] = i21c, /* 0x1f */
  [32] = i22c, /* 0x20 */
  [33] = i12x, /* 0x21 */
  [34] = i21c, /* 0x22 */
  [35] = i22c, /* 0x23 */
  [36] = i35c, /* 0x24 */
  [37] = i3rc, /* 0x25 */
  [38] = i31t, /* 0x26 */
  [39] = i11x, /* 0x27 */
  [40] = i10t, /* 0x28 */
  [41] = i20t, /* 0x29 */
  [42] = i30t, /* 0x2a */
  [43] = i31t, /* 0x2b */
  [44] = i31t, /* 0x2c */
  [45] = i23x, /* 0x2d */
  [46] = i23x, /* 0x2e */
  [47] = i23x, /* 0x2f */
  [48] = i23x, /* 0x30 */
  [49] = i23x, /* 0x31 */
  [50] = i22t, /* 0x32 */
  [51] = i22t, /* 0x33 */
  [52] = i22t, /* 0x34 */
  [53] = i22t, /* 0x35 */
  [54] = i22t, /* 0x36 */
  [55] = i22t, /* 0x37 */
  [56] = i21t, /* 0x38 */
  [57] = i21t, /* 0x39 */
  [58] = i21t, /* 0x3a */
  [59] = i21t, /* 0x3b */
  [60] = i21t, /* 0x3c */
  [61] = i21t, /* 0x3d */
  [62] = i10x, /* 0x3e */
  [63] = i10x, /* 0x3f */
  [64] = i10x, /* 0x40 */
  [65] = i10x, /* 0x41 */
  [66] = i10x, /* 0x42 */
  [67] = i10x, /* 0x43 */
  [68] = i23x, /* 0x44 */
  [69] = i23x, /* 0x45 */
  [70] = i23x, /* 0x46 */
  [71] = i23x, /* 0x47 */
  [72] = i23x, /* 0x48 */
  [73] = i23x, /* 0x49 */
  [74] = i23x, /* 0x4a */
  [75] = i23x, /* 0x4b */
  [76] = i23x, /* 0x4c */
  [77] = i23x, /* 0x4d */
  [78] = i23x, /* 0x4e */
  [79] = i23x, /* 0x4f */
  [80] = i23x, /* 0x50 */
  [81] = i23x, /* 0x51 */
  [82] = i22c, /* 0x52 */
  [83] = i22c, /* 0x53 */
  [84] = i22c, /* 0x54 */
  [85] = i22c, /* 0x55 */
  [86] = i22c, /* 0x56 */
  [87] = i22c, /* 0x57 */
  [88] = i22c, /* 0x58 */
  [89] = i22c, /* 0x59 */
  [90] = i22c, /* 0x5a */
  [91] = i22c, /* 0x5b */
  [92] = i22c, /* 0x5c */
  [93] = i22c, /* 0x5d */
  [94] = i22c, /* 0x5e */
  [95] = i22c, /* 0x5f */
  [96] = i21c, /* 0x60 */
  [97] = i21c, /* 0x61 */
  [98] = i21c, /* 0x62 */
  [99] = i21c, /* 0x63 */
  [100] = i21c, /* 0x64 */
  [101] = i21c, /* 0x65 */
  [102] = i21c, /* 0x66 */
  [103] = i21c, /* 0x67 */
  [104] = i21c, /* 0x68 */
  [105] = i21c, /* 0x69 */
  [106] = i21c, /* 0x6a */
  [107] = i21c, /* 0x6b */
  [108] = i21c, /* 0x6c */
  [109] = i21c, /* 0x6d */
  [110] = i35c, /* 0x6e */
  [111] = i35c, /* 0x6f */
  [112] = i35c, /* 0x70 */
  [113] = i35c, /* 0x71 */
  [114] = i35c, /* 0x72 */
  [115] = i10x, /* 0x73 */
  [116] = i3rc, /* 0x74 */
  [117] = i3rc, /* 0x75 */
  [118] = i3rc, /* 0x76 */
  [119] = i3rc, /* 0x77 */
  [120] = i3rc, /* 0x78 */
  [121] = i10x, /* 0x79 */
  [122] = i10x, /* 0x7a */
  [123] = i12x, /* 0x7b */
  [124] = i12x, /* 0x7c */
  [125] = i12x, /* 0x7d */
  [126] = i12x, /* 0x7e */
  [127] = i12x, /* 0x7f */
  [128] = i12x, /* 0x80 */
  [129] = i12x, /* 0x81 */
  [130] = i12x, /* 0x82 */
  [131] = i12x, /* 0x83 */
  [132] = i12x, /* 0x84 */
  [133] = i12x, /* 0x85 */
  [134] = i12x, /* 0x86 */
  [135] = i12x, /* 0x87 */
  [136] = i12x, /* 0x88 */
  [137] = i12x, /* 0x89 */
  [138] = i12x, /* 0x8a */
  [139] = i12x, /* 0x8b */
  [140] = i12x, /* 0x8c */
  [141] = i12x, /* 0x8d */
  [142] = i12x, /* 0x8e */
  [143] = i12x, /* 0x8f */
  [144] = i23x, /* 0x90 */
  [145] = i23x, /* 0x91 */
  [146] = i23x, /* 0x92 */
  [147] = i23x, /* 0x93 */
  [148] = i23x, /* 0x94 */
  [149] = i23x, /* 0x95 */
  [150] = i23x, /* 0x96 */
  [151] = i23x, /* 0x97 */
  [152] = i23x, /* 0x98 */
  [153] = i23x, /* 0x99 */
  [154] = i23x, /* 0x9a */
  [155] = i23x, /* 0x9b */
  [156] = i23x, /* 0x9c */
  [157] = i23x, /* 0x9d */
  [158] = i23x, /* 0x9e */
  [159] = i23x, /* 0x9f */
  [160] = i23x, /* 0xa0 */
  [161] = i23x, /* 0xa1 */
  [162] = i23x, /* 0xa2 */
  [163] = i23x, /* 0xa3 */
  [164] = i23x, /* 0xa4 */
  [165] = i23x, /* 0xa5 */
  [166] = i23x, /* 0xa6 */
  [167] = i23x, /* 0xa7 */
  [168] = i23x, /* 0xa8 */
  [169] = i23x, /* 0xa9 */
  [170] = i23x, /* 0xaa */
  [171] = i23x, /* 0xab */
  [172] = i23x, /* 0xac */
  [173] = i23x, /* 0xad */
  [174] = i23x, /* 0xae */
  [175] = i23x, /* 0xaf */
  [176] = i12x, /* 0xb0 */
  [177] = i12x, /* 0xb1 */
  [178] = i12x, /* 0xb2 */
  [179] = i12x, /* 0xb3 */
  [180] = i12x, /* 0xb4 */
  [181] = i12x, /* 0xb5 */
  [182] = i12x, /* 0xb6 */
  [183] = i12x, /* 0xb7 */
  [184] = i12x, /* 0xb8 */
  [185] = i12x, /* 0xb9 */
  [186] = i12x, /* 0xba */
  [187] = i12x, /* 0xbb */
  [188] = i12x, /* 0xbc */
  [189] = i12x, /* 0xbd */
  [190] = i12x, /* 0xbe */
  [191] = i12x, /* 0xbf */
  [192] = i12x, /* 0xc0 */
  [193] = i12x, /* 0xc1 */
  [194] = i12x, /* 0xc2 */
  [195] = i12x, /* 0xc3 */
  [196] = i12x, /* 0xc4 */
  [197] = i12x, /* 0xc5 */
  [198] = i12x, /* 0xc6 */
  [199] = i12x, /* 0xc7 */
  [200] = i12x, /* 0xc8 */
  [201] = i12x, /* 0xc9 */
  [202] = i12x, /* 0xca */
  [203] = i12x, /* 0xcb */
  [204] = i12x, /* 0xcc */
  [205] = i12x, /* 0xcd */
  [206] = i12x, /* 0xce */
  [207] = i12x, /* 0xcf */
  [208] = i22s, /* 0xd0 */
  [209] = i22s, /* 0xd1 */
  [210] = i22s, /* 0xd2 */
  [211] = i22s, /* 0xd3 */
  [212] = i22s, /* 0xd4 */
  [213] = i22s, /* 0xd5 */
  [214] = i22s, /* 0xd6 */
  [215] = i22s, /* 0xd7 */
  [216] = i22b, /* 0xd8 */
  [217] = i22b, /* 0xd9 */
  [218] = i22b, /* 0xda */
  [219] = i22b, /* 0xdb */
  [220] = i22b, /* 0xdc */
  [221] = i22b, /* 0xdd */
  [222] = i22b, /* 0xde */
  [223] = i22b, /* 0xdf */
  [224] = i22b, /* 0xe0 */
  [225] = i22b, /* 0xe1 */
  [226] = i22b, /* 0xe2 */
  [227] = i10x, /* 0xe3 */
  [228] = i10x, /* 0xe4 */
  [229] = i10x, /* 0xe5 */
  [230] = i10x, /* 0xe6 */
  [231] = i10x, /* 0xe7 */
  [232] = i10x, /* 0xe8 */
  [233] = i10x, /* 0xe9 */
  [234] = i10x, /* 0xea */
  [235] = i10x, /* 0xeb */
  [236] = i10x, /* 0xec */
  [237] = i10x, /* 0xed */
  [238] = i10x, /* 0xee */
  [239] = i10x, /* 0xef */
  [240] = i10x, /* 0xf0 */
  [241] = i10x, /* 0xf1 */
  [242] = i10x, /* 0xf2 */
  [243] = i10x, /* 0xf3 */
  [244] = i10x, /* 0xf4 */
  [245] = i10x, /* 0xf5 */
  [246] = i10x, /* 0xf6 */
  [247] = i10x, /* 0xf7 */
  [248] = i10x, /* 0xf8 */
  [249] = i10x, /* 0xf9 */
  [250] = i10x, /* 0xfa */
  [251] = i10x, /* 0xfb */
  [252] = i10x, /* 0xfc */
  [253] = i10x, /* 0xfd */
  [254] = i10x, /* 0xfe */
  [255] = i10x, /* 0xff */
};

// Byte code value to identifier mapping
const char *bytecode_name_list[256] = {
  [0x00] = "nop", 
  [0x01] = "move", 
  [0x02] = "move/from16", 
  [0x03] = "move/16", 
  [0x04] = "move-wide", 
  [0x05] = "move-wide/from16", 
  [0x06] = "move-wide/16", 
  [0x07] = "move-object", 
  [0x08] = "move-object/from16", 
  [0x09] = "move-object/16", 
  [0x0a] = "move-result", 
  [0x0b] = "move-result-wide", 
  [0x0c] = "move-result-object", 
  [0x0d] = "move-exception", 
  [0x0e] = "return-void", 
  [0x0f] = "return", 
  [0x10] = "return-wide", 
  [0x11] = "return-object", 
  [0x12] = "const/4", 
  [0x13] = "const/16", 
  [0x14] = "const", 
  [0x15] = "const/high16", 
  [0x16] = "const-wide/16", 
  [0x17] = "const-wide/32", 
  [0x18] = "const-wide", 
  [0x19] = "const-wide/high16", 
  [0x1a] = "const-string", 
  [0x1b] = "const-string/jumbo", 
  [0x1c] = "const-class", 
  [0x1d] = "monitor-enter", 
  [0x1e] = "monitor-exit", 
  [0x1f] = "check-cast", 
  [0x20] = "instance-of", 
  [0x21] = "array-length", 
  [0x22] = "new-instance", 
  [0x23] = "new-array", 
  [0x24] = "filled-new-array", 
  [0x25] = "filled-new-array/range", 
  [0x26] = "fill-array-data", 
  [0x27] = "throw", 
  [0x28] = "goto", 
  [0x29] = "goto/16", 
  [0x2a] = "goto/32", 
  [0x2b] = "packed-switch", 
  [0x2c] = "sparse-switch", 
  [0x2d] = "cmpl-float", 
  [0x2e] = "cmpg-float", 
  [0x2f] = "cmpl-double", 
  [0x30] = "cmpg-double", 
  [0x31] = "cmp-long", 
  [0x32] = "if-eq", 
  [0x33] = "if-ne", 
  [0x34] = "if-lt", 
  [0x35] = "if-ge", 
  [0x36] = "if-gt", 
  [0x37] = "if-le", 
  [0x38] = "if-eqz", 
  [0x39] = "if-nez", 
  [0x3a] = "if-ltz", 
  [0x3b] = "if-gez", 
  [0x3c] = "if-gtz", 
  [0x3d] = "if-lez", 
  [0x44] = "aget", 
  [0x45] = "aget-wide", 
  [0x46] = "aget-object", 
  [0x47] = "aget-boolean", 
  [0x48] = "aget-byte", 
  [0x49] = "aget-char", 
  [0x4a] = "aget-short", 
  [0x4b] = "aput", 
  [0x4c] = "aput-wide", 
  [0x4d] = "aput-object", 
  [0x4e] = "aput-boolean", 
  [0x4f] = "aput-byte", 
  [0x50] = "aput-char", 
  [0x51] = "aput-short", 
  [0x52] = "iget", 
  [0x53] = "iget-wide", 
  [0x54] = "iget-object", 
  [0x55] = "iget-boolean", 
  [0x56] = "iget-byte", 
  [0x57] = "iget-char", 
  [0x58] = "iget-short", 
  [0x59] = "iput", 
  [0x5a] = "iput-wide", 
  [0x5b] = "iput-object", 
  [0x5c] = "iput-boolean", 
  [0x5d] = "iput-byte", 
  [0x5e] = "iput-char", 
  [0x5f] = "iput-short", 
  [0x60] = "sget", 
  [0x61] = "sget-wide", 
  [0x62] = "sget-object", 
  [0x63] = "sget-boolean", 
  [0x64] = "sget-byte", 
  [0x65] = "sget-char", 
  [0x66] = "sget-short", 
  [0x67] = "sput", 
  [0x68] = "sput-wide", 
  [0x69] = "sput-object", 
  [0x6a] = "sput-boolean", 
  [0x6b] = "sput-byte", 
  [0x6c] = "sput-char", 
  [0x6d] = "sput-short", 
  [0x6e] = "invoke-virtual", 
  [0x6f] = "invoke-super", 
  [0x70] = "invoke-direct", 
  [0x71] = "invoke-static", 
  [0x72] = "invoke-interface", 
  [0x74] = "invoke-virtual/range", 
  [0x75] = "invoke-super/range", 
  [0x76] = "invoke-direct/range", 
  [0x77] = "invoke-static/range", 
  [0x78] = "invoke-interface/range", 
  [0x7b] = "neg-int", 
  [0x7c] = "not-int", 
  [0x7d] = "neg-long", 
  [0x7e] = "not-long", 
  [0x7f] = "neg-float", 
  [0x80] = "neg-double", 
  [0x81] = "int-to-long", 
  [0x82] = "int-to-float", 
  [0x83] = "int-to-double", 
  [0x84] = "long-to-int", 
  [0x85] = "long-to-float", 
  [0x86] = "long-to-double", 
  [0x87] = "float-to-int", 
  [0x88] = "float-to-long", 
  [0x89] = "float-to-double", 
  [0x8a] = "double-to-int", 
  [0x8b] = "double-to-long", 
  [0x8c] = "double-to-float", 
  [0x8d] = "int-to-byte", 
  [0x8e] = "int-to-char", 
  [0x8f] = "int-to-short", 
  [0x90] = "add-int", 
  [0x91] = "sub-int", 
  [0x92] = "mul-int", 
  [0x93] = "div-int", 
  [0x94] = "rem-int", 
  [0x95] = "and-int", 
  [0x96] = "or-int", 
  [0x97] = "xor-int", 
  [0x98] = "shl-int", 
  [0x99] = "shr-int", 
  [0x9a] = "ushr-int", 
  [0x9b] = "add-long", 
  [0x9c] = "sub-long", 
  [0x9d] = "mul-long", 
  [0x9e] = "div-long", 
  [0x9f] = "rem-long", 
  [0xa0] = "and-long", 
  [0xa1] = "or-long", 
  [0xa2] = "xor-long", 
  [0xa3] = "shl-long", 
  [0xa4] = "shr-long", 
  [0xa5] = "ushr-long", 
  [0xa6] = "add-float", 
  [0xa7] = "sub-float", 
  [0xa8] = "mul-float", 
  [0xa9] = "div-float", 
  [0xaa] = "rem-float", 
  [0xab] = "add-double", 
  [0xac] = "sub-double", 
  [0xad] = "mul-double", 
  [0xae] = "div-double", 
  [0xaf] = "rem-double", 
  [0xb0] = "add-int/2addr", 
  [0xb1] = "sub-int/2addr", 
  [0xb2] = "mul-int/2addr", 
  [0xb3] = "div-int/2addr", 
  [0xb4] = "rem-int/2addr", 
  [0xb5] = "and-int/2addr", 
  [0xb6] = "or-int/2addr", 
  [0xb7] = "xor-int/2addr", 
  [0xb8] = "shl-int/2addr", 
  [0xb9] = "shr-int/2addr", 
  [0xba] = "ushr-int/2addr", 
  [0xbb] = "add-long/2addr", 
  [0xbc] = "sub-long/2addr", 
  [0xbd] = "mul-long/2addr", 
  [0xbe] = "div-long/2addr", 
  [0xbf] = "rem-long/2addr", 
  [0xc0] = "and-long/2addr", 
  [0xc1] = "or-long/2addr", 
  [0xc2] = "xor-long/2addr", 
  [0xc3] = "shl-long/2addr", 
  [0xc4] = "shr-long/2addr", 
  [0xc5] = "ushr-long/2addr", 
  [0xc6] = "add-float/2addr", 
  [0xc7] = "sub-float/2addr", 
  [0xc8] = "mul-float/2addr", 
  [0xc9] = "div-float/2addr", 
  [0xca] = "rem-float/2addr", 
  [0xcb] = "add-double/2addr", 
  [0xcc] = "sub-double/2addr", 
  [0xcd] = "mul-double/2addr", 
  [0xce] = "div-double/2addr", 
  [0xcf] = "rem-double/2addr", 
  [0xd0] = "add-int/lit16", 
  [0xd1] = "rsub-int", 
  [0xd2] = "mul-int/lit16", 
  [0xd3] = "div-int/lit16", 
  [0xd4] = "rem-int/lit16", 
  [0xd5] = "and-int/lit16", 
  [0xd6] = "or-int/lit16", 
  [0xd7] = "xor-int/lit16", 
  [0xd8] = "add-int/lit8", 
  [0xd9] = "rsub-int/lit8", 
  [0xda] = "mul-int/lit8", 
  [0xdb] = "div-int/lit8", 
  [0xdc] = "rem-int/lit8", 
  [0xdd] = "and-int/lit8", 
  [0xde] = "or-int/lit8", 
  [0xdf] = "xor-int/lit8", 
  [0xe0] = "shl-int/lit8", 
  [0xe1] = "shr-int/lit8", 
  [0xe2] = "ushr-int/lit8", 
};

}
}
}
