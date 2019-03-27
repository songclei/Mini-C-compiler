Var *vartab = NULL;   // pointer to begining of varible symtable
Func *functab = NULL;  // pointer to function symtable
Var *end_vartab = NULL;  // pointer to end of varible symtable
Func *end_functab = NULL;  // pointer to end of function symtable


void printSymtab();
Var *newVariable(vartype type);
Func *newFunction();
void insert_vartab(Var *t);
void insert_functab(Func *t);
void delete_var(int lineno);
Var *find_var(char *s);
Func *find_func(char *s);

extern int yylineno;
