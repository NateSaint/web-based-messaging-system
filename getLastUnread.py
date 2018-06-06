#!/usr/bin/python3

import sys
import os
import subprocess

#get the int at the end of strings in various text files
def getNum(fileName, username):
	subprocess.call(["./db", "-getLastUnread", filename, username])
	f = open("temp", "r")
	result = f.readline()
	f.close()
	os.remove("temp")
	return int(result)

username = ""
stream = ""
userstream = 0

#get user name entered from command line
for arg in range(1, len(sys.argv)):
	if sys.argv[arg] == '.':
		userstream = 1
	if userstream == 0:
		if arg != 1:
			username += " " + sys.argv[arg]
		else:
			username += sys.argv[arg]
	elif sys.argv[arg] != '.':
		if arg + 1 != len(sys.argv):
			stream += sys.argv[arg] + " "
		else:
			stream += sys.argv[arg]

filename = stream

subprocess.call(["./db", "-verifyStream", stream])
f = open("temp", "r")
result = f.readline()
f.close()
os.remove("temp");

#exists
if result[0] == '1':
	print (getNum(filename, username))
else:
	print (-1)
