project('c_kfc', ['c'],default_options: ['warning_level=0','c_std=gnu99',],subproject_dir: 'submodules/c_deps/subprojects',version: '0.0.1')
cc = meson.get_compiler('c')
unlink_program = find_program('unlink', required : true)
passh_program = find_program('passh', required : true)
sqlite3_program = find_program('sqlite3', required : true)
sh_program = find_program('sh', required : true)
wg_program = find_program('wg', required : true)
scc_program = find_program('scc', required : true)
passh_program = find_program('passh', required : true)
timeout_bin = find_program('timeout', required : true)
sudo_bin = find_program('sudo', required : true)
git = find_program('git', required: true)
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
