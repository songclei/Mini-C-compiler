#include "def.h"
#include "print.h"



void printWhitespace(int whitespace)
{
	if(whitespace < 0)   whitespace = 0;
	int i;
	for(i = 0;i < whitespace;++ i)  printf(" ");
}

void printTree(treenode *root, int whitespace)
{
	if(root == NULL)  return;
	if(root->treetype == STMT_TYPE)
	{
		Var *i;
		Var *j;
		Func *f;
		treenode *exp;
		treenode *exp1;
		treenode *exp2;
		int savetag;
		int savetag1;
		int savetag2;
		int t;
		int t1;
		int t2;
		switch(root->type.stmttype)
		{
			case VARDEF_TYPE:
				if(root->num == -1)
					i = newVariable(INT_VAR);
				else   i = newVariable(ARRAY_VAR);
				i->name = copyString(root->name);
				i->num = globalvar;
				i->prefix = 'T';
				i->lineno = root->lineno;
				globalvar ++;
				insert_vartab(i);
				
				printWhitespace(whitespace);
				if(root->num == -1)
					printf("var %c%d\n", i->prefix, i->num);
				else
					printf("var %d %c%d\n", 4*root->num, i->prefix, i->num);
				break;
			case IF_TYPE:
				t = printExp(root->child[0], whitespace);
				if(root->child[2] == NULL)  // if then
				{
					if(t == -1)  // if(n)
					{
						printWhitespace(whitespace);
						printf("if %s == 0 goto l%d\n", root->child[0]->name, tagnum);
						savetag = tagnum;
						tagnum ++;
						printTree(root->child[1], whitespace);
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag);
					}	
					else if(t == -2)  // if(1)
					{
						printWhitespace(whitespace);
						printf("if %d == 0 goto l%d\n", root->child[0]->num, tagnum);
						savetag = tagnum;
						tagnum ++;
						printTree(root->child[1], whitespace);
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag);
					}
					else
					{
						printWhitespace(whitespace);
						printf("if t%d == 0 goto l%d\n", t, tagnum);
						savetag = tagnum;
						tagnum ++;
						printTree(root->child[1], whitespace);
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag);
					}
				}
				else  // if then else 
				{
					if(t == -1)
					{
						printWhitespace(whitespace);
						printf("if %s == 0 goto l%d\n", root->child[0]->name, tagnum);
						savetag1 = tagnum;
						tagnum ++;
						printTree(root->child[1], whitespace);
						printWhitespace(whitespace);
						printf("goto l%d\n", tagnum);
						savetag2 = tagnum;
						tagnum ++;
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag1);
						printTree(root->child[2], whitespace);
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag2);
					}	
					else if(t == -2)
					{
						printWhitespace(whitespace);
						printf("if %d == 0 goto l%d\n", root->child[0]->num, tagnum);
						savetag1 = tagnum;
						tagnum ++;
						printTree(root->child[1], whitespace);
						printWhitespace(whitespace);
						printf("goto l%d\n", tagnum);
						savetag2 = tagnum;
						tagnum ++;
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag1);
						printTree(root->child[2], whitespace);
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag2);
					}
					else
					{
						printWhitespace(whitespace);
						printf("if t%d == 0 goto l%d\n", t, tagnum);
						savetag1 = tagnum;
						tagnum ++;
						printTree(root->child[1], whitespace);
						printWhitespace(whitespace);
						printf("goto l%d\n", tagnum);
						savetag2 = tagnum;
						tagnum ++;
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag1);
						printTree(root->child[2], whitespace);
						printWhitespace(whitespace-2);
						printf("l%d:\n", savetag2);
					}
				}
				delete_var(root->lineno);
				break;
			case WHILE_TYPE:
				printWhitespace(whitespace-2);
				printf("l%d:\n", tagnum);
				t = printExp(root->child[0], whitespace);
				savetag1 = tagnum;
				tagnum ++;
				if(t == -1)  // while(n)
				{
					printWhitespace(whitespace);
					printf("if %s == 0 goto l%d\n", root->child[0]->name, tagnum);
					savetag2 = tagnum;
					tagnum ++;
					printTree(root->child[1], whitespace);
					printWhitespace(whitespace);
					printf("goto l%d\n", savetag1);
					printWhitespace(whitespace-2);
					printf("l%d:\n", savetag2);
				}	
				else if(t == -2)  // if(1)
				{
					printWhitespace(whitespace);
					printf("if %d == 0 goto l%d\n", root->child[0]->num, tagnum);
					savetag2 = tagnum;
					tagnum ++;
					printTree(root->child[1], whitespace);
					printWhitespace(whitespace);
					printf("goto l%d\n", savetag1);
					printWhitespace(whitespace-2);
					printf("l%d:\n", savetag2);
				}
				else
				{
					printWhitespace(whitespace);
					printf("if t%d == 0 goto l%d\n", t, tagnum);
					savetag2 = tagnum;
					tagnum ++;
					printTree(root->child[1], whitespace);
					printWhitespace(whitespace);
					printf("goto l%d\n", savetag1);
					printWhitespace(whitespace-2);
					printf("l%d:\n", savetag2);
				}
				delete_var(root->lineno);
				break;
			case ASSIGN_TYPE:
				exp = root->child[0];
				i = find_var(root->name);
				#ifdef CHECK
				if(i == NULL)
					yyerror("the variable has not been defined!");
				else if(i->type != INT_VAR)
					yyerror("the variable is not an interger!");
				#endif
				if(exp->type.exptype == CONST_EXP)  // a = 1
				{
					printWhitespace(whitespace);
					printf("%c%d = %d\n", i->prefix, i->num, exp->num);
				}
				else if(exp->type.exptype == ID_EXP)  // a = b
				{
					j = find_var(exp->name);
					#ifdef CHECK
					if(j == NULL)
						yyerror("the variable has not been defined!");
					else if(j->type != INT_VAR)
						yyerror("the variable is not an interger!");
					#endif
					printWhitespace(whitespace);
					printf("%c%d = %c%d\n", i->prefix, i->num, j->prefix, j->num);
				}
				else if(exp->type.exptype == ARRAY_EXP)  // array
				{
					t = printExp(exp->child[0], whitespace);
					j = find_var(exp->name);
					#ifdef CHECK
					if(j == NULL)
						yyerror("the variable has not been defined");
					else if(j->type != ARRAY_VAR)
						yyerror("the variable is not an array");
					#endif
					if(t == -2)  // n = a[1]
					{
						printWhitespace(whitespace);
						printf("%c%d = %c%d [%d]\n", i->prefix, i->num, j->prefix, j->num, 4*exp->child[0]->num);
					}
					else if(t == -1)  // n = a[i]
					{
						Var *k = find_var(exp->child[0]->name);
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * %c%d\n", tempvar, k->prefix, k->num);
						printWhitespace(whitespace);
						printf("%c%d = %c%d [t%d]\n", i->prefix, i->num, j->prefix, j->num, tempvar);
						tempvar ++;
					}
					else 
					{
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * t%d\n", tempvar, t);
						printWhitespace(whitespace);
						printf("%c%d = %c%d [t%d]\n", i->prefix, i->num, j->prefix, j->num, tempvar);
						tempvar ++;
					}
				}
				else if(exp->type.exptype == OP_EXP)// a = exp OP exp
				{

					exp1 = root->child[0]->child[0];
					exp2 = root->child[0]->child[1];
					t1 = printExp(exp1, whitespace);
					t2 = printExp(exp2, whitespace);
					if(t2 == -3)  // NOT or NEG
					{
						if(t1 == -1)  // !a
						{
							j = find_var(exp1->name);
							#ifdef CHECK
							if(j == NULL)
								yyerror("the variable has not been defined!");
							else if(j->type != INT_VAR)
								yyerror("the variable is not an interger!");
							#endif
							printWhitespace(whitespace);
							if(exp->optype == NEG_TYPE)
								printf("%c%d = - %c%d\n", i->prefix, i->num, j->prefix, j->num);
							else if(exp->optype == NOT_TYPE)
								printf("%c%d = ! %c%d\n", i->prefix, i->num, j->prefix, j->num);
						}
						else if(t1 == -2)  // !1
						{
							printWhitespace(whitespace);
							if(exp->optype == NEG_TYPE)
								printf("%c%d = - %d\n", i->prefix, i->num, exp1->num);
							else if(exp->optype == NOT_TYPE)
								printf("%c%d = ! %d\n", i->prefix, i->num, exp1->num);
						}
						else
						{
							printWhitespace(whitespace);
							if(exp->optype == NEG_TYPE)
								printf("%c%d = - t%d\n", i->prefix, i->num, t1);
							else if(exp->optype == NOT_TYPE)
								printf("%c%d = ! t%d\n", i->prefix, i->num, t1);
						}
					}
					else // other operation
					{
						if(t1 == -1)  
						{
							j = find_var(exp1->name);
							#ifdef CHECK
							if(j == NULL)
								yyerror("the variable has not been defined!");
							else if(j->type != INT_VAR)
								yyerror("the variable is not an interger!");
							#endif
							if(t2 == -1)  // a OP b
							{
								Var *k = find_var(exp2->name);
								#ifdef CHECK
								if(k == NULL)
									yyerror("the variable has not been defined!");
								else if(k->type != INT_VAR)
									yyerror("the variable is not an interger!");
								#endif
								printWhitespace(whitespace);
								printf("%c%d = %c%d %s %c%d\n", i->prefix, i->num, j->prefix, j->num, getOp(exp->optype), k->prefix, k->num);
							}
							else if(t2 == -2)  // a OP 1
							{
								printWhitespace(whitespace);
								printf("%c%d = %c%d %s %d\n", i->prefix, i->num, j->prefix, j->num, getOp(exp->optype), exp2->num);
							
							}
							else // a OP t1
							{
								printWhitespace(whitespace);
								printf("%c%d = %c%d %s t%d\n", i->prefix, i->num, j->prefix, j->num, getOp(exp->optype), t2);
							
							}
						}
						else if(t1 == -2)
						{
							if(t2 == -1)  // 1 OP b
							{
								Var *k = find_var(exp2->name);
								#ifdef CHECK
								if(k == NULL)
									yyerror("the variable has not been defined!");
								else if(k->type != INT_VAR)
									yyerror("the variable is not an interger!");
								#endif
								printWhitespace(whitespace);
								printf("%c%d = %d %s %c%d\n", i->prefix, i->num, exp1->num, getOp(exp->optype), k->prefix, k->num);
							
							}
							else if(t2 == -2)  // 1 OP 1
							{
								printWhitespace(whitespace);
								printf("%c%d = %d %s %d\n", i->prefix, i->num, exp1->num, getOp(exp->optype), exp2->num);
							
							}
							else  // 1 OP t1
							{
								printWhitespace(whitespace);
								printf("%c%d = %d %s t%d\n",i->prefix, i->num, exp1->num, getOp(exp->optype), t2);
							
							}
						}
						else
						{
							if(t2 == -1)  // t1 OP a
							{	
								Var *k = find_var(exp2->name);
								#ifdef CHECK
								if(k == NULL)
									yyerror("the variable has not been defined!");
								else if(k->type != INT_VAR)
									yyerror("the variable is not an interger!");
								#endif
								printWhitespace(whitespace);
								printf("%c%d = t%d %s %c%d\n", i->prefix, i->num, t1, getOp(exp->optype), k->prefix, k->num);
							}
							else if(t2 == -2)  // t1 OP 1
							{
								printWhitespace(whitespace);
								printf("%c%d = t%d %s %d\n", i->prefix, i->num, t1, getOp(exp->optype), exp2->num);
							}
							else  // t1 OP t2
							{
								printWhitespace(whitespace);
								printf("%c%d = t%d %s t%d\n", i->prefix, i->num, t1, getOp(exp->optype), t2);
							}
						}
					}
				}
				else  // function
				{
					exp = root->child[0];
					f = find_func(exp->name);
					#ifdef CHECK
					if(f == NULL)
						yyerror("the function has not been declared!");
					#endif
					j = exp->para;
					#ifdef CHECK
					if(j == NULL)
					{
						if(f->num != 0)
							yyerror("wrong parameters!");
					}
					else if(j->length != f->num)  yyerror("wrong parameters!");
					#endif
					Var *k = f->para;
					while(j!=NULL && k!=NULL)
					{
						Var *p = find_var(j->name);
						#ifdef CHECK
						if(p == NULL)  yyerror("wrong parameters!");
						else if(p->type != k->type) yyerror("wrong parameters!");
						else
						{
							#endif
							printWhitespace(whitespace);
							printf("param %c%d\n", p->prefix, p->num);
						#ifdef CHECK
						}
						#endif
						j = j->next;
						k = k->next;
					}
					printWhitespace(whitespace);
					printf("%c%d = call f_%s\n", i->prefix, i->num, exp->name);
				}
				break;
			case ARRAYASSIGN_TYPE:
				i = find_var(root->name);
				#ifdef CHECK
				if(i == NULL) 
					yyerror("the variable has not been defined!");
				else if(i->type != ARRAY_VAR)
					yyerror("the variable is not an array!");
				#endif
				exp1 = root->child[0];
				exp2 = root->child[1];
				t1 = printExp(exp1, whitespace);
				t2 = printExp(exp2, whitespace);
				if(t1 == -1)
				{
					j = find_var(exp1->name);
					if(t2 == -1)  // a[i] = j
					{
						Var *k = find_var(exp2->name);
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * %c%d\n", tempvar, j->prefix, j->num);
						printWhitespace(whitespace);
						printf("%c%d [t%d] = %c%d\n", i->prefix, i->num, tempvar, k->prefix, k->num);
						tempvar ++;
					}
					else if(t2 == -2)  // a[i] = 1
					{
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * %c%d\n", tempvar, j->prefix, j->num);
						printWhitespace(whitespace);
						printf("%c%d [t%d] = %d\n", i->prefix, i->num, tempvar, exp2->num);
						tempvar ++;
					}
					else  // a[i] = t0
					{
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * %c%d\n", tempvar, j->prefix, j->num);
						printWhitespace(whitespace);
						printf("%c%d [t%d] = t%d\n", i->prefix, i->num, tempvar, t2);
						tempvar ++;
					}
				}
				else if(t1 == -2)
				{
					if(t2 == -1)  // a[1] = i
					{
						Var *k = find_var(exp2->name);
						printWhitespace(whitespace);
						printf("%c%d [%d] = %c%d\n", i->prefix, i->num, 4*exp1->num, k->prefix, k->num);				
					}
					else if(t2 == -2)  // a[1] = 1
					{
						printWhitespace(whitespace);
						printf("%c%d [%d] = %d\n", i->prefix, i->num, 4*exp1->num, exp2->num);
					}
					else
					{
						printWhitespace(whitespace);
						printf("%c%d [%d] = t%d\n", i->prefix, i->num, 4*exp1->num, t2);
					}
				}
				else
				{
					if(t2 == -1)  // a[t0] = i
					{
						Var *k = find_var(exp2->name);
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * t%d\n", tempvar, t1);
						printWhitespace(whitespace);
						printf("%c%d [t%d] = %c%d\n", i->prefix, i->num, tempvar, k->prefix, k->num);
						tempvar ++;
					}
					else if(t2 == -2)  // a[t0] = 1
					{
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * t%d\n", tempvar, t1);
						printWhitespace(whitespace);
						printf("%c%d [t%d] = %d\n", i->prefix, i->num, tempvar, exp2->num);
						tempvar ++;
					}
					else  // a[t0] = t1
					{
						printWhitespace(whitespace);
						printf("var t%d\n", tempvar);
						printWhitespace(whitespace);
						printf("t%d = 4 * t%d\n", tempvar, t1);
						printWhitespace(whitespace);
						printf("%c%d [t%d] = t%d\n", i->prefix, i->num, tempvar, t2);
						tempvar ++;
					}
				}
				break;
			case RETURN_TYPE:
				t = printExp(root->child[0], whitespace);
				if(t == -1)  // return a 
				{
					Var *i;
					i = find_var(root->child[0]->name);
					#ifdef CHECK
					if(i == NULL)  yyerror("the variable has not been defined!");
					else if(i->type == ARRAY_VAR)
						yyerror("the variable is not an interger!");
					#endif
					printWhitespace(whitespace);
					printf("return %c%d\n", i->prefix, i->num);
				}
				else if(t == -2)  // return 0
				{
					printWhitespace(whitespace);
					printf("return %d\n", root->child[0]->num);
				}
				else  // return exp
				{
					printWhitespace(whitespace);
					printf("return t%d\n", t);
				}
				break;
			case FUNCDEF_TYPE:
				f = find_func(root->name);
				if(f != NULL)  
				{
					f->lineno = root->lineno;
					#ifdef CHECK
					Var *i = root->para;
					Var *j = f->para;
					if(i==NULL && f->num!=0)
						yyerror("contradictory function defination!");
					else if(i!=NULL && i->length!=f->num)
						yyerror("contradictory function defination!");
					else
					{
						while(i!=NULL && j!=NULL)
						{
							if(i->type != j->type)
							{
								yyerror("contradictory function defination!");
								break;
							}
							i = i->next;
							j = j->next;
						}
					}
					#endif
				}
				else
				{
					f = newFunction();
					f->name = copyString(root->name);
					if(root->para == NULL)
						f->num = 0;
					else
					{
						f->para = root->para;
						f->num = root->para->length;
					}
					f->lineno = root->lineno;
					insert_functab(f);
				}
				i = root->para;
				t = 0;
				while(i != NULL)
				{
					Var *v = newVariable(i->type);
					v->name = copyString(i->name);
					v->num = t;
					t ++;
					v->prefix = 'p';
					v->lineno = i->lineno;
					insert_vartab(v);
					i = i->next;
				}

				printWhitespace(whitespace);
				printf("f_%s [%d]\n", root->name, root->num);
				printTree(root->child[0], whitespace+2);
				printWhitespace(whitespace);
				printf("end f_%s\n", root->name);
				delete_var(root->lineno);
				break;
			case FUNC_TYPE:
				f = find_func(root->name);
				#ifdef CHECK
				if(f == NULL)
					yyerror("the function has not been declared!");
				#endif
				i = root->para;
				#ifdef CHECK
				if(i==NULL)
				{
					if(f->num != 0)
						yyerror("wrong parameters!");
				}
				else if(i->length != f->num)  yyerror("wrong parameters!");
				#endif
				j = f->para;
				while(i != NULL)
				{
					Var *p = find_var(i->name);
					#ifdef CHECK
					if(p == NULL)  yyerror("wrong parameters!");
					else if(p->type != j->type)  yyerror("wrong parameters!");
					else
					{
					#endif
						printWhitespace(whitespace);
						printf("param %c%d\n", p->prefix, p->num);
					#ifdef CHECK
					}
					#endif
					i = i->next;
					j = j->next;
				}
				printWhitespace(whitespace);
				printf("call f_%s\n", root->name);
				break;
			case FUNCDECL_TYPE:
				if((f=find_func(root->name)) == NULL)
				{
					f = newFunction();
					f->name = copyString(root->name);
					if(root->para == NULL)
						f->num = 0;
					else
					{
						f->para = root->para;
						f->num = root->para->length;
					}
					insert_functab(f);
				}
				#ifdef CHECK
				else
				{
					Var *i = root->para;
					Var *j = f->para;
					if(i==NULL && f->num!=0)
						yyerror("contradictory function defination!");
					else if(i!=NULL && i->length!=f->num)
						yyerror("contradictory function defination!");
					else
					{
						while(i!=NULL && j!=NULL)
						{
							if(i->type != j->type)
							{
								yyerror("contradictory function defination!");
								break;
							}
							i = i->next;
							j = j->next;
						}
					}
				}
				#endif
				break;
		}
	}
	if(root->next != NULL)
		printTree(root->next, whitespace);
}

/* handle with expression, and return a tamperate variable storing the result */
int printExp(treenode *root, int whitespace)  
{
	if(root == NULL)  return -3;
	// int var
	if(root->type.exptype == ID_EXP)
	{
		#ifdef CHECK
		Var *i;
		i = find_var(root->name);
		if(i == NULL)  yyerror("the variable has not been defined!");
		else if(i->type != INT_VAR)
			yyerror("the variable is not an interger!");
		#endif
		return -1;
	}
	// const
	else if(root->type.exptype == CONST_EXP)
		return -2;
	// array
	else if(root->type.exptype == ARRAY_EXP)
	{
		#ifdef CHECK
		Var *i;
		i = find_var(root->name);
		if(i == NULL)  yyerror("the variable has not been defined!");
		else if(i->type != ARRAY_VAR)
			yyerror("the variable is not an array!");
		#endif
		int t;
		t = printArray(root, whitespace);
		return t;		
	}
	// operation
	else if(root->type.exptype == OP_EXP)
	{
		int t;
		t = printOp(root, whitespace);
		return t;
	}
	// function
	else
	{
		int t;
		t = printFunc(root, whitespace);
		return t;
	}
}

/* print the value of an array number and return the tamperary variable */
int printArray(treenode *root, int whitespace)
{
	Var *j;
	j = find_var(root->name);
	int t;
	t = printExp(root->child[0], whitespace);
	if(t == -1)  // the array's offset is an int var
	{
		Var *i;
		i = find_var(root->child[0]->name);
		#ifdef CHECK
		if(i == NULL)  yyerror("the variable has not been defined!");
		else if(i->type != INT_VAR)
			yyerror("the variable is not an interger!");
		#endif
		printWhitespace(whitespace);
		printf("var t%d\n", tempvar);
		printWhitespace(whitespace);
		printf("t%d = 4 * %c%d\n", tempvar, i->prefix, i->num);
		tempvar ++;
		printWhitespace(whitespace);
		printf("var t%d\n", tempvar);
		printWhitespace(whitespace);
		printf("t%d = %c%d [t%d]\n", tempvar, j->prefix, j->num, tempvar-1);
		tempvar ++;
		return tempvar-1;
	}
	else if(t == -2)  // the array's offset is a const
	{
		printWhitespace(whitespace);
		printf("var t%d\n", tempvar);
		printWhitespace(whitespace);
		printf("t%d = %c%d [%d]\n", tempvar, j->prefix, j->num, 4*root->child[0]->num);
		tempvar ++;
		return tempvar -1;
	}
	else // the array's offset is an expression
	{
		printWhitespace(whitespace);
		printf("var t%d\n", tempvar);
		printWhitespace(whitespace);
		printf("t%d = 4 * t%d\n", tempvar, t);
		tempvar ++;
		printWhitespace(whitespace);
		printf("var t%d\n", tempvar);
		printWhitespace(whitespace);
		printf("t%d = %c%d [t%d]\n", tempvar, j->prefix, j->num, tempvar-1);
		tempvar ++;
		return tempvar-1;
	}
}

/* print an operation expression and return the temperary variable */
int printOp(treenode *root, int whitespace)
{
	int t1;
	int t2;
	t1 = printExp(root->child[0], whitespace);
	t2 = printExp(root->child[1], whitespace);
	treenode *exp1 = root->child[0];
	treenode *exp2 = root->child[1];
	Var *i;
	Var *j;
	if(t2 == -3)  // NOT or NEG
	{
		if(t1 == -1)  // !a
		{
			i = find_var(exp1->name);
			#ifdef CHECK
			if(i == NULL)
				yyerror("the variable has not been defined!");
			else if(i->type != INT_VAR)
			yyerror("the variable is not an interger!");
			#endif
			printWhitespace(whitespace);
			printf("var t%d\n", tempvar);
			printWhitespace(whitespace);
			if(root->optype == NEG_TYPE)
				printf("t%d = - %c%d\n", tempvar, i->prefix, i->num);
			else if(root->optype == NOT_TYPE)
				printf("t%d = ! %c%d\n", tempvar, i->prefix, i->num);
			tempvar ++;
			return tempvar-1;
		}
		else if(t1 == -2)  // !1
		{
			printWhitespace(whitespace);
			printf("var t%d\n", tempvar);
			printWhitespace(whitespace);
			if(root->optype == NEG_TYPE)
				printf("t%d = - %d\n", tempvar, root->child[0]->num);
			else if(root->optype == NOT_TYPE)
				printf("t%d = ! %d\n", tempvar, root->child[0]->num);
			tempvar ++;
			return tempvar-1;
		}
		else
		{
			printWhitespace(whitespace);
			printf("var t%d\n", tempvar);
			printWhitespace(whitespace);
			if(root->optype == NEG_TYPE)
				printf("t%d = - t%d\n", tempvar, t1);
			else if(root->optype == NOT_TYPE)
				printf("t%d = ! t%d\n", tempvar, t1);
			tempvar ++;
			return tempvar-1;
		}
	}
	else  // other operation
	{
		if(t1 == -1)  
		{
			i = find_var(exp1->name);
			#ifdef CHECK
			if(i == NULL)
				yyerror("the variable has not been defined!");
			else if(i->type != INT_VAR)
				yyerror("the variable is not an interger!");
			#endif
			if(t2 == -1)  // a OP b
			{
				j = find_var(exp2->name);
				#ifdef CHECK
				if(j == NULL)
					yyerror("the variable has not been defined!");
				else if(j->type != INT_VAR)
					yyerror("the variable is not an interger!");
				#endif
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = %c%d %s %c%d\n", tempvar, i->prefix, i->num, getOp(root->optype), j->prefix, j->num);
				tempvar ++;
				return tempvar-1;
			}
			else if(t2 == -2)  // a OP 1
			{
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = %c%d %s %d\n", tempvar, i->prefix, i->num, getOp(root->optype), root->child[1]->num);
				tempvar ++;
				return tempvar-1;
			}
			else // a OP t1
			{
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = %c%d %s t%d\n", tempvar, i->prefix, i->num, getOp(root->optype), t2);
				tempvar ++;
				return tempvar-1;
			}
		}
		else if(t1 == -2)
		{
			if(t2 == -1)  // 1 OP b
			{
				j = find_var(exp2->name);
				#ifdef CHECK
				if(j == NULL)
					yyerror("the variable has not been defined!");
				else if(j->type != INT_VAR)
					yyerror("the variable is not an interger!");
				#endif
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = %d %s %c%d\n", tempvar, root->child[0]->num, getOp(root->optype), j->prefix, j->num);
				tempvar ++;
				return tempvar-1;
			}
			else if(t2 == -2)  // 1 OP 1
			{
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = %d %s %d\n", tempvar, root->child[0]->num, getOp(root->optype), root->child[1]->num);
				tempvar ++;
				return tempvar-1;
			}
			else  // 1 OP t1
			{
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = %d %s t%d\n", tempvar, root->child[0]->num, getOp(root->optype), t2);
				tempvar ++;
				return tempvar-1;
			}
		}
		else
		{
			if(t2 == -1)  // t1 OP a
			{
				j = find_var(exp2->name);
				#ifdef CHECK
				if(j == NULL)
					yyerror("the variable has not been defined!");
				else if(j->type != INT_VAR)
					yyerror("the variable is not an interger!");
				#endif
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = t%d %s %c%d\n", tempvar, t1, getOp(root->optype), j->prefix, j->num);
				tempvar ++;
				return tempvar-1;
			}
			else if(t2 == -2)  // t1 OP 1
			{
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = t%d %s %d\n", tempvar, t1, getOp(root->optype), root->child[1]->num);
				tempvar ++;
				return tempvar-1;
			}
			else  // t1 OP t2
			{
				printWhitespace(whitespace);
				printf("var t%d\n", tempvar);
				printWhitespace(whitespace);
				printf("t%d = t%d %s t%d\n", tempvar, t1, getOp(root->optype), t2);
				tempvar ++;
				return tempvar-1;
			}
		}
	}
}

 /* print a function expression and return the tmperary variable */
int printFunc(treenode *root, int whitespace)
{
	Func *f;
	Var *i;
	Var *j;
	f = find_func(root->name);
	#ifdef CHECK
	if(f == NULL)
		yyerror("the function has not been declared!");
	#endif
	i = root->para;
	#ifdef CHECK
	if(i == NULL)
	{
		if(f->num != 0)
			yyerror("wrong parameters!");
	}
	else if(i->length != f->num)  yyerror("wrong parameters!");
	#endif
	printWhitespace(whitespace);
	printf("var t%d\n", tempvar);
	j = f->para;
	while(i!=NULL && j!=NULL)
	{
		Var *p = find_var(i->name);
		#ifdef CHECK
		if(p == NULL)  yyerror("wrong parameters!");
		else if(p->type != j->type)  yyerror("wrong parameters!");
		else
		{
		#endif
			printWhitespace(whitespace);
			printf("param %c%d\n", p->prefix, p->num);
		#ifdef CHECK
		}
		#endif
		i = i->next;
		j = j->next;
	}

	printWhitespace(whitespace);
	printf("t%d = call f_%s\n", tempvar, root->name);
	tempvar ++;
	return tempvar-1;
}

char *getOp(int i)
{
	switch(i)
	{
		case ADD_TYPE:
			return "+";
		case SUB_TYPE:
			return "-";
		case MUL_TYPE:
			return "*";
		case DIV_TYPE:
			return "/";
		case MOD_TYPE:
			return "%";
		case AND_TYPE:
			return "&&";
		case OR_TYPE:
			return "||";
		case LT_TYPE:
			return "<";
		case BT_TYPE:
			return ">";
		case EQ_TYPE:
			return "==";
		case NEQ_TYPE:
			return "!=";
	}
}
