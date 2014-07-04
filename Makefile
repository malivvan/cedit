all: full
full:
	@gcc -DSYNTAX -static cedit.c -o cedit
lite:
	@gcc -static cedit.c -o cedit
clean:
	@rm -f cedit
