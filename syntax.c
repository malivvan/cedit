int syntax_isStartDelimiter(char chr)
{
	if(chr == ' ' || chr == '(' || chr == ';') return 1;
	return 0;
}

int syntax_isEndDelimiter(char chr)
{
	if(chr == ' ' || chr == ')') return 1;
	return 0; 
}
int syntax_isQut(uint32_t chr)
{
	if(chr == 34 || chr == 39) return 1;
	return 0;
}

// 34 == "
// 39 == '

