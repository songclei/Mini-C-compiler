#include "def.h"
#include "analyse.h"


void CodeAnalyse(int n, char part[][LEN])
{
	if(strcmp(part[0], "var") == 0)
	{
		/* global integer use a register throughout the program */
		if(n == 2)  // vardecl
		{
			Variable *var = NewVar(part[1], GLOBAL_INT);
			InsertVar(var);
			var->reg = globalint;
			reg[var->reg] = 1;
		}
		else  // array decl
		{
			Variable *var = NewVar(part[2], GLOBAL_ARRAY);
			InsertVar(var);
			printf("v%d = malloc %s\n", var->num.globalarray, part[1]);
		}
	}
	else if(part[0][0] == 'f')
	{
		char funcname[LEN];
		int i;
		for(i = 2;i < strlen(part[0]);++ i)
			funcname[i-2] = part[0][i];
		funcname[i-2] = '\0';

		FuncAnalyse(funcname, part[1]);
	}
}


/* analyse a funciton and store code, allocate registers */
void FuncAnalyse(char *funcname, char *paramnum)
{
#ifdef DEBUG
	printf("function %s:\n", funcname);
#endif

	/* get number of parameters */
	char tmp[10];
	int k = 1;
	for(k;k < strlen(paramnum)-1;++ k)
		tmp[k-1] = paramnum[k];
	tmp[k-1] = '\0';
	para_num = atoi(tmp);  // number of parameters


	/* initialize related variable for when a function begins */
	code_head = code_tail = NULL;  
	localint = 0;
	line = 0;
	space = 0;
	funccall = 0;
	callee_begin = 0;
	caller_begin = 0;
	caller_saved = callee_saved = 0;
	memset(labelline, 0, sizeof(labelline));
	memset(reg, 0, sizeof(reg));
	Variable *var = vartab_head;
	/* global integer always occupy a register */
	while(var != NULL)
	{
		reg[var->reg] = 1;
		var = var->next;
	}

	while(1)
	{
		char code[50];
		memset(code, 0, sizeof(code));
		char part[10][LEN];
		memset(part, 0,sizeof(part));
		fgets(code, 50, fp);
		int n = split(code, part);

		int end = StoreCodeInfo(n, part);
		if(end)
			break;
	}

	if(localint != 0)
		RegAllocation();
	
	CalculateSpace(funcname);
	
#ifdef DEBUG
	var = vartab_head;
	while(var != NULL)
	{
		printf("%s : %s\n", var->name, regname[var->reg]);
		var = var->next;
	}
	printf("---------------------------------------------\n");
	printf("register used:\n");
	for(int i = 0;i < 28;++ i)
		printf("%d ", reg[i]);
	printf("\n");
	printf("---------------------------------------------\n");
#endif

	
	printf("f_%s %s [%d]\n", funcname, paramnum, space);
	
	// store caller saved register
	if(strcmp(funcname, "main") != 0)
	{
		int tmp_space = caller_begin;
		for(int i = globalint+1;i < 13;++ i)
			if(reg[i] == 1)
			{
				printf("  store %s %d\n", regname[i], tmp_space);
				tmp_space ++;
			}
	}

	PrintCode(funcname, para_num);
	
	printf("end f_%s\n\n", funcname);
	
	DeletLocalVar();
}


int StoreCodeInfo(int n, char part[][LEN])
{
	if(strcmp(part[0], "var") == 0)
	{
		if(n == 2)  // vardecl
		{
			//printf("vardecl\n");
			Variable *var = NewVar(part[1], LOCAL_INT);
			InsertVar(var);
			MidCode *midcode = NewCode(VAR_DECL);
			for(int i = 0;i < 2;++ i)
				strcpy(midcode->part[i], part[i]);
			midcode->define = var->num.localint;
			InsertCode(midcode);
		}
		else  // array decl
		{
			//printf("array decl\n");
			int length = atoi(part[1]);
			length /= 4;
			Variable *var = NewVar(part[2], LOCAL_ARRAY);
			var->num.localarray = space;
			space += length;
			InsertVar(var);
		}
	}
	else if(strcmp(part[0], "if") == 0)  // if
	{
		//printf("if\n");
		MidCode *midcode = NewCode(IF_STAT);
		for(int i = 0;i < 6;++ i)
			strcpy(midcode->part[i], part[i]);
		if(IsVar(part[1]))
		{
			Variable *var = FindVar(part[1]);
			if(var->type == LOCAL_INT)
				midcode->use[0] = var->num.localint;
		}
		InsertCode(midcode);
	}
	else if(strcmp(part[0], "goto") == 0)  // goto
	{
		//printf("goto\n");
		MidCode *midecode = NewCode(GOTO_STAT);
		for(int i = 0;i < 2;++ i)
			strcpy(midecode->part[i], part[i]);
		InsertCode(midecode);
	}
	else if(strcmp(part[0], "return") == 0)  // return
	{
		//printf("return\n");
		MidCode *midcode = NewCode(RETURN_STAT);
		for(int i = 0;i < 2;++ i)
			strcpy(midcode->part[i], part[i]);
		if(IsVar(part[1]))
		{
			Variable *var = FindVar(part[1]);
			if(var->type == LOCAL_INT)
				midcode->use[0] = var->num.localint;
		}
		InsertCode(midcode);
	}
	else if(strcmp(part[0], "param") == 0)  // param
	{
		//printf("param\n");
		MidCode *midcode = NewCode(PARAM_STAT);
		for(int i = 0;i < 2;++ i)
			strcpy(midcode->part[i], part[i]);
		if(IsVar(part[1]))
		{
			Variable *var = FindVar(part[1]);
			if(var->type == LOCAL_INT)
				midcode->use[0] = var->num.localint;
		}
		InsertCode(midcode);
	}
	else if(strcmp(part[0], "end") == 0)  // function end
		return 1;
	else if(strcmp(part[0], "call") == 0)  // function call
	{
		//printf("function call\n");
		funccall = 1;
		MidCode *midcode = NewCode(FUNC_CALL);
		for(int i = 0;i < 2;++ i)
			strcpy(midcode->part[i], part[i]);
		InsertCode(midcode);
	}
	else if(strcmp(part[1], "=") == 0)
	{
		if(strcmp(part[2], "call") == 0)  // var = call func 
		{
			//printf("var = call func\n");
			funccall = 1;
			MidCode *midcode = NewCode(VAR_FUNCASSIGN);
			for(int i = 0;i < 4;++ i)
				strcpy(midcode->part[i], part[i]);
			if(IsVar(part[0]))
			{
				Variable *var = FindVar(part[0]);
				if(var->type == LOCAL_INT)
					midcode->define = var->num.localint;
			}
			InsertCode(midcode);
		}
		else if(n == 5)  // var = var op var
		{
			//printf("var = var op var\n");
			MidCode *midcode = NewCode(VAR_TWOOPASSIGN);
			for(int i = 0;i < 5;++ i)
				strcpy(midcode->part[i], part[i]);
			if(IsVar(part[0]))
			{
				Variable *var = FindVar(part[0]);
				if(var->type == LOCAL_INT)
					midcode->define = var->num.localint;
			}
			int n = 0;
			if(IsVar(part[2]))
			{
				Variable *var = FindVar(part[2]);
				if(var->type == LOCAL_INT)
					midcode->use[n] = var->num.localint;
				n ++;
			}
			if(IsVar(part[4]))
			{
				Variable *var = FindVar(part[4]);
				if(var->type == LOCAL_INT)
					midcode->use[n] = var->num.localint;
				n ++;
			}
			InsertCode(midcode);
		}
		else if(n == 4)  
		{
			if(part[3][0] == '[')  // var = array
			{
				//printf("var = array\n");
				MidCode *midcode = NewCode(VAR_ARRAYASSIGN);
				for(int i = 0;i < 4;++ i)
					strcpy(midcode->part[i], part[i]);
				if(IsVar(part[0]))
				{
					Variable *var = FindVar(part[0]);
					if(var->type == LOCAL_INT)
						midcode->define = var->num.localint;
				}
				char name[10];
				int i;
				for(i = 1;i < strlen(part[3])-1;++ i)
					name[i-1] = part[3][i];
				name[i-1] = '\0';
				if(IsVar(name))
				{
					Variable *var = FindVar(name);
					if(var->type == LOCAL_INT)
						midcode->use[0] = var->num.localint;
				}
				InsertCode(midcode);
			}
			else  // var = op var
			{ 
				//printf("var = op var\n");
				MidCode *midcode = NewCode(VAR_ONEOPASSIGN);
				for(int i = 0;i < 4;++ i)
					strcpy(midcode->part[i], part[i]);
				if(IsVar(part[0]))
				{
					Variable *var = FindVar(part[0]);
					if(var->type == LOCAL_INT)
						midcode->define = var->num.localint;
				}
				if(IsVar(part[3]))
				{
					Variable *var = FindVar(part[3]);
					if(var->type == LOCAL_INT)
						midcode->use[0] = var->num.localint;
				} 
				InsertCode(midcode);
			}
		}
		else if(n == 3)  // var = var
		{
			//printf("var = var\n");
			MidCode *midcode = NewCode(VAR_ASSIGN);
			for(int i = 0;i < 3;++ i)
				strcpy(midcode->part[i], part[i]);
			if(IsVar(part[0]))
			{
				Variable *var = FindVar(part[0]);
				if(var->type == LOCAL_INT)
					midcode->define = var->num.localint;
			}
			if(IsVar(part[2]))
			{
				Variable *var = FindVar(part[2]);
				if(var->type == LOCAL_INT)
					midcode->use[0] = var->num.localint;
			}
			InsertCode(midcode);
		}
	}
	else if(strcmp(part[2], "=") == 0)  // array = var
	{
		//printf("array = var\n");
		MidCode *midcode = NewCode(ARRAY_ASSIGN);
		for(int i = 0;i < 4;++ i)
			strcpy(midcode->part[i], part[i]);
		char name[10];
		int i;
		for(i = 1;i < strlen(part[1])-1;++ i)
			name[i-1] = part[1][i];
		name[i-1] = '\0';
		if(IsVar(name))
		{
			Variable *var = FindVar(name);
			if(var->type == LOCAL_INT)
				midcode->use[0] = var->num.localint;
		}
		if(IsVar(part[3]))
		{
			Variable *var = FindVar(part[3]);
			if(var->type == LOCAL_INT)
				midcode->use[1] = var->num.localint;
		}
		InsertCode(midcode);
	}
	else if(part[0][0] == 'l')  // label
	{
		//printf("label\n");
		MidCode *midcode = NewCode(LABEL);
		strcpy(midcode->part[0], part[0]);
		InsertCode(midcode);
		char labelnum[10];
		int i;
		for(i = 1;i < strlen(part[0])-1;++ i)
			labelnum[i-1] = part[0][i];
		labelnum[i-1] = '\0';
		labelline[atoi(labelnum)] = line;
	}
	return 0;
}


void RegAllocation()
{
	bool modified = 1;
	bool activity[line+1][localint];
	memset(activity, 0, sizeof(activity));
	while(modified == 1)
	{
		modified = 0;
		MidCode *code = code_tail;
		int l = line-1;
		while(code != NULL)
		{
			bool tmp[localint];
			memset(tmp, 0, sizeof(tmp));
			if(code->type == IF_STAT)
			{
				char gotonum[5];
				int i;
				for(i = 1;i < strlen(code->part[5]);++ i)
					gotonum[i-1] = code->part[5][i];
				gotonum[i-1] = '\0';
				int gotoline = labelline[atoi(gotonum)];
				for(i = 0;i < localint;++ i)
					tmp[i] = activity[l+1][i] + activity[gotoline][i];
				if(code->use[0] != -1)
					tmp[code->use[0]] = 1;
				for(i = 0;i < localint;++ i)
				{
					if(tmp[i] != activity[l][i])
					{
						modified = 1;
						activity[l][i] = tmp[i];
					}
				}
				code = code->before;
				l --;
			}
			else if(code->type == GOTO_STAT)
			{
				char gotonum[5];
				int i;
				for(i = 1;i < strlen(code->part[1]);++ i)
					gotonum[i-1] = code->part[1][i];
				gotonum[i-1] = '\0';
				int gotoline = labelline[atoi(gotonum)];
				for(i = 0;i < localint;++ i)
				{
					if(activity[l][i] != activity[gotoline][i])
					{
						modified = 1;
						activity[l][i] = activity[gotoline][i];
					}
				}
				code = code->before;
				l --;
			}
			else if(code->type == RETURN_STAT)
			{
				if(code->use[0] != -1)
					tmp[code->use[0]] = 1;
				for(int i = 0;i < localint;++ i)
				{
					if(activity[l][i] != tmp[i])
					{
						modified = 1;
						activity[l][i] = tmp[i];
					}	
				}
				code = code->before;
				l --;
			}
			else
			{
				for(int i = 0;i < localint;++ i)
					tmp[i] = activity[l+1][i];
				if(code->define != -1)
					tmp[code->define] = 0;
				for(int i = 0;i < 2;++ i)
					if(code->use[i] != -1)
						tmp[code->use[i]] = 1;
				for(int i = 0;i < localint;++ i)
				{
					if(activity[l][i] != tmp[i])
					{
						modified = 1;
						activity[l][i] = tmp[i];
					}
				}
				code = code->before;
				l --;
			}	
		}
	}


	int last_active[localint]; 
	for(int i = 0; i < localint; ++i)
		last_active[i] = -1;
	for(int i = 0; i < localint; ++i)
	{
		int j = line-1;
		while(j >= 0)
		{
			if(activity[j][i] == 1)
			{
				last_active[i] = j;
				break;
			}
			j--;
		}
	}

	
	MidCode *code = code_head;
	while(code != NULL)
	{
		if(code->define>=0 && code->line>last_active[code->define])
		{
			if(code->type == VAR_FUNCASSIGN)
			{
				code->type = FUNC_CALL;
				strcpy(code->part[1], code->part[3]);
			}
			else 
				code->type = NONE_TYPE;
		}
		code = code->next;
	}

#ifdef DEBUG
	printf("local int number:\n");
	Variable *t = vartab_head;
	while(t != NULL)
	{
		if(t->type == LOCAL_INT)
			printf("%s : %d\n", t->name, t->num.localint);
		t = t->next;
	}
	printf("---------------------------------------------\n");
	for(int i = 0;i < line;++ i)
	{
		printf("line %2d :", i);
		for(int j = 0;j < localint;++ j)
			printf("%d", activity[i][j]);
		printf("\n");
	}
	printf("---------------------------------------------\n");
#endif

	bool vargraph[localint][localint];
	memset(vargraph, 0, sizeof(vargraph));

	/* build interference graph */
	for(int i = 0;i < line;++ i)
	{
		for(int j = 0;j < localint;++ j)
			for(int k = j+1;k < localint;++ k)
				if(activity[i][j]==1 && activity[i][k]==1)
					vargraph[j][k] = vargraph[k][j] = 1;
	}

#ifdef DEBUG
	printf("var graph:\n");
	for(int i = 0;i < localint;++ i)
	{
		for(int j = 0;j < localint;++ j)
			printf("%d ", vargraph[i][j]);
		printf("\n");
	}
	printf("---------------------------------------------\n");
#endif

	/* graph coloring */
	int color[localint];
	memset(color, 0, sizeof(color));
	for(int i = 0;i < localint;++ i)
		color[i] = globalint;
	int n = 0;
	while(n >= 0)
	{
		color[n] = color[n] + 1;
		while(color[n] < 20)
		{
			bool ok = 1;
			for(int i = 0;i < localint;++ i)
				if(vargraph[i][n]==1 && color[i]==color[n])
				{
					ok = 0;
					break;
				}
			if(ok)  break;
			else  color[n] += 1;
		}
		if(n == localint-1)  break;
		else if(color[n] < 20)
			n += 1;
		else
		{
			color[n] = globalint;
			n -= 1;
		}
	}


	for(int i = 0;i < localint;++ i)
	{
		Variable *var = FindLocalVar(i);
		var->reg = color[i];
		reg[var->reg] = 1;
	}

} 


void CalculateSpace(char *funcname)
{
	// main function need not to store caller saved registers
	if(strcmp(funcname, "main") != 0)
	{
		for(int i = globalint+1;i < 13;++ i)
			if(reg[i] == 1)  
				caller_saved += 1;
		caller_begin = space;
		space += caller_saved;
	}

	/* only need to store a callee saved registers when
		there is a function call */
	if(funccall == 1)
	{
		for(int i = max(globalint+1,13);i <20;++ i)
			if(reg[i] == 1)
				callee_saved += 1;
		callee_begin = space;
		space += callee_saved;

		space += para_num;
	}
}



int max(int a,int b)
{
	return a > b ? a : b;
}



