%{
#include "def.h"
#include "y.tab.h"

%}

%x COMMENT

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"while"         {return WHILE;}
"&&"            {return AND;}
"||"            {return OR;}
"=="            {return EQ;}
"!="            {return NOTEQ;}
"!"             {return '!';}
"<"             {return '<';}
">"             {return '>';}
"+"             {return '+';}
"-"             {return '-';}
"*"             {return '*';}
"/"             {return '/';}
"%"             {return '%';}
"="             {return '=';}
"("             {return '(';}
")"             {return ')';}
"["             {return '[';}
"]"             {return ']';}
";"             {return ';';}
","				{return ',';}
"//"            {BEGIN COMMENT;}
"return"        {return RETURN;}
"int"           {return INT;}
"main"          {return MAIN;}
{number}        {yylval.num = atoi(yytext); return NUM;}
{identifier}    {yylval.name = strdup(yytext); return ID;}
{newline}		{yylineno ++;}
{whitespace}    {/* skip whitespace */}
"{"				{return '{';}
"}"				{return '}';}
.				{return yytext[0]; }
<COMMENT>.		{}
<COMMENT>\n 	{BEGIN INITIAL; }


%%


int yywrap()
{
	return 1;
}
