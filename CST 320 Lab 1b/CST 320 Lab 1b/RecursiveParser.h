#ifndef RECURSIVE_PARSER
#define RECURSIVE_PARSER
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include "Token.h"
#include "SymbolTable.h"
using namespace std;

class RecursiveParser
{
public:
	RecursiveParser(list<Token> tokens, SymbolTable _symbolTable);
	bool Parse();
	void PrintErrors();
	void PrintSymbolTable();

private:
	void FetchNext();
	void BackOne();
	void Print(string _print);
	
	bool DefineVariable(string key, Token token);
	
	bool Program();
	bool Start();
	bool Definition();
	bool D2();
	bool Data_Definition_List();
	bool Data_Definition();
	bool Function_Definition();
	bool Identifier_List();
	bool Id2();
	bool Func_Args();
	bool Paramater_List();
	bool Brackets();
	bool Statement_Group();
	bool S2();
	bool Statement();
	bool Else();
	bool Return();
	bool Expression();
	bool Unary_Expression();
	bool Primary();
	bool P2(list<Token>::iterator _StatementStart);


	bool m_RunTime;
	bool m_Done;

	list<string> m_errors;

	list<Token> m_tokens;
	list<Token>::iterator m_currentToken;
	//list<Token>::iterator _endToken;

	stack<string> m_ruleTree;

	SymbolTable m_SymbolTable;
};

#endif