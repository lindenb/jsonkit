#ifndef JSON_STREAM_HH
#define JSON_STREAM_HH
#include <iostream>
#include <vector>
#include <string>

enum JsonToken
	{
	JSON_BEGIN_ARRAY
	};

#define BOUM(a) {\
	std::ostringstream _os; \\
	_os << a ;\
	throw _os.str();\
	}
#define CHECK_STATE_PRESENT \
	if( this->states.empty() ) \
		{ \
		BOUM("State trace is empty"); \
		}

#define CHECK_ANY \
	CHECK_STATE_PRESENT \
	switch( states.back()  ) \
		{\
		case EXPECT_ANY : this->states.pop_back(); break; \
		case ARRAY_EXPECT_ANY : this->states.back() = ARRAY_EXPECT_COMMA_OR_END ; break; \
		case OBJECT_EXPECT_ANY : this->states.back() = OBJECT_EXPECT_COMMA_OR_END ; break; \
		default: BOUM(""); break; \
		}
#define MUST_READ(C) { int c_ = this->in.read(); if (c_ != C ) boum("");}
class JsonReader
	{
	public:
	private:
		enum state_t {
			EXPECT_ANY,
			ARRAY_EXPECT_ANY_COMMA_OR_END,
			ARRAY_EXPECT_COMMA_OR_END,
			OBJECT_EXPECT_NAME,
			OBJECT_EXPECT_COLON,
			OBJECT_EXPECT_ANY_COMMA_OR_END
			OBJECT_EXPECT_COMMA_OR_END
			};
		
		std::istream& in;
		std::vector<state_t> states;
		bool eof_met;
	public:
		JsonReader(std::istream& in):in(in),eof_met(false)
			{
			states.push_back(EXPECT_ANY);
			}
		
		bool hasNext() {
			while(this->opcode == -1 && !eof_met)
				{
				int c = in.read();
				switch(c)
					{
					case -1 : 
						{
						if( ! states.empty() )
							{
							boum();
							}
						eof_met = true;
						return false;
						}
					case ':' : {
						if( states.empty() || !(states.back() == OBJECT_EXPECT_COLON))
							{
							boum();
							}
						if( OBJECT_EXPECT_COLON )
							{
							states.back() = OBJECT_EXPECT_COMMA_OR_END;
							states.push(EXPECT_ANY);
							}
						break;
						}
					case ',' : 
						{
						if( states.empty() )
							{
							boum();
							}
						
						switch( states.back()  )
							{
							case( OBJECT_EXPECT_COMMA_OR_END )
								{
								states.back() = OBJECT_EXPECT_ANY;
								break;
								}
							case( ARRAY_EXPECT_COMMA_OR_END )
								{
								states.back() = ARRAY_EXPECT_ANY;
								break;
								}
							default:
								boum();
								break;
							}
						break;
						}
					case ' ': case '\r' : case '\t' : case '\n': break;
					case 'f':
						{
						CHECK_ANY
						
						MUST_READ('a');
						MUST_READ('l');
						MUST_READ('s');
						MUST_READ('e');
						this->curr_bool = false;
						this->opcode = BOOLEAN;
						break;
						}
					case 't':
						{
						CHECK_ANY
						MUST_READ('r');
						MUST_READ('u');
						MUST_READ('e');
						this->curr_bool = true;
						this->opcode = BOOLEAN;
						break;
						}
					case 'n':
						{
						CHECK_ANY
						MUST_READ('u');
						MUST_READ('l');
						MUST_READ('l');
						this->opcode = NULL;
						break;
						}
					case '[':
						{
						CHECK_ANY
						if( !EXPECT_ANY )
							{
							
							}
						this->opcode = ARRAY_START;
						break;
						}
					
					case '{':
						{
						CHECK_ANY
						states.push_back(OBJECT_EXPECT_NAME);
						this->opcode = OBJECT_START;
						break;
						}
					case '}':
						{
						if( OBJECT_EXPECT_COMMA_OR_END )
							{
							states.pop_back();
							}
						break;
						}
					case '"':
					case '\'':
						{
						int quote_symbol = c;
						CHECK_STATE_PRESENT
						if( states.back() == OBJECT_EXPECT_NAME )
							{
							states.back() = OBJECT_EXPECT_COLON
							}
						else
							{
							
							}
						std::ostringstream sb;
						while((c = in.read())!=-1)
							{
							if(c== quote_symbol ) break;
							if( c=='\\')
								{
								c= in.read();
								if(c==-1) boum();
								switch(c)
									{
									case 'n': c='\n';break;
									case 'r': c='\r';break;
									case 't': c='\t';break;
									case '\'': c='\'';break;
									case '\"': c='\"';break;
									case '\\': c='\\';break;
									default:boum();break;
									}
								}
							sb << (char)c;
							}
						this->opcode = OBJECT_STRING;
						break;
						}
					case '+': case '-': case '.':
					case '0': case '1': case '2': case '3': case '4':
					case '5': case '6': case '7': case '8': case '9':
						{
						CHECK_ANY
						std::ostringstream sb;
						sb << (char)c;
						while((c = in.read())!=-1)
							{
							case '+': case '-': case '.': 
							case 'e': case 'E':
							case '0': case '1': case '2': case '3': case '4':
							case '5': case '6': case '7': case '8': case '9':
								{
								sb << (char)c;
								break;
								}
							default:  break;
							}
						break;
						}
					}
				}
			return this->opcode!=-1;
			}
		
		int next()
			{
			if( !hasNext()) boum();
			int op = this->opcode;
			this->opcode = -1;
			return op;
			}
		
		int peek() {
			if(!hasNext()) return -1;
			return this->opcode;
			}
			
			int c = skipws();
			switch(c)
				{
				case '[' : 
					states.push(STATE_ARRAY);
					return JSON_BEGIN_ARRAY;
				case '{' : 
					states.push(STATE_ARRAY);
					return JSON_BEGIN_OBJET;
				case 't':
					in.putback(c);
					return JSON_BOOL;
				case 'f':
					in.putback(c);
					return JSON_BOOL;
				case 'n':
					in.putback(c);
					return JSON_NULL;
				case '"':
					in.putback(c);
					return JSON_STRING;
					
				default: break;
				}
			return -1;
			}
		
		void beginArray() throws IOException {
        	if( peek() != ARRAY_START ) boum("");
        	this->opcode=-1;
    		}
		
		std::string nextString()
			{
			if(peek() != JSON_STRING) fail();
			int c = in.read();
			if( c!= '"') fail(); 
			std::string sb;
			while((c = in.read()) != -1)
				{
				if(c=='"') return sb;
				
				}
			return "";
			}
	};

#endif

