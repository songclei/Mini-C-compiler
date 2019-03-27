int tempvar = 0;
int tagnum = 0;
int globalvar = 0;


void printWhitespace(int whitespace);
void printTree(treenode *root, int whitespace);
int printExp(treenode *root, int whitespace);
int printArray(treenode *root, int whitespace);
int printOp(treenode *root, int whitespace);
int printFunc(treenode *root, int whitespace);
char *getOp(int i);


extern Var *newVariable(vartype type);
extern Func *newFunction();
extern void insert_vartab(Var *t);
extern void insert_functab(Func *t);
extern void delete_var(int lineno);
extern Var *find_var(char *s);
extern Func *find_func(char *s);
extern int yyerror(char *msg);