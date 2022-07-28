project('c_kfc', ['c'],default_options: ['warning_level=0','c_std=gnu99',],subproject_dir: 'submodules/meson_deps/subprojects')
cc = meson.get_compiler('c')
pwd = meson.current_source_dir()
meson_deps_dir = join_paths(meson.current_source_dir(),'submodules/meson_deps')
subdir('meson')
inc = [include_directories(
  'kfc-utils',
  'kfc-test',
  '.',
  'submodules','submodules/meson_deps',
  'submodules/meson_deps/submodules/c_ansi',
)]
#############################################################################
subdir('submodules')
#############################################################################
subdir('kfc-utils')
subdir('kfc-cli')
subdir('kfc-test')
#//////////////////////////////////////////////////////////////////////
