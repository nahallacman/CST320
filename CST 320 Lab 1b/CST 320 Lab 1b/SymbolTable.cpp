#include "SymbolTable.h"

SymbolTable::SymbolTable()
{

}


void SymbolTable::addSymbolLevel(string key, int level, Token token)
{
	pair<string, int> a;
	a.first = key;
	a.second = level;
	m_SymbolTable2[a] = token;
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

void SymbolTable::PrintTableLevel()
{
	cout << "--------Begin Symbol Table--------" << endl;

	auto itr = m_SymbolTable2.begin();
	auto itr2 = m_SymbolTable2.end();
	while (itr != itr2)
	{
		DisplayLevel(itr->first.first, itr->first.second, itr->second);
		++itr;
	}

	cout << "---------End Symbol Table---------" << endl;
}


void SymbolTable::DisplayLevel(string key, int level, Token print)
{
	cout << "Level: " << level << " Symbol: " << key << " ";
	switch (print.getTokenType())
	{
	case UNDEFINED:
		cout << " Error, somehow an undefined made it this far!" << endl;
		break;
	case NUMCONSTANT:
		cout << " Error, somehow a numconstant made it this far!" << endl;
		break;
	case STRCONSTANT:
		cout << " Error, somehow a strconstant made it this far!" << endl;
		break;
	case SYMBOL:
		cout << " Error, somehow a symbol made it this far!" << endl;
		break;
	case OPERATOR:
		cout << " Error, somehow an operator made it this far!" << endl;
		break;
	case KEYWORD:
		cout << " Error, somehow a keyword made it this far!" << endl;
		break;
	case LABEL:
		cout << " Error, somehow a label made it this far!" << endl;
		break;
	case VARIABLE:
		cout << " is a variable " << print.getString();
		if (print.getIsDefined())
		{
			cout << " defined as: " << print.getValue() << endl;
		}
		else
		{
			cout << " which was declared but not defined." << endl;
		}
		break;
	case DEFINE:
		cout << " is a #define " << print.getString() << " # defined as: " << print.getValue() << endl;
		break;
	case FUNCTION:
		cout << " is a Function " << print.getString() << " defined as: " << print.getValue() << endl;
		break;
	default:
		cout << " ERROR unknown symbol type" << print.getString() << endl;
		break;
	}
}

void SymbolTable::Display(string key, Token print)
{
	cout << "Symbol: " << key << " ";
	switch (print.getTokenType())
	{
	case UNDEFINED:
		cout << " Error, somehow an undefined made it this far!" << endl;
		break;
	case NUMCONSTANT:
		cout << " Error, somehow a numconstant made it this far!" << endl;
		break;
	case STRCONSTANT:
		cout << " Error, somehow a strconstant made it this far!" << endl;
		break;
	case SYMBOL: 
		cout << " Error, somehow a symbol made it this far!" << endl;
		break;
	case OPERATOR:
		cout << " Error, somehow an operator made it this far!" << endl;
		break;
	case KEYWORD: 
		cout << " Error, somehow a keyword made it this far!" << endl;
		break;
	case LABEL: 
		cout << " Error, somehow a label made it this far!" << endl;
		break;
	case VARIABLE:
		cout << " is a variable " << print.getString();
		if (print.getIsDefined())
		{	
			cout << " defined as: " << print.getValue() << endl;
		}
		else
		{
			cout << " which was declared but not defined." << endl;
		}
		break;
	case DEFINE:
		cout << " is a #define " << print.getString() << " # defined as: " << print.getValue() << endl;
		break;
	case FUNCTION:
		cout << " is a Function " << print.getString() << " defined as: " << print.getValue() << endl;
		break;
	default: 
		cout << " ERROR unknown symbol type" << print.getString() << endl;
		break;
	}
}

bool SymbolTable::checkSymbolTableLevel(string _key, int _level)
{
	pair<string, int> a;
	a.first = _key;
	a.second = _level;
	if (m_SymbolTable2[a].getString() == "")
	{
		return true;
	}
	else
	{
		return false;
	}
}

Token SymbolTable::GetTokenLevel(string _key, int _level)
{
	pair<string, int> a;
	a.first = _key;
	a.second = _level;
	return m_SymbolTable2[a];
}

list<Token> SymbolTable::GetFunctionDefinitionLevel(string _key, int _level)
{
	pair<string, int> a;
	a.first = _key;
	a.second = _level;
	return m_SymbolTable2[a].getTokenList();
}

void SymbolTable::ClearSymbolTableBesidesFunctions()
{
	map<string, Token>::iterator itr = m_SymbolTable.begin();
	map<string, Token>::iterator itr2 = itr;
	while( itr != m_SymbolTable.end() )
	{
		itr2 = itr;
		itr++;
		if (itr2->second.getTokenType() != TokenType::FUNCTION)
		{
			m_SymbolTable.erase(itr2->first);
		}
	}
}

void SymbolTable::ClearSymbolTableLevelBesidesFunctions()
{
	map<pair<string,int>, Token>::iterator itr = m_SymbolTable2.begin();
	map<pair<string, int>, Token>::iterator itr2 = itr;
	while (itr != m_SymbolTable2.end())
	{
		itr2 = itr;
		itr++;
		if (itr2->second.getTokenType() != TokenType::FUNCTION)
		{
			m_SymbolTable2.erase(itr2->first);
		}
	}
}