#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
using namespace::std;

enum symbolType { NONE, DEFINE };

class Symbol{

public:
	Symbol();
	Symbol(symbolType symbol);
	Symbol(symbolType symbol, string value);
	string getValue();
	symbolType getSymbolType();
	//void setString(string value);

private:
	symbolType m_symbol;
	string m_value;
};

#endif