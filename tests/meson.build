runner = find_program('test-runner.py')
args = [ '-UNDEBUG' ]
dependencies = [ megatech_assertions_dep ]
test_assert_fail_exe = executable('test-assert-fail', files('test_assert_fail.cpp'),
                                  dependencies: dependencies, cpp_args: args)
test_assert_pass_exe = executable('test-assert-pass', files('test_assert_pass.cpp'),
                                  dependencies: dependencies, cpp_args: args)
test_assert_msg_fail_exe = executable('test-assert-msg-fail', files('test_assert_msg_fail.cpp'),
                                      dependencies: dependencies, cpp_args: args)
test_assert_msg_fail_printf_exe = executable('test-assert-msg-fail-printf', files('test_assert_msg_fail_printf.cpp'),
                                             dependencies: dependencies, cpp_args: args)
test_assert_msg_fail_format_exe = disabler()
test_assert_msg_fail_disable_format_exe = disabler()
if meson.get_compiler('cpp').has_header('format')
  test_assert_msg_fail_format_exe = executable('test-assert-msg-fail-format', files('test_assert_msg_fail_format.cpp'),
                                               dependencies: dependencies, cpp_args: args)
  test_assert_msg_fail_disable_format_exe = executable('test-assert-msg-fail-disable-format',
                                                       files('test_assert_msg_fail_disable_format.cpp'),
                                                       dependencies: dependencies, cpp_args: args)
endif
test_disable_assertions_exe = executable('test-disable-assertions', files('test_disable_assertions.cpp'),
                                         dependencies: dependencies, cpp_args: args)
test('Assertion Failure', runner, args: [ test_assert_fail_exe.full_path(), '1 != 1' ])
test('Assertion Pass', runner, args: [ '--expect-success', test_assert_pass_exe.full_path() ])
test('Assertion Failure with Message', runner, args: [ test_assert_msg_fail_exe.full_path(), 'test passed' ])
test('Assertion Failure with Message and "printf" Formatting', runner,
     args: [ test_assert_msg_fail_printf_exe.full_path(), 'test passed' ])
test('Assertion Failure with Message and "format" Formatting', runner,
     args: [ test_assert_msg_fail_format_exe.full_path(), 'test passed' ])
test('Disable "format" Formatting', runner, args: [ test_assert_msg_fail_disable_format_exe.full_path(), 'test {}' ])
test('Disable Assertions', runner, args: [ '--expect-success', test_disable_assertions_exe.full_path() ])
