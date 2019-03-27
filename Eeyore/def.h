#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/************************** the following part is for symbol table ***************************/


typedef int vartype;
#define INT_VAR 0
#define ARRAY_VAR 1


typedef char pretype;
#define GLOBAL_VAR 'T'
#define FUNC_VAR 'p'


typedef struct Variable
{
	struct Variable *next;
	char *name;
	vartype type;
	int length;
	// length of array, also used as number of parameters in a function
	int lineno;
	int num;  // Tnum
	pretype prefix; 
}Var;



typedef struct Function 
{
	struct Function *next;
	struct Variable *para;
	char *name;
	int lineno;
	int num;  // number of parameters
}Func;


/************************** the following part is for syntaxtree ***************************/


typedef int TreeType;
#define STMT_TYPE 0
#define EXP_TYPE 1

/* used for statemnt */
typedef int StmtType;
#define VARDEF_TYPE 0
#define IF_TYPE 1
#define WHILE_TYPE 2
#define ASSIGN_TYPE 3
#define ARRAYASSIGN_TYPE 4
#define RETURN_TYPE 5
#define	FUNCDEF_TYPE 6
#define FUNC_TYPE 7
#define FUNCDECL_TYPE 8

/* used for expression */
typedef int ExpType;
#define OP_EXP 0
#define CONST_EXP 1
#define ID_EXP 2
#define FUNC_EXP 3
#define ARRAY_EXP 4

typedef int OPTYPE;
#define ADD_TYPE 0  // +
#define SUB_TYPE 1  // -
#define MUL_TYPE 2  // *
#define DIV_TYPE 3  // /
#define MOD_TYPE 4  // %
#define NOT_TYPE 5  // !
#define NEG_TYPE 6  // -
#define AND_TYPE 7  // &&
#define OR_TYPE  8  // ||
#define LT_TYPE  9  // <
#define BT_TYPE  10 // >
#define EQ_TYPE  11 // ==
#define NEQ_TYPE 12 // !=

typedef struct TreeNode
{
	TreeType treetype;  
	struct TreeNode *next;  
	struct TreeNode *child[3];  
	struct Variable *para;
	union
	{
		StmtType stmttype;
		ExpType exptype;
	} type;
	int lineno;  
	char *name;
	int num;  // when VARDEF_TYPE it means length of array.
			  // -1 means it is an integer
	OPTYPE optype;
} treenode;


/***************************************************************************************/


typedef union
{
	treenode *Treenode;
	Var *Varnode;
	int num;
	char *name;
} Node;

#define YYSTYPE Node
