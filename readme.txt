****
****
PLEASE NOTE: For the assignment, it was manditory to keep all files in one directory for grading purposes or we got a 0%, hence the ugliness! (Professor's request)

Even though the project might be a little odd to follow and understand the manditory conventions that were to be followed, this project is certainly significant in demonstrating coding conventions and the use of C, C++, Python, PHP, HTML, MySQL, creating libraries, and makefiles. 
****
****

This is my course project for CIS * 2750, Software Systems Integration and Development. CIS * 2750 is known for having a high drop rate, often nearing the 60%-70% range. As you might expect, there were complications provided as part of the project, an overview of the project is provided just below describing what the project ended up as and what the main task for each assignment was.

	
	SUMMARY OF PROJECT:
	In the end, we were left with a web-based messaging system where users can create an account, as well as subscribe, view, create, and post messages to streams. 
	
	Assignment 1:
	    - C++ to C converter, enabling code written in C++ with classes to be converted to C,
	      the requirement was to code
	    - a1.c, convert.c, parser.c
	    
	Assignment 2:
	    - Utilize the Assignment one C++ to C converter by coding in C++ and compiling it as C
	    - Implement a stream system to write, view, and save messages under a user name
	    - All using a command line interface that creates/removes users, adds/removes them 
	      to a stream, allows a user to post to a stream, and manage streams
	    - User interface for viewing the screens involved a 80x24 character command line screen
	      to mimic scrolling through different posts in a stream (Using Python)
	    
	Assignment 3:
	    - Develop a markup language(.wpml) to display elements to a web browser
	    - Parse .wpml files using a C program which then outputs the PHP to be displayed to the 
	      web broswer
	    - Use the created markup language to design a web app for the messaging system on the 
	      provided web server
	    - Use the command line interfaces by calling the C executables from PHP
	    
	Assignment 4:
	    - Transfer the storage of messages, users, and stream data to a database (MySQL)
	    - Create a command line database management program (db.c) to perform maintenance on the 
	      database, such as clearing all streams, viewing all data from a stream, viewing all 
	      users, etc.

********
I included the makefile I used to compile all of the code.

Login credentials have been removed since the original project.
AGAIN NOTE: For the assignment, we were required to keep all files in one directory for grading purposes (Professor's request)
********

-- Original readme (from last assignment) begins here--

Nathen St. Germain

CIS * 2750 - Assignment 4 (Final assignment)

readme.txt

If you have any confusion with running my program feel free to email me: nstgerma@uoguelph.ca

To run:
	-Enter "make" on the command line, this will create executables needed to run program

	-For database reset program enter "./db -<flag>", where flag is a flag specified in the spec
		-Multiple flags may be used, they will be executed in order from left to right
		-Help flag ("-help") will print a message to help with usage of program, when active, 
		it will also print some small additional information on other flags called at the 
		same time from the commadn line

	-Parser: enter "./parseConfig <config file name>"

	-Webpage: start at index.php, if theres still the same permission issues from A3, there 
	will probably be issues in running my program.

Database reset program:
	-For "-reset" and "-clear", the table name having the action performed on will be printed.
	-I used two tables to keep track of current streams and users, if you are wondering why 
	they are shown being cleared and dropped...
		-Using the "-clear" and "-reset" flags will also deal with the streams and users tables
	-If you enter no arguments ("./db"), the program will print flags specified in assignment, 
	not my custom flags.

UI design:
	-User starts at a login page where they can then login, if user does not exist they can 
	login anyway and add or remove author and return to home after that.
	-Home contains the post, add/remove author, view and login as new user buttons
	-Each respective button takes you to a page to do what the button is labelled as

Input into propmts on webpage:
	-Add author and remove author: when entering the streams, format is 
	<stream1,stream2,stream3,stream4> (without <>)
	-Posts are a max of 4000 charcters

WARNINGS:
	-Everything you enter is case sensitive.

	-I implemented the login and select stream page using command line arguments, so if you 
	use certain special characters (Such as a "'", "[", etc.), depending on the shell, those 
	characters have additional functionality on the command line when used.  My program may 
	not behave as expected when this is the case.

	-As mentioned above, I have added additional flags to db.c to implement python functionality 
	with mysql in c, if you happen to use one of these flags by accident, they expect parameters, 
	so the program will probably not run as expected.

	-sort by names doesn't work

end of readme.txt

-- Original readme (from last assignment) ends here--
