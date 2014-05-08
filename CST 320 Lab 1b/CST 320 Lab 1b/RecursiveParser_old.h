#ifndef RECURSIVE_PARSER_OLD
#define RECURSIVE_PARSER_OLD
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include "Token.h"
using namespace std;

class RecursiveParser_Old
{
	void FetchNext();
	void BackOne();
	//void PrintErrors();

	
	bool Program();
	bool DeclarationList();
	bool Type();
	bool DeclarationType();
	bool ExtraVar();
	bool Params();
	bool ParamList();
	bool CompoundStatement();
	bool LocalDeclarations();
	bool StatementList();
	bool Statement();
	bool SelectionStatement();
	bool SelectionX();
	bool IterationStatement();
	bool ReturnStatement();
	bool SimpleExpression();
	bool RelX();
	bool RelExpression();
	bool AddX();
	bool AdditiveExpression();
	bool TermX();
	bool Term();
	bool UnaryExpression();
	bool UrnX();
	bool Factor();
	bool IDX();
	bool Args();
	bool ArgList();
	bool LogicOpp();
	bool RelOpp();
	bool AddOpp();
	bool UnaryOpp();
	bool MulOpp();
	bool FunctionCall();
	bool FunctionCallParams();
	bool FunctionParamList();
	bool _Done;
	
	list<string> _errors;

	list<Token> _tokens;
	list<Token>::iterator _currentToken;
	//list<Token>::iterator _endToken;

	stack<string> _ruleTree;
public:
	RecursiveParser_Old(list<Token> tokens);
	bool Parse();
};

#endif