
%{
#include "utils.h"
#include "event.tab.h"
%}

%x IN_STRING
%option noyywrap

letter   [a-zA-Z_]
digit    [0-9]
number	 {digit}+
signednumber [-+]?{number}
alphanum {letter}|{digit}
ws       [ \t\n]


%%

<IN_STRING>{
\"	BEGIN(INITIAL); return JSON_EVENT_STRING;
.|\n	;
}


\"	jseventlval.str.s=(char*)malloc(1);jseventlval.str.s[0]=0; jseventlval.str.length=0;BEGIN(IN_STRING); 
{signednumber}(\.{digit}*([eE]{signednumber})?)?	jseventlval.str.s = strdup(yytext); jseventlval.str.length = strlen(jseventlval.str.s); return JSON_EVENT_NUMBER;
true return JSON_EVENT_TRUE;
false return JSON_EVENT_FALSE;
null return JSON_EVENT_NULL;
\[ return JSON_EVENT_ARRAY_OPEN;
\] return JSON_EVENT_ARRAY_CLOSE;
\{ return JSON_EVENT_OBJECT_OPEN;
\} return JSON_EVENT_OBJECT_CLOSE;
,	return JSON_EVENT_COMMA;
\:	return JSON_EVENT_COLON;

{ws}
.  ;

%%

int main(int argc,char** argv)
	{
	return jseventparse();
	}


