project(
  'megatech-assertions',
  [ 'cpp' ],
  version: '1.0.0',
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
description = 'Megatech Assertion Library'
includes = [ include_directories('include') ]
config = configuration_data()
config.set('CONFIG_MAX_CODE_POINT_SIZE', get_option('max_code_point_size'))
config.set('CONFIG_ASSERTION_BUFFER_SIZE', get_option('assertion_buffer_size'))
if get_option('thread_safe_assertions').allowed()
  config.set('CONFIG_THREAD_SAFE_ASSERTIONS_ENABLED', 1)
endif
config_header = configure_file(configuration: config, input: files('generated/config.hpp'), output: 'config.hpp')
sources = [ config_header, files('src/megatech/assertions.cpp') ]
megatech_assertions_lib = library(meson.project_name(), sources, include_directories: includes,
                                  version: meson.project_version(), install: true)
megatech_assertions_dep = declare_dependency(link_with: megatech_assertions_lib, include_directories: includes)
install_headers(files('include/megatech/assertions.hpp'), install_dir: 'include/megatech')
pkgconfig = import('pkgconfig')
pkgconfig.generate(megatech_assertions_lib, url: 'https://github.com/gn0mesort/megatech-assertions',
                   description: description)
if get_option('tests').allowed()
  subdir('tests')
endif
doxygen = find_program('doxygen', disabler: true)
doc_env = environment()
doc_env.set('DOXYGEN_PROJECT_NAME', meson.project_name())
doc_env.set('DOXYGEN_PROJECT_NUMBER', meson.project_version())
doc_env.set('DOXYGEN_PROJECT_BRIEF', 'Debug Assertion Library')
doc_env.set('DOXYGEN_OUTPUT_DIRECTORY', meson.current_build_dir() / 'documentation')
doc_inputs = [ meson.current_source_dir() / 'include', files('documentation/namespaces.dox')[0].full_path(),
               files('README.md')[0].full_path() ]
doc_env.set('DOXYGEN_USE_MDFILE_AS_MAINPAGE', doc_inputs[2])
doc_env.set('DOXYGEN_PREDEFINED', '__DOXYGEN__=1')
doc_env.set('DOXYGEN_INPUT', ' '.join(doc_inputs))
doc_strips = [ meson.current_source_dir() / 'include' ]
doc_env.set('DOXYGEN_STRIP_FROM_PATH', ' '.join(doc_strips))
extra_css = files('documentation/extra.css')[0]
doc_env.set('DOXYGEN_HTML_EXTRA_STYLESHEET', extra_css.full_path())
doc_env.set('DOXYGEN_ENABLED_SECTIONS', ' '.join(get_option('enabled_doxygen_sections')))
custom_target(output: 'documentation', build_always_stale: true,
              command: [ doxygen, files('Doxyfile') ], env: doc_env)
