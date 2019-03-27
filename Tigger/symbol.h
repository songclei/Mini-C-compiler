
MidCode *code_head = NULL;
MidCode *code_tail = NULL;

void InsertCode(MidCode *midcode);
MidCode *NewCode(CodeType type);

Variable *vartab_head = NULL;
Variable *vartab_tail = NULL;

Variable *NewVar(char *name, VarType type);
void InsertVar(Variable *var);
Variable *FindVar(char *name);
void DeletLocalVar();
Variable *FindLocalVar(int num);


extern int line;
extern int globalarray;
extern int globalint;
extern int localint;