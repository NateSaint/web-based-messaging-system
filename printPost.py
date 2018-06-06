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

#update stream users to the passed amount
def updateUsers(fileName, toSet, username):
	#nothing to be printed here
	subprocess.call(["./db", "-updateViewed", fileName, str(toSet), username])

#print the post at the currPos
def printPost(currPos, streamname):
	#by default not printed, return 1 if printed
	printed = 0

	#call to get max posts in the stream
	subprocess.call(["./db", "-getMax", streamname])
	f = open("temp", "r")
	result = f.readline()
	max = int(result)
	f.close()
	os.remove("temp")

	#check if no new posts must be displayed to user
	if max == currPos:
		return 0

	#call to get file of post
	subprocess.call(["./db", "-printPost", streamname, str(currPos + 1)])
	f = open("temp", "r")
	for line in f:
		print (line[0:len(line) - 1],end="<br>")
		printed = 1
	f.close()
	os.remove("temp")

	return printed

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

filename = stream

printed = printPost(currMessage, filename)

if printed == 1:
	if currMessage >= getNum(filename, username):
		updateUsers(filename, 1 + currMessage, username)
else:
	print ("No New Messages To Display<br>", end="")
