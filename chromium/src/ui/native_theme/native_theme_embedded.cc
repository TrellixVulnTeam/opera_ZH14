// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Modified by Opera Software ASA

#include "ui/native_theme/native_theme_embedded.h"

#include "base/basictypes.h"
#include "base/logging.h"

namespace ui {

// static
NativeTheme* NativeTheme::instance() {
  return NativeThemeEmbedded::instance();
}

// static
NativeThemeEmbedded* NativeThemeEmbedded::instance() {
  CR_DEFINE_STATIC_LOCAL(NativeThemeEmbedded, s_native_theme, ());
  return &s_native_theme;
}

SkColor NativeThemeEmbedded::GetSystemColor(ColorId color_id) const {
  NOTIMPLEMENTED();
  return SK_ColorBLACK;
}

NativeThemeEmbedded::NativeThemeEmbedded() {
}

NativeThemeEmbedded::~NativeThemeEmbedded() {
}

}  // namespace ui
