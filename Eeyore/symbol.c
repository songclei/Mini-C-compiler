#include "def.h"
#include "symbol.h"

void printSymtab()
{
	Var *i = vartab;
	while(i != NULL)
	{
		printf("%s %d %d\n", i->name, i->type, i->lineno);
		i = i->next;
	}
}


/* build a new variable */
Var *newVariable(vartype type)
{
	Var *t = (Var *) malloc(sizeof(Var));
	t->name = NULL;
	t->next = NULL;
	t->type = type;
	t->lineno = yylineno;
	return t;
}

/* build a new function */
Func *newFunction()
{
	Func *t = (Func *) malloc(sizeof(Func));
	t->name = NULL;
	t->next = NULL;
	t->para = NULL;
	t->lineno = yylineno;
	return t;
}

/* insert a new variable into variable table */
void insert_vartab(Var *t)
{
	if(vartab == NULL)
	{
		vartab = t;
		end_vartab = t;
	}
	else
	{
		end_vartab->next = t;
		end_vartab = t;
	}
}

/* insert a new function into function table */
void insert_functab(Func *t)
{
	if(functab == NULL)
	{
		functab = t;
		end_functab = t;
	}
	else 
	{
		end_functab->next = t;
		end_functab = t;
	}
}

/* when a function is end, the temprary variable is no longer used */
void delete_var(int lineno)
{
	Var *i = vartab;
	Var *j = NULL;
	while(i != NULL)
	{
		if(i->lineno < lineno)
		{
			j = i;
			i = i->next;
		}
		else  break;
	}
	end_vartab = j;
	if(j == NULL)  vartab = NULL;
	else  j->next = NULL;
}

/* find whether a varible has been defined before */
Var *find_var(char *s)
{
	Var *t = vartab;
	Var *i = NULL;
	while(t != NULL)
	{
		if(strcmp(s, t->name) == 0)  
		{
			i = t;
			t = t->next;
		}
		else 
			t = t->next;
	}
	if(i != NULL)  return i;
	else  
		return NULL;
}

/* find whether a function has been defined before */
Func *find_func(char *s)
{
	Func *t = functab;
	while(t != NULL)
	{
		if(strcmp(s, t->name) == 0)
			return t;
		else 
			t = t->next;
	}
	return NULL;
}
