#include "SymbolTable.h"

SymbolTable::SymbolTable()
{

}

void SymbolTable::addSymbol(string key, Token token)
{
	m_SymbolTable[key] = token;
}

void SymbolTable::PrintTable()
{
	cout << "--------Begin Symbol Table--------" << endl;

	auto itr = m_SymbolTable.begin(); 
	auto itr2 = m_SymbolTable.end();
	while (itr != itr2) 
	{
		Display(itr->first,itr->second);
		++itr;
	}

	cout << "---------End Symbol Table---------" << endl;
}

void SymbolTable::Display(string key, Token print)
{
	cout << "Symbol: " << key;
	switch (print.getTokenType())
	{
	case DEFINE:
		cout << " # defined as " << print.getString() << endl;
		break;
	default: 
		cout << " unknown symbol type" << print.getString() << endl;
		break;
	}
}