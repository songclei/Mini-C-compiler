
char *regname[28] = {"x0", "s0", "s1", "s2", "s3", "s4", "s5",
					"s6", "s7", "s8", "s9", "s10", "s11", "t0",
					"t1", "t2", "t3", "t4", "t5", "t6", "a0",
					"a1", "a2", "a3", "a4", "a5", "a6", "a7"};

extern Variable *vartab_head;
extern Variable *vartab_tail;
extern MidCode *code_head;
extern MidCode *code_tail;
extern int globalint;
extern int caller_begin;
extern int callee_begin;
extern bool reg[28];


void PrintCode(char *funcname, int para_num);
bool IsVar(char *name);

extern Variable *NewVar(char *name, VarType type);
extern void InsertVar(Variable *var);
extern void InsertCode(MidCode *midcode);
extern MidCode *NewCode(CodeType type);
extern Variable *FindVar(char *name);
extern void DeletLocalVar();
extern Variable *FindLocalVar(int num);
extern int max(int a,int b);