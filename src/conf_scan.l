%{
#include <string.h>

#include "conf_gram.h"

%}

number [0-9]

%%

\{         return OPENBRACE;
\}         return ENDBRACE;
task       return TOKENTASK;
name       return TOKENNAME;
path       return TOKENPATH;
args       return TOKENARGS;
env        return TOKENENV;
\"         return QUOTE;
\;         return SEMICOLON;
{number}+  yylval.string=strdup(yytext); return NUMBER;
[0-9]+[a-zA-Z]*         yylval.string=strdup(yytext); return ALNUMWORD;
[a-zA-Z][a-zA-Z0-9-]*   yylval.string=strdup(yytext); return ALPHAWORD;
\/[a-zA-Z0-9]+(\/[-_+a-zA-Z0-9]+)* yylval.string=strdup(yytext); return PATH;
-(-)?[^ \t\n;0-9-]+             yylval.string=strdup(yytext); return ARGKEY;
[a-zA-Z][a-zA-Z0-9]*=[^;\n]+    yylval.string=strdup(yytext); return ENVAR;
\n         ;
[ \t]+     ;

%%