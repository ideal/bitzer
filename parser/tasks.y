%{
#include <stdio.h>

void yyerror(const char *str)
{
    fprintf(stderr, "error occured: %s\n", str);
}

int yywrap()
{
    return 1;
}

int main(int argc, char *argv[])
{
    yyparse();
}

%}

%token OPENBRACE ENDBRACE TOKENTASK TOKENNAME TOKENPATH TOKENARGS TOKENENV STRING NUMBER ALNUMWORD ALPHAWORD PATH ARGKEY ENVAR SEMICOLON

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
    }
    ;

taskname:
    STRING
    |
    ALPHAWORD
    ;

path:
    TOKENPATH PATH SEMICOLON
    ;

args:
    TOKENARGS argslist SEMICOLON
    ;

argslist:
    | argslist arg

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
    ;

env:
   TOKENENV ENVAR SEMICOLON
   ;

%%
