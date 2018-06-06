#!/usr/bin/python3

import sys
import os
import subprocess
from sys import stdin

username = ""
exists = 0

#get user name entered from command line
for arg in range(1, len(sys.argv)):
        if arg != 1:
                username += " " + sys.argv[arg]
        else:
                username += sys.argv[arg]

subprocess.call(["./db", "-checkLogin", username])
f = open("temp", "r");
line = f.readline()
if line[0] == '1':
	exists = 1
else:
	exists = 0
f.close()
os.remove("temp")

if username == "":
	exists = -1

print (exists)
