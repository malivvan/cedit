int syntax_isStartDelimiter(char buf)
{
	if(chr == ' ' || chr == '(' || chr == ';') return 1;
	return 0;
}

int syntax_isEndDelimiter(char chr)
{
	if(chr == ' ' || chr == ')') return 1;
	return 0; 
}
