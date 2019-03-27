#include "print.h"


void PrintHeader(char *filename)
{
	char name[100];
	memset(name, 0, sizeof(name));
	int i;
	for(i = strlen(filename)-1;i >= 0;-- i)
		if(filename[i] == '/') 
			break;
	i = i + 1;
	int j;
	for(j = i;j < strlen(filename);++ j)
		name[j-i] = filename[j];
	name[j-i] = '\0';
	printf("%c.file   \"%s\"\n", blank, name);
	printf("%c.option nopic\n", blank);
}


void PrintComm(char *name, int len)
{
	printf("%c.comm   %s,%d,8\n", blank, name, len);
}


int PrintFuncBegin(char *name, int s)
{
	printf("%c.text\n", blank);
	printf("%c.align  2\n", blank);
	printf("%c.globl  %s\n", blank, name);
	printf("%c.type   %s, @function\n", blank, name);
	printf("%s:\n", name);
	int size = ((s+1)/4+1) * 16;
	printf("%caddi    sp,sp,-%d\n", blank, size);
	printf("%csd      ra,%d(sp)\n", blank, size-8);
	return size;
}


void PrintFuncCall(char *name)
{
	printf("%ccall    %s\n", blank, name);
}


void PrintRtypeOp(char *r1, char *r2, char *rd, char *op)
{
	if(r2 == NULL)
	{
		if(strcmp(op, "-") == 0)
			printf("%csubw    %s,x0,%s\n", blank, rd, r1);
		else if(strcmp(op, "!") == 0)
			printf("%cseqz    %s,%s\n", blank, rd, r1);
	}
	else 
	{
		if(strcmp(op, "!=") == 0)
		{
			if(strcmp(r1, r2) == 0)
				printf("%cli      %s,0\n", blank, rd);
			else
			{
				printf("%csubw    %s,%s,%s\n", blank, rd, r1, r2);
				printf("%csnez    %s,%s\n", blank, rd, rd);
			}
		}
		else if(strcmp(op, "==") == 0)
		{
			if(strcmp(r1, r2) == 0)
				printf("%cli      %s,1\n", blank, rd);
			else
			{
				printf("%csubw    %s,%s,%s\n", blank, rd, r1, r2);
				printf("%cseqz    %s,%s\n", blank, rd, rd);
			}
		}
		else if(strcmp(op, ">") == 0)
			printf("%csgt     %s,%s,%s\n", blank, rd, r1, r2);
		else if(strcmp(op, "<") == 0)
			printf("%cslt     %s,%s,%s\n", blank, rd, r1, r2);
		else if(strcmp(op, "&&") == 0)
		{
			int label1 = label;
			int label2 = label + 1;
			label += 2;
			printf("%cbeqz    %s,.Label%d\n", blank, r1, label1);
			printf("%cbeqz    %s,.Label%d\n", blank, r2, label1);
			printf("%cli      %s,1\n", blank, rd);
			printf("%cj       .Label%d\n", blank, label2);
			printf(".Label%d:\n", label1);
			printf("%cli      %s,0\n", blank, rd);
			printf(".Label%d:\n", label2);
		}
		else if(strcmp(op, "||") == 0)
		{
			int label1 = label;
			int label2 = label + 1;
			int label3 = label + 2;
			label += 3;
			printf("%cbnez    %s,.Label%d\n", blank, r1, label1);
			printf("%cbeqz    %s,.Label%d\n", blank, r2, label2);
			printf(".Label%d:\n", label1);
			printf("%cli      %s,1\n", blank, rd);
			printf("%cj       .Label%d\n", blank, label3);
			printf(".Label%d:\n", label2);
			printf("%cli      %s,0\n", blank, rd);
			printf(".Label%d:\n", label3);
		}
		else if(strcmp(op, "+") == 0)
			printf("%caddw    %s,%s,%s\n", blank, rd, r1, r2);
		else if(strcmp(op, "-") == 0)
			printf("%csubw    %s,%s,%s\n", blank, rd, r1, r2);
		else if(strcmp(op, "*") == 0)
			printf("%cmulw    %s,%s,%s\n", blank, rd, r1, r2);
		else if(strcmp(op, "/") == 0)
			printf("%cdivw    %s,%s,%s\n", blank, rd, r1, r2);
		else if(strcmp(op, "%") == 0)
			printf("%cremw    %s,%s,%s\n", blank, rd, r1, r2);
	}
}


void PrintItypeOp(char *rs, int imm, char *rd, char *op)
{
	if(strcmp(op, "+") == 0)
		printf("%caddiw   %s,%s,%d\n", blank, rd, rs, imm);
	else if(strcmp(op, "<") == 0)
		printf("%cslti    %s,%s,%d\n", blank, rd, rs, imm);
}


void PrintJmp(char *target)
{
	printf("%cj       .%s\n", blank, target);
}


void PrintLabel(char *label)
{
	printf(".%s\n", label);
}


void PrintIf(char *r1, char *target)
{
	printf("%cbeqz    %s,.%s\n", blank, r1, target);
}


void PrintReturn(int size)
{
	printf("%cld      ra,%d(sp)\n", blank, size-8);
	printf("%caddi    sp,sp,%d\n", blank, size);
	printf("%cjr      ra\n", blank);
}


void PrintStore(int offset, char *r1, char *r2)
{
	printf("%csw      %s,%d(%s)\n", blank, r1, offset, r2);
}	


void PrintFuncEnd(char *name)
{
	printf("%c.size   %s, .-%s\n", blank, name, name);
}


void PrintLoad(int offset, char *r1, char *r2)
{
	printf("%clw      %s,%d(%s)\n", blank, r1, offset, r2);
}


void PrintLoadAddr(char *var, char *rd)
{
	printf("%clui     %s,%%hi(%s)\n", blank, rd, var);
	printf("%caddi    %s,%s,%%lo(%s)\n", blank, rd, rd, var);
}


void PrintFooter()
{
	printf("%c.ident  \"GCC: (GNU) 7.2.0\"\n", blank);
}