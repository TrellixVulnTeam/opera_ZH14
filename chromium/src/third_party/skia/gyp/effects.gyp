{
  'targets': [
    {
      'target_name': 'effects',
      'product_name': 'skia_effects',
      'type': 'static_library',
      'standalone_static_library': 1,
      'includes': [
        'effects.gypi',
      ],
      'include_dirs': [
        '../include/config',
        '../include/core',
        '../include/effects',
        '../include/utils',
        '../src/core',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '../include/effects',
        ],
      },
      'sources': [
        'effects.gypi', # Makes the gypi appear in IDEs (but does not modify the build).
      ],
      'conditions': [
        ['skia_gpu == 1', {
          'include_dirs': [
            '../include/gpu',
            '../src/gpu',
          ],
        }],
      ],
    },
  ],
}

# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
