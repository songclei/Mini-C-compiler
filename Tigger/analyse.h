
int globalint = 0;  // number of global int variable
int globalarray = 0;  // number of global array
int localint = 0;  // number of local int variable for every function
int space = 0;  // space needed for every function 
int labelline[50];  // the next instr line of every label
int line = 0;  // line number within every function
bool funccall = 0;  // wether there is a function call within the function
int para_num = 0;  // number of parameters
int caller_saved = 0;  // caller_saved regiest number for every function
int callee_saved = 0;  // callee_saved regiest number for every function
int caller_begin = 0;  // which space to store caller saved register 
int callee_begin = 0;  // which space to store callee saved register
bool reg[28];  // wether regester is used for temporary function

extern FILE *fp;
extern Variable *vartab_head;
extern Variable *vartab_tail;
extern MidCode *code_head;
extern MidCode *code_tail;
extern char *regname[28];


void FuncAnalyse(char *funcname, char *paramnum);
void CodeAnalyse(int n, char part[][LEN]);
int StoreCodeInfo(int n, char part[][LEN]);
void RegAllocation();
void CalculateSpace(char *funcname);
int max(int a,int b);



extern int split(char *code, char part[][LEN]);
extern Variable *NewVar(char *name, VarType type);
extern void InsertVar(Variable *var);
extern void InsertCode(MidCode *midcode);
extern MidCode *NewCode(CodeType type);
extern Variable *FindVar(char *name);
extern void DeletLocalVar();
extern Variable *FindLocalVar(int num);
extern bool IsVar(char *name);
extern void PrintCode(char *funcname, int para_num);