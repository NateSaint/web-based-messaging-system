# Nathen St. Germain
# makefile for A4 CIS * 2750
MSG = default_commit_message
PASS = default

assignment-2: post addauthor parseConfig db
	chmod +x checkLogin.py
	chmod +x getStreams.py
	chmod +x getLastUnread.py
	chmod +x printPost.py
	chmod +x next.py
	chmod +x prev.py
	chmod +x markall.py

post: post.c libstream.a
	gcc -o post post.c -L. -lstream -ansi -Wall -g -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

addauthor: addauthor.c libstream.a
	gcc -o addauthor addauthor.c -L. -lstream -ansi -Wall -g -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

parseConfig: parseConfig.c
	gcc -o parseConfig parseConfig.c -ansi -Wall -g

post.c: a1 post.cc
	./a1 post.cc

a1: a1.c parser.c convert.c
	gcc -o a1 a1.c parser.c convert.c -ansi -Wall -g

libstream.a: stream.c
	gcc -c stream.c -o stream.o -ansi -Wall -g -lmysqlclient -L/usr/lib/x86_64-linux-gnu/;\
	ar cr libstream.a stream.o

db: db.c db.h
	gcc -o db db.c -ansi -Wall -g -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

git:
	git add *.c *.cc *.h *.py *.wpml *.php readme.txt makefile;\
	git commit -m "$(MSG)"
	git push

upload:
	tar cf a4.tar *.c *.h *.cc *.py index.php *.wpml readme.txt makefile;\
	sftp nstgerma@2750web.socs.uoguelph.ca;\

web:
	ssh nstgerma@2750web.socs.uoguelph.ca

tar:
	tar cf a4.tar *.c *.h *.cc *.py index.php *.wpml readme.txt makefile;
