project('c_kfc', ['c'],
  default_options: ['warning_level=1','c_std=gnu99','default_library=static'],
  subproject_dir: 'submodules/c_deps/subprojects',version: '0.0.1'
  )
cc = meson.get_compiler('c')
passh_program = find_program('passh', required : true)
sh_program = find_program('sh', required : true)
timeout_bin = find_program('timeout', required : true)
git = find_program('git', required: true)
pwd = meson.current_source_dir()
c_deps_dir = join_paths(meson.current_source_dir(),'submodules/c_deps')
make_program = find_program('make', required: true)
inc = [include_directories(
  '.',
  'kfc-utils',
  'submodules',
  'submodules/c_deps',
  'submodules/c_deps/submodules',
  'submodules/c_deps/submodules/iowow',
  'submodules/c_deps/submodules/iowow/src',
  'submodules/c_deps/submodules/iowow/src/log',
  'submodules/c_deps/submodules/iowow/src/fs',
  'submodules/c_deps/submodules/iowow/src/platform',
  'submodules/c_deps/submodules/incbin',
  'submodules/c_deps/submodules/c_ansi',
  'submodules/c_deps/submodules/c_darwin',
  'submodules/c_deps/submodules/c_deps/submodules/reproc',
  'submodules/c_deps/submodules/c_deps/submodules/reproc/reproc',
  'submodules/c_deps/submodules/c_deps/submodules/reproc/reproc/include',
  'submodules/c_deps/submodules/c_deps/submodules/reproc/reproc/include/reproc',
)]
palettes_hash = run_command(make_program, 'palettes-hash', check: true).stdout().strip()
#############################################################################
subdir('submodules')
#############################################################################
subdir('kfc-utils')
subdir('kfc-cli')
#//////////////////////////////////////////////////////////////////////
