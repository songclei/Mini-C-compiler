#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cstring>

//#define DEBUG

#define LEN 30

/******************** store code information ********************/

typedef int CodeType;

#define VAR_DECL 0
#define ARRAY_DECL 1
#define IF_STAT 2
#define GOTO_STAT 3
#define RETURN_STAT 4
#define PARAM_STAT 5
#define FUNC_END 6
#define FUNC_CALL 7
#define FUNC_BEGIN 8
#define VAR_FUNCASSIGN 9
#define VAR_TWOOPASSIGN 10
#define VAR_ARRAYASSIGN 11
#define VAR_ONEOPASSIGN 12
#define VAR_ASSIGN 13
#define ARRAY_ASSIGN 14
#define LABEL 15
#define NONE_TYPE 16  // used in dead code elimitation


typedef struct Midcode {
	int line;
	CodeType type;
	int define;  // delete from list
	int use[2];  // add to list
	char part[10][LEN];
	struct Midcode *next;
	struct Midcode *before;
} MidCode;



/************************ store variable ************************/
typedef int VarType;

#define GLOBAL_INT 0
#define GLOBAL_ARRAY 1
#define LOCAL_INT 2
#define LOCAL_ARRAY 3


typedef struct variable
{
	char name[5];
	int reg;  // allocated reg number
	union Num{
		int localint;
		int globalint;
		int globalarray;
		int localarray;  // local array address
	} num;
	VarType type;
	struct variable *next;
} Variable;