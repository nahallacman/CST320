#include "RecursiveParser.h"

RecursiveParser::RecursiveParser(list<Token> tokens, SymbolTable _symbolTable) :_tokens(tokens), m_SymbolTable(_symbolTable)
{
	_currentToken = _tokens.begin();
	//_endToken = _tokens.end();
}

bool RecursiveParser::Parse()
{
	/*
	if(!Program())
	{
	cout << "Program failed to compile\n";
	//PrintErrors();
	}
	*/
	if (Start())
	{
		while (!_ruleTree.empty())
		{
			cout << _ruleTree.top() << endl;
			_ruleTree.pop();
		}
		return true;
	}
	else return false;
}



void RecursiveParser::PrintErrors()
{
	for(auto itr = _errors.begin(); itr != _errors.end(); ++itr)
	cout << *itr << endl;
}



void RecursiveParser::FetchNext()
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

void RecursiveParser::BackOne()
{
	if (_currentToken != _tokens.begin())
	{
		--_currentToken;
	}
}

void RecursiveParser::Print(string _print)
{
	cout << _print << endl;
}

void RecursiveParser::PrintSymbolTable()
{
	m_SymbolTable.PrintTable();
}

/*
START = PROGRAM
*/
bool RecursiveParser::Start()
{
	_ruleTree.push("Start");
	return Program();
}

/*
 
PROGRAM = DEFINITION PROGRAM
     	| DEFINITION
     	| lambda
 */

 bool RecursiveParser::Program()
 {
	 _ruleTree.push("Program");
	 if (_Done == true) // lambda
	 {
		 return true;
	 }
	 else if ( Definition() )
	 {
		 //FetchNext(); // this is a maybe
		 if (_Done == true) // DEFINITION
		 {
			 return true;
		 }
		 else // DEFINITION PROGRAM
		 {
			 return Program();
		 }
	 }
 }

 /*
DEFINITION = ‘int’ D2
     	| ‘int’ D2                     	
 */

 bool RecursiveParser::Definition()
 {
	 _ruleTree.push("Definition");

	 //if (_currentToken->getString() == "char" || _currentToken->getString() == "void" || _currentToken->getString() == "int")
	 if (_currentToken->getString() == "int")
	 {
		 FetchNext();
		 return D2();
	 }
	 else
	 {
		 _ruleTree.pop();
		 return false;
	 }
 }

/*
D2 = 	DATA_DEFINITION |
     	FUNCTION_DEFINITION
*/
 bool RecursiveParser::D2()
 {
	 _ruleTree.push("D2");
	 if (Data_Definition()) // DATA_DEFINITION
	 {
		 return true;
	 }
	 else if (Function_Definition()) // FUNCTION_DEFINITION
	 {
		 return true;
	 }
	 else
	 {
		 _ruleTree.pop(); 
		 return false;
	 }
 }

 /*
DATA_DEFINITION_LIST = ‘int’ DATA_DEFINITION DATA_DEFINITION_LIST
     	| lambda
 */

 bool RecursiveParser::Data_Definition_List()
 {
	 _ruleTree.push("Data_Definition_List");
	if (_currentToken->getString() == "int")
	{
		 FetchNext();
		 if (Data_Definition())
		 {
			 FetchNext();
			 return Data_Definition_List();
		 }
		 else
		 {
			 BackOne();
			 BackOne();
			 return false;
		 }
	 }
	 else                    //lambda
	 {
		 _ruleTree.pop();
		 BackOne();
		 return true;
	 }
}

 /*
DATA_DEFINITION = IDENTIFIER_LIST ';'    
*/
 
bool RecursiveParser::Data_Definition()
{
	_ruleTree.push("Data_Definition");
	list<Token>::iterator _dataDefinion = _currentToken;
	if (Identifier_List())
	{
		list<Token>::iterator _endOfList = _currentToken;
		//FetchNext();
		if (_currentToken->getString() == ";")
		{
			//data has been declared (but maybe not defined)
			//search backwards looking for variables using predeclared iterators
			for (list<Token>::iterator x = _dataDefinion; x != _endOfList; x++)
			{
				if (x->getString() == ",")
				{
					x++;
				}
				if (m_SymbolTable.checkSymbolTable(x->getString()))//check if it was in the table
				{
					//variable definition starts here
					Token var_def(x->getString(), TokenType::VARIABLE, false);
					m_SymbolTable.addSymbol(x->getString(), var_def);
				}
				else
				{
					string error;
					Token Terror = m_SymbolTable.GetToken(x->getString());
					error += "Varible ";
					error += x->getString();
					if (Terror.getIsDefined())
					{
						error += " is already defined as ";
						error += Terror.getValue();
					}
					else
					{
						error += " has already been declared but not defined.";
					}
					_errors.push_back(error);
					return false; // this return here is a big maybe, it might break things
				}
			}

			return true;
		}
		else
		{
			BackOne();
			return false;
		}
	}
	else
	{
		_ruleTree.pop();
		return false;
	}
}

/*
FUNCTION_DEFINITION = IDENTIFIER '(' FUNC_ARGS ')' PARAMETER_LIST BRACKETS         	
*/

/*
bool RecursiveParser::Function_Definition()
{
	if (_currentToken->getTokenType() == TokenType::VARIABLE | _currentToken->getString() == "main")  // TODO: main definition should not be in this statement
	{
		FetchNext();
		if (_currentToken->getString() == "(")
		{
			Func_Args(); //because Func_Args can be lambda

			FetchNext();
			if (_currentToken->getString() == ")")
			{
				FetchNext();
				Paramater_List();
				Brackets();
				return true; // not positive about this
			}
			else
			{
				BackOne();
				return false;
			}
		}
		else
		{
			BackOne();
			return false;
		}
	}
	else
	{
		return false;
	}
}
*/

/*
FUNCTION_DEFINITION = IDENTIFIER '(' FUNC_ARGS ')' BRACKETS
*/

bool RecursiveParser::Function_Definition()
{
	list<Token>::iterator _Function_DefinitionStart = _currentToken;
	list<Token>::iterator _functionHeader;
	_ruleTree.push("Function_Definition");
	if ((_currentToken->getTokenType() == TokenType::VARIABLE) | (_currentToken->getString() == "main"))  // TODO: main definition should not be in this statement
	{
		//check the variable to see if it already used. if it is not in the symboltable then add it to the symbol table as a type function
		if (m_SymbolTable.checkSymbolTable(_currentToken->getString()))
		{
			Token fun_def(_currentToken->getString(), TokenType::FUNCTION, false);
			m_SymbolTable.addSymbol(_currentToken->getString(), fun_def);
		}
		else
		{
			string error;
			error += "Symbol ";
			error += m_SymbolTable.GetToken(_currentToken->getString()).getString();
			error += " has already been defined as: ";
			error += m_SymbolTable.GetToken(_currentToken->getString()).getValue();
			_errors.push_back(error);
		}
		//begin function declaration
		_functionHeader = _currentToken;

		FetchNext();
		if (_currentToken->getString() == "(")
		{
			Func_Args(); //because Func_Args can be lambda

			FetchNext();
			if (_currentToken->getString() == ")")
			{
				string headerStr;
				FetchNext();

				//end function defintion here (_functionHeader - > _currentToken)
				for (list<Token>::iterator x = _functionHeader; x != _currentToken; x++)
				{
					headerStr += x->getString();
					headerStr += " ";
				}
				Token fun_def(headerStr, TokenType::FUNCTION, false);
				m_SymbolTable.addSymbol(_functionHeader->getString(), fun_def);

				list<Token>::iterator fun_body = _currentToken;
				string bodyStr;
				//function content here
				if (Brackets())
				{
					for (list<Token>::iterator x = fun_body; x != _currentToken; x++)
					{
						bodyStr += x->getString();
						bodyStr += " ";
					}
					bodyStr += _currentToken->getString();
					Token fun_body(headerStr, TokenType::FUNCTION, true, bodyStr);
					m_SymbolTable.addSymbol(_functionHeader->getString(), fun_body);

					return true;
				}
				else
				{
					_errors.push_back("Error in Brackets() within Function_Definition()");
					return false;
				}
			}
			else
			{
				_errors.push_back("Missing ) in Function_Definition()");
				_currentToken = _Function_DefinitionStart;
				return false;
			}
		}
		else
		{
			_errors.push_back("Missing ( in Function_Definition()");
			_currentToken = _Function_DefinitionStart;
			return false;
		}
	}
	else
	{
		_errors.push_back("Missing Identifier at beginning of Function_Definition()");
		_ruleTree.pop();
		return false;
	}
}

/*
IDENTIFIER_LIST= IDENTIFIER ID2                               	
*/

bool RecursiveParser::Identifier_List()
{
	_ruleTree.push("Identifier_List");
	if (_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		FetchNext();
		return Id2();
	}
	else
	{
		_ruleTree.pop();
		return false;
	}
}

/*
ID2 = , IDENTIFIER_LIST
| lambda
*/

bool RecursiveParser::Id2()
{
	_ruleTree.push("Id2");
	if (_currentToken->getString() == ",")
	{
		FetchNext();
		return Identifier_List();
	}
	else
	{
		_ruleTree.pop();
		return true; // lambda
	}
}

/*
FUNC_ARGS = IDENTIFIER_LIST
     	| lambda
 */

bool RecursiveParser::Func_Args()
{
	_ruleTree.push("Func_Args");
	Identifier_List();
	return true;
}

/*
PARAMETER_LIST = 'int' IDENTIFIER_LIST ';' PARAMETER_LIST
     	| lambda
*/     

bool RecursiveParser::Paramater_List()
{
	_ruleTree.push("Paramater_List");
	FetchNext();
	if (_currentToken->getString() == "int")
	{
		FetchNext();
		if(Identifier_List())
		{
			FetchNext();
			if (_currentToken->getString() == ";")
			{
				FetchNext();
				return Paramater_List();
			}
			else
			{
				_errors.push_back("Missing ; from end of Paramater_List()");
				BackOne();
				BackOne();
				BackOne();
				return false;
			}
		}
		else
		{
			_errors.push_back("Missing Identifier_List() after 'int' in Paramater_List()");
			BackOne();
			BackOne();
			return false;
		}
	}
	else
	{
		_ruleTree.pop();
		BackOne();
		return true;
	}
}

/*
BRACKETS = '{' DATA_DEFINITION_LIST STATEMENT_GROUP '}
 */

bool RecursiveParser::Brackets()
{
	list<Token>::iterator _BracketsStart = _currentToken;
	_ruleTree.push("Brackets");
	if (_currentToken->getString() == "{")
	{
		FetchNext();
		if (Data_Definition_List())
		{
			FetchNext();
			if (Statement_Group())
			{
				//FetchNext(); this breaks things when going for the first closing }
				if (_currentToken->getString() == "}")
				{
					FetchNext(); //testing this here, possible that brackets() would get to the } and never most past
					return true;
				}
				else
				{
					_errors.push_back("Missing a } for Brackets()");
					_currentToken = _BracketsStart;
					return false;
				}
			}
			else
			{
				_errors.push_back("Malformed Statement_Group() in Brackets()");
				_currentToken = _BracketsStart;
				return false;
			}
		}
		else
		{
			_errors.push_back("Malformed Data_Definition_List() in Brackets()");
			_currentToken = _BracketsStart;
			return false;
		}
	}
	else
	{
		_errors.push_back("Missing { from beginning of Brackets()");
		_ruleTree.pop();
		return false;
	}
}

/*
STATEMENT_GROUP = STATEMENT STATEMENT_GROUP
     	| STATEMENT
*/
/*
bool RecursiveParser::Statement_Group()
{
	Statement();
	if (_Done == false)
	{
		Statement_Group();
	}
	else
	{
		return true;
	}
}
*/

/*
STATEMENT_GROUP = STATEMENT S2
*/

bool RecursiveParser::Statement_Group()
{
	_ruleTree.push("Statement_Group");
	if (Statement())
	{
		return S2();
	}
	else
	{
		_ruleTree.pop();
		return false;
	}
}

/*
S2 = STATEMENT_GROUP |
	lambda
*/

bool RecursiveParser::S2()
{
	_ruleTree.push("S2");
	if (Statement_Group())
	{
		if (!_Done)
		{
			return true;
		}
		else
		{
			FetchNext(); //testing this recursive loop
			return true;
		}
	}
	else
	{
		_ruleTree.pop();
		return true;
	}

}

/*
STATEMENT =   	'if' '(' EXPRESSION ')' BRACKETS ELSE |
				'while'  '(' EXPRESSION ')' BRACKETS  |
				input '(' INPUT ')' ';' |
				output '(' INPUT ')' ';' |
				'return' RETURN ';' |
				EXPRESSION ';'
*/

bool RecursiveParser::Statement()
{
	_ruleTree.push("Statement");
	list<Token>::iterator _StatementStart = _currentToken;
	if (_currentToken->getString() == "if") //'if' '(' EXPRESSION ')' BRACKETS ELSE
	{
		FetchNext();
		if (_currentToken->getString() == "(")
		{
			FetchNext();
			Expression();	// eventually I will need to add in an examination of this expression 
			if (_currentToken->getString() == ")")
			{
				FetchNext();
				if (Brackets())
				{
					return Else();
				}
				else
				{
					_errors.push_back("no brackets after an if statement");
					//BackOnes won't work. Need to copy pointer as it comes into the function and return it if it doesn't work
					_currentToken = _StatementStart;
					return false;
				}
			}
			else
			{
				_errors.push_back("no matching ) after an if statement");
				_currentToken = _StatementStart;
				return false;
			}
		}
		else
		{
			_errors.push_back("no ( after an if statement");
			_currentToken = _StatementStart;
			return false;
		}
	}
	else if (_currentToken->getString() == "while") //'while'  '(' EXPRESSION ')' BRACKETS
	{
		FetchNext();
		if (_currentToken->getString() == "(")		// '('
		{
			FetchNext();
			Expression();							// EXPRESSION
			if (_currentToken->getString() == ")")	//')'
			{
				FetchNext();
				if (Brackets())						//BRACKETS
				{
					return true;
				}
				else
				{
					_errors.push_back("no brackets after a while statement");
					_currentToken = _StatementStart;
					return false;
				}
			}
			else
			{
				_errors.push_back("no matching ) after a while statement");
				_currentToken = _StatementStart;
				return false;
			}
		}
		else
		{
			_errors.push_back("no ( after a while statement");
			_currentToken = _StatementStart;
			return false;
		}
	}
	else if (_currentToken->getString() == "input") // input '(' variable ')' ';'
	{
		FetchNext();
		if (_currentToken->getString() == "(")		// '('
		{
			FetchNext();
			if (_currentToken->getTokenType() == TokenType::VARIABLE) // check for a variable
			{
				//the variable here will eventually need to be checked if it is defined or declared
				FetchNext();
				if (_currentToken->getString() == ")")	//')'
				{
					FetchNext();
					if (_currentToken->getString() == ";")		// ';'				
					{
						FetchNext();
						return true; // completed input statement
					}
					else
					{
						_errors.push_back("no ; after an input statement");
						_currentToken = _StatementStart;
						return false;
					}
				}
				else
				{
					_errors.push_back("no matching ) after an input statement");
					_currentToken = _StatementStart;
					return false;
				}
			}
			else
			{
				_errors.push_back("no variable in input statement");
				_currentToken = _StatementStart;
				return false;
			}
		}
		else
		{
			_errors.push_back("no ( after an input statement");
			_currentToken = _StatementStart;
			return false;
		}
	}
	else if (_currentToken->getString() == "output") // output '(' OUTPUT ')' ';'
	{
		_errors.push_back("output statements not yet implimented");
		return false;
	}
	else if (_currentToken->getString() == "return") // 'return' RETURN ';' |
	{
		FetchNext();
		Return();
		if (_currentToken->getString() == ";")
		{
			FetchNext();
			return true;
		}
		else
		{
			_errors.push_back("no ; after a return statement");
			_currentToken = _StatementStart;
			return false;
		}
	}
	else if (Expression())
	{
		//FetchNext(); // this may break things
		if (_currentToken->getString() == ";")
		{
			FetchNext(); //maybe? testing why c = 1 ; breaks on the ;
			return true;
		}
		else
		{
			_errors.push_back("no ; after a EXPRESSION");
			_currentToken = _StatementStart;
			return false;
		}
	}
	//this is a maybe
	/*else if (Brackets())
	{
		return true;
	}
	*/
	else
	{
		string error;
		error += "No rules matched in Statement(). _currentToken is: ";
		error += _currentToken->getString();
		_errors.push_back(error);
		_ruleTree.pop();
		return false;
	}
}

/* 
OUTPUT = variable
		| string

		THIS IS NOT COMPLETE

*/

/*
ELSE = ‘else’ STATEMENT
     	| lambda
*/

bool RecursiveParser::Else()
{
	_ruleTree.push("Else");
	FetchNext();
	if (_currentToken->getString() == "else")
	{
		return Statement();
	}
	else
	{
		BackOne();
		_ruleTree.pop();
		return true;
	}
}

/*
RETURN = EXPRESSION
     	| lambda
*/

bool RecursiveParser::Return()
{
	_ruleTree.push("Return");
	//if (Expression())
	if (Expression())
	{
		 // maybe?
		return true;
	}
	else
	{
		_ruleTree.pop();
		BackOne();
		return true;
	}
}

/*
EXPRESSION  =	 IDENTIFIER '=' EXPRESSION |
             	IDENTIFIER '+=' EXPRESSION   |
             	IDENTIFIER '-=' EXPRESSION   |
             	IDENTIFIER '*=' EXPRESSION   |
             	IDENTIFIER '/=' EXPRESSION   |
             	IDENTIFIER '%=' EXPRESSION   |
             	EXPRESSION '==' EXPRESSION   |
             	EXPRESSION '!=' EXPRESSION   |
             	EXPRESSION '<'  EXPRESSION   |
             	EXPRESSION '<=' EXPRESSION   |
             	EXPRESSION '>'  EXPRESSION   |
             	EXPRESSION '>=' EXPRESSION   |
             	EXPRESSION '+'  EXPRESSION   |
             	EXPRESSION '-'  EXPRESSION   |
             	EXPRESSION '*'  EXPRESSION   |
             	EXPRESSION '/'  EXPRESSION   |
             	EXPRESSION '%'  EXPRESSION   |
             	UNARY_EXPRESSION
*/
/*
cheating rule: 
EXPRESSION =	  PRIMARY
*/

bool RecursiveParser::Expression()
{
	return Primary();
}

/*
UNARY_EXPRESSION =    	'++' IDENTIFIER |
     	     	'--' IDENTIFIER |
             	PRIMARY
*/

bool RecursiveParser::Unary_Expression()
{
	return false;
}

/*
PRIMARY =      	'(' EXPRESSION ')' |
				CONSTANT |
             	IDENTIFIER P2
*/

/*
bool RecursiveParser::Primary()
{
	if (_currentToken->getString() == "(") //'(' EXPRESSION ')'
	{
		FetchNext();
		if (Expression())
		{
			FetchNext();
			if (_currentToken->getString() == ")")
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				BackOne();
				return false;
			}
		}
		else
		{
			BackOne();
			BackOne();
			return false;
		}
	}
	else if (_currentToken->getTokenType() == TokenType::NUMCONSTANT) //CONSTANT
	{
		return true;
	}
	else if (_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		FetchNext();
		return P2();
	}
	else
	{
		return false;
	}
}
*/

/*
PRIMARY =	CONSTANT |
			IDENTIFIER P2
*/

bool RecursiveParser::Primary()
{
	_ruleTree.push("Primary");
	if (_currentToken->getTokenType() == TokenType::NUMCONSTANT) //CONSTANT
	{
		FetchNext();
		return true;
	}
	else if (_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		//check if variable has been declared AND defined. If it is, move on. If it isn't, error message and return false;
		if (m_SymbolTable.checkSymbolTable(_currentToken->getString()))
		{
			//variable hasn't been declared yet
			string error;
			error += "Variable ";
			error += _currentToken->getString();
			error += " was used without being declared first";
			_errors.push_back(error);
			//Token fun_def(_currentToken->getString(), TokenType::VARIABLE, false);
			//m_SymbolTable.addSymbol(_currentToken->getString(), fun_def);
			return false;
		}
		else
		{
			//variable has been declared
			//check if it is defined
			if (m_SymbolTable.GetToken(_currentToken->getString()).getIsDefined())
			{
				list<Token>::iterator _IdentifierStart = _currentToken;
				//it is defined! when building the final project, this might be where you extract the value for use
				FetchNext();
				return P2(_IdentifierStart);
			}
			else
			{
				list<Token>::iterator _IdentifierStart = _currentToken;
				//it is not defined, so try to define it
				FetchNext();
				return P2(_IdentifierStart);
			}
		}
	}
	else
	{
		_ruleTree.pop();
		return false;
	}
}

/*
P2 = '(' FUNC_ARGS ')' |
	 '=' EXPRESSION |
		lambda
*/

bool RecursiveParser::P2(list<Token>::iterator _StatementStart)
{
	list<Token>::iterator _P2Start = _currentToken;
	_ruleTree.push("P2");
	if (_currentToken->getString() == "(")
	{
		FetchNext();
		if (_currentToken->getString() == ")")
		{
			FetchNext();//possibly, testing this
			return true;
		}
		else
		{
			if (Func_Args())
			{
				FetchNext();
				if (_currentToken->getString() == ")")
				{
					return true;
				}
				else
				{
					_errors.push_back("no ) after func_args() in P2()");
					_currentToken = _P2Start;
					return false;
				}
			}
			else
			{
				_errors.push_back("error in func_args() in P2()");
				_currentToken = _P2Start;
				return false;
			}
		}
	}
	else if (_currentToken->getString() == "=")
	{

		FetchNext();
		list<Token>::iterator _EqualsStart = _currentToken;
		if (Expression())
		{
			if (m_SymbolTable.GetToken(_StatementStart->getString()).getIsDefined())
			{
				//redefine!
			}
			else
			{
				//define
				//key from _StatementStart, value from _EqualsStart to _currentToken (hopefully ;?)
			}
			// since both paths end up defining,
			string bodyStr;
			for (list<Token>::iterator x = _EqualsStart; x != _currentToken; x++)
			{
				bodyStr += x->getString();
				bodyStr += " ";
			}
			//bodyStr += _currentToken->getString();
			Token equals_body(_StatementStart->getString(), TokenType::VARIABLE, true, bodyStr);
			m_SymbolTable.addSymbol(_StatementStart->getString(), equals_body);



			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		_ruleTree.pop();
		return true;
	}
}

/*
OTHER RULES THAT SHOULD BE IMPLIMENTED IN LINE POSSIBLY?
OR A FUNCTION THAT CHECKS IF IT IS AN IDENTIFIER OR WHATEVER AND RETURNS A BOOL
IDENTIFIER = Letter {Letter | Digit}

CONSTANT = Digit {Digit}

letter = "A" | "B" | "C" | "D" | "E" | "F" | "G"
   	| "H" | "I" | "J" | "K" | "L" | "M" | "N"
   	| "O" | "P" | "Q" | "R" | "S" | "T" | "U"
   	| "V" | "W" | "X" | "Y" | "Z" ;
 
digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
*/

