%{
#include "bitzer.h"
#include "conf_gram.h"

%}

%option reentrant
%option noyywrap
%option bison-bridge
%option bison-locations

number [0-9]

%%

\{         return OPENBRACE;
\}         return ENDBRACE;
task       return TOKENTASK;
name       return TOKENNAME;
path       return TOKENPATH;
args       return TOKENARGS;
env        return TOKENENV;
dir        return TOKENDIR;
\"         return QUOTE;
\;         return SEMICOLON;
{number}+  yylval->string=strdup(yytext); return NUMBER;
[0-9]+[_a-zA-Z-]*        yylval->string=strdup(yytext); return ALNUMWORD;
[a-zA-Z][_a-zA-Z0-9-]*   yylval->string=strdup(yytext); return ALPHAWORD;
\/[a-zA-Z0-9]+(\/[-_+a-zA-Z0-9]+)* yylval->string=strdup(yytext); return PATH;
-(-)?[^ \t\n;0-9-]+             yylval->string=strdup(yytext); return ARGKEY;
[a-zA-Z][a-zA-Z0-9]*=[^;\n]+    yylval->string=strdup(yytext); return ENVAR;
\n         ;
[ \t]+     ;

%%

