%{
#include "bitzer.h"
#include "conf_gram.h"
#include "conf_scan.h"

static task_t *task;

#define check_task()                    \
do {                                    \
    if (!task) {                        \
        task = task_create(conf->ctx);  \
        if (task) {                     \
            task_init(task, conf->ctx); \
        }                               \
    }                                   \
} while(0)

void yyerror(YYLTYPE *locp, conf_t *cnf, yyscan_t scanner, const char *str)
{
    bz_log_stderr("error in you config file: %s near line: %d", str, locp->first_line);
}

%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%locations
%define api.pure
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
    {
        list_add(&task->list, &conf->tasks_list);
        task = NULL;
    }
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
        check_task();
        task->name = $2;
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
        check_task();
        task->path = $2;
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
