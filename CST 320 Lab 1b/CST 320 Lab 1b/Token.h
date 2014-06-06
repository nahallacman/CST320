#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <list>
using namespace::std;

enum TokenType {UNDEFINED, NUMCONSTANT, STRCONSTANT, SYMBOL, OPERATOR, KEYWORD, LABEL, VARIABLE, DEFINE, FUNCTION };

class Token{

public:
	Token();
	Token(string key, TokenType type);
	Token(string key, TokenType type, bool isDefined);
	Token(string key, TokenType type, bool isDefined, string value);
	Token(string key, TokenType type, bool isDefined, string value, list<Token> _Tokens);
	TokenType getTokenType();
	string getString();
	void setString(string key);
	bool getIsDefined();
	void setIsDefined(bool defined);
	void setValue(string value);
	string getValue();
	list<Token> getTokenList();

private:
	TokenType m_type;
	string m_key;
	string m_value;
	bool m_isDefined;
	list<Token> m_Tokens;
};

#endif