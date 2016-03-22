#include <iostream>
#include <string>

class Tokenizer {

public:
	static const int T_EOF;
	static const int T_OBJECT_OPEN;
	static const int T_OBJECT_CLOSE;
	static const int T_ARRAY_OPEN;
	static const int T_ARRAY_CLOSE;
	static const int T_TRUE;
	static const int T_FALSE;
	static const int T_NULL;
	static const int T_STRING;
	static const int T_NUMBER;
	static const int T_COLON;
	static const int T_COMMA;
private:
	std::istream& in;
	std::string* str;
	
	int read();
	void expect(const char* s);
public:
	Tokenizer(std::istream& in);
	~Tokenizer();
	int next();
private:
	void _number();
};