#include "Token.h"
Token::Token()
{
	m_isDefined = false;
	m_key = "";
	m_value = "";
	m_type = TokenType::UNDEFINED;
}

Token::Token(string key, TokenType type) : m_key(key), m_type(type)
{
	m_isDefined = false;
}

Token::Token(string key, TokenType type, bool isDefined) : m_key(key), m_type(type), m_isDefined(isDefined)
{
	m_value = "";
}

Token::Token(string key, TokenType type, bool isDefined, string value) : m_key(key), m_type(type), m_isDefined(isDefined), m_value(value)
{

}

Token::Token(string key, TokenType type, bool isDefined, string value, list<Token> _Tvalue) : m_key(key), m_type(type), m_isDefined(isDefined), m_value(value), m_Tokens(_Tvalue)
{

}

TokenType Token::getTokenType()
{
	return m_type;
}

string Token::getString()
{
	return m_key;
}
	
void Token::setString(string key)
{
	m_key = key;
}

bool Token::getIsDefined()
{
	return m_isDefined;
}

void Token::setIsDefined(bool defined)
{
	m_isDefined = defined;
}

void Token::setValue(string value)
{
	m_value = value;
}
string Token::getValue()
{
	return m_value;
}

list<Token> Token::getTokenList()
{
	return m_Tokens;
}