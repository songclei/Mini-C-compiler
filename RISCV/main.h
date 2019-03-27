#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"

typedef int bool;
#define FALSE 0
#define TRUE 1

FILE *fp = NULL;

extern char blank;
extern void PrintHeader(char *filename);
extern void PrintComm(char *name, int len);
extern int PrintFuncBegin(char *name, int s);
extern void PrintFuncCall(char *name);
extern void PrintRtypeOp(char *r1, char *r2, char *rd, char *op);
extern void PrintItypeOp(char *rs, int imm, char *rd, char *op);
extern void PrintJmp(char *target);
extern void PrintLabel(char *label);
extern void PrintIf(char *r1, char *target);
extern void PrintReturn(int size);
extern void PrintStore(int offset, char *r1, char *r2);
extern void PrintFuncEnd(char *name);
extern void PrintLoad(int offset, char *r1, char *r2);
extern void PrintLoadAddr(char *var, char *rd);
extern void PrintFooter();


#define LEN 20
int split(char *code, char part[][LEN]);
void AnalyseCode(char *code);
bool IsReg(char *s);
bool IsInt(char *s);