#include "LL1Parser.h"

LL1Parser::LL1Parser(list<Token> tokens) :_tokens(tokens)
{
	_Done == false;
	Token push("$", TokenType::UNDEFINED);
	_tokens.push_back(push);
	_currentToken = _tokens.begin();
	buildRuleTable();
}

bool LL1Parser::Parse()
{
	bool retVal(true);
	string errorString;
	string test = "";
	string test_2 = "";
	char space = ' ';
	while ( _currentToken != _tokens.end() || retVal == true || _Done == false )
	{
		test = "";
		if (_ruleStack.top() == _currentToken->getString())
		{
			cout << "Matched a character: " << _currentToken->getString() << endl;
			string getstringvalue = _currentToken->getString();
			string dollarsign = "$";
			if (getstringvalue == dollarsign)
			{
				_Done == true;
			}
			else
			{
				FetchNext();
				_ruleStack.pop();
			}
			
		}
		else
		{
			test = GetRule(_ruleStack.top(), _currentToken->getString());
			if (test != "")
			{
				_ruleStack.pop();
				//push entire "test" string onto rule stack in reverse order
				test_2 = string(test.rbegin(), test.rend());
				for (auto itr = test_2.begin(); itr != test_2.end(); ++itr)
				{
					if (*itr == space)
					{
						//do nothing
					}
					else
					{
						string s;
						s += *itr;
						_ruleStack.push(s);
						cout << *itr << endl;
					}
				}
			}
			else
			{
				errorString += "Error when trying to find rule [ ";
				errorString += _ruleStack.top();
				errorString += " , ";
				errorString += _currentToken->getString();
				errorString += " ]";
				_errors.push_back(errorString);
				retVal = false;
			}
		}
		//if the rule matching was successful 
		//_currentToken++;
	}
	return retVal;
}

//could make this return true if the value was already assigned if needed later
bool LL1Parser::AddRule(string _key, string _input, string _result)
{
	pair<string, string> _pair; 
	_pair.first = _key;
	_pair.second = _input;
	_ruleMap[_pair] = _result;
	return false;
}

string LL1Parser::GetRule(string _key, string _input)
{
	pair<string, string> _pair;
	_pair.first = _key;
	_pair.second = _input;
	return _ruleMap[_pair];
}

void LL1Parser::FetchNext()
{
	++_currentToken;

	if (_currentToken != _tokens.end())
	{
		_Done = false;
		//++_currentToken;
	}
	else
	{
		if (_currentToken == _tokens.end())
		{
			--_currentToken; //testing this, may want a conditional on the fetch next, not this construct.
		}
		_Done = true;
		//++_currentToken;
		_errors.push_back("unexpected end of program durring FetchNext");
	}
}

/*
for language set:
S - > a S a
S - > b S b
S - > c
*/
void LL1Parser::buildRuleTable()
{
	_ruleStack.push("$");
	_ruleStack.push("S");
	AddRule("S", "a", "a S a");
	AddRule("S", "b", "b S b");
	AddRule("S", "c", "c");
}