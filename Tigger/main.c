#include "def.h"
#include "main.h"




int main(int argc, char *argv[])
{
	memset(reg, 0, sizeof(reg));
	if((fp = fopen(argv[1], "r")) == NULL)
	{
		exit(0);
	}
	while(!feof(fp))
	{
		char code[50];
		char part[10][LEN];
		memset(code, 0, sizeof(code));
		memset(part, 0,sizeof(part));
		fgets(code, 50, fp);
		int n = split(code, part);

		CodeAnalyse(n, part);
	}
	return 0;
}


/* split a code into several part */
int split(char *code, char part[][LEN])
{
	char *tmp = code;
	int i = 0;
	while(*tmp == ' ')  tmp ++;  // omit pre-blank
	while(*tmp!='\n' && *tmp!='\0')
	{
		int j = 0;
		while(*tmp!=' ' && *tmp!='\n')
		{
			part[i][j] = *tmp;
			tmp ++;
			j ++;
		}
		part[i][j] = '\0';
		i ++;
		while(*tmp == ' ')  tmp ++;
	}
	return i;
}



