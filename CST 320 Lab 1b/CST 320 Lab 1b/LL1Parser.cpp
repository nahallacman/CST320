#include "LL1Parser.h"

LL1Parser::LL1Parser(list<Token> tokens) :_tokens(tokens)
{
	_Done = false;
	Token push("$", TokenType::UNDEFINED);
	_tokens.push_back(push);
	_currentToken = _tokens.begin();
	buildRuleTable();
}

bool LL1Parser::Parse()
{
	bool retVal(true);
	string errorString;
	
	string test_2 = "";
	char space = ' ';
	while ( _currentToken != _tokens.end() && retVal == true && _Done == false )
	{
		list<Token> test;
		while (test.empty() != true)
		{
			test.pop_front();
		}
		
		if (_ruleStack.top().getString() == _currentToken->getString() || _ruleStack.top().getTokenType() == _currentToken->getTokenType()) // might need an OR top of the stack TYPE is the same as current token TYPE
		{
			cout << "Matched a token: " << _currentToken->getString() << endl;
			//cout << "Matched a character: " << _currentToken->getString() << endl;
			if (_currentToken->getString() == "$")
			{
				_Done = true;
			}
			else
			{
				FetchNext();
				_ruleStack.pop();
			}
			
		}
		else if (_ruleStack.top().getString() == " ")
		{
			cout << "lambda at: " << _currentToken->getString() << endl;
			//FetchNext();
			_ruleStack.pop();
		}
		else
		{
			test = GetRule(_ruleStack.top().getString(), _currentToken->getString());
			if (test.empty() != true)
			{
				_ruleStack.pop();
				//push entire "test" token list onto rule stack in reverse order
				list<Token> List = test;
				List.reverse();
				for (auto itr = List.begin(); itr != List.end(); ++itr)
				{
					Token Pushme(itr->getString(), itr->getTokenType());
					_ruleStack.push(Pushme);
					test_2 = itr->getString();
					/*
					else
					{
						string s;
						s += *itr;
						Token s2(s, TokenType::UNDEFINED);
						_ruleStack.push(s2);
						cout << *itr << endl;
					}
					*/
				}
			}
			else
			{
				errorString += "Error when trying to find rule [ ";
				errorString += _ruleStack.top().getString();
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

void LL1Parser::PrintErrors()
{
	for (auto itr = _errors.begin(); itr != _errors.end(); ++itr)
		cout << *itr << endl;
}

//could make this return true if the value was already assigned if needed later
bool LL1Parser::AddRule(string _key, string _input, list<Token> _result)
{
	pair<string, string> _pair; 
	_pair.first = _key;
	_pair.second = _input;
	_ruleMap[_pair] = _result;
	return false;
}

list<Token> LL1Parser::GetRule(string _key, string _input)
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
/*
void LL1Parser::buildRuleTable()
{
	Token DS("$", TokenType::UNDEFINED);
	Token S("S", TokenType::UNDEFINED);
	_ruleStack.push(DS);
	_ruleStack.push(S);


	list<Token> Sa;
	list<Token> Sb;
	list<Token> Sc;

	Token a("a", TokenType::UNDEFINED);
	//Token S("S", TokenType::UNDEFINED);
	Token b("b", TokenType::UNDEFINED);
	Token c("c", TokenType::UNDEFINED);

	Sa.push_front(a);
	Sa.push_front(S);
	Sa.push_front(a);

	Sb.push_front(b);
	Sb.push_front(S);
	Sb.push_front(b);

	Sc.push_front(c);

	AddRule("S", "a", Sa);
	AddRule("S", "b", Sb);
	AddRule("S", "c", Sc);
}
*/
/*
For language set:
S->Line NUM NUM NUM NUM S
S->Circle NUM NUM NUM S
S->Pixel NUM NUM S
S->Rect NUM NUM NUM NUM S
S->Define ID S end S
S->ID S
S->lambda
*/

void LL1Parser::buildRuleTable()
{
	Token DS("$", TokenType::UNDEFINED);
	Token S("S", TokenType::UNDEFINED);
	_ruleStack.push(DS);
	_ruleStack.push(S);


	list<Token> SLine;
	list<Token> SCircle;
	list<Token> SPixel;
	list<Token> SRect;
	list<Token> SDefine;
	list<Token> SID;
	list<Token> SDsign;
	list<Token> SEnd;
	list<Token> Empty;
	//might somehow need a rule for lambda, not exactly sure how that would be done

	Token Num("NUM", TokenType::NUMCONSTANT);
	Token Line("Line", TokenType::UNDEFINED);
	Token Circle("Circle", TokenType::UNDEFINED);
	Token Pixel("Pixel", TokenType::UNDEFINED);
	Token Rect("Rect", TokenType::UNDEFINED);
	Token Define("Define", TokenType::UNDEFINED);
	Token ID("ID", TokenType::VARIABLE); // not sure if type::FUNCTION is correct here
	Token End("end", TokenType::UNDEFINED);
	Token Dsign("$", TokenType::UNDEFINED);
	Token Space(" ", TokenType::UNDEFINED);

	SLine.push_back(Line);			//S->Line NUM NUM NUM NUM S
	SLine.push_back(Num);
	SLine.push_back(Num);
	SLine.push_back(Num);
	SLine.push_back(Num);
	SLine.push_back(S);

	SCircle.push_back(Circle);		//S->Circle NUM NUM NUM S
	SCircle.push_back(Num);
	SCircle.push_back(Num);
	SCircle.push_back(Num);
	SCircle.push_back(S);

	SPixel.push_back(Pixel); 		//S->Pixel NUM NUM S
	SPixel.push_back(Num);
	SPixel.push_back(Num);
	SPixel.push_back(S);

	SRect.push_back(Rect);			//S->Rect NUM NUM NUM NUM S
	SRect.push_back(Num);
	SRect.push_back(Num);
	SRect.push_back(Num);
	SRect.push_back(Num);
	SRect.push_back(S);

	SDefine.push_back(Define); 	    //S->Define ID S end S
	SDefine.push_back(ID);
	SDefine.push_back(S);
	SDefine.push_back(End);
	SDefine.push_back(S);
	
	SID.push_back(ID);				//S->ID S
	SID.push_back(S);

	SDsign.push_back(Dsign);

	SEnd.push_back(Space);

	AddRule("S", "Line", SLine);
	AddRule("S", "Circle", SCircle);
	AddRule("S", "Pixel", SPixel);
	AddRule("S", "Rect", SRect);
	AddRule("S", "Define", SDefine);
	AddRule("S", "ID", SID);
	//AddRule("S", "$", SDsign);
	AddRule("S", "end", SEnd);
}
