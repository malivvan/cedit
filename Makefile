all: compile
compile:
	@gcc -static cedit.c -o cedit
clean:
	@rm -f cedit
