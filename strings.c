// Function to copy one string to another
void strcp(char *s1, char *s2, int n)
{
	int flag = 0;
	if (!n)
		flag = 1;

	while(*s2 != '\0' && (n-- > 0 || flag))
		*s1++ = *s2++;
	*s1 = '\0';
}

// Function to compare 2 strings
int strcomp(const char *s1, const char *s2, int n)
{
	int flag = 0;
	if (n > 0)
		flag = 1;
	for ( ; *s1 == *s2; s1++, s2++)
		if (*s1 == '\0' || (--n == 0 && flag)){
			return 0;
		}
	return *s1 - *s2;
}

int mypow(int a, int b)
{
	if (b == 0)
		return 1;
	else
		return a*mypow(a, b-1);
}