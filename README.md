# Megatech Assertions

[Assertions](https://en.wikipedia.org/wiki/Assertion_(software_development)) are an essential debugging feature.
However, C++20 only provides the most minimal run-time assertions. The `assert` macro doesn't even support the
feature set of `static_assert`. The Megatech Assertions library provides an expanded set of assertion macros for C++20.
This includes assertions with custom formatted messages, type-safe formatting based on the standard format library,
and thread-safe assertions.

# Dependencies

Megatech Assertions maintains a minimal set of dependencies. To build the library you will need:

- A standard C++20 compiler (e.g., a recent version of GCC or Clang) and run-time environment.
- A working implementation of the [Meson](https://mesonbuild.com/) build system and a compatible build backend (e.g.,
  [Ninja](https://ninja-build.org/))

At run-time the library only requires a standard C++20 run-time environment.

To build the library's tests, you will also need:

- A working Python 3 environment.

To build the HTML documentation, you will need:

- [Doxygen](https://doxygen.nl)

# Building

## Preparing the Build Environment

Before you can build Megatech Assertions, you need to fulfill its dependencies. To do this, you will have to install
a standard C++20 compiler environment in a system specific manner. You will also need Meson, and a valid backend for
it. If you have Python 3 available on your system, you can ensure Meson and Ninja are installed by creating a virtual
environment. To do this run:

```sh
# Create a virtual Python environment in the ".venv" directory
python3 -m venv .venv
```

Then you will need to activate the virtual environment. The exact method is system (and shell) dependent, but if
you're using [Bash](https://www.gnu.org/software/bash/) you can run:

```sh
source .venv/bin/activate
```

Finally, to install Meson and Ninja you would run:

```sh
python3 -m pip install -r requirements.txt
```

## Configuring the Build System

Before you can build the library, the build system will need to be configured. To generate a default build
configuration use:

```sh
# Create a directory called "build" and configure it using default options.
meson setup build
```

You can then display the current build configuration, including project specific options, using:

```sh
meson configure build
```

There are a few build options of note. First, Megatech Assertions is built as a static library by default. This isn't
Meson's default behavior, and if you prefer a dynamic library you will need to manually change it by running:

```sh
meson configure build -Ddefault_library=dynamic
```

Second, Megatech Assertions will attempt to use thread-safe assertions by default. This behavior creates more points
of failure, but helps to ensure that multiple assertion failures across parallel thread are reported before the
program aborts. To disable this behavior run:

```sh
meson configure build -Dthread_safe_assertions=disabled
```

Finally, formatted assertion messages are written into a static or thread-local buffer during assertion failure
processing. This helps to avoid dynamic allocation errors, but does require allocating a bunch of memory (per thread)
to ensure the buffer is a reasonable size. The exact amount of memory required can be controlled by configuring
the code point size (i.e., how many bytes are required to encode one character) and the number of code points in the
buffer (i.e., how many characters may appear in a message before truncation). To do this use:

```sh
meson configure build -Dmax_code_point_size=4
meson configure build -Dassertion_buffer_size=1000
```

The default size of the formatting buffer is 4000 bytes (i.e., big enough to code 1000 4-byte UTF-8 code points). If
either size is 0, then message formatting is totally disabled. This results in formatted assertions deferring to the
simpler unformatted code path.

## Compiling

Compiling works like any other Meson project. Just run:

```sh
meson compile -C build
```

This will generate all of the necessary files, and compile the library. To compile tests, you first need to enable
with:

```sh
meson configure build -Dtests=enabled
```

Then you can compile and run the test suite with:

```sh
meson test -C build
```

Many tests are dependent on your exact build configuration. For example, tests requiring the thread-safe assertion
feature aren't built when that feature is disabled. Despite assertions being disabled, generally, in release builds,
the test programs all explicitly enable assertions. This means that they will run for any build type.

# Installing

After building the library, you can install it with:

```sh
# Install all the files described by the configuration located in "build".
# You can add --dry-run to determine what would be installed without actually performing the installation.
meson install -C build
```

This installs the library file, the `megatech/assertions.hpp` header, and a
[`pkg-config`](https://www.freedesktop.org/wiki/Software/pkg-config/) generated by Meson.

# HTML Documentation

Megatech Assertions provides inline documentation for the Doxygen documentation generator. With Doxygen installed,
you can run:

```sh
# Build the target "documentation" in the current Meson project as described in the "build" directory.
meson compile -C build ./documentation
```

to generate HTML documentation. By default, only public functionality is documented. If you want the internal
documentation to be generated as well run:

```sh
meson configure build -Denabled_doxygen_sections="INTERNAL"
```

Finally, regenerate the documentation.

# Library Usage

Megatech Assertions provides two distinct interfaces. First, there is the `MEGATECH_ASSERT` interface. This interface
is a drop in replacement for the standard C++ `assert` macro. In addition to assertions, this library provides macros
for preconditions (`MEGATECH_PRECONDITION`) and postconditions (`MEGATECH_POSTCONDITION`). Precondition and
postcondition macros behave identically to assertions, but they might be clearer to readers in the appropriate
situations. Second, there is the `MEGATECH_ASSERT_MSG` interface. This interface allows client programs to provide a
custom format string and parameters along with the assertion expression. When an assertion fails, the message
formatting interface will render the message using the parameters it was given and write it to standard error
alongside the usual assertion diagnostics. The exact syntax and behavior of the formatter is dependent on your system
and configuration. When the standard `format` header is available, type-safe formatting will be used. Otherwise, the
fallback is to use `printf`-style formatting.

## Explicit Formatter Selection

Since exact behavior of the default `MEGATECH_*_MSG` macros might change depending on the available standard library
features, you can select the desired behavior specifically. To explicitly use `printf`-style formatting, replace
`MEGATECH_*_MSG` macros with `MEGATECH_*_MSG_PRINTF` macros. To explicitly use `format`-style formatting, replace
`MEGATECH_*_MSG` macros with `MEGATECH_*_MSG_FORMAT` macros.

## Thread Safety

The Megatech Assertions library attempts to be thread-safe. This means that it should capture assertion failures
across parallel threads. If two assertions fail relatively simultaneous a diagnostic message should be written to
standard error for each failure. Afterwards, the program aborts as normal. Thread-safe assertions are enabled by
default, but they can be disabled at compile time. When thread-safe assertions are disabled the library will
immediately print its diagnostic and abort regardless of other threads.

Due to the buffer used for assertion message formatting, thread-safe assertions can require one significant memory.
If the memory footprint of this is undesirable, you can disable either the buffer or thread-safe assertions.

## Possible Points of Failure

Essentially, there are three points of failure in this library: message formatting, thread synchronization, and
diagnostic reporting. When the library encounters an error it will still attempt to report as much information as it
can about failed assertions (e.g., the failing expression, its location, and what kind of error occurred). If you need
to minimize potential errors, you should disable the thread safe assertion feature and set the assertion buffer size
to 0 like so:

```sh
meson configure build -Dthread_safe_assertions=disabled -Dassertion_buffer_size=0
```

This will leave only one point of failure: diagnostic reporting. Very little can be done if the library can't write
to standard error, so this is silently ignored. Regardless of errors, the library always calls `std::abort()` on an
assertion failure.

Further usage information is provided in the HTML documentation.

# Licensing

Copyright (C) 2024 Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along with this program. If not, see
<[https://www.gnu.org/licenses/](https://www.gnu.org/licenses/)>.
