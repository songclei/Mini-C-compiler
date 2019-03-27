#include "main.h"

int space = 0;  // space used for a function
int label = 0;

int main(int argc, char *argv[])
{
	if((fp = fopen(argv[1], "r")) == NULL)
	{
		exit(0);
	}

	PrintHeader(argv[2]);
	
	while(!feof(fp))
	{
		char content[50];
		memset(content, 0, sizeof(content));
		fgets(content, 50, fp);
		AnalyseCode(content);
	}

	PrintFooter();
	return 0;
}


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


void AnalyseCode(char *code)
{
	char part[10][LEN];
	memset(part, 0, sizeof(part));
	int k = split(code, part);
	if(strcmp(part[2], "malloc") == 0)  //  v = malloc int
		PrintComm(part[0], atoi(part[3]));
	else if(part[0][0] == 'f')  // f_xx [] []
	{
		part[2][strlen(part[2])-1] = '\0';
		int num = atoi(part[2]+1);
		space = PrintFuncBegin(part[0]+2, num);
	}
	else if(strcmp(part[0], "call") == 0)  // call func
		PrintFuncCall(part[1]+2);
	else if(strcmp(part[1], "=")==0 && k==5 && IsReg(part[4]))  // reg = reg op reg
		PrintRtypeOp(part[2], part[4], part[0], part[3]);
	else if(strcmp(part[1], "=")==0 && k==5)  // reg = reg op int
		PrintItypeOp(part[2], atoi(part[4]), part[0], part[3]);
	else if(strcmp(part[1], "=")==0 && k==4 && IsReg(part[3]))  // reg = op reg 
		PrintRtypeOp(part[3], NULL, part[0], part[2]);
	else if(strcmp(part[1], "=")==0 && k==3 && IsReg(part[2]))  // reg = reg
		PrintItypeOp(part[2], 0, part[0], "+");
	else if(strcmp(part[1], "=")==0 && k==3)  // reg = int
		PrintItypeOp("x0", atoi(part[2]), part[0], "+");
	else if(strcmp(part[2], "=")==0 && part[1][0]=='[')  // reg [int] = reg
	{
		part[1][strlen(part[1])-1] = '\0';
		int offset = atoi(part[1]+1);
		PrintStore(offset, part[3], part[0]);
	} 
	else if(strcmp(part[1], "=")==0 && part[3][0]=='[')  // reg = reg [int]
	{
		part[3][strlen(part[3])-1] = '\0';
		int offset = atoi(part[3]+1);
		PrintLoad(offset, part[0], part[2]);
	}
	else if(strcmp(part[0], "if") == 0)  // if reg == x0 goto label
		PrintIf(part[1], part[5]);
	else if(strcmp(part[0], "goto") == 0)  // goto label
		PrintJmp(part[1]);
	else if(part[0][strlen(part[0])-1] == ':')  // label:
		PrintLabel(part[0]);
	else if(strcmp(part[0], "store") == 0)  // store reg int
	{
		int n = atoi(part[2]);
		int offset = 4 * n;
		PrintStore(offset, part[1], "sp");
	}
	else if(strcmp(part[0], "load") == 0)  // load int reg
	{
		int n = atoi(part[1]);
		int offset = 4 * n;
		PrintLoad(offset, part[2], "sp");
	}
	else if(strcmp(part[0], "loadaddr")==0 && IsInt(part[1]))  // loadaddr int reg
	{
		int n = atoi(part[1]);
		int offset  = 4 * n;
		printf("%caddi    %s,sp,%d\n", blank, part[2], offset);
	}
	else if(strcmp(part[0], "loadaddr") == 0)  // loadaddr v reg
		PrintLoadAddr(part[1], part[2]);
	else if(strcmp(part[0], "return") == 0)  // return
		PrintReturn(space);
	else if(strcmp(part[0], "end") == 0)  // end f_xx
		PrintFuncEnd(part[1]+2);
}


bool IsReg(char *s)
{
	if(s[0]=='s' || s[0]=='a' || s[0]=='t' || s[0]=='x')
		return TRUE;
	else  
		return FALSE;
}


bool IsInt(char *s)
{
	if(s[0]>='0' && s[0]<='9')
		return TRUE;
	else 
		return FALSE;
}