/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
*/


/*Generated file, included by vp9_entropy.c*/

#if CONFIG_BALANCED_COEFTREE
static const vp9_coeff_probs_model default_coef_probs_4x4[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        {   6, 213, 178 },
        {  26, 113, 132 },
        {  34,  17,  68 }
      }, { /* Coeff Band 1 */
        {  66,  96, 178 },
        {  63,  96, 174 },
        {  67,  54, 154 },
        {  62,  28, 126 },
        {  48,   9,  84 },
        {  20,   1,  32 }
      }, { /* Coeff Band 2 */
        {  64, 144, 206 },
        {  70,  99, 191 },
        {  69,  36, 152 },
        {  55,   9, 106 },
        {  35,   1,  60 },
        {  14,   1,  22 }
      }, { /* Coeff Band 3 */
        {  82, 154, 222 },
        {  83, 112, 205 },
        {  81,  31, 164 },
        {  62,   7, 118 },
        {  42,   1,  74 },
        {  18,   1,  30 }
      }, { /* Coeff Band 4 */
        {  52, 179, 233 },
        {  64, 132, 214 },
        {  73,  36, 170 },
        {  59,   8, 116 },
        {  38,   1,  65 },
        {  15,   1,  26 }
      }, { /* Coeff Band 5 */
        {  29, 175, 238 },
        {  26, 169, 223 },
        {  41,  80, 182 },
        {  39,  32, 127 },
        {  26,  10,  69 },
        {  11,   2,  28 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {  21, 226, 234 },
        {  52, 182, 212 },
        {  80, 112, 177 }
      }, { /* Coeff Band 1 */
        { 111, 164, 243 },
        {  88, 152, 231 },
        {  90,  43, 186 },
        {  70,  12, 132 },
        {  44,   2,  76 },
        {  19,   1,  33 }
      }, { /* Coeff Band 2 */
        {  96, 185, 246 },
        {  99, 127, 231 },
        {  88,  21, 177 },
        {  64,   5, 122 },
        {  38,   1,  69 },
        {  18,   1,  30 }
      }, { /* Coeff Band 3 */
        {  84, 206, 249 },
        {  94, 147, 237 },
        {  95,  33, 187 },
        {  71,   8, 131 },
        {  47,   1,  83 },
        {  26,   1,  44 }
      }, { /* Coeff Band 4 */
        {  38, 221, 252 },
        {  58, 177, 241 },
        {  78,  46, 188 },
        {  59,   9, 122 },
        {  34,   1,  66 },
        {  18,   1,  34 }
      }, { /* Coeff Band 5 */
        {  21, 216, 253 },
        {  21, 206, 244 },
        {  42,  93, 200 },
        {  43,  41, 146 },
        {  36,  13,  93 },
        {  31,   1,  55 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        {   7, 213, 219 },
        {  23, 139, 182 },
        {  38,  60, 125 }
      }, { /* Coeff Band 1 */
        {  69, 156, 220 },
        {  52, 178, 213 },
        {  69, 111, 190 },
        {  69,  58, 155 },
        {  58,  21, 104 },
        {  39,   7,  60 }
      }, { /* Coeff Band 2 */
        {  68, 189, 228 },
        {  70, 158, 221 },
        {  83,  64, 189 },
        {  73,  18, 141 },
        {  48,   4,  88 },
        {  23,   1,  41 }
      }, { /* Coeff Band 3 */
        {  99, 194, 236 },
        {  91, 138, 224 },
        {  91,  53, 189 },
        {  74,  20, 142 },
        {  48,   6,  90 },
        {  22,   1,  41 }
      }, { /* Coeff Band 4 */
        {  52, 203, 244 },
        {  60, 168, 231 },
        {  75,  62, 189 },
        {  61,  18, 132 },
        {  38,   4,  72 },
        {  17,   1,  39 }
      }, { /* Coeff Band 5 */
        {  33, 192, 247 },
        {  31, 185, 234 },
        {  46,  85, 185 },
        {  39,  35, 132 },
        {  28,  15,  80 },
        {  13,   5,  38 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {   5, 247, 246 },
        {  28, 209, 228 },
        {  65, 137, 203 }
      }, { /* Coeff Band 1 */
        {  69, 208, 250 },
        {  54, 207, 242 },
        {  81,  92, 204 },
        {  70,  54, 153 },
        {  58,  40, 108 },
        {  58,  35,  71 }
      }, { /* Coeff Band 2 */
        {  65, 215, 250 },
        {  72, 185, 239 },
        {  92,  50, 197 },
        {  75,  14, 147 },
        {  49,   2,  99 },
        {  26,   1,  53 }
      }, { /* Coeff Band 3 */
        {  70, 220, 251 },
        {  76, 186, 241 },
        {  90,  65, 198 },
        {  75,  26, 151 },
        {  58,  12, 112 },
        {  34,   6,  49 }
      }, { /* Coeff Band 4 */
        {  34, 224, 253 },
        {  44, 204, 245 },
        {  69,  85, 204 },
        {  64,  31, 150 },
        {  44,   2,  78 },
        {   1,   1, 128 }
      }, { /* Coeff Band 5 */
        {  25, 216, 253 },
        {  21, 215, 248 },
        {  47, 108, 214 },
        {  47,  48, 160 },
        {  26,  20,  90 },
        {  64, 171, 128 }
      }
    }
  }
};
static const vp9_coeff_probs_model default_coef_probs_8x8[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        {   9, 203, 199 },
        {  26,  92, 128 },
        {  28,  11,  55 }
      }, { /* Coeff Band 1 */
        {  99,  54, 160 },
        {  78,  99, 155 },
        {  80,  44, 138 },
        {  71,  17, 115 },
        {  51,   5,  80 },
        {  27,   1,  40 }
      }, { /* Coeff Band 2 */
        { 135,  81, 190 },
        { 113,  61, 182 },
        {  93,  16, 153 },
        {  70,   4, 115 },
        {  41,   1,  68 },
        {  16,   1,  27 }
      }, { /* Coeff Band 3 */
        { 155, 103, 214 },
        { 129,  48, 199 },
        {  95,  10, 159 },
        {  63,   1, 110 },
        {  32,   1,  58 },
        {  12,   1,  21 }
      }, { /* Coeff Band 4 */
        { 163, 149, 231 },
        { 137,  69, 213 },
        {  95,  11, 164 },
        {  62,   3, 108 },
        {  32,   1,  57 },
        {  13,   1,  22 }
      }, { /* Coeff Band 5 */
        { 136, 189, 239 },
        { 123, 102, 223 },
        {  97,  19, 170 },
        {  66,   4, 111 },
        {  38,   1,  60 },
        {  18,   1,  26 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {  24, 226, 244 },
        {  54, 178, 211 },
        {  80,  74, 152 }
      }, { /* Coeff Band 1 */
        { 145, 153, 236 },
        { 101, 163, 223 },
        { 108,  50, 187 },
        {  90,  22, 145 },
        {  66,   8,  97 },
        {  42,   4,  50 }
      }, { /* Coeff Band 2 */
        { 150, 159, 238 },
        { 128,  90, 218 },
        {  94,   9, 163 },
        {  64,   3, 110 },
        {  34,   1,  61 },
        {  13,   1,  24 }
      }, { /* Coeff Band 3 */
        { 151, 162, 242 },
        { 135,  80, 222 },
        {  93,   9, 166 },
        {  61,   3, 111 },
        {  31,   1,  59 },
        {  12,   1,  22 }
      }, { /* Coeff Band 4 */
        { 161, 170, 245 },
        { 140,  84, 228 },
        {  99,   8, 174 },
        {  64,   1, 116 },
        {  34,   1,  63 },
        {  14,   1,  26 }
      }, { /* Coeff Band 5 */
        { 138, 197, 246 },
        { 127, 109, 233 },
        { 100,  16, 179 },
        {  66,   3, 119 },
        {  37,   1,  66 },
        {  16,   1,  30 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        {   6, 216, 212 },
        {  25, 134, 171 },
        {  43,  48, 118 }
      }, { /* Coeff Band 1 */
        {  93, 112, 209 },
        {  66, 159, 206 },
        {  82,  78, 184 },
        {  75,  28, 148 },
        {  46,   4,  82 },
        {  18,   1,  28 }
      }, { /* Coeff Band 2 */
        { 108, 148, 220 },
        {  90, 130, 216 },
        {  92,  40, 186 },
        {  73,  10, 135 },
        {  46,   1,  79 },
        {  20,   1,  35 }
      }, { /* Coeff Band 3 */
        { 125, 173, 232 },
        { 109, 117, 223 },
        {  97,  31, 183 },
        {  71,   7, 127 },
        {  44,   1,  76 },
        {  21,   1,  36 }
      }, { /* Coeff Band 4 */
        { 133, 195, 236 },
        { 112, 121, 224 },
        {  97,  23, 178 },
        {  69,   3, 122 },
        {  42,   1,  72 },
        {  19,   1,  34 }
      }, { /* Coeff Band 5 */
        { 132, 180, 238 },
        { 119, 102, 225 },
        { 101,  18, 179 },
        {  71,   3, 124 },
        {  42,   1,  70 },
        {  17,   1,  28 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {   5, 242, 250 },
        {  26, 198, 226 },
        {  58,  98, 168 }
      }, { /* Coeff Band 1 */
        {  82, 201, 246 },
        {  50, 219, 237 },
        {  94, 107, 205 },
        {  89,  61, 167 },
        {  77,  31, 131 },
        {  57,  14,  91 }
      }, { /* Coeff Band 2 */
        {  99, 202, 247 },
        {  96, 165, 234 },
        { 100,  31, 190 },
        {  72,   8, 131 },
        {  41,   1,  72 },
        {  14,   1,  24 }
      }, { /* Coeff Band 3 */
        { 108, 204, 248 },
        { 107, 156, 235 },
        { 103,  27, 186 },
        {  71,   4, 124 },
        {  39,   1,  66 },
        {  14,   1,  19 }
      }, { /* Coeff Band 4 */
        { 120, 211, 248 },
        { 118, 149, 234 },
        { 107,  19, 182 },
        {  72,   3, 126 },
        {  40,   1,  69 },
        {  16,   1,  24 }
      }, { /* Coeff Band 5 */
        { 127, 199, 245 },
        { 122, 125, 232 },
        { 112,  20, 186 },
        {  82,   3, 136 },
        {  55,   1,  88 },
        {  10,   1,  38 }
      }
    }
  }
};
static const vp9_coeff_probs_model default_coef_probs_16x16[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        {  25,   9, 101 },
        {  25,   2,  67 },
        {  15,   1,  28 }
      }, { /* Coeff Band 1 */
        {  67,  30, 118 },
        {  61,  56, 116 },
        {  60,  31, 105 },
        {  52,  11,  85 },
        {  34,   2,  54 },
        {  14,   1,  22 }
      }, { /* Coeff Band 2 */
        { 107,  58, 149 },
        {  92,  53, 147 },
        {  78,  14, 123 },
        {  56,   3,  87 },
        {  35,   1,  56 },
        {  17,   1,  27 }
      }, { /* Coeff Band 3 */
        { 142,  61, 171 },
        { 111,  30, 162 },
        {  80,   4, 128 },
        {  53,   1,  87 },
        {  31,   1,  52 },
        {  14,   1,  24 }
      }, { /* Coeff Band 4 */
        { 171,  73, 200 },
        { 129,  28, 184 },
        {  86,   3, 140 },
        {  54,   1,  90 },
        {  28,   1,  49 },
        {  12,   1,  21 }
      }, { /* Coeff Band 5 */
        { 193, 129, 227 },
        { 148,  28, 200 },
        {  90,   2, 144 },
        {  53,   1,  90 },
        {  28,   1,  50 },
        {  13,   1,  22 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {  60,   7, 234 },
        {  64,   4, 184 },
        {  56,   1, 104 }
      }, { /* Coeff Band 1 */
        { 150, 111, 210 },
        {  87, 185, 202 },
        { 101,  81, 177 },
        {  90,  34, 142 },
        {  67,  11,  95 },
        {  38,   2,  51 }
      }, { /* Coeff Band 2 */
        { 153, 139, 218 },
        { 120,  72, 195 },
        {  90,  11, 147 },
        {  63,   3, 101 },
        {  39,   1,  61 },
        {  20,   1,  33 }
      }, { /* Coeff Band 3 */
        { 171, 132, 223 },
        { 131,  56, 200 },
        {  92,   6, 147 },
        {  58,   1,  95 },
        {  32,   1,  52 },
        {  14,   1,  23 }
      }, { /* Coeff Band 4 */
        { 183, 137, 227 },
        { 139,  48, 204 },
        {  91,   3, 148 },
        {  55,   1,  91 },
        {  28,   1,  47 },
        {  13,   1,  21 }
      }, { /* Coeff Band 5 */
        { 198, 149, 234 },
        { 153,  32, 208 },
        {  95,   2, 148 },
        {  55,   1,  90 },
        {  30,   1,  51 },
        {  16,   1,  25 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        {   7, 209, 217 },
        {  31, 106, 151 },
        {  40,  21,  86 }
      }, { /* Coeff Band 1 */
        { 101,  71, 184 },
        {  74, 131, 177 },
        {  88,  50, 158 },
        {  78,  16, 129 },
        {  51,   2,  82 },
        {  18,   1,  29 }
      }, { /* Coeff Band 2 */
        { 116, 115, 199 },
        { 102,  88, 191 },
        {  94,  22, 160 },
        {  74,   6, 122 },
        {  47,   1,  77 },
        {  18,   1,  30 }
      }, { /* Coeff Band 3 */
        { 157, 124, 210 },
        { 130,  53, 201 },
        { 102,  10, 165 },
        {  73,   1, 120 },
        {  42,   1,  69 },
        {  16,   1,  27 }
      }, { /* Coeff Band 4 */
        { 174, 147, 225 },
        { 134,  67, 212 },
        { 100,  10, 168 },
        {  66,   1, 111 },
        {  36,   1,  60 },
        {  16,   1,  27 }
      }, { /* Coeff Band 5 */
        { 185, 165, 232 },
        { 147,  56, 214 },
        { 105,   5, 165 },
        {  66,   1, 108 },
        {  35,   1,  59 },
        {  16,   1,  27 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {   3, 232, 245 },
        {  18, 162, 210 },
        {  38,  64, 131 }
      }, { /* Coeff Band 1 */
        {  84, 187, 239 },
        {  35, 231, 231 },
        {  82, 150, 209 },
        {  87,  97, 181 },
        {  81,  64, 151 },
        {  67,  60, 119 }
      }, { /* Coeff Band 2 */
        { 107, 185, 239 },
        { 100, 149, 224 },
        { 107,  34, 185 },
        {  83,  12, 141 },
        {  49,   4,  92 },
        {  21,   1,  40 }
      }, { /* Coeff Band 3 */
        { 125, 184, 243 },
        { 121, 127, 228 },
        { 113,  25, 185 },
        {  82,   6, 134 },
        {  48,   1,  82 },
        {  26,   1,  38 }
      }, { /* Coeff Band 4 */
        { 143, 185, 245 },
        { 133, 115, 231 },
        { 114,  14, 184 },
        {  77,   3, 126 },
        {  43,   1,  68 },
        {  34,   1,  40 }
      }, { /* Coeff Band 5 */
        { 170, 194, 241 },
        { 151,  80, 226 },
        { 118,   9, 180 },
        {  81,   1, 130 },
        {  51,   1,  78 },
        {  18,   1,  49 }
      }
    }
  }
};
static const vp9_coeff_probs_model default_coef_probs_32x32[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        {  29,  42, 137 },
        {  26,   3,  60 },
        {  13,   1,  23 }
      }, { /* Coeff Band 1 */
        {  69,  36, 122 },
        {  63,  57, 123 },
        {  60,  33, 112 },
        {  52,  11,  90 },
        {  32,   2,  52 },
        {  10,   1,  15 }
      }, { /* Coeff Band 2 */
        { 107,  55, 143 },
        {  86,  69, 143 },
        {  74,  24, 116 },
        {  52,   5,  78 },
        {  29,   1,  44 },
        {  12,   1,  18 }
      }, { /* Coeff Band 3 */
        { 137,  71, 160 },
        { 107,  34, 152 },
        {  73,   6, 114 },
        {  44,   1,  69 },
        {  25,   1,  40 },
        {  12,   1,  18 }
      }, { /* Coeff Band 4 */
        { 165,  70, 174 },
        { 118,  24, 159 },
        {  74,   3, 117 },
        {  45,   1,  73 },
        {  26,   1,  43 },
        {  12,   1,  19 }
      }, { /* Coeff Band 5 */
        { 220,  93, 223 },
        { 153,  10, 187 },
        {  86,   2, 131 },
        {  49,   1,  79 },
        {  26,   1,  43 },
        {  12,   1,  20 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {  30,  58, 227 },
        {  35,  10, 172 },
        {  24,  23, 112 }
      }, { /* Coeff Band 1 */
        { 117, 145, 219 },
        {  51, 221, 216 },
        {  75, 169, 196 },
        {  88,  96, 165 },
        {  77,  43, 117 },
        {  53,  18,  60 }
      }, { /* Coeff Band 2 */
        { 128, 176, 225 },
        { 108, 114, 202 },
        {  92,  19, 152 },
        {  65,   4, 103 },
        {  38,   1,  61 },
        {  19,   1,  30 }
      }, { /* Coeff Band 3 */
        { 146, 184, 228 },
        { 122,  95, 205 },
        {  92,  11, 149 },
        {  62,   1,  98 },
        {  35,   1,  57 },
        {  17,   1,  26 }
      }, { /* Coeff Band 4 */
        { 165, 192, 230 },
        { 132,  81, 206 },
        {  93,   6, 147 },
        {  58,   1,  94 },
        {  32,   1,  52 },
        {  15,   1,  24 }
      }, { /* Coeff Band 5 */
        { 204, 223, 234 },
        { 156,  49, 204 },
        {  97,   3, 145 },
        {  59,   1,  92 },
        {  33,   1,  52 },
        {  15,   1,  24 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        {   7, 184, 200 },
        {  25,  67, 113 },
        {  30,   9,  59 }
      }, { /* Coeff Band 1 */
        {  92,  42, 158 },
        {  65, 121, 159 },
        {  77,  56, 146 },
        {  70,  22, 120 },
        {  47,   4,  76 },
        {  18,   1,  26 }
      }, { /* Coeff Band 2 */
        { 113,  81, 177 },
        {  96,  75, 167 },
        {  84,  24, 136 },
        {  63,   8, 100 },
        {  37,   1,  58 },
        {  13,   1,  19 }
      }, { /* Coeff Band 3 */
        { 147,  85, 194 },
        { 119,  36, 178 },
        {  88,   8, 139 },
        {  59,   1,  93 },
        {  31,   1,  49 },
        {  10,   1,  18 }
      }, { /* Coeff Band 4 */
        { 169, 108, 210 },
        { 131,  41, 191 },
        {  92,   5, 144 },
        {  56,   1,  88 },
        {  29,   1,  47 },
        {  14,   1,  22 }
      }, { /* Coeff Band 5 */
        { 210, 106, 223 },
        { 148,  14, 192 },
        {  89,   2, 138 },
        {  52,   1,  84 },
        {  29,   1,  47 },
        {  14,   1,  23 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {   3, 207, 245 },
        {  12, 102, 213 },
        {  18,  33, 144 }
      }, { /* Coeff Band 1 */
        {  85, 205, 245 },
        {  18, 249, 242 },
        {  59, 221, 229 },
        {  91, 166, 213 },
        {  88, 117, 183 },
        {  70,  95, 149 }
      }, { /* Coeff Band 2 */
        { 114, 193, 241 },
        { 104, 155, 221 },
        { 100,  33, 181 },
        {  78,  10, 132 },
        {  43,   2,  75 },
        {  15,   1,  48 }
      }, { /* Coeff Band 3 */
        { 118, 198, 244 },
        { 117, 142, 224 },
        { 111,  25, 179 },
        {  83,   4, 134 },
        {  57,   1,  84 },
        {   1,   1,   1 }
      }, { /* Coeff Band 4 */
        { 144, 201, 248 },
        { 136, 130, 234 },
        { 124,  12, 188 },
        {  83,   1, 130 },
        {  61,   1,  66 },
        {  64, 171, 128 }
      }, { /* Coeff Band 5 */
        { 174, 227, 250 },
        { 165, 118, 242 },
        { 132,  21, 197 },
        {  84,   3, 134 },
        {  70,   1,  69 },
        {   1,   1,   1 }
      }
    }
  }
};
#else
static const vp9_coeff_probs_model default_coef_probs_4x4[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        { 195,  29, 183 },
        {  84,  49, 136 },
        {   8,  42,  71 }
      }, { /* Coeff Band 1 */
        {  31, 107, 169 },
        {  35,  99, 159 },
        {  17,  82, 140 },
        {   8,  66, 114 },
        {   2,  44,  76 },
        {   1,  19,  32 }
      }, { /* Coeff Band 2 */
        {  40, 132, 201 },
        {  29, 114, 187 },
        {  13,  91, 157 },
        {   7,  75, 127 },
        {   3,  58,  95 },
        {   1,  28,  47 }
      }, { /* Coeff Band 3 */
        {  69, 142, 221 },
        {  42, 122, 201 },
        {  15,  91, 159 },
        {   6,  67, 121 },
        {   1,  42,  77 },
        {   1,  17,  31 }
      }, { /* Coeff Band 4 */
        { 102, 148, 228 },
        {  67, 117, 204 },
        {  17,  82, 154 },
        {   6,  59, 114 },
        {   2,  39,  75 },
        {   1,  15,  29 }
      }, { /* Coeff Band 5 */
        { 156,  57, 233 },
        { 119,  57, 212 },
        {  58,  48, 163 },
        {  29,  40, 124 },
        {  12,  30,  81 },
        {   3,  12,  31 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        { 191, 107, 226 },
        { 124, 117, 204 },
        {  25,  99, 155 }
      }, { /* Coeff Band 1 */
        {  29, 148, 210 },
        {  37, 126, 194 },
        {   8,  93, 157 },
        {   2,  68, 118 },
        {   1,  39,  69 },
        {   1,  17,  33 }
      }, { /* Coeff Band 2 */
        {  41, 151, 213 },
        {  27, 123, 193 },
        {   3,  82, 144 },
        {   1,  58, 105 },
        {   1,  32,  60 },
        {   1,  13,  26 }
      }, { /* Coeff Band 3 */
        {  59, 159, 220 },
        {  23, 126, 198 },
        {   4,  88, 151 },
        {   1,  66, 114 },
        {   1,  38,  71 },
        {   1,  18,  34 }
      }, { /* Coeff Band 4 */
        { 114, 136, 232 },
        {  51, 114, 207 },
        {  11,  83, 155 },
        {   3,  56, 105 },
        {   1,  33,  65 },
        {   1,  17,  34 }
      }, { /* Coeff Band 5 */
        { 149,  65, 234 },
        { 121,  57, 215 },
        {  61,  49, 166 },
        {  28,  36, 114 },
        {  12,  25,  76 },
        {   3,  16,  42 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        { 214,  49, 220 },
        { 132,  63, 188 },
        {  42,  65, 137 }
      }, { /* Coeff Band 1 */
        {  85, 137, 221 },
        { 104, 131, 216 },
        {  49, 111, 192 },
        {  21,  87, 155 },
        {   2,  49,  87 },
        {   1,  16,  28 }
      }, { /* Coeff Band 2 */
        {  89, 163, 230 },
        {  90, 137, 220 },
        {  29, 100, 183 },
        {  10,  70, 135 },
        {   2,  42,  81 },
        {   1,  17,  33 }
      }, { /* Coeff Band 3 */
        { 108, 167, 237 },
        {  55, 133, 222 },
        {  15,  97, 179 },
        {   4,  72, 135 },
        {   1,  45,  85 },
        {   1,  19,  38 }
      }, { /* Coeff Band 4 */
        { 124, 146, 240 },
        {  66, 124, 224 },
        {  17,  88, 175 },
        {   4,  58, 122 },
        {   1,  36,  75 },
        {   1,  18,  37 }
      }, { /* Coeff Band 5 */
        { 141,  79, 241 },
        { 126,  70, 227 },
        {  66,  58, 182 },
        {  30,  44, 136 },
        {  12,  34,  96 },
        {   2,  20,  47 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        { 229,  99, 249 },
        { 143, 111, 235 },
        {  46, 109, 192 }
      }, { /* Coeff Band 1 */
        {  82, 158, 236 },
        {  94, 146, 224 },
        {  25, 117, 191 },
        {   9,  87, 149 },
        {   3,  56,  99 },
        {   1,  33,  57 }
      }, { /* Coeff Band 2 */
        {  83, 167, 237 },
        {  68, 145, 222 },
        {  10, 103, 177 },
        {   2,  72, 131 },
        {   1,  41,  79 },
        {   1,  20,  39 }
      }, { /* Coeff Band 3 */
        {  99, 167, 239 },
        {  47, 141, 224 },
        {  10, 104, 178 },
        {   2,  73, 133 },
        {   1,  44,  85 },
        {   1,  22,  47 }
      }, { /* Coeff Band 4 */
        { 127, 145, 243 },
        {  71, 129, 228 },
        {  17,  93, 177 },
        {   3,  61, 124 },
        {   1,  41,  84 },
        {   1,  21,  52 }
      }, { /* Coeff Band 5 */
        { 157,  78, 244 },
        { 140,  72, 231 },
        {  69,  58, 184 },
        {  31,  44, 137 },
        {  14,  38, 105 },
        {   8,  23,  61 }
      }
    }
  }
};
static const vp9_coeff_probs_model default_coef_probs_8x8[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        { 125,  34, 187 },
        {  52,  41, 133 },
        {   6,  31,  56 }
      }, { /* Coeff Band 1 */
        {  37, 109, 153 },
        {  51, 102, 147 },
        {  23,  87, 128 },
        {   8,  67, 101 },
        {   1,  41,  63 },
        {   1,  19,  29 }
      }, { /* Coeff Band 2 */
        {  31, 154, 185 },
        {  17, 127, 175 },
        {   6,  96, 145 },
        {   2,  73, 114 },
        {   1,  51,  82 },
        {   1,  28,  45 }
      }, { /* Coeff Band 3 */
        {  23, 163, 200 },
        {  10, 131, 185 },
        {   2,  93, 148 },
        {   1,  67, 111 },
        {   1,  41,  69 },
        {   1,  14,  24 }
      }, { /* Coeff Band 4 */
        {  29, 176, 217 },
        {  12, 145, 201 },
        {   3, 101, 156 },
        {   1,  69, 111 },
        {   1,  39,  63 },
        {   1,  14,  23 }
      }, { /* Coeff Band 5 */
        {  57, 192, 233 },
        {  25, 154, 215 },
        {   6, 109, 167 },
        {   3,  78, 118 },
        {   1,  48,  69 },
        {   1,  21,  29 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        { 202, 105, 245 },
        { 108, 106, 216 },
        {  18,  90, 144 }
      }, { /* Coeff Band 1 */
        {  33, 172, 219 },
        {  64, 149, 206 },
        {  14, 117, 177 },
        {   5,  90, 141 },
        {   2,  61,  95 },
        {   1,  37,  57 }
      }, { /* Coeff Band 2 */
        {  33, 179, 220 },
        {  11, 140, 198 },
        {   1,  89, 148 },
        {   1,  60, 104 },
        {   1,  33,  57 },
        {   1,  12,  21 }
      }, { /* Coeff Band 3 */
        {  30, 181, 221 },
        {   8, 141, 198 },
        {   1,  87, 145 },
        {   1,  58, 100 },
        {   1,  31,  55 },
        {   1,  12,  20 }
      }, { /* Coeff Band 4 */
        {  32, 186, 224 },
        {   7, 142, 198 },
        {   1,  86, 143 },
        {   1,  58, 100 },
        {   1,  31,  55 },
        {   1,  12,  22 }
      }, { /* Coeff Band 5 */
        {  57, 192, 227 },
        {  20, 143, 204 },
        {   3,  96, 154 },
        {   1,  68, 112 },
        {   1,  42,  69 },
        {   1,  19,  32 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        { 212,  35, 215 },
        { 113,  47, 169 },
        {  29,  48, 105 }
      }, { /* Coeff Band 1 */
        {  74, 129, 203 },
        { 106, 120, 203 },
        {  49, 107, 178 },
        {  19,  84, 144 },
        {   4,  50,  84 },
        {   1,  15,  25 }
      }, { /* Coeff Band 2 */
        {  71, 172, 217 },
        {  44, 141, 209 },
        {  15, 102, 173 },
        {   6,  76, 133 },
        {   2,  51,  89 },
        {   1,  24,  42 }
      }, { /* Coeff Band 3 */
        {  64, 185, 231 },
        {  31, 148, 216 },
        {   8, 103, 175 },
        {   3,  74, 131 },
        {   1,  46,  81 },
        {   1,  18,  30 }
      }, { /* Coeff Band 4 */
        {  65, 196, 235 },
        {  25, 157, 221 },
        {   5, 105, 174 },
        {   1,  67, 120 },
        {   1,  38,  69 },
        {   1,  15,  30 }
      }, { /* Coeff Band 5 */
        {  65, 204, 238 },
        {  30, 156, 224 },
        {   7, 107, 177 },
        {   2,  70, 124 },
        {   1,  42,  73 },
        {   1,  18,  34 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        { 225,  86, 251 },
        { 144, 104, 235 },
        {  42,  99, 181 }
      }, { /* Coeff Band 1 */
        {  85, 175, 239 },
        { 112, 165, 229 },
        {  29, 136, 200 },
        {  12, 103, 162 },
        {   6,  77, 123 },
        {   2,  53,  84 }
      }, { /* Coeff Band 2 */
        {  75, 183, 239 },
        {  30, 155, 221 },
        {   3, 106, 171 },
        {   1,  74, 128 },
        {   1,  44,  76 },
        {   1,  17,  28 }
      }, { /* Coeff Band 3 */
        {  73, 185, 240 },
        {  27, 159, 222 },
        {   2, 107, 172 },
        {   1,  75, 127 },
        {   1,  42,  73 },
        {   1,  17,  29 }
      }, { /* Coeff Band 4 */
        {  62, 190, 238 },
        {  21, 159, 222 },
        {   2, 107, 172 },
        {   1,  72, 122 },
        {   1,  40,  71 },
        {   1,  18,  32 }
      }, { /* Coeff Band 5 */
        {  61, 199, 240 },
        {  27, 161, 226 },
        {   4, 113, 180 },
        {   1,  76, 129 },
        {   1,  46,  80 },
        {   1,  23,  41 }
      }
    }
  }
};
static const vp9_coeff_probs_model default_coef_probs_16x16[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        {   7,  27, 153 },
        {   5,  30,  95 },
        {   1,  16,  30 }
      }, { /* Coeff Band 1 */
        {  50,  75, 127 },
        {  57,  75, 124 },
        {  27,  67, 108 },
        {  10,  54,  86 },
        {   1,  33,  52 },
        {   1,  12,  18 }
      }, { /* Coeff Band 2 */
        {  43, 125, 151 },
        {  26, 108, 148 },
        {   7,  83, 122 },
        {   2,  59,  89 },
        {   1,  38,  60 },
        {   1,  17,  27 }
      }, { /* Coeff Band 3 */
        {  23, 144, 163 },
        {  13, 112, 154 },
        {   2,  75, 117 },
        {   1,  50,  81 },
        {   1,  31,  51 },
        {   1,  14,  23 }
      }, { /* Coeff Band 4 */
        {  18, 162, 185 },
        {   6, 123, 171 },
        {   1,  78, 125 },
        {   1,  51,  86 },
        {   1,  31,  54 },
        {   1,  14,  23 }
      }, { /* Coeff Band 5 */
        {  15, 199, 227 },
        {   3, 150, 204 },
        {   1,  91, 146 },
        {   1,  55,  95 },
        {   1,  30,  53 },
        {   1,  11,  20 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {  19,  55, 240 },
        {  19,  59, 196 },
        {   3,  52, 105 }
      }, { /* Coeff Band 1 */
        {  41, 166, 207 },
        { 104, 153, 199 },
        {  31, 123, 181 },
        {  14, 101, 152 },
        {   5,  72, 106 },
        {   1,  36,  52 }
      }, { /* Coeff Band 2 */
        {  35, 176, 211 },
        {  12, 131, 190 },
        {   2,  88, 144 },
        {   1,  60, 101 },
        {   1,  36,  60 },
        {   1,  16,  28 }
      }, { /* Coeff Band 3 */
        {  28, 183, 213 },
        {   8, 134, 191 },
        {   1,  86, 142 },
        {   1,  56,  96 },
        {   1,  30,  53 },
        {   1,  12,  20 }
      }, { /* Coeff Band 4 */
        {  20, 190, 215 },
        {   4, 135, 192 },
        {   1,  84, 139 },
        {   1,  53,  91 },
        {   1,  28,  49 },
        {   1,  11,  20 }
      }, { /* Coeff Band 5 */
        {  13, 196, 216 },
        {   2, 137, 192 },
        {   1,  86, 143 },
        {   1,  57,  99 },
        {   1,  32,  56 },
        {   1,  13,  24 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        { 211,  29, 217 },
        {  96,  47, 156 },
        {  22,  43,  87 }
      }, { /* Coeff Band 1 */
        {  78, 120, 193 },
        { 111, 116, 186 },
        {  46, 102, 164 },
        {  15,  80, 128 },
        {   2,  49,  76 },
        {   1,  18,  28 }
      }, { /* Coeff Band 2 */
        {  71, 161, 203 },
        {  42, 132, 192 },
        {  10,  98, 150 },
        {   3,  69, 109 },
        {   1,  44,  70 },
        {   1,  18,  29 }
      }, { /* Coeff Band 3 */
        {  57, 186, 211 },
        {  30, 140, 196 },
        {   4,  93, 146 },
        {   1,  62, 102 },
        {   1,  38,  65 },
        {   1,  16,  27 }
      }, { /* Coeff Band 4 */
        {  47, 199, 217 },
        {  14, 145, 196 },
        {   1,  88, 142 },
        {   1,  57,  98 },
        {   1,  36,  62 },
        {   1,  15,  26 }
      }, { /* Coeff Band 5 */
        {  26, 219, 229 },
        {   5, 155, 207 },
        {   1,  94, 151 },
        {   1,  60, 104 },
        {   1,  36,  62 },
        {   1,  16,  28 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        { 233,  29, 248 },
        { 146,  47, 220 },
        {  43,  52, 140 }
      }, { /* Coeff Band 1 */
        { 100, 163, 232 },
        { 179, 161, 222 },
        {  63, 142, 204 },
        {  37, 113, 174 },
        {  26,  89, 137 },
        {  18,  68,  97 }
      }, { /* Coeff Band 2 */
        {  85, 181, 230 },
        {  32, 146, 209 },
        {   7, 100, 164 },
        {   3,  71, 121 },
        {   1,  45,  77 },
        {   1,  18,  30 }
      }, { /* Coeff Band 3 */
        {  65, 187, 230 },
        {  20, 148, 207 },
        {   2,  97, 159 },
        {   1,  68, 116 },
        {   1,  40,  70 },
        {   1,  14,  29 }
      }, { /* Coeff Band 4 */
        {  40, 194, 227 },
        {   8, 147, 204 },
        {   1,  94, 155 },
        {   1,  65, 112 },
        {   1,  39,  66 },
        {   1,  14,  26 }
      }, { /* Coeff Band 5 */
        {  16, 208, 228 },
        {   3, 151, 207 },
        {   1,  98, 160 },
        {   1,  67, 117 },
        {   1,  41,  74 },
        {   1,  17,  31 }
      }
    }
  }
};
static const vp9_coeff_probs_model default_coef_probs_32x32[BLOCK_TYPES] = {
  { /* block Type 0 */
    { /* Intra */
      { /* Coeff Band 0 */
        {  17,  38, 140 },
        {   7,  34,  80 },
        {   1,  17,  29 }
      }, { /* Coeff Band 1 */
        {  37,  75, 128 },
        {  41,  76, 128 },
        {  26,  66, 116 },
        {  12,  52,  94 },
        {   2,  32,  55 },
        {   1,  10,  16 }
      }, { /* Coeff Band 2 */
        {  50, 127, 154 },
        {  37, 109, 152 },
        {  16,  82, 121 },
        {   5,  59,  85 },
        {   1,  35,  54 },
        {   1,  13,  20 }
      }, { /* Coeff Band 3 */
        {  40, 142, 167 },
        {  17, 110, 157 },
        {   2,  71, 112 },
        {   1,  44,  72 },
        {   1,  27,  45 },
        {   1,  11,  17 }
      }, { /* Coeff Band 4 */
        {  30, 175, 188 },
        {   9, 124, 169 },
        {   1,  74, 116 },
        {   1,  48,  78 },
        {   1,  30,  49 },
        {   1,  11,  18 }
      }, { /* Coeff Band 5 */
        {  10, 222, 223 },
        {   2, 150, 194 },
        {   1,  83, 128 },
        {   1,  48,  79 },
        {   1,  27,  45 },
        {   1,  11,  17 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        {  36,  41, 235 },
        {  29,  36, 193 },
        {  10,  27, 111 }
      }, { /* Coeff Band 1 */
        {  85, 165, 222 },
        { 177, 162, 215 },
        { 110, 135, 195 },
        {  57, 113, 168 },
        {  23,  83, 120 },
        {  10,  49,  61 }
      }, { /* Coeff Band 2 */
        {  85, 190, 223 },
        {  36, 139, 200 },
        {   5,  90, 146 },
        {   1,  60, 103 },
        {   1,  38,  65 },
        {   1,  18,  30 }
      }, { /* Coeff Band 3 */
        {  72, 202, 223 },
        {  23, 141, 199 },
        {   2,  86, 140 },
        {   1,  56,  97 },
        {   1,  36,  61 },
        {   1,  16,  27 }
      }, { /* Coeff Band 4 */
        {  55, 218, 225 },
        {  13, 145, 200 },
        {   1,  86, 141 },
        {   1,  57,  99 },
        {   1,  35,  61 },
        {   1,  13,  22 }
      }, { /* Coeff Band 5 */
        {  15, 235, 212 },
        {   1, 132, 184 },
        {   1,  84, 139 },
        {   1,  57,  97 },
        {   1,  34,  56 },
        {   1,  14,  23 }
      }
    }
  }, { /* block Type 1 */
    { /* Intra */
      { /* Coeff Band 0 */
        { 181,  21, 201 },
        {  61,  37, 123 },
        {  10,  38,  71 }
      }, { /* Coeff Band 1 */
        {  47, 106, 172 },
        {  95, 104, 173 },
        {  42,  93, 159 },
        {  18,  77, 131 },
        {   4,  50,  81 },
        {   1,  17,  23 }
      }, { /* Coeff Band 2 */
        {  62, 147, 199 },
        {  44, 130, 189 },
        {  28, 102, 154 },
        {  18,  75, 115 },
        {   2,  44,  65 },
        {   1,  12,  19 }
      }, { /* Coeff Band 3 */
        {  55, 153, 210 },
        {  24, 130, 194 },
        {   3,  93, 146 },
        {   1,  61,  97 },
        {   1,  31,  50 },
        {   1,  10,  16 }
      }, { /* Coeff Band 4 */
        {  49, 186, 223 },
        {  17, 148, 204 },
        {   1,  96, 142 },
        {   1,  53,  83 },
        {   1,  26,  44 },
        {   1,  11,  17 }
      }, { /* Coeff Band 5 */
        {  13, 217, 212 },
        {   2, 136, 180 },
        {   1,  78, 124 },
        {   1,  50,  83 },
        {   1,  29,  49 },
        {   1,  14,  23 }
      }
    }, { /* Inter */
      { /* Coeff Band 0 */
        { 197,  13, 247 },
        {  82,  17, 222 },
        {  25,  17, 162 }
      }, { /* Coeff Band 1 */
        { 126, 186, 247 },
        { 234, 191, 243 },
        { 176, 177, 234 },
        { 104, 158, 220 },
        {  66, 128, 186 },
        {  55,  90, 137 }
      }, { /* Coeff Band 2 */
        { 111, 197, 242 },
        {  46, 158, 219 },
        {   9, 104, 171 },
        {   2,  65, 125 },
        {   1,  44,  80 },
        {   1,  17,  91 }
      }, { /* Coeff Band 3 */
        { 104, 208, 245 },
        {  39, 168, 224 },
        {   3, 109, 162 },
        {   1,  79, 124 },
        {   1,  50, 102 },
        {   1,  43, 102 }
      }, { /* Coeff Band 4 */
        {  84, 220, 246 },
        {  31, 177, 231 },
        {   2, 115, 180 },
        {   1,  79, 134 },
        {   1,  55,  77 },
        {   1,  60,  79 }
      }, { /* Coeff Band 5 */
        {  43, 243, 240 },
        {   8, 180, 217 },
        {   1, 115, 166 },
        {   1,  84, 121 },
        {   1,  51,  67 },
        {   1,  16,   6 }
      }
    }
  }
};
#endif
