project('c_kfc', ['c'],default_options: ['warning_level=0','c_std=gnu99',],subproject_dir: 'submodules/c_deps/subprojects',version: '0.0.1')
cc = meson.get_compiler('c')
pwd = meson.current_source_dir()
c_deps_dir = join_paths(meson.current_source_dir(),'submodules/c_deps')
make_program = find_program('make', required: true)
subdir('meson')
inc = [include_directories(
  'kfc-utils',
  'kfc-test',
  '.',
  'submodules',
  'submodules/c_deps',
  'submodules/c_deps/submodules',
  'submodules/c_deps/submodules/c_ansi',
  'submodules/c_deps/submodules/c_darwin',
  'submodules/c_deps/submodules/reproc',
)]
palettes_hash = run_command(make_program, 'palettes-hash', check: true).stdout().strip()
#############################################################################
subdir('submodules')
#############################################################################
subdir('kfc-utils')
subdir('kfc-cli')
subdir('kfc-test')

#//////////////////////////////////////////////////////////////////////
