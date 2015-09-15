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

%token OPENBRACE ENDBRACE TOKENTASK TOKENNAME TOKENPATH TOKENARGS TOKENENV QUOTE STRING SEMICOLON PATH ENV ARGSNAME WORD NUMBER

%%

tasks:
     | tasks task
     ;

task:
    TOKENTASK OPENBRACE taskdescs ENDBRACE
    ;

taskdescs:
    | taskdescs taskdesc
    ;

taskdesc:
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
    TOKENNAME realname SEMICOLON
    {
    }
    ;

realname:
    WORD
    |
    STRING
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
    ARGSNAME
    |
    WORD
    |
    NUMBER
    ;

env:
   TOKENENV ENV SEMICOLON
   ;

%%
