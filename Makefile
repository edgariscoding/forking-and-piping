SOURCES = minor6.c

start:	minor6.c
	gcc -std=c99 -Wall -o minor6 $(SOURCES)

run: minor6
	./minor6

clean:
	rm -rf minor6