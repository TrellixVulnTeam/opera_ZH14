/*
 *  Copyright (c) 2012 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "third_party/googletest/src/include/gtest/gtest.h"

extern "C" {
#include "vp9_rtcd.h"
void vp9_short_idct8x8_add_c(short *input, uint8_t *output, int pitch);
}

#include "acm_random.h"
#include "vpx/vpx_integer.h"

using libvpx_test::ACMRandom;

namespace {

TEST(VP9Fdct8x8Test, SignBiasCheck) {
  ACMRandom rnd(ACMRandom::DeterministicSeed());
  int16_t test_input_block[64];
  int16_t test_output_block[64];
  const int pitch = 16;
  int count_sign_block[64][2];
  const int count_test_block = 100000;

  memset(count_sign_block, 0, sizeof(count_sign_block));

  for (int i = 0; i < count_test_block; ++i) {
    // Initialize a test block with input range [-255, 255].
    for (int j = 0; j < 64; ++j)
      test_input_block[j] = rnd.Rand8() - rnd.Rand8();

    vp9_short_fdct8x8_c(test_input_block, test_output_block, pitch);

    for (int j = 0; j < 64; ++j) {
      if (test_output_block[j] < 0)
        ++count_sign_block[j][0];
      else if (test_output_block[j] > 0)
        ++count_sign_block[j][1];
    }
  }

  for (int j = 0; j < 64; ++j) {
    const int diff = abs(count_sign_block[j][0] - count_sign_block[j][1]);
    const int max_diff = 1125;
    EXPECT_LT(diff, max_diff)
        << "Error: 8x8 FDCT has a sign bias > "
        << 1. * max_diff / count_test_block * 100 << "%"
        << " for input range [-255, 255] at index " << j
        << " count0: " << count_sign_block[j][0]
        << " count1: " << count_sign_block[j][1]
        << " diff: " << diff;
  }

  memset(count_sign_block, 0, sizeof(count_sign_block));

  for (int i = 0; i < count_test_block; ++i) {
    // Initialize a test block with input range [-15, 15].
    for (int j = 0; j < 64; ++j)
      test_input_block[j] = (rnd.Rand8() >> 4) - (rnd.Rand8() >> 4);

    vp9_short_fdct8x8_c(test_input_block, test_output_block, pitch);

    for (int j = 0; j < 64; ++j) {
      if (test_output_block[j] < 0)
        ++count_sign_block[j][0];
      else if (test_output_block[j] > 0)
        ++count_sign_block[j][1];
    }
  }

  for (int j = 0; j < 64; ++j) {
    const int diff = abs(count_sign_block[j][0] - count_sign_block[j][1]);
    const int max_diff = 10000;
    EXPECT_LT(diff, max_diff)
        << "Error: 4x4 FDCT has a sign bias > "
        << 1. * max_diff / count_test_block * 100 << "%"
        << " for input range [-15, 15] at index " << j
        << " count0: " << count_sign_block[j][0]
        << " count1: " << count_sign_block[j][1]
        << " diff: " << diff;
  }
};

TEST(VP9Fdct8x8Test, RoundTripErrorCheck) {
  ACMRandom rnd(ACMRandom::DeterministicSeed());
  int max_error = 0;
  double total_error = 0;
  const int count_test_block = 100000;
  for (int i = 0; i < count_test_block; ++i) {
    int16_t test_input_block[64];
    int16_t test_temp_block[64];
    uint8_t dst[64], src[64];

    for (int j = 0; j < 64; ++j) {
      src[j] = rnd.Rand8();
      dst[j] = rnd.Rand8();
    }
    // Initialize a test block with input range [-255, 255].
    for (int j = 0; j < 64; ++j)
      test_input_block[j] = src[j] - dst[j];

    const int pitch = 16;
    vp9_short_fdct8x8_c(test_input_block, test_temp_block, pitch);
    for (int j = 0; j < 64; ++j){
        if(test_temp_block[j] > 0) {
          test_temp_block[j] += 2;
          test_temp_block[j] /= 4;
          test_temp_block[j] *= 4;
        } else {
          test_temp_block[j] -= 2;
          test_temp_block[j] /= 4;
          test_temp_block[j] *= 4;
        }
    }
    vp9_short_idct8x8_add_c(test_temp_block, dst, 8);

    for (int j = 0; j < 64; ++j) {
      const int diff = dst[j] - src[j];
      const int error = diff * diff;
      if (max_error < error)
        max_error = error;
      total_error += error;
    }
  }

  EXPECT_GE(1, max_error)
      << "Error: 8x8 FDCT/IDCT has an individual roundtrip error > 1";

  EXPECT_GE(count_test_block/5, total_error)
      << "Error: 8x8 FDCT/IDCT has average roundtrip error > 1/5 per block";
};

TEST(VP9Fdct8x8Test, ExtremalCheck) {
  ACMRandom rnd(ACMRandom::DeterministicSeed());
  int max_error = 0;
  double total_error = 0;
  const int count_test_block = 100000;
  for (int i = 0; i < count_test_block; ++i) {
    int16_t test_input_block[64];
    int16_t test_temp_block[64];
    uint8_t dst[64], src[64];

    for (int j = 0; j < 64; ++j) {
      src[j] = rnd.Rand8() % 2 ? 255 : 0;
      dst[j] = src[j] > 0 ? 0 : 255;
    }
    // Initialize a test block with input range [-255, 255].
    for (int j = 0; j < 64; ++j)
      test_input_block[j] = src[j] - dst[j];

    const int pitch = 16;
    vp9_short_fdct8x8_c(test_input_block, test_temp_block, pitch);
    vp9_short_idct8x8_add_c(test_temp_block, dst, 8);

    for (int j = 0; j < 64; ++j) {
      const int diff = dst[j] - src[j];
      const int error = diff * diff;
      if (max_error < error)
        max_error = error;
      total_error += error;
    }

    EXPECT_GE(1, max_error)
        << "Error: Extremal 8x8 FDCT/IDCT has an"
        << " individual roundtrip error > 1";

    EXPECT_GE(count_test_block/5, total_error)
        << "Error: Extremal 8x8 FDCT/IDCT has average"
        << " roundtrip error > 1/5 per block";
  }
};

}  // namespace
