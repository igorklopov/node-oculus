#!/usr/bin/python

{
  'targets': [
    {
      'target_name': 'nodeOculus',
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        'vendor/LibOVR/Include',
      ],
      'sources': [
        'Device.cc',
        'nodeOculus.cc',
      ],
      'conditions': [
        ['OS=="win"', {
          'libraries': [
            '-lLibOVR.lib',
          ],
          'configurations': {
            'Release': {
              'msvs_settings': {
                'VCLinkerTool': {
                  'AdditionalLibraryDirectories': [
                    '../vendor/LibOVR/Lib/Windows/x64/Release/VS2013',
                  ],
                },
              },
            },
          },
        }],
      ],
    },
  ],
}
