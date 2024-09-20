project(
  'megatech-assertions',
  [ 'cpp' ],
  version: '0.1.0',
  license: 'AGPL-3.0-or-later',
  license_files: [ 'LICENSE' ],
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'b_ndebug=if-release',
    'b_pie=true',
    'cpp_args=-fstrict-aliasing -Wstrict-aliasing=3',
    'default_library=static'
  ]
)
includes = [ include_directories('include') ]
sources = [ files('src/megatech/assertions.cpp') ]

megatech_assertions_lib = library(meson.project_name(), sources, include_directories: includes,
                                  version: meson.project_version())
megatech_assertions_dep = declare_dependency(link_with: megatech_assertions_lib, include_directories: includes)
if get_option('tests').allowed()
  subdir('tests')
endif
