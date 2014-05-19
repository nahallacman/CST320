#ifndef LL1PARSER
#define LL1PARSER
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include "Token.h"
#include "SymbolTable.h"
using namespace std;

class LL1Parser
{
public:
	LL1Parser(list<Token> tokens);
	bool Parse();
	void PrintErrors();
	//void PrintSymbolTable();

private:
	bool AddRule(string _key, string _input, list<Token> _result);
	list<Token> GetRule(string _key, string _input);
	void buildRuleTable();

	void FetchNext();
	//void BackOne();
	//void Print(string _print);

	bool _Done;

	list<string> _errors;

	list<Token> _tokens;
	list<Token>::iterator _currentToken;
	//list<Token>::iterator _endToken;

	//stack<string> _ruleTree;
	stack<Token> _ruleStack;

	map<pair<string, string>, list<Token>> _ruleMap;

	//SymbolTable m_SymbolTable;
};

#endif