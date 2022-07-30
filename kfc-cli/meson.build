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
  kfc_utils_dep,
  bytes_dep,
  hsluv_c_dep,
  debug_memory_dep,
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
    test('kfc', kfc_cli_exec, args: ['--help'])
  endif
endif
