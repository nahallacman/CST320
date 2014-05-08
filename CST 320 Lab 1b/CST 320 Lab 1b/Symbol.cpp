#include "Symbol.h"

Symbol::Symbol()
{
	m_symbol = NONE;
	m_value = "";
}

Symbol::Symbol(symbolType symbol):m_symbol(symbol)
{
	m_value = "";
}

Symbol::Symbol(symbolType symbol, string value):m_symbol(symbol), m_value(value)
{}

string Symbol::getValue()
{
	return m_value;
}

symbolType Symbol::getSymbolType()
{
	return m_symbol;
}