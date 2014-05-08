#include "SymbolTable_old.h"

SymbolTable_Old::SymbolTable_Old()
{

}

void SymbolTable_Old::addSymbol(string key, Symbol symbol)
{
	m_SymbolTable[key] = symbol;
}

void SymbolTable_Old::PrintTable()
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

void SymbolTable_Old::Display(string key, Symbol print)
{
	cout << "Symbol: " << key;
	switch (print.getSymbolType())
	{
	case DEFINE:
		cout << " # defined as " << print.getValue() << endl;
		break;
	default: 
		cout << " unknown symbol type" << print.getValue() << endl;
		break;
	}
}