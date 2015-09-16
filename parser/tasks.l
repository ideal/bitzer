%{
#include <stdio.h>
#include "y.tab.h"
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
\".*\"     return STRING;
\;         return SEMICOLON;
{number}+  return NUMBER;
[0-9]+[a-zA-Z]*         return ALNUMWORD;
[a-zA-Z][a-zA-Z0-9-]*   return ALPHAWORD;
\/[a-zA-Z0-9]+(\/[a-zA-Z0-9]+)* return PATH;
-(-)?[^ \t\n;0-9-]+             return ARGKEY;
[a-zA-Z][a-zA-Z0-9]*=[^;\n]+    return ENVAR;
\n         ;
[ \t]+     ;

%%
