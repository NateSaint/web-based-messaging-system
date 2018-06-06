#!/usr/bin/python3

import sys
import os
import subprocess
from sys import stdin

username = ""
exists = 0
streams = ""

#get user name entered from command line
for arg in range(1, len(sys.argv)):
        if arg != 1:
                username += " " + sys.argv[arg]
        else:
                username += sys.argv[arg]

subprocess.call(["./db", "-getStreams", username]);
