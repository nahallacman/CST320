#include "RecursiveParser.h"

RecursiveParser::RecursiveParser(list<Token> tokens, SymbolTable _symbolTable) :m_tokens(tokens), m_SymbolTable(_symbolTable)
{
	m_cursorLocation = 0;
	m_currentToken = m_tokens.begin();
	m_Processing = true;
	_original_m_tokens = m_tokens;
	m_currentRunLevel = 0;
	//_endToken = m_tokens.end();
}

bool RecursiveParser::Run()
{
	m_currentRunLevel = 1;
	m_cursorLocation = 0; // this is a maybe, testing this line
	m_Processing = false;
	m_Done = false;
	m_tokens.erase(m_tokens.begin(), m_tokens.end());
	m_SymbolTable.ClearSymbolTableBesidesFunctions();
	m_SymbolTable.ClearSymbolTableLevelBesidesFunctions();

	//m_tokens = m_SymbolTable.GetFunctionDefinition("main");
	m_tokens = m_SymbolTable.GetFunctionDefinitionLevel("main", 0);

	m_currentToken = m_tokens.begin();
	//if (Start())
	if (Brackets())
	{
		cout << "Begin stack based rule tree. Printing from the top of the stack." << endl;
		while (!m_ruleTree.empty())
		{
			cout << m_ruleTree.top() << endl;
			m_ruleTree.pop();
		}
		cout << "End stack based rule tree." << endl;
		return true;
	}
	else return false;
}

bool RecursiveParser::Parse()
{
	if (Start())
	{
		cout << "Begin stack based rule tree. Printing from the top of the stack." << endl;
		while (!m_ruleTree.empty())
		{
			cout << m_ruleTree.top() << endl;
			m_ruleTree.pop();
		}
		cout << "End stack based rule tree." << endl;
		return true;
	}
	else return false;
}



void RecursiveParser::PrintErrors()
{
	for(auto itr = m_errors.begin(); itr != m_errors.end(); ++itr)
	cout << *itr << endl;
}



void RecursiveParser::FetchNext()
{
	++m_currentToken;

	if (m_currentToken != m_tokens.end())
	{
		m_Done = false;
		++m_cursorLocation;
		//++m_currentToken;
	}
	else
	{
		if (m_currentToken == m_tokens.end())
		{
			--m_currentToken; //testing this, may want a conditional on the fetch next, not this construct.
		}
		m_Done = true;
		//++m_currentToken;
		m_errors.push_back("unexpected end of program durring FetchNext");
	}
}

void RecursiveParser::BackOne()
{
	if (m_currentToken != m_tokens.begin())
	{
		--m_currentToken;
		--m_cursorLocation;
	}
}

void RecursiveParser::Print(string _print)
{
	cout << _print << endl;
}

void RecursiveParser::PrintSymbolTable()
{
	//m_SymbolTable.PrintTable();
	m_SymbolTable.PrintTableLevel();
}

/*
START = PROGRAM
*/
bool RecursiveParser::Start()
{
	m_ruleTree.push("Start");
	return Program();
}

/*
PROGRAM = DEFINITION PROGRAM
     	| DEFINITION
     	| lambda
 */

 bool RecursiveParser::Program()
 {
	 m_ruleTree.push("Program");
	 if (m_Done == true) // lambda
	 {
		 return true;
	 }
	 else if ( Definition() )
	 {
		 //FetchNext(); // this is a maybe
		 if (m_Done == true) // DEFINITION
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
 */

 bool RecursiveParser::Definition()
 {
	 m_ruleTree.push("Definition");

	 //if (m_currentToken->getString() == "char" || m_currentToken->getString() == "void" || m_currentToken->getString() == "int")
	 if (m_currentToken->getString() == "int")
	 {
		 FetchNext();
		 return D2();
	 }
	 else
	 {
		 m_ruleTree.pop();
		 return false;
	 }
 }

/*
D2 = 	DATA_DEFINITION |
     	FUNCTION_DEFINITION
*/
 bool RecursiveParser::D2()
 {
	 m_ruleTree.push("D2");
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
		 m_ruleTree.pop(); 
		 return false;
	 }
 }

 /*
DATA_DEFINITION_LIST = ‘int’ DATA_DEFINITION DATA_DEFINITION_LIST
     	| lambda
 */

 bool RecursiveParser::Data_Definition_List()
 {
	 m_ruleTree.push("Data_Definition_List");
	if (m_currentToken->getString() == "int")
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
		 m_ruleTree.pop();
		 BackOne();
		 return true;
	 }
}

 /*
DATA_DEFINITION = IDENTIFIER_LIST ';'    
*/
 
bool RecursiveParser::Data_Definition()
{
	m_ruleTree.push("Data_Definition");
	list<Token>::iterator _dataDefinion = m_currentToken;
	if (Identifier_List())
	{
		list<Token>::iterator _endOfList = m_currentToken;
		//FetchNext();
		if (m_currentToken->getString() == ";")
		{
			//data has been declared (but maybe not defined)
			//search backwards looking for variables using predeclared iterators
			for (list<Token>::iterator x = _dataDefinion; x != _endOfList; x++)
			{
				if (x->getString() == ",")
				{
					x++;
				}
				//if (m_SymbolTable.checkSymbolTable(x->getString()))//check if it was in the table
				if (m_SymbolTable.checkSymbolTableLevel(x->getString(), m_currentRunLevel))//check if it was in the table
				{
					//variable definition starts here
					Token var_def(x->getString(), TokenType::VARIABLE, false);
					DefineVariable(x->getString(), var_def);
				}
				else
				{
					string error;
					//Token Terror = m_SymbolTable.GetToken(x->getString());
					Token Terror = m_SymbolTable.GetTokenLevel(x->getString(), m_currentRunLevel);
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
					m_errors.push_back(error);
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
		m_ruleTree.pop();
		return false;
	}
}

/*
FUNCTION_DEFINITION = IDENTIFIER '(' FUNC_ARGS ')' PARAMETER_LIST BRACKETS         	
*/

/*
bool RecursiveParser::Function_Definition()
{
	if (m_currentToken->getTokenType() == TokenType::VARIABLE | m_currentToken->getString() == "main")  // TODO: main definition should not be in this statement
	{
		FetchNext();
		if (m_currentToken->getString() == "(")
		{
			Func_Args(); //because Func_Args can be lambda

			FetchNext();
			if (m_currentToken->getString() == ")")
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
	list<Token>::iterator _Function_DefinitionStart = m_currentToken;
	list<Token>::iterator _functionHeader;
	m_ruleTree.push("Function_Definition");
	if ((m_currentToken->getTokenType() == TokenType::VARIABLE) | (m_currentToken->getString() == "main"))  // TODO: main definition should not be in this statement
	{
		//check the variable to see if it already used. if it is not in the symboltable then add it to the symbol table as a type function
		//if (m_SymbolTable.checkSymbolTable(m_currentToken->getString()))
		if (m_SymbolTable.checkSymbolTableLevel(m_currentToken->getString(), m_currentRunLevel)) // make sure anywhere you are checking for a function the run level has to be 0
		{
			Token fun_def(m_currentToken->getString(), TokenType::FUNCTION, false);
			//m_SymbolTable.addSymbol(m_currentToken->getString(), fun_def);
			m_SymbolTable.addSymbolLevel(m_currentToken->getString(), 0, fun_def);
		}
		else
		{
			string error;
			error += "Symbol ";
			//error += m_SymbolTable.GetToken(m_currentToken->getString()).getString();
			error += m_SymbolTable.GetTokenLevel(m_currentToken->getString(), 0).getString();
			error += " has already been defined as: ";
			//error += m_SymbolTable.GetToken(m_currentToken->getString()).getValue();
			error += m_SymbolTable.GetTokenLevel(m_currentToken->getString(), 0).getValue();
			m_errors.push_back(error);
		}
		//begin function declaration
		_functionHeader = m_currentToken;

		FetchNext();
		if (m_currentToken->getString() == "(")
		{
			Func_Args(); //because Func_Args can be lambda

			FetchNext();
			if (m_currentToken->getString() == ")")
			{
				string headerStr;
				FetchNext();

				//end function defintion here (_functionHeader - > m_currentToken)
				for (list<Token>::iterator x = _functionHeader; x != m_currentToken; x++)
				{
					headerStr += x->getString();
					headerStr += " ";
				}
				Token fun_def(headerStr, TokenType::FUNCTION, false);
				//m_SymbolTable.addSymbol(_functionHeader->getString(), fun_def);
				m_SymbolTable.addSymbolLevel(_functionHeader->getString(), m_currentRunLevel, fun_def);

				list<Token>::iterator fun_body = m_currentToken;
				string bodyStr;
				//function content here
				if (Brackets())
				{
					list<Token> _temp_list;
					for (list<Token>::iterator x = fun_body; x != m_currentToken; ++x)
					{
						bodyStr += x->getString();
						bodyStr += " ";
						_temp_list.push_back(*x);
					}
					if (m_currentToken->getString() == "}")
					{
						bodyStr += "}";
						_temp_list.push_back(*m_currentToken);
					}
					//bodyStr += m_currentToken->getString();
					Token fun_body(headerStr, TokenType::FUNCTION, true, bodyStr, _temp_list);
					//m_SymbolTable.addSymbolLevel(_functionHeader->getString(), m_currentRunLevel, fun_body);
					m_SymbolTable.addSymbolLevel(_functionHeader->getString(), 0, fun_body);
					return true;
				}
				else
				{
					m_errors.push_back("Error in Brackets() within Function_Definition()");
					return false;
				}
			}
			else
			{
				m_errors.push_back("Missing ) in Function_Definition()");
				m_currentToken = _Function_DefinitionStart;
				return false;
			}
		}
		else
		{
			m_errors.push_back("Missing ( in Function_Definition()");
			m_currentToken = _Function_DefinitionStart;
			return false;
		}
	}
	else
	{
		m_errors.push_back("Missing Identifier at beginning of Function_Definition()");
		m_ruleTree.pop();
		return false;
	}
}

/*
IDENTIFIER_LIST= IDENTIFIER ID2                               	
*/

bool RecursiveParser::Identifier_List()
{
	m_ruleTree.push("Identifier_List");
	if (m_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		FetchNext();
		return Id2();
	}
	else
	{
		m_ruleTree.pop();
		return false;
	}
}

/*
ID2 = , IDENTIFIER_LIST
| lambda
*/

bool RecursiveParser::Id2()
{
	m_ruleTree.push("Id2");
	if (m_currentToken->getString() == ",")
	{
		FetchNext();
		return Identifier_List();
	}
	else
	{
		m_ruleTree.pop();
		return true; // lambda
	}
}

/*
FUNC_ARGS = IDENTIFIER_LIST
     	| lambda
 */

bool RecursiveParser::Func_Args()
{
	m_ruleTree.push("Func_Args");
	Identifier_List();
	return true;
}

/*
PARAMETER_LIST = 'int' IDENTIFIER_LIST ';' PARAMETER_LIST
     	| lambda
*/     

bool RecursiveParser::Paramater_List()
{
	m_ruleTree.push("Paramater_List");
	FetchNext();
	if (m_currentToken->getString() == "int")
	{
		FetchNext();
		if(Identifier_List())
		{
			FetchNext();
			if (m_currentToken->getString() == ";")
			{
				FetchNext();
				return Paramater_List();
			}
			else
			{
				m_errors.push_back("Missing ; from end of Paramater_List()");
				BackOne();
				BackOne();
				BackOne();
				return false;
			}
		}
		else
		{
			m_errors.push_back("Missing Identifier_List() after 'int' in Paramater_List()");
			BackOne();
			BackOne();
			return false;
		}
	}
	else
	{
		m_ruleTree.pop();
		BackOne();
		return true;
	}
}

/*
BRACKETS = '{' DATA_DEFINITION_LIST STATEMENT_GROUP '}
 */

bool RecursiveParser::Brackets()
{
	list<Token>::iterator _BracketsStart = m_currentToken;
	m_ruleTree.push("Brackets");
	if (m_currentToken->getString() == "{")
	{
		FetchNext();
		if (Data_Definition_List())
		{
			FetchNext();
			if (Statement_Group())
			{
				//FetchNext(); this breaks things when going for the first closing }
				if (m_currentToken->getString() == "}")
				{
					FetchNext(); //testing this here, possible that brackets() would get to the } and never most past
					return true;
				}
				else
				{
					m_errors.push_back("Missing a } for Brackets()");
					m_currentToken = _BracketsStart;
					return false;
				}
			}
			else
			{
				m_errors.push_back("Malformed Statement_Group() in Brackets()");
				m_currentToken = _BracketsStart;
				return false;
			}
		}
		else
		{
			m_errors.push_back("Malformed Data_Definition_List() in Brackets()");
			m_currentToken = _BracketsStart;
			return false;
		}
	}
	else
	{
		m_errors.push_back("Missing { from beginning of Brackets()");
		m_ruleTree.pop();
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
	m_ruleTree.push("Statement_Group");
	if (Statement())
	{
		return S2();
	}
	else
	{
		m_ruleTree.pop();
		return false;
	}
}

/*
S2 = STATEMENT_GROUP |
	lambda
*/

bool RecursiveParser::S2()
{
	m_ruleTree.push("S2");
	if (Statement_Group())
	{
		if (!m_Done)
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
		m_ruleTree.pop();
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
	m_ruleTree.push("Statement");
	list<Token>::iterator _StatementStart = m_currentToken;
	if (m_currentToken->getString() == "if") //'if' '(' EXPRESSION ')' BRACKETS ELSE
	{
		FetchNext();
		if (m_currentToken->getString() == "(")
		{
			FetchNext();
			Expression();	// eventually I will need to add in an examination of this expression 
			if (m_currentToken->getString() == ")")
			{
				FetchNext();
				if (Brackets())
				{
					return Else();
				}
				else
				{
					m_errors.push_back("no brackets after an if statement");
					//BackOnes won't work. Need to copy pointer as it comes into the function and return it if it doesn't work
					m_currentToken = _StatementStart;
					return false;
				}
			}
			else
			{
				m_errors.push_back("no matching ) after an if statement");
				m_currentToken = _StatementStart;
				return false;
			}
		}
		else
		{
			m_errors.push_back("no ( after an if statement");
			m_currentToken = _StatementStart;
			return false;
		}
	}
	else if (m_currentToken->getString() == "while") //'while'  '(' EXPRESSION ')' BRACKETS
	{
		FetchNext();
		if (m_currentToken->getString() == "(")		// '('
		{
			FetchNext();
			Expression();							// EXPRESSION
			if (m_currentToken->getString() == ")")	//')'
			{
				FetchNext();
				if (Brackets())						//BRACKETS
				{
					return true;
				}
				else
				{
					m_errors.push_back("no brackets after a while statement");
					m_currentToken = _StatementStart;
					return false;
				}
			}
			else
			{
				m_errors.push_back("no matching ) after a while statement");
				m_currentToken = _StatementStart;
				return false;
			}
		}
		else
		{
			m_errors.push_back("no ( after a while statement");
			m_currentToken = _StatementStart;
			return false;
		}
	}
	else if (m_currentToken->getString() == "input") // input '(' variable ')' ';'
	{
		FetchNext();
		if (m_currentToken->getString() == "(")		// '('
		{
			FetchNext();
			if (m_currentToken->getTokenType() == TokenType::VARIABLE) // check for a variable
			{
				//define the variable
				Token definedAs(m_currentToken->getString(), m_currentToken->getTokenType() );
				FetchNext();
				if (m_currentToken->getString() == ")")	//')'
				{
					FetchNext();
					if (m_currentToken->getString() == ";")		// ';'				
					{
						if (m_Processing == false)
						{
							int a;
							cout << "Prompt::Input(" << definedAs.getString() << "):";
							cin >> a;
							string b = std::to_string(a);
							definedAs.setValue(b);
							DefineVariable(definedAs.getString(), definedAs);
						}
						FetchNext();
						return true; // completed input statement
					}
					else
					{
						m_errors.push_back("no ; after an input statement");
						m_currentToken = _StatementStart;
						return false;
					}
				}
				else
				{
					m_errors.push_back("no matching ) after an input statement");
					m_currentToken = _StatementStart;
					return false;
				}
			}
			else
			{
				m_errors.push_back("no variable in input statement");
				m_currentToken = _StatementStart;
				return false;
			}
		}
		else
		{
			m_errors.push_back("no ( after an input statement");
			m_currentToken = _StatementStart;
			return false;
		}
	}
	else if (m_currentToken->getString() == "output") // output '(' OUTPUT ')' ';'
	{
		FetchNext();
		if (m_currentToken->getString() == "(")		// '('
		{
			FetchNext();
			if (m_currentToken->getTokenType() == TokenType::VARIABLE) // check for a variable
			{
				Token outputMe(m_currentToken->getString(), m_currentToken->getTokenType()); //get variable for output
				FetchNext();
				if (m_currentToken->getString() == ")")	//')'
				{
					FetchNext();
					if (m_currentToken->getString() == ";")		// ';'				
					{
						if (m_Processing == false)
						{
							//cout << "Output(" << outputMe.getString() << "): " << m_SymbolTable.GetToken(outputMe.getString()).getValue() << endl;
							cout << "Output(" << outputMe.getString() << "): " << m_SymbolTable.GetTokenLevel(outputMe.getString(), m_currentRunLevel).getValue() << endl;
						}
						FetchNext();
						return true; // completed output statement
					}
					else
					{
						m_errors.push_back("no ; after an output statement");
						m_currentToken = _StatementStart;
						return false;
					}
				}
				else
				{
					m_errors.push_back("no matching ) after an output statement");
					m_currentToken = _StatementStart;
					return false;
				}
			}
			else
			{
				m_errors.push_back("no variable in output statement");
				m_currentToken = _StatementStart;
				return false;
			}
		}
		else
		{
			m_errors.push_back("no ( after an output statement");
			m_currentToken = _StatementStart;
			return false;
		}
		//m_errors.push_back("output statements not yet implimented");
		//return false;
	}
	else if (m_currentToken->getString() == "return") // 'return' RETURN ';' |
	{
		FetchNext(); //here is where I would save the return value for that level when I change the symbol table structure
		Return();
		if (m_currentToken->getString() == ";")
		{
			FetchNext();
			return true;
		}
		else
		{
			m_errors.push_back("no ; after a return statement");
			m_currentToken = _StatementStart;
			return false;
		}
	}
	else if (Expression())
	{
		//FetchNext(); // this may break things
		if (m_currentToken->getString() == ";")
		{
			FetchNext(); //maybe? testing why c = 1 ; breaks on the ;
			return true;
		}
		else
		{
			m_errors.push_back("no ; after a EXPRESSION");
			m_currentToken = _StatementStart;
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
		error += "No rules matched in Statement(). m_currentToken is: ";
		error += m_currentToken->getString();
		m_errors.push_back(error);
		m_ruleTree.pop();
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
	m_ruleTree.push("Else");
	FetchNext();
	if (m_currentToken->getString() == "else")
	{
		return Statement();
	}
	else
	{
		BackOne();
		m_ruleTree.pop();
		return true;
	}
}

/*
RETURN = EXPRESSION
     	| lambda
*/

bool RecursiveParser::Return()
{
	m_ruleTree.push("Return");
	//if (Expression())
	if (Expression())
	{
		 // maybe?
		return true;
	}
	else
	{
		m_ruleTree.pop();
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
	if (m_currentToken->getString() == "(") //'(' EXPRESSION ')'
	{
		FetchNext();
		if (Expression())
		{
			FetchNext();
			if (m_currentToken->getString() == ")")
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
	else if (m_currentToken->getTokenType() == TokenType::NUMCONSTANT) //CONSTANT
	{
		return true;
	}
	else if (m_currentToken->getTokenType() == TokenType::VARIABLE)
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
	m_ruleTree.push("Primary");
	if (m_currentToken->getTokenType() == TokenType::NUMCONSTANT) //CONSTANT
	{
		FetchNext();
		return true;
	}
	else if (m_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		//check if variable has been declared AND defined. If it is, move on. If it isn't, error message and return false;
		//if (m_SymbolTable.checkSymbolTable(m_currentToken->getString()))
		//if (m_SymbolTable.checkSymbolTableLevel(m_currentToken->getString(), m_currentRunLevel) || m_SymbolTable.checkSymbolTableLevel(m_currentToken->getString(), 0))
		if (m_SymbolTable.checkSymbolTableLevel(m_currentToken->getString(), m_currentRunLevel) == true && m_SymbolTable.checkSymbolTableLevel(m_currentToken->getString(), 0) == true)
		{
			//variable hasn't been declared yet
			string error;
			error += "Variable ";
			error += m_currentToken->getString();
			error += " was used without being declared first";
			m_errors.push_back(error);
			//Token fun_def(m_currentToken->getString(), TokenType::VARIABLE, false);
			//m_SymbolTable.addSymbol(m_currentToken->getString(), fun_def);
			return false;
		}
		else
		{
			//variable has been declared
			//check if it is defined
			
			//if (m_SymbolTable.GetToken(m_currentToken->getString()).getIsDefined())
			if (m_SymbolTable.GetTokenLevel(m_currentToken->getString(), m_currentRunLevel).getIsDefined()) // variable defined at this run level
			{
				//list<Token>::iterator _IdentifierStart = m_currentToken;
				int _tokenLocation = m_cursorLocation;
				//it is defined! when building the final project, this might be where you extract the value for use
				FetchNext();
				//return P2(_IdentifierStart);
				return P2(_tokenLocation);
			}
			else if (m_SymbolTable.GetTokenLevel(m_currentToken->getString(), 0).getIsDefined()) // global variable or functions
			{
				int _tokenLocation = m_cursorLocation;
				//it is defined! when building the final project, this might be where you extract the value for use
				FetchNext();
				return P2(_tokenLocation);
			}
			else
			{
				//list<Token>::iterator _IdentifierStart = m_currentToken;
				int _tokenLocation = m_cursorLocation;
				//it is not defined, so try to define it
				FetchNext();
				//return P2(_IdentifierStart);
				return P2(_tokenLocation);
			}
		}
	}
	else
	{
		m_ruleTree.pop();
		return false;
	}
}

/*
P2 = '(' FUNC_ARGS ')' |
	 '=' EXPRESSION |
		lambda
*/

bool RecursiveParser::P2(int _StatementStart)
{
	list<Token>::iterator _P2Start = m_currentToken;
	m_ruleTree.push("P2");
	if (m_currentToken->getString() == "(")
	{
		FetchNext();
		if (m_currentToken->getString() == ")")
		{
			// A function call was just made. Need to replace the function call with the code it represents.
			// take a range of tokens for the function call itself, process the function call (with maybe brackets()?) then replace the function call with the return value. If there is no return value, just check the function call for correct code, the values won't matter in the end.
			if (m_Processing == false)		//only do it for the actual run, not for the code evaluation and saving.
			{
				int m_originalcursorLocation = m_cursorLocation;
				MoveCurrentToken(_StatementStart);
				FunctionCall(*m_currentToken);
				MoveCurrentToken(m_originalcursorLocation);
			}
			FetchNext();//possibly, testing this
			return true;
		}
		else
		{
			if (Func_Args())
			{
				FetchNext();
				if (m_currentToken->getString() == ")")
				{
					return true;
				}
				else
				{
					m_errors.push_back("no ) after func_args() in P2()");
					m_currentToken = _P2Start;
					return false;
				}
			}
			else
			{
				m_errors.push_back("error in func_args() in P2()");
				m_currentToken = _P2Start;
				return false;
			}
		}
	}
	else if (m_currentToken->getString() == "=")
	{

		FetchNext();
		list<Token>::iterator _EqualsStart = m_currentToken;
		if (Expression())
		{
			int m_originalcursorLocation1 = m_cursorLocation;
			MoveCurrentToken(_StatementStart);
			//if (m_SymbolTable.GetToken(m_currentToken->getString()).getIsDefined()) 
			if (m_SymbolTable.GetTokenLevel(m_currentToken->getString(), m_currentRunLevel).getIsDefined())
			{
				//redefine!
			}
			else
			{
				//define
				//key from _StatementStart, value from _EqualsStart to m_currentToken (hopefully ;?)
			}
			MoveCurrentToken(m_originalcursorLocation1);

			// since both paths end up defining,
			string bodyStr;
			for (list<Token>::iterator x = _EqualsStart; x != m_currentToken; x++)
			{
				bodyStr += x->getString();
				bodyStr += " ";
			}
			int m_originalcursorLocation2 = m_cursorLocation;
			MoveCurrentToken(_StatementStart);


			//bodyStr += m_currentToken->getString();
			Token equals_body(m_currentToken->getString(), TokenType::VARIABLE, true, bodyStr);
			//m_SymbolTable.addSymbol(_StatementStart->getString(), equals_body);
			DefineVariable(m_currentToken->getString(), equals_body);
			MoveCurrentToken(m_originalcursorLocation2);


			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		m_ruleTree.pop();
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

bool RecursiveParser::DefineVariable(string key, Token token)
{
	bool retval = false;
	//if (!m_SymbolTable.checkSymbolTable(key))
	if (!m_SymbolTable.checkSymbolTableLevel(key, m_currentRunLevel))
	{
		//cout << "Prompt::Variable: " << key << " already defined as " << m_SymbolTable.GetToken(key).getValue() << endl;
		cout << "Prompt::Variable: " << key << " already defined as " << m_SymbolTable.GetTokenLevel(key, m_currentRunLevel).getValue() << endl;
	}
	cout << "Prompt::Variable: " << key << " now defined as " << token.getValue() << endl;
	token.setIsDefined(true);
	//m_SymbolTable.addSymbol(key, token);
	m_SymbolTable.addSymbolLevel(key, m_currentRunLevel, token);
	return retval;
}

Token RecursiveParser::FunctionCall(Token _FuncName)
{
	
	//save a list of tokens 
	_original_m_tokens = m_tokens;
	//and a count of where we were
	int _original_m_cursorLocation = m_cursorLocation;
	//get the fucntion definition
	//m_tokens = m_SymbolTable.GetFunctionDefinition(_FuncName.getString());
	m_tokens = m_SymbolTable.GetFunctionDefinitionLevel(_FuncName.getString(), 0);
	//move the token iterator to the beginning of the function definition
	m_currentToken = m_tokens.begin();
	m_cursorLocation = 0;

	//I might possibly want to increase the symbol table depth at this point. "depth" could be achieved by manipulating the symbol table.
	//level 0 is reserved for globals and function definitions, level 1 is "main", level 2 is a function call foo() from within main(), level 3 is a function call foo2() from within foo(), ect.
	m_currentRunLevel++;
	
	Brackets();
	// I need a way to handle return values at this point
	// that could be accomplished by a function that reduces the symbol table depth then takes the last found "return" value if it was defined. If it was not defined for that level, don't bother returning anything.

	m_currentRunLevel--;

	//after the function is dealt with, return the tokens and iterators
	m_tokens = _original_m_tokens;

	MoveCurrentToken(_original_m_cursorLocation);
	/*
	m_currentToken = m_tokens.begin();
	m_cursorLocation = 0;
	for (int i = 1; i < _original_m_cursorLocation; i++)
	{
		FetchNext();
	}
	*/

	Token retval;
	return retval;
}

void RecursiveParser::MoveCurrentToken(int _numToMove)
{
	m_currentToken = m_tokens.begin();
	m_cursorLocation = 0;
	for (int i = 0; i < _numToMove; i++)
	{
		FetchNext();
	}
}