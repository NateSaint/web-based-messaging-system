#!/usr/bin/python3

import sys
import os

currMessage = 0
stream = ""
username = ""
userstream = 0

#get user name entered from command line
for arg in range(1, len(sys.argv)):
        if sys.argv[arg] == '.':
                userstream = userstream + 1
        if userstream == 0:
                currMessage = int(sys.argv[arg])
        elif sys.argv[arg] != '.' and userstream == 1:
                if sys.argv[arg + 1] != '.':
                        stream += sys.argv[arg] + " "
                else:
                        stream += sys.argv[arg]
        elif sys.argv[arg] != '.' and userstream == 2:
                if arg + 1 != len(sys.argv):
                        username += sys.argv[arg] + " "
                else:
                        username += sys.argv[arg]

filename = "messages/" + stream + "Stream"

if currMessage != 0:
	print (currMessage - 1)
else:
	print (currMessage)
