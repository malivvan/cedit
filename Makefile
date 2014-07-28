all: full
full:
	@gcc -DSYNTAX -static cedit.c -o cedit
lite:
	@gcc -static cedit.c -o cedit
debug:
	@gcc -DSYNTAX -static -g -O0 cedit.c -o cedit
clean:
	@rm -f cedit
