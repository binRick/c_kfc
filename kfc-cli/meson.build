kfc_cli_srcs = [
  'kfc-cli.c',
  'kfc-cli.h',
]
kfc_include_dirs = [
  '.',
  '../kfc-utils',
  inc,
]
kfc_include_cargs = [
  '-D__PALETTES_HASH__="'+palettes_hash+'"',
  ]
kfc_cli_deps = [
  cargs_dep,
  tempdir_dep,
  rgba_dep,
  vt100utils_dep,
  kfc_utils_dep,
  bytes_dep,
  hsluv_c_dep,
  logh_dep,
  c_stringfn_dep,
  reproc_dep,
  #//debug_memory_dep,
]
if get_option('enable-binaries')
  kfc_cli_exec = executable('kfc',
     kfc_cli_srcs,
     dependencies: kfc_cli_deps,
     include_directories: kfc_include_dirs,
     c_args: kfc_include_cargs,
     install: true,
  )
  if get_option('enable-binary-clis')
  if get_option('enable-scc-report')
    test('Source Report', scc_program,
      workdir:meson.current_source_dir()+'/../../',
      args: [
        '-i','c','--not-match','./submodules/*',
        '--by-file','--wide','--exclude-dir','submodules',
        '--not-match','backup','--not-match','kfc-utils/kfc-utils-palettes.c',
      ],
      )
  endif
    test('KFC Help', kfc_cli_exec, args: ['--help'])
    test('KFC List Palettes', kfc_cli_exec, args: ['-l'])
    test('KFC Test Colors', kfc_cli_exec, args: ['--test-colors'])
    test('KFC Test Kitty Socket', kfc_cli_exec, args: ['--test-kitty-socket'])
    test('KFC Color Demo', kfc_cli_exec, args: ['-R'])
    test('KFC Palettes Table', kfc_cli_exec, args: ['--palettes-table'])
    test('KFC Terminal Detection', kfc_cli_exec, args: ['--detect-terminal'])
    test('KFC Unique Palettes', kfc_cli_exec, args: ['-U'])
    test('KFC Reset Terminal Palette', kfc_cli_exec, args: ['--reset-terminal'])
    test('KFC Invalid Properties', kfc_cli_exec, args: ['-I'])
    test('KFC Dark Background Palettes', kfc_cli_exec, args: ['--dark-backgrounds'])
    test('KFC Very Dark Background Palettes', kfc_cli_exec, args: ['--dark-backgrounds','-B','98'])
    test('KFC Bright Background Palettes', kfc_cli_exec, args: ['--bright-backgrounds'])
    test('KFC Very Bright Background Palettes', kfc_cli_exec, args: ['--bright-backgrounds','-B','2'])
    test('KFC Render Palettes Template', kfc_cli_exec, args: ['-u'], workdir:meson.current_source_dir()+'/../', )
    test('KFC Random Palette Sequence', kfc_cli_exec, args: ['--random-palette','--print-palette-sequence'])
    test('KFC Random Palette Data', kfc_cli_exec, args: ['--random-palette','--print-palette-data'])
    test('KFC Palette History', kfc_cli_exec, args: ['--random-palette','--print-palette-history'])
    test('KFC Random Palette Properties Table', kfc_cli_exec, args: ['--random-palette','--palette-properties-table'])
  endif
endif
