#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace::std;

enum TokenType {UNDEFINED, NUMCONSTANT, STRCONSTANT, SYMBOL, OPERATOR, KEYWORD, LABEL, VARIABLE, DEFINE };

class Token{

public:
	Token();
	Token(string value, TokenType type);
	Token(string value, TokenType type, bool isDefined);
	TokenType getTokenType();
	string getString();
	void setString(string value);
	bool getIsDefined();
	void setIsDefined(bool defined);


private:
	TokenType m_type;
	string m_value;
	bool m_isDefined;
};

#endif