%{
#include "def.h"
#include "main.h"

//#define CHECK
//#define DEBUG
%}

%token IF ELSE WHILE AND OR EQ NOTEQ NOT 
%token RETURN INT MAIN ERROR
%token <num> NUM
%token <name> ID
%type <Treenode> program goal vardefn funcdefn mainfunc funcdecl
%type <Treenode> body stmt_seq stmt //matched_stmt open_stmt other
%type <Treenode> func_stmt func_exp while_stmt assign_stmt if_stmt
%type <Treenode> return_stmt exp expr simple_exp factor type
%type <Varnode> vardecl param funcfactor funcfact 
%left OR
%left AND 
%left EQ NOTEQ
%left '<' '>' 
%left '+' '-'
%left '*' '/' '%'
%right '!'


%% 
program		: goal 	
				{ 
					#ifdef DEBUG
						printf("%d: program->goal\n", yylineno);
					#endif
					savedTree = $1; 
				}
			;
goal    	: mainfunc	
				{ 
					#ifdef DEBUG
						printf("%d: goal->mainfunc\n", yylineno);
					#endif
					$$ = $1; 
				}
        	| vardefn goal
        		{  
        			#ifdef DEBUG
						printf("%d: goal->vardefn goal\n", yylineno);
					#endif
        			$1->next = $2;
        			$$ = $1;
        		}
        	| funcdefn goal
        		{
        			#ifdef DEBUG
						printf("%d: goal->funcdefn goal\n", yylineno);
					#endif
        			$1->next = $2;
        			$$ = $1;
        		}
        	| funcdecl goal
        		{	
        			#ifdef DEBUG
						printf("%d: goal->funcdecl goal\n", yylineno);
					#endif
        			$1->next = $2;
        			$$ = $1;
        		}
        	;
vardefn 	: type ID ';'
				{	
					#ifdef DEBUG
						printf("%d: vardefn->type id\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = VARDEF_TYPE;
					$$->name = copyString($2);
					$$->num = -1;
				}
        	| type ID '[' NUM ']' ';'
        		{	
        			#ifdef DEBUG
						printf("%d: vardefn->type id[num]\n", yylineno);
					#endif
        			$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = VARDEF_TYPE;
					$$->name = copyString($2);
					$$->num = $4;
        		}
        	;
vardecl 	: type ID
				{	
					#ifdef DEBUG
						printf("%d: vardecl->type id\n", yylineno);
					#endif
					$$ = newVariable(INT_VAR);
					$$->name = copyString($2);
				}
			| type ID '[' ']'
				{	
					#ifdef DEBUG
						printf("%d: vardecl->type id[]\n", yylineno);
					#endif
					$$ = newVariable(ARRAY_VAR);
					$$->name = copyString($2);
				}
			| type ID '[' NUM ']'
				{	
					#ifdef DEBUG
						printf("%d: vardecl->type id[num]\n", yylineno);
					#endif
					$$ = newVariable(ARRAY_VAR);
					$$->name = copyString($2);
				}
        	;
param		:  vardecl ',' param
				{	
					#ifdef DEBUG
						printf("%d: param->vardecl,param\n", yylineno);
					#endif
					$1->next = $3;
					$$ = $1;
					$$->length = $3->length + 1;
				}
			|  vardecl
				{ 	
					#ifdef DEBUG
						printf("%d: param->vardecl\n", yylineno);
					#endif
					$$ = $1; 
					$$->length = 1;
				}
			;
funcdefn	: type ID '(' param ')'  body 
				{	
					#ifdef DEBUG
						printf("%d: funcdefn1\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = FUNCDEF_TYPE;
					$$->para = $4;
					$$->name = copyString($2);
					$$->lineno = $1->lineno;
					if($4 == NULL)
						$$->num = 0;
					else  $$->num = $4->length;
					$$->child[0] = $6;
				}
			| type ID '(' ')'  body 
				{
					#ifdef DEBUG
						printf("%d: funcdefn2\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->lineno = $1->lineno;
					$$->type.stmttype = FUNCDEF_TYPE;
					$$->para = NULL;
					$$->name = copyString($2);
					$$->num = 0;
					$$->child[0] = $5;
				}
        	;
funcdecl	: type ID '(' param ')' ';'
				{	
					#ifdef DEBUG
						printf("%d: funcdecl1\n", yylineno);
					#endif
					$$->lineno = $1->lineno;
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = FUNCDECL_TYPE;
					$$->name = copyString($2);
					$$->para = $4;
					if($4 == NULL)
						$$->num = 0;
					else  $$->num = $4->length;
				}
			| type ID '(' ')' ';'
				{
					#ifdef DEBUG
						printf("%d: funcdecl2\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->lineno = $1->lineno;
					$$->type.stmttype = FUNCDECL_TYPE;
					$$->name = copyString($2);
					$$->para = NULL;
					$$->num = 0;
				}
			;
mainfunc	: INT MAIN '(' ')'  body 
				{	
					#ifdef DEBUG
						printf("%d: mainfunc\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = FUNCDEF_TYPE;
					$$->name = copyString("main");
					$$->num = 0;
					$$->child[0] = $5;
				}
        	;
type    	: INT
				{ 
					#ifdef DEBUG
						printf("%d: type->int\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE); 
				}
       	 	; 
body		: '{' '}'
				{ 
					#ifdef DEBUG
						printf("%d: body->{}\n", yylineno);
					#endif
					$$ = NULL; 
				}
			| '{' stmt_seq '}' 
				{ 
					#ifdef DEBUG
						printf("%d: body->{stmt_seq}\n", yylineno);
					#endif
					$$ = $2; 
				}
			| stmt
				{ 
					#ifdef DEBUG
						printf("%d: body->stmt\n", yylineno);
					#endif
					$$ = $1; 
				}
			;
stmt_seq	: stmt stmt_seq
				{	
					#ifdef DEBUG
						printf("%d: stmt_seq->stmt stmt_seq\n", yylineno);
					#endif
					$1->next = $2;
					$$ = $1;
				}
			| stmt
				{ 	
					#ifdef DEBUG
						printf("%d: stmt_seq->stmt\n", yylineno);
					#endif
					$$ = $1; 
				}
			;
stmt 		: if_stmt
				{ 
					#ifdef DEBUG
						printf("%d: stmt->if_stmt\n", yylineno);
					#endif
					$$ = $1; 
				}
			| while_stmt
				{ 
					#ifdef DEBUG
						printf("%d: stmt->while_stmt\n", yylineno);
					#endif
					$$ = $1; 
				}
			| assign_stmt
				{ 
					#ifdef DEBUG
						printf("%d: stmt->assign_stmt\n", yylineno);
					#endif
					$$ = $1; 
				}
			| vardefn
				{ 
					#ifdef DEBUG
						printf("%d: stmt->vardefn\n", yylineno);
					#endif
					$$ = $1; 
				}
			| return_stmt 
				{ 
					#ifdef DEBUG
						printf("%d: stmt->return_stmt\n", yylineno);
					#endif
					$$ = $1; 
				}
			| func_stmt
				{ 
					#ifdef DEBUG
						printf("%d: stmt->func_stmt\n", yylineno);
					#endif
					$$ = $1; 
				}
			| funcdecl
				{
					#ifdef DEBUG
						printf("%d: stmt->funcdecl\n", yylineno);
					#endif
					$$ = $1;
				}
			;
if_stmt		: IF '(' exp ')' body ELSE body
				{	
					#ifdef DEBUG
						printf("%d: if2\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->lineno = $3->lineno;
					$$->type.stmttype = IF_TYPE;
					$$->child[0] = $3;
					$$->child[1] = $5;
					$$->child[2] = $7;
				}
			| IF '(' exp ')' body 
				{	
					#ifdef DEBUG
						printf("%d: if1\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->lineno = $3->lineno;
					$$->type.stmttype = IF_TYPE;
					$$->child[0] = $3;
					$$->child[1] = $5;
				}

func_stmt 	: ID '(' funcfactor ')' ';'
				{	
					#ifdef DEBUG
						printf("%d: func_stmt\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = FUNC_TYPE;
					$$->para = $3;
					$$->name = copyString($1);
				}
			;
func_exp	: ID '(' funcfactor ')'
				{	
					#ifdef DEBUG
						printf("%d: func_exp\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = FUNC_EXP;
					$$->name = copyString($1);
					$$->para = $3;
				}
			;
funcfact	: funcfact ',' ID
				{	
					#ifdef DEBUG
						printf("%d: funcfact->funcfact, id\n", yylineno);
					#endif
					Var *j = newVariable(0);
					j->name = copyString($3);
					Var *i = $1;
					while(i->next != NULL)
						i = i->next;
					i->next = j;
					$1->length += 1;
					$$ = $1;
				}
			| ID
				{	
					#ifdef DEBUG
						printf("%d: funcfact->id\n", yylineno);
					#endif
					$$ = newVariable(0);
					$$->name = copyString($1);
					$$->length = 1;
				}
			; 
funcfactor  : funcfact
				{ 
					#ifdef DEBUG
						printf("%d: funcfactor->funcfact\n", yylineno);
					#endif
					$$ = $1; 
				}
			|
				{ 
					#ifdef DEBUG
						printf("%d: funcfactor->\n", yylineno);
					#endif
					$$ = NULL; 
				}
			;
while_stmt  : WHILE '(' exp ')'  body
				{	
					#ifdef DEBUG
						printf("%d: while_stmt\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = WHILE_TYPE;
					$$->lineno = $3->lineno;
					$$->child[0] = $3;
					$$->child[1] = $5;
				}
			;
assign_stmt : ID '=' exp ';'
				{	
					#ifdef DEBUG
						printf("%d: assign_stmt1\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = ASSIGN_TYPE;
					$$->child[0] = $3;
					$$->name = copyString($1);
				}
			| ID '[' exp ']' '=' exp ';'
	 			{	
	 				#ifdef DEBUG
						printf("%d: assign_stmt2\n", yylineno);
					#endif
	 				$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = ARRAYASSIGN_TYPE;
					$$->child[0] = $3;
					$$->child[1] = $6;
					$$->name = copyString($1);
				}
			;
return_stmt : RETURN exp ';'
				{	
					#ifdef DEBUG
						printf("%d: return_stmt\n", yylineno);
					#endif
					$$ = newTreeNode(STMT_TYPE);
					$$->type.stmttype = RETURN_TYPE;
					$$->child[0] = $2;
				}
			;
exp			: expr OR expr
				{	
					#ifdef DEBUG
						printf("%d: exp-> or\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = OR_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| expr AND expr
				{	
					#ifdef DEBUG
						printf("%d: exp-> and\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = AND_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| expr
				{ 	
					#ifdef DEBUG
						printf("%d: exp->expr\n", yylineno);
					#endif
					$$ = $1; 
				}
			;
expr		: simple_exp EQ simple_exp
				{	
					#ifdef DEBUG
						printf("%d: expr-> EQ\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = EQ_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp NOTEQ simple_exp
				{	
					#ifdef DEBUG
						printf("%d: expr->NOTEQ\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = NEQ_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp '>' simple_exp
				{	
					#ifdef DEBUG
						printf("%d: expr-> >\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = BT_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp '<' simple_exp
				{	
					#ifdef DEBUG
						printf("%d: expr-> <\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = LT_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp
				{ 	
					#ifdef DEBUG
						printf("%d: expr->simple_exp\n", yylineno);
					#endif
					$$ = $1; 
				}
			;
simple_exp 	: simple_exp '+' simple_exp
				{	
					#ifdef DEBUG
						printf("%d: simple_exp-> +\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = ADD_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp '-' simple_exp
				{	
					#ifdef DEBUG
						printf("%d: simple_exp-> -\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = SUB_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp '*' simple_exp
				{	
					#ifdef DEBUG
						printf("%d: simple_exp-> *\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = MUL_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp '/' simple_exp
				{	
					#ifdef DEBUG
						printf("%d: simple_exp-> /\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = DIV_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| simple_exp '%' simple_exp
				{	
					#ifdef DEBUG
						printf("%d: simple_exp-> %\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = MOD_TYPE;
					$$->child[0] = $1;
					$$->child[1] = $3;
				}
			| factor
				{ 	
					#ifdef DEBUG
						printf("%d: simple_exp->factor\n", yylineno);
					#endif
					$$ = $1; 
				}
			;
factor 		: '(' exp ')'	
				{ 
					#ifdef DEBUG
						printf("%d: factor->(exp)\n", yylineno);
					#endif
					$$ = $2; 
				}
			| NUM	
				{	
					#ifdef DEBUG
						printf("%d: factor->num\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = CONST_EXP;
					$$->num = $1;
				}
			| ID
				{	
					#ifdef DEBUG
						printf("%d: factor->id\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = ID_EXP;
					$$->name = copyString($1);
				}
			| ID '[' exp ']'
				{	
					#ifdef DEBUG
						printf("%d: factor->id[exp]\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = ARRAY_EXP;
					$$->name = copyString($1);
					$$->child[0] = $3;
				}
			| func_exp
				{ 	
					#ifdef DEBUG
						printf("%d: factor->func_exp\n", yylineno);
					#endif
					$$ = $1; }
			| '-' factor
				{	
					#ifdef DEBUG
						printf("%d: factor-> -factor\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = NEG_TYPE;
					$$->child[0] = $2;
				}
			| '!' factor
				{	
					#ifdef DEBUG
						printf("%d: factor->!factor\n", yylineno);
					#endif
					$$ = newTreeNode(EXP_TYPE);
					$$->type.exptype = OP_EXP;
					$$->optype = NOT_TYPE;
					$$->child[0] = $2;
				}
			;
			
%%


int main(int argc, char *argv[])
{
	yyin = fopen(argv[1], "r"); 
	//yyout = fopen("mid_out", "w");
	yyparse();
	printTree(savedTree, 0);
	#ifdef DEBUG
	printSymtab();
	#endif
	fclose(yyin);
	//fclose(yyout);
}

int yyerror(char *msg)
{
	printf("Error encountered: %s \n", msg);
	#ifdef CHECK
	exit(1);
	#endif
}


treenode *newTreeNode(TreeType type)
{
	treenode *t = (treenode *) malloc(sizeof(treenode));
	t->treetype = type;
	int i;
	for(i = 0;i < 3;++ i)  t->child[i] = NULL;
	t->para = NULL;
	t->next = NULL;
	t->lineno = yylineno;
	t->name = NULL;
	return t;
}


/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char *copyString(char *s)
{ 
	int n;
  	char *t;
  	if(s == NULL) return NULL;
  	n = strlen(s) + 1;
  	t = malloc(n);
  	strcpy(t,s);
  	return t;
}
