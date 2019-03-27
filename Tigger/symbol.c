#include "def.h"
#include "symbol.h"


MidCode *NewCode(CodeType type)
{
	MidCode *t = (MidCode *)malloc(sizeof(MidCode));
	t->type = type;
	memset(t->part, 0, sizeof(t->part));
	t->next = NULL;
	t->before = NULL;
	t->use[0] = t->use[1] = -1;
	t->define = -1;
	t->line = line;
	line ++;
	return t;
}


/* insert a code into code chain */
void InsertCode(MidCode *midcode)
{
	if(code_head == NULL)
		code_head = code_tail = midcode;
	else
	{
		midcode->before = code_tail;
		code_tail->next = midcode;
		code_tail = midcode;
	}
}


Variable *NewVar(char *name, VarType type)
{
	Variable *t = (Variable *)malloc(sizeof(Variable));
	strcpy(t->name, name);
	t->type = type;
	t->next = NULL;
	t->reg = -1;
	if(type == GLOBAL_ARRAY)
	{
		t->num.globalarray = globalarray;
		globalarray ++;
	}
	else if(type == GLOBAL_INT)
	{
		t->num.globalint = globalint;
		globalint ++;
	}
	else if(type == LOCAL_INT)
	{
		t->num.localint = localint;
		localint ++;
	}
	return t;
}


/* insert a variable into variable table */
void InsertVar(Variable *var)
{
	if(vartab_head == NULL)
		vartab_head = vartab_tail = var;
	else
	{
		vartab_tail->next = var;
		vartab_tail = var;
	}
}


/* find a variable use its name */
Variable *FindVar(char *name)
{
	Variable *tmp = vartab_head;
	while(tmp != NULL)
	{
		if(strcmp(tmp->name, name) == 0)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}


/* find a local variable use its local integer number */
Variable *FindLocalVar(int num)
{
	Variable *tmp = vartab_head;
	while(tmp != NULL)
	{
		if(tmp->type==LOCAL_INT && tmp->num.localint==num)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}


/* when a function ends, delete all the local variables 
	defined within the function */
void DeletLocalVar()
{
	#ifdef PRINT
	printf("delete\n");
	#endif

	Variable *tmp = vartab_head;
	if(tmp == NULL)  return;
	if(tmp->type==LOCAL_INT || tmp->type==LOCAL_ARRAY)
	{
		vartab_head = vartab_tail = NULL;
	}
	else
	{
		while(tmp->type==GLOBAL_ARRAY || tmp->type==GLOBAL_INT)
		{
			if(tmp->next == NULL)  break;
			else if(tmp->next->type==GLOBAL_INT || tmp->next->type==GLOBAL_ARRAY)
				tmp = tmp->next;
			else  
				break;
		}
		vartab_tail = tmp;
		vartab_tail->next = NULL;
	}
}
