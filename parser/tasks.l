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
\"         return QUOTE;
\".*\"     return WORD;
;          return SEMICOLON;
[a-zA-Z][a-zA-Z0-9-]* return NAME;
\/[a-zA-Z0-9]+(\/[a-zA-Z0-9]+)* return PATH;
[a-zA-Z][a-zA-Z0-9]*=.+       return ENV;
-[^ \t\n0-9]+                 return ARGSNAME;
[^ \t\n]+                     return ARGSVALUE;
\n         ;
[ \t]+     ;

%%
