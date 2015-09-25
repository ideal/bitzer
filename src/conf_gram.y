%{
#include "bitzer.h"
#include "conf_gram.h"
#include "conf_scan.h"

#define CONF_DEFAULT_LEN 10

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

#define check_task_member(member, needsize, incrsize)               \
do {                                                                \
    if (task->member ## _len + needsize > task->member ## _total) { \
        /* TODO: check if realloc failed */                         \
        task->member = (char **)realloc(task->member,               \
                               (task->member ## _total + incrsize)  \
                               * sizeof(char *));                   \
        task->member ## _total += incrsize;                         \
    }                                                               \
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
        size_t i;

        // there must be args
        check_task_member(args, 2, 2);
        for (i = task->args_len; i > 0; i--) {
            task->args[i] = task->args[i - 1];
        }
        task->args[0] = (char *)task->path;
        task->args_len++;
        task->args[task->args_len++] = NULL;

        // envp is optional
        if (task->envp) {
            check_task_member(envp, 1, 1);
            task->envp[task->envp_len++] = NULL;
        }

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
        check_task();
        check_task_member(args, 1, CONF_DEFAULT_LEN);
        task->args[task->args_len++] = $2;
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
        check_task();
        check_task_member(envp, 1, CONF_DEFAULT_LEN);
        task->envp[task->envp_len++] = $2;
    }
    ;

%%
