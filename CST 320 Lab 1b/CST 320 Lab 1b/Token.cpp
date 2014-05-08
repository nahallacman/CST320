#include "Token.h"
Token::Token()
{
	m_isDefined = false;
	m_value = "";
	m_type = TokenType::UNDEFINED;
}

Token::Token(string value, TokenType type) : m_value(value), m_type(type)
{
	m_isDefined = false;
}

Token::Token(string value, TokenType type, bool isDefined) : m_value(value), m_type(type), m_isDefined(isDefined)
{
}

TokenType Token::getTokenType()
{
	return m_type;
}

string Token::getString()
{
	return m_value;
}
	
void Token::setString(string value)
{
	m_value = value;
}

bool Token::getIsDefined()
{
	return m_isDefined;
}

void Token::setIsDefined(bool defined)
{
	m_isDefined = defined;
}