#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"

char blank = '\t';

extern int label;


void PrintFile(char *filename);
void PrintOption();
void PrintComm(char *name, int len);
int PrintFuncBegin(char *name, int s);
void PrintFuncCall(char *name);
void PrintRtypeOp(char *r1, char *r2, char *rd, char *op);
void PrintItypeOp(char *rs, int imm, char *rd, char *op);
void PrintJmp(char *target);
void PrintLabel(char *label);
void PrintIf(char *r1, char *target);
void PrintReturn(int size);
void PrintStore(int offset, char *r1, char *r2);
void PrintFuncEnd(char *name);
void PrintLoad(int offset, char *r1, char *r2);
void PrintLoadAddr(char *var, char *rd);
void PrintFooter();