kfc_test_srcs = [
  'kfc-test.c',
  'kfc-test.h',
]
kfc_include_dirs = [
  '.',
  '../kfc-utils',
  inc,
]
kfc_include_cargs = [
  ]
kfc_test_deps = [
  kfc_utils_dep,
  #//  debug_memory_dep,
]
if get_option('enable-binaries')
  kfc_test_exec = executable('kfc-test',
     kfc_test_srcs,
     dependencies: kfc_test_deps,
     include_directories: kfc_include_dirs,
     c_args: kfc_include_cargs,
  )
  if get_option('enable-binary-tests')
  endif
endif
