treenode *savedTree;  // syntax tree

int yyerror(char *msg);
treenode *newTreeNode(TreeType type);
char *copyString(char *s);


extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;

extern int yylex();
extern void printSymtab();
extern void printTree(treenode *root, int whitespace);
