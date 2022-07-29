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
  ]
kfc_cli_deps = [
  cargs_dep,
  kfc_utils_dep,
  debug_memory_dep,
]
if get_option('enable-binaries')
  kfc_cli_exec = executable('kfc-cli',
     kfc_cli_srcs,
     dependencies: kfc_cli_deps,
     include_directories: kfc_include_dirs,
     c_args: kfc_include_cargs,
     install: true,
  )
  if get_option('enable-binary-clis')
    test('kfc-cli', passh_program, args: [kfc_cli_exec,'-l'])
  endif
endif
