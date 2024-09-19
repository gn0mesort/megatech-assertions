#!/usr/bin/env python3

from argparse import ArgumentParser
from pathlib import Path

import subprocess
import sys

def main() -> None:
    parser = ArgumentParser(description="Custom unit test script for assertion tests.")
    parser.add_argument("--expect-success", action="store_true", default=False,
                        help="Expect that the program will exit with a result of 0 and don't check for assertions.")
    parser.add_argument("--can-fail", action="store_true", default=False,
                        help="Expect that the program will exit with a non-zero result or fail to match the expected "
                             "output. A warning will be reported but the test will still pass.")
    parser.add_argument("PROGRAM", help="The test program to run.", type=Path)
    parser.add_argument("EXPECTED", nargs="?", default="",
                        help="The output that the test script should expect on stderr.", type=str)
    args = parser.parse_args()
    completed = subprocess.run([ args.PROGRAM.absolute() ], capture_output=True)
    if args.expect_success:
        if completed.returncode == 0:
            exit(0)
        print(f"A result of 0 was expected, but {completed.returncode} was received.", file=sys.stderr)
        if args.can_fail:
            exit(0)
        exit(1)
    if f"\"{args.EXPECTED}\"" in completed.stderr.decode('utf-8'):
        exit(0)
    print(f"\"{args.EXPECTED}\" was not in \"{completed.stderr.decode('utf-8').strip()}\"", file=sys.stderr)
    if args.can_fail:
        exit(0)
    exit(1)

if __name__ == "__main__":
    main()
