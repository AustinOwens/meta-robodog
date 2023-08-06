#!/usr/bin/env python3

import os
import signal
import sys
import time

RGB_BAR = "0x41260000"
RGB_BAR_MAX_OFFSET = 0x3F
TIME_INTERVAL = 0.2


def sig_handler(sig, frame):
    os.system("devmem2 {} b 0x00 > /dev/null".format(RGB_BAR))
    sys.exit(0)


signal.signal(signal.SIGINT, sig_handler)

rgb1 = {
    "": 0b000_000,
    "r": 0b000_001,
    "g": 0b000_010,
    "y": 0b000_011,
    "b": 0b000_100,
    "m": 0b000_101,
    "c": 0b000_110,
    "w": 0b000_111,
}
rgb2 = {
    "": 0b000_000,
    "r": 0b001_000,
    "g": 0b010_000,
    "y": 0b011_000,
    "b": 0b100_000,
    "m": 0b101_000,
    "c": 0b110_000,
    "w": 0b111_000,
}

pattern = [
    ["", ""],
    ["", "r"],
    ["r", "r"],
    ["r", "g"],
    ["g", "g"],
    ["g", "b"],
    ["b", "b"],
    ["b", "y"],
    ["y", "y"],
    ["y", "m"],
    ["m", "m"],
    ["m", "c"],
    ["c", "c"],
    ["c", "w"],
    ["w", "w"],
    ["w", "c"],
    ["c", "c"],
    ["c", "m"],
    ["m", "m"],
    ["m", "y"],
    ["y", "y"],
    ["y", "b"],
    ["b", "b"],
    ["b", "g"],
    ["g", "g"],
    ["g", "r"],
    ["r", "r"],
    ["r", ""],
    ["", ""],
]

while True:
    for rgb1_val, rgb2_val in pattern:
        os.system(
            "devmem2 {} b {} > /dev/null".format(
                RGB_BAR, rgb1[rgb1_val] | rgb2[rgb2_val]
            )
        )
        time.sleep(TIME_INTERVAL)

    for rgb1_val, rgb2_val in pattern[::-1]:
        os.system(
            "devmem2 {} b {} > /dev/null".format(
                RGB_BAR, rgb1[rgb1_val] | rgb2[rgb2_val]
            )
        )
        time.sleep(TIME_INTERVAL)
