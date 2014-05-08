#ifndef SYMBOLTABLE_OLD_H
#define SYMBOLTABLE_OLD_H

#include <string>
#include <list>
#include <map>
#include "Symbol.h"
#include <iostream>
using namespace::std;


class SymbolTable_Old{

public:
	SymbolTable_Old();
	void addSymbol(string key, Symbol symbol);
	void PrintTable();
	void Display(string key, Symbol print);

private:
	map<string,Symbol> m_SymbolTable;
};

#endif