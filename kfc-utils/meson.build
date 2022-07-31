kfc_utils_srcs = [
  'kfc-utils-palettes.c',
  'kfc-utils-module.c',
  'kfc-utils-data.c',
  'kfc-utils.c',
]
kfc_utils_link_flags = []
kfc_utils_c_args = [
  '-D__PALETTES_HASH__="'+palettes_hash+'"',
]
kfc_utils_deps = [
  ansi_codes_dep,
  djbhash_dep,
  termpaint_dep,
  process_dep,
  kitty_dep,
  c_stringfn_dep,
  tiny_regex_c_dep,
  b64_dep,
  module_dep,
  c_string_buffer_dep,
  fort_dep,
  c_vector_dep,
  ansi_utils_dep,
  debug_memory_dep,
  which_dep,
  strdup_dep,
  parson_dep,
  c_fsio_dep,  
  exec_fzf_dep,  
  ansi_rgb_utils_dep,  
  hsluv_c_dep,  
  string_utils_dep,
  bytes_dep,
  dbg_dep,
  bench_dep,
  timestamp_dep,
  tempdir_dep,
]
kfc_utils_dirs = [
  '.',
  inc,
]


kfc_utils_lib = static_library('kfc-utils',
  kfc_utils_srcs,
  install: false,
  dependencies: kfc_utils_deps,
  c_args: kfc_utils_c_args,
  include_directories: kfc_utils_dirs,
  link_args: kfc_utils_link_flags,
)
kfc_utils_dep = declare_dependency(
  include_directories: kfc_utils_dirs,
  link_with: kfc_utils_lib,
  link_args: kfc_utils_link_flags,
)
