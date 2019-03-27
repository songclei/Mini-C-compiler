#include "def.h"
#include "print.h"


void PrintCode(char *funcname, int para_num)
{
	MidCode *code = code_head;
	while(code != NULL)
	{
		if(code->type == VAR_DECL)
			code = code->next;
		else if(code->type == IF_STAT)  
		{
			if(code->part[1][0] == 'p')
			{
				code->part[1][0] = 'a';
				printf("  if %s == x0 goto %s\n", code->part[1], code->part[5]);
			}
			else if(IsVar(code->part[1]))
			{
				Variable *var = FindVar(code->part[1]);
				printf("  if %s == x0 goto %s\n", regname[var->reg], code->part[5]);
			}
			else 
				printf("  if %s == x0 goto %s\n", code->part[1], code->part[5]);
			code = code->next;
		}
		else if(code->type == GOTO_STAT)
		{
			printf("  goto %s\n", code->part[1]);
			code = code->next;
		}
		else if(code->type == RETURN_STAT)
		{
			if(code->part[1][0] == 'p')
			{
				code->part[1][0] == 'a';
				if(code->part[1][1] != '0')
					printf("  a0 = %s\n", code->part[1]);
			}
			else if(IsVar(code->part[1]))
			{
				Variable *var = FindVar(code->part[1]);
				printf("  a0 = %s\n", regname[var->reg]);
			}
			else
				printf("  a0 = %s\n", code->part[1]);
			// recover caller saved register
			if(strcmp(funcname, "main") != 0)
			{
				int tmp_space = caller_begin;
				for(int i = globalint+1;i < 13;++ i)
					if(reg[i] == 1)
					{
						printf("  load %d %s\n", tmp_space, regname[i]);
						tmp_space ++;
					}
			}
			printf("  return\n");
			code = code->next;
		}
		else if(code->type == PARAM_STAT)
		{
			// store callee saved registers
			int tmp_space = callee_begin;
			for(int i = max(globalint+1,13);i <20;++ i)
				if(reg[i] == 1) 
				{
					printf("  store %s %d\n", regname[i], tmp_space);
					tmp_space ++;
				}
			for(int i = 0;i < para_num;++ i)
			{
				printf("  store %s %d\n", regname[i+20], tmp_space);
				tmp_space ++;
			}

			int n = 0;
			printf("  a%d = ", n);
			n ++;
			if(code->part[1][0] == 'p')
			{
				code->part[1][0] = 'a';
				printf("%s\n", code->part[1]);
			}
			else if(IsVar(code->part[1]))
			{
				Variable *var = FindVar(code->part[1]);
				printf("%s\n", regname[var->reg]);
			}
			else 
				printf("%s\n", code->part[1]);

			while(code->next!=NULL && code->next->type==PARAM_STAT)
			{
				code = code->next;
				printf("  a%d = ", n);
				n ++;
				if(code->part[1][0] == 'p')
				{
					code->part[1][0] = 'a';
					printf("%s\n", code->part[1]);
				}
				else if(IsVar(code->part[1]))
				{
					Variable *var = FindVar(code->part[1]);
					printf("%s\n", regname[var->reg]);
				}
				else printf("%s\n", code->part[1]);
			}
			
			code = code->next;
		}
		else if(code->type == FUNC_CALL)
		{
			printf("  call %s\n", code->part[1]);
			// restore callee saved registers
			int tmp_space = callee_begin;
			for(int i = max(globalint+1,13);i <20;++ i)
				if(reg[i] == 1) 
				{
					printf("  load %d %s\n", tmp_space, regname[i]);
					tmp_space ++;
				}
			for(int i = 0;i < para_num;++ i)
			{
				printf("  load %d %s\n", tmp_space, regname[i+20]);
				tmp_space ++;
			}
			code = code->next;
		}
		else if(code->type == VAR_FUNCASSIGN)
		{
			printf("  call %s\n", code->part[3]);
			if(code->part[0][0] == 'p')
			{
				code->part[0][0] = 'a';
				printf("  %s = a0\n", code->part[0]);
			}
			else
			{
				Variable *var = FindVar(code->part[0]);
				printf("  %s = a0\n", regname[var->reg]);
			}

			// restore callee saved registers
			int tmp_space = callee_begin;
			for(int i = max(globalint+1,13);i <20;++ i)
				if(reg[i] == 1) 
				{
					printf("  load %d %s\n", tmp_space, regname[i]);
					tmp_space ++;
				}
			for(int i = 0;i < para_num;++ i)
			{
				printf("  load %d %s\n", tmp_space, regname[i+20]);
				tmp_space ++;
			}
			code = code->next;
		}
		else if(code->type == VAR_TWOOPASSIGN)
		{

			if((IsVar(code->part[2])||code->part[2][0]=='p') && 
				(IsVar(code->part[4])||code->part[4][0]=='p'))   // reg op reg
			{
				if(code->part[0][0] == 'p')
				{
					code->part[0][0] = 'a';
					printf("  %s = ", code->part[0]);
				}
				else
				{
					Variable *var = FindVar(code->part[0]);
					printf("  %s = ", regname[var->reg]);
				}

				if(code->part[2][0] == 'p')
				{
					code->part[2][0] = 'a';
					printf("%s %s ", code->part[2], code->part[3]);
				}
				else
				{
					Variable *var = FindVar(code->part[2]);
					printf("%s %s ", regname[var->reg], code->part[3]);
				}

				if(code->part[4][0] == 'p')
				{
					code->part[4][0] = 'a';
					printf("%s\n", code->part[4]);
				}
				else 
				{
					Variable *var = FindVar(code->part[4]);
					printf("%s\n", regname[var->reg]);
				}
			}
			else if(IsVar(code->part[2]) || code->part[2][0]=='p')  // reg op int
			{
				if(atoi(code->part[4]) == 0)  // the int is 0
				{
					if(code->part[0][0] == 'p')
					{
						code->part[0][0] = 'a';
						printf("  %s = ", code->part[0]);
					}
					else
					{
						Variable *var = FindVar(code->part[0]);
						printf("  %s = ", regname[var->reg]);
					}

					if(code->part[2][0] == 'p')
					{
						code->part[2][0] = 'a';
						printf("%s %s x0\n", code->part[2], code->part[3]);
					}
					else
					{
						Variable *var = FindVar(code->part[2]);
						printf("%s %s x0\n", regname[var->reg], code->part[3]);
					}
				}
				else if(strcmp(code->part[3], "+")==0 ||
				   strcmp(code->part[3], "<")==0 ||
				   strcmp(code->part[3], "-")==0)
				{
					if(code->part[0][0] == 'p')
					{
						code->part[0][0] = 'a';
						printf("  %s = ", code->part[0]);
					}
					else
					{
						Variable *var = FindVar(code->part[0]);
						printf("  %s = ", regname[var->reg]);
					}
					
					if(code->part[2][0] == 'p')
					{
						code->part[2][0] = 'a';
						printf("%s ", code->part[2]);
					}
					else
					{
						Variable *var = FindVar(code->part[2]);
						printf("%s ", regname[var->reg]);
					}

					if(strcmp(code->part[3], "-") == 0)
					{
						int op = atoi(code->part[4]);
						printf("+ %d\n", -op);
					}
					else
						printf("%s %s\n", code->part[3], code->part[4]);
				}
				else
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					printf("  %s = %s\n", regname[i], code->part[4]);
					
					if(code->part[0][0] == 'p')
					{
						code->part[0][0] = 'a';
						printf("  %s = ", code->part[0]);
					}
					else
					{
						Variable *var = FindVar(code->part[0]);
						printf("  %s = ", regname[var->reg]);
					}
					
					if(code->part[2][0] == 'p')
					{
						code->part[2][0] = 'a';
						printf("%s %s %s\n", code->part[2], code->part[3], regname[i]);
					}
					else
					{
						Variable *var = FindVar(code->part[2]);
						printf("%s %s %s\n", regname[var->reg], code->part[3], regname[i]);
					}
				}
			}
			else if(IsVar(code->part[4]) || code->part[4][0]=='p')  // int op reg
			{
				if(atoi(code->part[2]) == 0)  // the int is 0
				{
					if(code->part[0][0] == 'p')
					{
						code->part[0][0] = 'a';
						printf("  %s = ", code->part[0]);
					}
					else
					{
						Variable *var = FindVar(code->part[0]);
						printf("  %s = ", regname[var->reg]);
					}

					if(code->part[4][0] == 'p')
					{
						code->part[4][0] = 'a';
						printf("x0 %s %s\n", code->part[3], code->part[4]);
					}
					else
					{
						Variable *var = FindVar(code->part[4]);
						printf("x0 %s %s\n", code->part[3], regname[var->reg]);
					}
				}

				else if(strcmp(code->part[3], "+")==0 ||
				   strcmp(code->part[3], ">")==0)
				{
					if(code->part[0][0] == 'p')
					{
						code->part[0][0] = 'a';
						printf("  %s = ", code->part[0]);
					}
					else
					{
						Variable *var = FindVar(code->part[0]);
						printf("  %s = ", regname[var->reg]);
					}

					if(code->part[4][0] == 'p')
					{
						code->part[4][0] = 'a';
						printf("%s ", code->part[4]);
					}
					else
					{
						Variable *var = FindVar(code->part[4]);
						printf("%s ", regname[var->reg]);
					}

					if(strcmp(code->part[3], ">") == 0)
						printf("< %s\n", code->part[2]);
					else
						printf("%s %s\n", code->part[3], code->part[2]);
					
				}

				else
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					printf("  %s = %s\n", regname[i], code->part[2]);
					
					if(code->part[0][0] == 'p')
					{
						code->part[0][0] = 'a';
						printf("  %s = ", code->part[0]);
					}
					else
					{
						Variable *var = FindVar(code->part[0]);
						printf("  %s = ", regname[var->reg]);
					}
					
					if(code->part[4][0] == 'p')
					{
						code->part[4][0] = 'a';
						printf("%s %s %s\n", regname[i], code->part[3], code->part[4]);
					}
					else
					{
						Variable *var = FindVar(code->part[4]);
						printf("%s %s %s\n", regname[i], code->part[3], regname[var->reg]);
					}
				}
			}
			else // int op int
			{
				if(code->part[0][0] == 'p')
				{
					code->part[0][0] = 'a';
					printf("  %s = ", code->part[0]);
				}
				else
				{
					Variable *var = FindVar(code->part[0]);
					printf("  %s = ", regname[var->reg]);
				}
				int op1 = atoi(code->part[2]);
				int op2 = atoi(code->part[4]);
				if(strcmp(code->part[3], "+") == 0)
					printf("%d\n", op1+op2);
				else if(strcmp(code->part[3], "-") == 0)
					printf("%d\n", op1-op2);
				else if(strcmp(code->part[3], "*") == 0)
					printf("%d\n", op1*op2);
				else if(strcmp(code->part[3], "/") == 0)
					printf("%d\n", op1/op2);
				else if(strcmp(code->part[3], "%") == 0)
					printf("%d\n", op1%op2);
				else if(strcmp(code->part[3], "&&") == 0)
					printf("%d\n", op1&&op2);
				else if(strcmp(code->part[3], "||") == 0)
					printf("%d\n", op1||op2);
				else if(strcmp(code->part[3], "<") == 0)
					printf("%d\n", op1<op2);
				else if(strcmp(code->part[3], "==") == 0)
					printf("%d\n", op1==op2);
				else if(strcmp(code->part[3], ">") == 0)
					printf("%d\n", op1>op2);
				else if(strcmp(code->part[3], "!=") == 0)
					printf("%d\n", op1!=op2);
			}
			code = code->next;
		}
		else if(code->type == VAR_ARRAYASSIGN)
		{
			Variable *array = FindVar(code->part[2]);
			char offset[10];
			int i;
			for(i = 1;i < strlen(code->part[3])-1;++ i)
				offset[i-1] = code->part[3][i];
			offset[i-1] = '\0';
			if(array->type == GLOBAL_ARRAY)
			{
				if(IsVar(offset) || offset[0]=='p')
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					printf("  loadaddr v%d %s\n", array->num.globalarray, regname[i]);
					
					char name[10];
					if(IsVar(offset))
					{
						Variable *var = FindVar(offset);
						strcpy(name, regname[var->reg]);
					}
					else
					{
						strcpy(name, offset);
						name[0] = 'a';
					}

					printf("  %s = %s + %s\n", regname[i], regname[i], name);
					if(IsVar(code->part[0]))
					{
						Variable *var1 = FindVar(code->part[0]);
						printf("  %s = ", regname[var1->reg]);
					}
					else if(code->part[0][0] == 'p')
					{
						code->part[0][0] = 'a';
						printf("  %s = ", code->part[0]);
					}
					printf("%s [0]\n", regname[i]);
				}
				else
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					printf("  loadaddr v%d %s\n", array->num.globalarray, regname[i]);

					char name[10];
					if(IsVar(code->part[0]))
					{
						Variable *var = FindVar(code->part[0]);
						strcpy(name, regname[var->reg]);
					}
					else if(code->part[0][0] == 'p')
					{
						strcpy(name, code->part[0]);
						name[0] = 'a';
					}

					printf("  %s = %s [%d]\n", name, regname[i], atoi(offset));
				}
			}
			else if(array->type == LOCAL_ARRAY)
			{
				if(IsVar(offset) || offset[0]=='p')
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					char name1[10];
					char name2[10];

					if(IsVar(code->part[0]))
					{
						Variable *var = FindVar(code->part[0]);
						strcpy(name1, regname[var->reg]);
					}
					else if(code->part[0][0] == 'p')
					{
						strcpy(name1, code->part[0]);
						name1[0] = 'a';
					}

					if(IsVar(offset))
					{
						Variable *var = FindVar(offset);
						strcpy(name2, regname[var->reg]);
					}
					else if(offset[0] == 'p')
					{
						strcpy(name2, offset);
						name2[0] = 'a';
					}

					printf("  loadaddr %d %s\n", array->num.localarray, regname[i]);
					printf("  %s = %s + %s\n", regname[i], regname[i], name2);
					printf("  %s = %s [0]\n", name1, regname[i]);
				}
				else
				{	
					char name[10];
					if(IsVar(code->part[0]))
					{
						Variable *var = FindVar(code->part[0]);
						strcpy(name, regname[var->reg]);
					}
					else if(code->part[0][0] == 'p')
					{
						strcpy(name, code->part[0]);
						name[0] = 'a';
					}
					printf("  load %d %s\n", array->num.localarray+atoi(offset)/4, name);
				}
			}
			code = code->next;
		}
		else if(code->type == VAR_ONEOPASSIGN)
		{
			if(code->part[0][0] == 'p')
			{
				code->part[0][0] = 'a';
				printf("  %s = ", code->part[0]);
			}
			else if(IsVar(code->part[0]))
			{
				Variable *var = FindVar(code->part[0]);
				printf("  %s = ", regname[var->reg]);
			}

			if(code->part[3][0] == 'p')
			{
				code->part[3][0] = 'a';
				printf("%s %s\n", code->part[2], code->part[3]);
			}
			else if(IsVar(code->part[3]))
			{
				Variable *var = FindVar(code->part[3]);
				printf("%s %s\n", code->part[2], regname[var->reg]);
			}
			else 
			{
				int op = atoi(code->part[3]);
				if(strcmp(code->part[2], "!") == 0)
					printf("%d\n", !op);
				else 
					printf("%d\n", -op);
			}
			code = code->next;
		}
		else if(code->type == VAR_ASSIGN)
		{
			if(code->part[0][0] == 'p')
			{
				code->part[0][0] = 'a';
				printf("  %s = ", code->part[0]);
			}
			else if(IsVar(code->part[0]))
			{
				Variable *var = FindVar(code->part[0]);
				printf("  %s = ", regname[var->reg]);
			}
			if(code->part[2][0] == 'p')
			{
				code->part[2][0] = 'a';
				printf("%s\n", code->part[2]);
			}
			else if(IsVar(code->part[2]))
			{
				Variable *var = FindVar(code->part[2]);
				printf("%s\n", regname[var->reg]);
			}
			else 
				printf("%s\n", code->part[2]);
			code = code->next;
		}
		else if(code->type == ARRAY_ASSIGN)
		{
			Variable *array = FindVar(code->part[0]);
			char offset[10];
			int i;
			for(i = 1;i < strlen(code->part[1])-1;++ i)
				offset[i-1] = code->part[1][i];
			offset[i-1] = '\0';
			if(array->type == GLOBAL_ARRAY)
			{
				if(IsVar(offset) || offset[0]=='p')
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					char name1[10];
					char name2[10];
					if(IsVar(offset))
					{
						Variable *var = FindVar(offset);
						strcpy(name1, regname[var->reg]);
					}
					else
					{
						strcpy(name1, offset);
						name1[0] = 'a';
					}

					printf("  loadaddr v%d %s\n", array->num.globalarray, regname[i]);
					if(IsVar(code->part[3]))
					{
						Variable *var = FindVar(code->part[3]);
						strcpy(name2, regname[var->reg]);
					}
					else if(code->part[3][0] == 'p')
					{
						strcpy(name2, code->part[3]);
						name2[0] = 'a';
					}
					else  
					{
						int j;
						for(j = i+1;j < 20;++ j)
							if(reg[j] == 0)  break;
						strcpy(name2, regname[j]);
						printf("  %s = %s\n", name2, code->part[3]);
					}

					printf("  %s = %s + %s\n", regname[i], regname[i], name1);
					printf("  %s [0] = %s\n", regname[i], name2);
				}
				else
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					printf("  loadaddr v%d %s\n", array->num.globalarray, regname[i]);
 					char name[10];
					if(IsVar(code->part[3]))
					{
						Variable *var = FindVar(code->part[3]);
						strcpy(name, regname[var->reg]);
					}
					else if(code->part[3][0] == 'p')
					{
						strcpy(name, code->part[3]);
						name[0] = 'a';
					}
					else
					{
						int j;
						for(j = i+1;j < 20;++ j)
							if(reg[j] == 0)  break;
						strcpy(name, regname[j]);
						printf("  %s = %s\n", name, code->part[3]);
					}

					printf("  %s [%s] = %s\n", regname[i], offset, name);
				}
			}
			else if(array->type == LOCAL_ARRAY)
			{
				if(IsVar(offset) || offset[0]=='p')
				{
					int i;
					for(i = 1;i < 20;++ i)
						if(reg[i] == 0)  break;
					printf("  loadaddr %d %s\n", array->num.localarray, regname[i]);

					char name1[10];
					char name2[10];
					if(IsVar(offset))
					{
						Variable *var = FindVar(offset);
						strcpy(name1, regname[var->reg]);
					}
					else
					{
						strcpy(name1, offset);
						name1[0] = 'a';
					}
					printf("  %s = %s + %s\n", regname[i], regname[i], name1);

					if(IsVar(code->part[3]))
					{
						Variable *var = FindVar(code->part[3]);
						strcpy(name2, regname[var->reg]);
					}
					else if(code->part[3][0] == 'p')
					{
						strcpy(name2, code->part[3]);
						name2[0] = 'a';
					}
					else
					{
						int j;
						for(j = i+1;j < 20;++ j)
							if(reg[j] == 0)  break;
						strcpy(name2, regname[j]);
						printf("  %s = %s\n", name2, code->part[3]);
					}
					printf("  %s [0] = %s\n", regname[i], name2);
				}
				else
				{
					char name[10];
					if(IsVar(code->part[3]))
					{
						Variable *var = FindVar(code->part[3]);
						strcpy(name, regname[var->reg]);
					}
					else if(code->part[3][0] == 'p')
					{
						strcpy(name, code->part[3]);
						name[0] = 'a';
					}
					else
					{
						int i;
						for(i = 1;i < 20;++ i)
							if(reg[i] == 0)  break;
						strcpy(name, regname[i]);
						printf("  %s = %s\n", name, code->part[3]);
					}

					printf("  store %s %d\n", name, array->num.localarray+atoi(offset)/4);
				}
			}
			code = code->next;
		}
		else if(code->type == LABEL)
		{
			printf("%s\n", code->part[0]);
			code = code->next;
		}
		else if(code->type == NONE_TYPE)
			code = code->next;
	}
}


/* find wether a expression is a global interger of local interger */
bool IsVar(char *name)
{
	if(name[0]=='T' || name[0]=='t')
		return true;
	return false; 
}
