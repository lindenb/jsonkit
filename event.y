
%{
#include <stdio.h>
#include "utils.h"
void jseventerror(char* message) {
fprintf(stderr,"[ERROR] %s\n",message);
}



%}


%union {
CString str;
};


%token JSON_EVENT_ARRAY_OPEN
%token JSON_EVENT_ARRAY_CLOSE
%token JSON_EVENT_OBJECT_OPEN
%token JSON_EVENT_OBJECT_CLOSE
%token JSON_EVENT_COLON
%token JSON_EVENT_COMMA
%token<str> JSON_EVENT_STRING
%token<str> JSON_EVENT_NUMBER
%token JSON_EVENT_TRUE
%token JSON_EVENT_FALSE
%token JSON_EVENT_NULL


%%

input: any;

any: object | array | JSON_EVENT_STRING | JSON_EVENT_NUMBER | JSON_EVENT_TRUE | JSON_EVENT_FALSE | JSON_EVENT_NULL;

array : JSON_EVENT_ARRAY_OPEN array_content JSON_EVENT_ARRAY_CLOSE ;

array_content: | array_list;

array_list: any | array_list JSON_EVENT_COMMA any;

object : JSON_EVENT_OBJECT_OPEN object_content JSON_EVENT_OBJECT_CLOSE ;

object_content: | object_list;

object_list: JSON_EVENT_STRING JSON_EVENT_COLON any | object_list JSON_EVENT_COMMA JSON_EVENT_STRING JSON_EVENT_COLON any;

%%


