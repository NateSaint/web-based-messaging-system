#!/usr/bin/python3

import sys
import os
import subprocess

#get the amount of posts in stream
def getMax(streamname):
        #call to get max posts in the stream
        subprocess.call(["./db", "-getMax", streamname])
        f = open("temp", "r")
        result = f.readline()
        max = int(result)
        f.close()
        os.remove("temp")
        return max

#update stream users to the passed amount
def updateUsers(fileName, toSet, username):
        #nothing to be printed here
        subprocess.call(["./db", "-updateViewed", fileName, str(toSet), username])

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

max = getMax(filename)

if max != 0:
        updateUsers(filename, max, username)
else:
	updateUsers(filename, 0, username)
