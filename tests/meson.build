pymod = import('python')
python3 = pymod.find_installation('python3')
runner = find_program('test-runner.py')
args = [ '-UNDEBUG' ]
buffer_size = get_option('max_code_point_size') * get_option('assertion_buffer_size')
max_test_threads = 25
buffer = [ ]
foreach i : range(buffer_size)
  buffer += 'a'
endforeach
test_truncation_result = '"@0@"'.format(''.join(buffer))
foreach i : range(100)
  buffer += 'b'
endforeach
test_parallel_assert_msg_fail_results = [ ]
dependencies = [ megatech_assertions_dep ]
config = configuration_data()
config.set_quoted('CONFIG_TEST_TRUNCATION_STRING', ''.join(buffer))
config.set('CONFIG_TEST_MAX_THREADS', max_test_threads)
config_header = configure_file(configuration: config, input: files('generated/config.hpp'), output: 'config.hpp')
test_assert_msg_fail_exe = disabler()
test_assert_msg_fail_printf_exe = disabler()
test_parallel_assert_msg_fail_exe = disabler()
test_truncate_assert_msg_fail_printf_exe = disabler()
if buffer_size > 0
  test_assert_msg_fail_exe = executable('test-assert-msg-fail', files('test_assert_msg_fail.cpp'),
                                        dependencies: dependencies, cpp_args: args)
  test_assert_msg_fail_printf_exe = executable('test-assert-msg-fail-printf', files('test_assert_msg_fail_printf.cpp'),
                                               dependencies: dependencies, cpp_args: args)
  if get_option('thread_safe_assertions').allowed() and max_test_threads > 1
    test_parallel_assert_msg_fail_exe = executable('test-parallel-assert-msg-fail',
                                                   [ config_header, files('test_parallel_assert_msg_fail.cpp') ],
                                                   dependencies: dependencies, cpp_args: args)
    foreach i : range(max_test_threads)
      test_parallel_assert_msg_fail_results += '"Thread @0@"'.format(i)
    endforeach
  endif
  test_truncate_assert_msg_fail_printf_exe = executable('test-truncate-assert-msg-fail-printf',
                                                        [ config_header,
                                                          files('test_truncate_assert_msg_fail_printf.cpp') ],
                                                        dependencies: dependencies, cpp_args: args)
endif
test_assert_fail_exe = executable('test-assert-fail', files('test_assert_fail.cpp'),
                                  dependencies: dependencies, cpp_args: args)
test_assert_pass_exe = executable('test-assert-pass', files('test_assert_pass.cpp'),
                                  dependencies: dependencies, cpp_args: args)
test_assert_msg_fail_format_exe = disabler()
test_assert_msg_fail_disable_format_exe = disabler()
test_truncate_assert_msg_fail_format_exe = disabler()
test_assert_msg_fail_format_error_exe = disabler()
if meson.get_compiler('cpp').has_header('format') and buffer_size > 0
  test_assert_msg_fail_format_exe = executable('test-assert-msg-fail-format', files('test_assert_msg_fail_format.cpp'),
                                               dependencies: dependencies, cpp_args: args)
  test_assert_msg_fail_disable_format_exe = executable('test-assert-msg-fail-disable-format',
                                                       files('test_assert_msg_fail_disable_format.cpp'),
                                                       dependencies: dependencies, cpp_args: args)
  test_truncate_assert_msg_fail_format_exe = executable('test-truncate-assert-msg-fail-format',
                                                        [ config_header,
                                                          files('test_truncate_assert_msg_fail_format.cpp') ],
                                                        dependencies: dependencies, cpp_args: args)
  test_assert_msg_fail_format_error_exe = executable('test-assert-msg-fail-format-error',
                                                     files('test_assert_msg_fail_format_error.cpp'),
                                                     dependencies: dependencies, cpp_args: args)
endif
test_disable_assertions_exe = executable('test-disable-assertions', files('test_disable_assertions.cpp'),
                                         dependencies: dependencies, cpp_args: args)
test_exact_assert_fail_exe = executable('test-exact-assert-fail', files('test_exact_assert_fail.cpp'),
                                        dependencies: dependencies, cpp_args: args)
test_exact_assert_msg_fail_exe = executable('test-exact-assert-msg-fail', files('test_exact_assert_msg_fail.cpp'),
                                        dependencies: dependencies, cpp_args: args)
test_precondition_is_assert_exe = executable('test-precondition-is-assert', files('test_precondition_is_assert.cpp'),
                                             dependencies: dependencies, cpp_args: args)
test_precondition_msg_is_assert_msg_exe = executable('test-precondition-msg-is-assert-msg',
                                                     files('test_precondition_msg_is_assert_msg.cpp'),
                                                     dependencies: dependencies, cpp_args: args)
test_postcondition_is_assert_exe = executable('test-postcondition-is-assert',
                                              files('test_postcondition_is_assert.cpp'),
                                              dependencies: dependencies, cpp_args: args)
test_postcondition_msg_is_assert_msg_exe = executable('test-postcondition-msg-is-assert-msg',
                                                      files('test_postcondition_msg_is_assert_msg.cpp'),
                                                      dependencies: dependencies, cpp_args: args)
test('Assertion Failure', runner, args: [ test_assert_fail_exe.full_path(), '"1 != 1"' ])
test('Assertion Pass', runner, args: [ '--expect-success', test_assert_pass_exe.full_path() ])
test('Assertion Failure with Message', runner, args: [ test_assert_msg_fail_exe.full_path(), '"test passed"' ])
test('Assertion Failure with Message and "printf" Formatting', runner,
     args: [ test_assert_msg_fail_printf_exe.full_path(), '"test passed"' ])
test('Assertion Failure with Message and "format" Formatting', runner,
     args: [ test_assert_msg_fail_format_exe.full_path(), '"test passed"' ])
test('Disable "format" Formatting', runner, args: [ test_assert_msg_fail_disable_format_exe.full_path(), '"test {}"' ])
test('Disable Assertions', runner, args: [ '--expect-success', test_disable_assertions_exe.full_path() ])
test('Parallel Assertions', runner,
     args: [ '--retry=5', test_parallel_assert_msg_fail_exe.full_path(), test_parallel_assert_msg_fail_results ])
test('Truncate Assertion Messages with "printf" Formatting', runner,
     args: [ test_truncate_assert_msg_fail_printf_exe.full_path(), test_truncation_result ])
test('Truncate Assertion Messages with "format" Formatting', runner,
     args: [ test_truncate_assert_msg_fail_format_exe.full_path(), test_truncation_result ])
test('Error During Assertion Failure', runner, args: [ test_assert_msg_fail_format_error_exe.full_path(),
                                                       'The following error' ])
path = meson.current_source_dir().replace(meson.source_root(), '..')
# These are brittle. If they break, double check that the output hasn't changed for some reason.
test('Exact Assertion Match', runner,
     args: [ '--exact', test_exact_assert_fail_exe.full_path(),
             '@0@/test_exact_assert_fail.cpp:4: int main(): The assertion "1 != 1" failed.\n'.format(path) ])
test('Exact Assertion Match with Message', runner,
     args: [ '--exact', test_exact_assert_msg_fail_exe.full_path(),
             '@0@/test_exact_assert_msg_fail.cpp:4: int main(): The assertion "1 != 1" failed with the message "test passed".\n'.format(path) ])
test('Precondition is Assertion', runner,
     args: [ '--exact', test_precondition_is_assert_exe.full_path(),
             '@0@/test_precondition_is_assert.cpp:4: int main(): The assertion "1 != 1" failed.\n'.format(path) ])
test('Precondition with Message is Assertion with Message', runner,
     args: [ '--exact', test_precondition_msg_is_assert_msg_exe.full_path(),
             '@0@/test_precondition_msg_is_assert_msg.cpp:4: int main(): The assertion "1 != 1" failed with the message "test passed".\n'.format(path) ])
test('Postcondition is Assertion', runner,
     args: [ '--exact', test_postcondition_is_assert_exe.full_path(),
             '@0@/test_postcondition_is_assert.cpp:4: int main(): The assertion "1 != 1" failed.\n'.format(path) ])
test('Postcondition with Message is Assertion with Message', runner,
     args: [ '--exact', test_postcondition_msg_is_assert_msg_exe.full_path(),
             '@0@/test_postcondition_msg_is_assert_msg.cpp:4: int main(): The assertion "1 != 1" failed with the message "test passed".\n'.format(path) ])
