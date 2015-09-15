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

%token OPENBRACE ENDBRACE TOKENTASK TOKENNAME TOKENPATH TOKENARGS TOKENENV QUOTE STRING SEMICOLON NAME PATH ENV ARGSNAME ARGSVALUE

%%

tasks:
     | tasks task
     ;

task:
    TOKENTASK  OPENBRACE taskdescs ENDBRACE
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
        printf("taskdesc\n");
    }
    ;

name:
    TOKENNAME realname SEMICOLON
    {
        printf("%s\n", $2);
    }
    ;

realname:
    NAME
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
    ARGSVALUE
    ;

env:
   TOKENENV ENV SEMICOLON
   ;

%%
