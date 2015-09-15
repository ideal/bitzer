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
    TOKENTASK  OPENBRACE taskdesc ENDBRACE
    ;

taskdesc:
    name
    |
    path
    |
    args
    |
    env
    ;

name:
    TOKENNAME realname SEMICOLON
    ;

realname:
    NAME
    |
    STRING
    {
        printf("name\n");
    }
    ;

path:
    TOKENPATH PATH SEMICOLON
    ;

args:
    TOKENARGS argslist SEMICOLON
    ;

argslist:
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
