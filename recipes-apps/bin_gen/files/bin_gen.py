#!/usr/bin/env python3
# By: Austin Owens
# Description: Creates a .bin file for the spidev_test tool

import sys

def display_help():
    """Display a help message."""
    help_message = """
Usage: bin_gen.py [OPTIONS] VALUES...

Convert given integer or hexadecimal values to raw binary and output to stdout.

Options:
  -h, --help    Display this help message and exit.

Examples:
  bin_gen.py 0x06 0x00 0x26 0x00 > tmp.bin
  bin_gen.py 6 0 44 0 > tmp.bin
    """
    print(help_message)

def convert_to_byte(arg):
    """Convert a string argument to a byte."""
    # If the argument starts with "0x", tread it as a hexadecimal.
    if arg.startswith("0x"):
        return int(arg, 16)
    return int(arg)

def main():
    # Check if help is needed or no arguments are provided.
    if not sys.argv[1:] or "-h" in sys.argv or "--help" in sys.argv:
        display_help()
        return

    # Convert arguments to raw bytes.
    raw_bytes = bytearray([convert_to_byte(arg) for arg in sys.argv[1:]])

    # Write the raw bytes to stdout.
    sys.stdout.buffer.write(raw_bytes)

if __name__ == "__main__":
    main()


