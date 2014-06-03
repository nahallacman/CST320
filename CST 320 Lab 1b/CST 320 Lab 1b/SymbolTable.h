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
	bool checkSymbolTableLevel(string _key, int _level);

	//void addSymbol(string key, Token symbol);
	void addSymbolLevel(string key, int level, Token token);

	void PrintTable();
	void PrintTableLevel();

	void Display(string key, Token print);
	void DisplayLevel(string key, int level, Token print);

	Token GetToken(string key);
	Token GetTokenLevel(string _key, int _level);

	//void addFunction(string key, list<Token> _funcBody);
	list<Token> GetFunctionDefinition(string key);
	list<Token> GetFunctionDefinitionLevel(string _key, int _level);

	void ClearSymbolTableBesidesFunctions();
	void ClearSymbolTableLevelBesidesFunctions();

private:
	map<string,Token> m_SymbolTable;
	map<pair<string, int>, Token> m_SymbolTable2;
	//map<string, list<Token> > m_FunctionTable;
};

#endif