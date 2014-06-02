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
	bool checkSymbolTable(string key);
	void addSymbol(string key, Token symbol);
	void PrintTable();
	void Display(string key, Token print);
	Token GetToken(string key);

	//void addFunction(string key, list<Token> _funcBody);
	list<Token> GetFunctionDefinition(string key);
	void ClearSymbolTableBesidesFunctions();

private:
	map<string,Token> m_SymbolTable;
	//map<string, list<Token> > m_FunctionTable;
};

#endif