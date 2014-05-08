#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <list>
#include <map>
#include <iostream>
#include "Token.h"
using namespace::std;


class SymbolTable{

public:
	SymbolTable();
	void addSymbol(string key, Token symbol);
	void PrintTable();
	void Display(string key, Token print);

private:
	map<string,Token> m_SymbolTable;
};

#endif