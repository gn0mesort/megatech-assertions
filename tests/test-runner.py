#!/usr/bin/env python3

from argparse import ArgumentParser
from pathlib import Path

import subprocess
import sys

def run_test_code(program: Path, tries: int = 1) -> int:
    for i in range(tries):
        completed = subprocess.run([ program ])
        if completed.returncode != 0:
            print(f"Attempt #{i}: A result of 0 was expected, but {completed.returncode} was received.",
                  file=sys.stderr)
        else:
            if (tries > 1):
                print(f"Attempt #{i} passed.", file=sys.stderr)
            return 0
        print(f"Attempt #{i} failed.", file=sys.stderr)
    return 1

def run_test_exact_str(program: Path, expected: list[str], tries: int = 1) -> int:
    for i in range(tries):
        completed = subprocess.run([ program ], capture_output=True)
        found = True
        for text in expected:
            if f"{text}" != completed.stderr.decode("utf-8"):
                print(f"Attempt #{i}: \"{text}\" was not \"{completed.stderr.decode('utf-8').strip()}\"",
                      file=sys.stderr)
                found = False
        if found:
            if (tries > 1):
                print(f"Attempt #{i} passed.", file=sys.stderr)
            return 0
        print(f"Attempt #{i} failed.", file=sys.stderr)
    return 1

def run_test_str(program: Path, expected: list[str], tries: int = 1) -> int:
    for i in range(tries):
        completed = subprocess.run([ program ], capture_output=True)
        found = True
        for text in expected:
            if f"{text}" not in completed.stderr.decode("utf-8"):
                print(f"Attempt #{i}: \"{text}\" was not in \"{completed.stderr.decode('utf-8').strip()}\"",
                      file=sys.stderr)
                found = False
        if found:
            if (tries > 1):
                print(f"Attempt #{i} passed.", file=sys.stderr)
            return 0
        print(f"Attempt #{i} failed.", file=sys.stderr)
    return 1

def main() -> None:
    parser = ArgumentParser(description="Custom unit test script for assertion tests.")
    parser.add_argument("--expect-success", action="store_true", default=False,
                        help="Expect that the program will exit with a result of 0 and don't check for assertions.")
    parser.add_argument("--retry", type=int, default=1,
                        help="Expect that the program MAY fail and retry the given number of times. If this is less "
                             "than 1, it will be ignored.")
    parser.add_argument("--exact", action="store_true", default=False,
                        help="Require that output exactly matches the expected value. This has no effect if "
                             "\"--expect-success\" is enabled.")
    parser.add_argument("PROGRAM", help="The test program to run.", type=Path)
    parser.add_argument("EXPECTED", nargs="*", default="",
                        help="The output that the test script should expect on stderr.", type=str)
    args = parser.parse_args()
    tries = args.retry if args.retry > 1 else 1
    if args.expect_success:
        exit(run_test_code(args.PROGRAM, tries))
    if args.exact:
        exit(run_test_exact_str(args.PROGRAM, args.EXPECTED, tries))
    exit(run_test_str(args.PROGRAM, args.EXPECTED, tries))

if __name__ == "__main__":
    main()
