%{
#include "bitzer.h"
#include "conf_gram.h"

typedef void* yyscan_t;

void yyerror(const char *str)
{
    fprintf(stderr, "error : %s", str);
}

int yywrap()
{
    return 1;
}

%}

%lex-param { yyscan_t scanner }
%parse-param { conf_t *conf }
%parse-param {  yyscan_t scanner }

%token OPENBRACE ENDBRACE TOKENTASK TOKENNAME TOKENPATH TOKENARGS TOKENENV SEMICOLON QUOTE

%union
{
    int   number;
    char *string;
}

%token <string> NUMBER
%token <string> ALNUMWORD
%token <string> ALPHAWORD
%token <string> STRING
%token <string> PATH
%token <string> ENVAR
%token <string> ARGKEY

%type <string> taskname
%type <string> arg
%type <string> innerarg

%%

tasks:
     | tasks task
     ;

task:
    TOKENTASK OPENBRACE taskopts ENDBRACE
    ;

taskopts:
    | taskopts taskopt
    ;

taskopt:
    name
    |
    path
    |
    args
    |
    env
    {
    }
    ;

name:
    TOKENNAME taskname SEMICOLON
    {
        printf("taskname: %s\n", $2);
    }
    ;

taskname:
    STRING
    |
    ALPHAWORD
    |
    QUOTE ALPHAWORD QUOTE
    {
        $$ = $2;
    }
    ;

path:
    TOKENPATH PATH SEMICOLON
    {
        printf("path: %s\n", $2);
    }
    ;

args:
    TOKENARGS argslist SEMICOLON
    ;

argslist:
    | argslist arg
    {
        printf("arg: %s\n", $2);
    }
    ;

arg:
    STRING
    |
    ARGKEY
    |
    ALPHAWORD
    |
    ALNUMWORD
    |
    NUMBER
    |
    QUOTE innerarg QUOTE
    {
        $$ = $2;
    }
    ;

innerarg:
    STRING
    |
    ARGKEY
    |
    ALPHAWORD
    |
    ALNUMWORD
    |
    NUMBER
    ;

env:
    TOKENENV ENVAR SEMICOLON
    {
        printf("env: %s\n", $2);
    }
    ;

%%
