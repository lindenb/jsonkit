#include <stdexcept>
#include <cctype>
#include "tokenizer.hh"

using namespace std;

const int Tokenizer::T_EOF=-1;
const int Tokenizer::T_OBJECT_OPEN='{';
const int Tokenizer::T_OBJECT_CLOSE='}';
const int Tokenizer::T_ARRAY_OPEN='[';
const int Tokenizer::T_ARRAY_CLOSE=']';
const int Tokenizer::T_TRUE='t';
const int Tokenizer::T_FALSE='f';
const int Tokenizer::T_NULL='n';
const int Tokenizer::T_STRING='"';
const int Tokenizer::T_NUMBER='9';
const int Tokenizer::T_COLON=':';
const int Tokenizer::T_COMMA=',';	


Tokenizer::Tokenizer(std::istream& in):in(in),str(0) {
}

Tokenizer::~Tokenizer() {
	if( str != 0) delete str;
	}

	
int Tokenizer::read() {
	return in.get();
	}
	
void Tokenizer::expect(const char* s) {
	int i=0;
	while(s[i]!=0)
		{
		int c= read();
		if(c!=s[i]) throw runtime_error("boum");
		++i;
		}
	}

int Tokenizer::next() {
if(str!=0) delete str;
for(;;) {
	int c=read();
	if(c==-1) return T_EOF;
	if(isspace(c)) continue;
	switch(c) {
		case '{': return T_OBJECT_OPEN; break;
		case '}': return T_OBJECT_CLOSE; break;
		case '[': return T_ARRAY_OPEN; break;
		case ']': return T_ARRAY_CLOSE; break;
		case ':': return T_COLON; break;
		case ',': return T_COMMA;break;
		case 't': expect("rue"); return T_TRUE;
		case 'f': expect("alse"); return T_FALSE;
		case 'n': expect("ull"); return T_NULL;
		case '\"':
			{
			str = new string;
			return T_STRING;
			}
		case '+':case '-':
			{
			str = new string(&c,1);
			//
			}
		case '0':case '1': case '2': case '3': case '4':
		case '5':case '6': case '7': case '8': case '9':
			{
			if(str==0) str = new string(&c,1);
			_number();
			c = read();
			str = new string;
			return T_NUMBER;
			}
		}
	}
}

Tokenizer::_integer() {
  int x=0;
  for(;;) {
  int c = this->read();
  if( isdigit(c) ) {
  	str->append((char)c);
  }
  else {
  this->in.unget();
  break;
  }
  x=1;
  }
}


/*
	int next() {
	for(;;) {
	int c=read();
	if(c==-1) return T_EOF;
	if(isspace(c)) continue;
	switch(c) {

		case '\"': 
			{
			str = new std:string;
			for(;;)	{
				int c = read();
				switch(c) {
				    case '\':
				    	{
						case '\"': return xxx;
						}
					default:
						{
						str->append((char)c);
						break;
						}
					}
			return T_STRING;
			break;
			}
			return 0;
			}
		}
	}
	}

*/
