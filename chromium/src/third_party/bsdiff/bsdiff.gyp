# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'bsdiff',
      'type': 'executable',
      'dependencies': [
        '../bspatch/bspatch.gyp:bspatch',
      ],
      'conditions': [
        ['OS=="win"', {
          'link_settings': {
            'libraries': [
              '-lWs2_32.lib',
            ],
          }
        }],
      ],
      'sources': [
        'mbsdiff.cc',
      ],
    },
  ],
}
