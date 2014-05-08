#include "RecursiveParser_Old.h"

RecursiveParser_Old::RecursiveParser_Old(list<Token> tokens):_tokens(tokens)
{
	_currentToken = _tokens.begin();
	//_endToken = _tokens.end();
}

bool RecursiveParser_Old::Parse()
{
	/*
	if(!Program())
	{
		cout << "Program failed to compile\n";
		//PrintErrors();
	}
	*/
	if(Program())
	{
		while(!_ruleTree.empty())
		{
			cout << _ruleTree.top() << endl;
			_ruleTree.pop();
		}
		return true;
	}
	else return false;
}

/*

void RecursiveParser_Old::PrintErrors()
{
	for(auto itr = _errors.begin(); itr != _errors.end(); ++itr)
		cout << *itr << endl;
}

*/

void RecursiveParser_Old::FetchNext()
{
	++_currentToken;

	if( _currentToken != _tokens.end())
	{
		_Done = false;
		//++_currentToken;
	}
	else
	{
		_Done = true;
		//++_currentToken;
		//_errors.push_back("unexpected end of program");
	}
}

void RecursiveParser_Old::BackOne()
{
	if(_currentToken != _tokens.begin())
	{
		--_currentToken;
	}
}

/*
program -> declaration-list
*/

bool RecursiveParser_Old::Program()
{
	return DeclarationList();
}

/*
type -> int 
	| void 
	| char
*/

bool RecursiveParser_Old::Type()
{
	_ruleTree.push("Type");

	if(_currentToken->getString() == "char" || _currentToken->getString() == "void" || _currentToken->getString() == "int")
	{
		return true;
	}
	else
	{
		_ruleTree.pop(); return false;
	}
}

/*
declaration-list -> type ID declaration-type ; declaration-list 
	| null
*/
bool RecursiveParser_Old::DeclarationList()
{
	_ruleTree.push("DeclarationList");

	//if end of token list -> return true
	if( _Done == true )
	{
		return true;
	}
	if(Type()) //type
	{
		FetchNext();
		if( _currentToken->getTokenType() == VARIABLE || _currentToken->getString() == "main") //ID
		{
			FetchNext();
			if( _currentToken->getString() == ";" ) //declaration-type == null -> ;
			{
				FetchNext();
				if(DeclarationList())
				{

					return true;
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			else
			{
				if(DeclarationType()) //delcaration-type != null
				{
					FetchNext();
					if(_currentToken->getString() == ";")
					{
						FetchNext();
						if(DeclarationList())
						{
							return true;
						}
						else
						{
							BackOne();
							BackOne();
							BackOne();
							BackOne();
							_ruleTree.pop(); return false;
						}
					}
					else
					{
						BackOne();
						BackOne();
						BackOne();
						_ruleTree.pop(); return false;
					}
				}
				else
				{
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //Lambda
	}
}
//declaration-type -> null 
//					| extra-var 
//					| ( params ) compound-statement
bool RecursiveParser_Old::DeclarationType()
{
	_ruleTree.push("DeclarationType");
	if(_currentToken->getString() == "(")
	{
		FetchNext();
		if(Params())
		{
			FetchNext();
			if(_currentToken->getString() == ")")
			{
				FetchNext();
				if(CompoundStatement())
				{
					return true;
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
				FetchNext();
				if(_currentToken->getString() == ";")
				{
					return true; //not sure about this return true, but I think it is appropriate.
				}
				else 
				{
					if(CompoundStatement())
					{
						return true;
					}
					else
					{
						BackOne();
						BackOne();
						BackOne();
						BackOne();
						_ruleTree.pop(); return false;
					}
				}
			}
			else 
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
			//if(_currentToken->getString() == ")")
			//{
			//	//FetchNext(); //maybe here is an extra incriment
			//	return true;
			//}
			//else
			//{
			//	_ruleTree.pop(); return false;
			//}
		}
	}
	else if(ExtraVar())
	{
		return true;
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//extra-var -> , ID extra-var 
//			| null
bool RecursiveParser_Old::ExtraVar() //this is not right... done for today. My head hurts.
{
	_ruleTree.push("ExtraVar");
	if(_currentToken->getString() == ",")
	{
		FetchNext();
		if(_currentToken->getTokenType() == TokenType::VARIABLE)
		{
			FetchNext();
			if(ExtraVar())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//params -> type ID id-brackets param-list
//			| null
bool RecursiveParser_Old::Params()
{
	_ruleTree.push("Params");
	if(Type())
	{
		FetchNext();
		if(_currentToken->getTokenType() == TokenType::VARIABLE)
		{
			FetchNext();
			if(_currentToken->getString() == "[")
			{
				FetchNext();
				if(_currentToken->getString() == "]")
				{
					FetchNext();
					if(ParamList())
					{
						return true;
					}
					else
					{
						BackOne();
						BackOne();
						BackOne();
						BackOne();
						_ruleTree.pop(); return false;
					}
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			if(ParamList())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true;	//lambda
	}

	////This is not right. Needs work.
	//if(_currentToken->getTokenType() == TokenType::VARIABLE)
	//{
	//	FetchNext();
	//	if(_currentToken->getString() == "[")
	//	{
	//		FetchNext();
	//		if(_currentToken->getString() == "]")
	//		{
	//			FetchNext();
	//		}
	//		else
	//		{

	//		}
	//	}
	//	else
	//	{

	//	}
	//}

	//_ruleTree.pop(); return false; 
}

//param-list -> , type ID id-brackets param-list
//			| null
bool RecursiveParser_Old::ParamList()
{
	_ruleTree.push("ParamList");
	if(_currentToken->getString() == ",")
	{
		FetchNext();
		if(Type())
		{
			FetchNext();
			if(_currentToken->getTokenType() == TokenType::VARIABLE)
			{
				FetchNext();
				if(_currentToken->getString() == "[")
				{
					FetchNext();
					if(_currentToken->getString() == "]")
					{
						FetchNext();
						if(ParamList())
						{
							return true;
						}
						else
						{
							BackOne();
							BackOne();
							BackOne();
							BackOne();
							BackOne();
							_ruleTree.pop(); return false;
						}
					}
					else
					{
						BackOne();
						BackOne();
						BackOne();
						BackOne();
						_ruleTree.pop(); return false;
					}
				}

				if(ParamList())
				{
					return true;
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//compound-statement -> { local-delcarations statement-list }
//						| ;
//						| null
bool RecursiveParser_Old::CompoundStatement()
{
	_ruleTree.push("CompoundStatement");
	if(_currentToken->getString() == ";")
	{
		return true;
	}
	else if(_currentToken->getString() == "{")
	{
		FetchNext();
		if(LocalDeclarations())
		{
			FetchNext();
			if(StatementList())
			{
				FetchNext();
				if(_currentToken->getString() == "}")
				{
					return true;
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
		//else if(StatementList())
		//{
		//	FetchNext();
		//	return _currentToken->getString() == "}";			
		//}
		//else return _currentToken->getString() == "}";
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//local-declarations -> type ID extra_var ; 
//						| null
bool RecursiveParser_Old::LocalDeclarations()
{ 
	_ruleTree.push("LocalDeclarations");
	if(Type())
	{
		FetchNext();
		if(_currentToken->getTokenType() == TokenType::VARIABLE)
		{
			FetchNext();
			if(ExtraVar())
			{
				FetchNext();
				if(_currentToken->getString() == ";")
				{
					return true;
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//statement-list -> statement statement-list
//					| null
bool RecursiveParser_Old::StatementList()
{
	_ruleTree.push("StatementList");
	if(Statement())
	{
		FetchNext();
		if(StatementList())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//statement -> simple-expression
//			| compound-statement 
//			| selection-statement 
//			| iteration-statement 
//			| return-statement
bool RecursiveParser_Old::Statement()
{
	_ruleTree.push("Statement");
	if(SimpleExpression())
	{
		return true;
	}
	if (FunctionCall())
	{
		return true;
	}
	if (SelectionStatement())
	{
		return true;
	}
	if( IterationStatement() ) 
	{
		return true;
	}
	if( ReturnStatement() )
	{
		return true;
	}

	_ruleTree.pop(); return false;
}

//FunctionCall->ID ( functioncallparams ) ;
bool RecursiveParser_Old::FunctionCall()
{
	_ruleTree.push("FunctionCall");
	if(_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		FetchNext();
		if(_currentToken->getString() == "(")
		{
			FetchNext();
			if(FunctionCallParams())
			{
				FetchNext();
				if(_currentToken->getString() == ")")
				{
					FetchNext();
					if(_currentToken->getString() == ";")
					{
						return true;
					}
					else
					{
						BackOne();
						BackOne();
						BackOne();
						BackOne();
						_ruleTree.pop(); return false;
					}
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false;
}

//FunctionCallParams->ID functionparamlist | null
bool RecursiveParser_Old::FunctionCallParams()
{
	_ruleTree.push("FunctionCallParams");
	if(_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		FetchNext();
		if(FunctionParamList())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true;
	}
}

//FunctionParamList->, ID functionparamlist | null
bool RecursiveParser_Old::FunctionParamList()
{
	_ruleTree.push("FunctionParamList");
	if(_currentToken->getString() == ",")
	{
		FetchNext();
		if(_currentToken->getTokenType() == TokenType::VARIABLE)
		{
			FetchNext();
			if(FunctionParamList())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true;	//lambda
	}
}

//selection-statement -> if ( simple-expresion ) statement selection-X
bool RecursiveParser_Old::SelectionStatement()
{
	_ruleTree.push("SelectionStatement");
	if(_currentToken->getString() == "if")
	{
		FetchNext();
		if(_currentToken->getString() == "(")
		{
			FetchNext();
			if(SimpleExpression())
			{
				FetchNext();
				if(_currentToken->getString() == ")")
				{
					FetchNext();
					if(Statement())
					{
						FetchNext();
						if(SelectionX())
						{
							return true;
						}
						else
						{
							BackOne();
							BackOne();
							BackOne();
							BackOne();
							BackOne();
							_ruleTree.pop(); return false;
						}
					}
					else
					{
						BackOne();
						BackOne();
						BackOne();
						BackOne();
						_ruleTree.pop(); return false;
					}
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	_ruleTree.pop(); return false; 
}

//selection-X -> else statement
//				| null
bool RecursiveParser_Old::SelectionX()
{
	_ruleTree.push("SelectionX");
	if(_currentToken->getString() == "else")
	{
		FetchNext();
		if(Statement())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//iteration-statement -> while ( simple-expression ) statement
bool RecursiveParser_Old::IterationStatement()
{ 
	_ruleTree.push("IterationStatement");
	if(_currentToken->getString() == "while")
	{
		FetchNext();
		if(_currentToken->getString() == "(")
		{
			FetchNext();
			if(SimpleExpression())
			{
				FetchNext();
				if(_currentToken->getString() == ")")
				{
					if(Statement())
					{
						return true;
					}
					else
					{
						BackOne();
						BackOne();
						BackOne();
						_ruleTree.pop(); return false;
					}
				}
				else
				{
					BackOne();
					BackOne();
					BackOne();
					_ruleTree.pop(); return false;
				}
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false; 
}

//return-statement -> return ; 
//					| return simple-expression ;
bool RecursiveParser_Old::ReturnStatement()
{
	_ruleTree.push("ReturnStatement");
	if(_currentToken->getString() == "return")
	{
		FetchNext();
		if(_currentToken->getString() == ";")
		{
			return true;
		}
		else if(SimpleExpression())
		{
			FetchNext();
			if(_currentToken->getString() == ";")
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else 
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false; 
}

//simple-expression -> rel-expression rel-X
bool RecursiveParser_Old::SimpleExpression()
{ 
	_ruleTree.push("SimpleExpression");
	if(RelExpression())
	{
		FetchNext();
		if(RelX())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false;
}

//rel-X -> logicop simple-expression rel-X
//		| null
bool RecursiveParser_Old::RelX()
{ 
	_ruleTree.push("Relx");
	if(LogicOpp())
	{
		FetchNext();
		if(SimpleExpression())
		{
			FetchNext();
			if(RelX())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}

		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}


bool RecursiveParser_Old::LogicOpp()
{
	return _currentToken->getString() == "&&" || 
			_currentToken->getString() == "||";
}

//rel-expression -> additive-expression add-X
bool RecursiveParser_Old::RelExpression()
{ 
	_ruleTree.push("RelExpression");
	if(AdditiveExpression())
	{
		FetchNext();
		if(AddX())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false; 
}

//add-X -> relop rel-expression add-X 
//		| null
bool RecursiveParser_Old::AddX()
{ 
	_ruleTree.push("AddX");
	if(RelOpp())
	{
		FetchNext();
		if(RelExpression())
		{
			FetchNext();
			if(AddX())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

bool RecursiveParser_Old::RelOpp()
{
	_ruleTree.push("RelOpp");
	if(_currentToken->getString() == "<")
	{
		FetchNext();
		if(_currentToken->getString() == "=")
		{
			return true;
		}
		else
		{
			BackOne();
			return true;
		}
	}
	else if(_currentToken->getString() == ">")
	{
		FetchNext();
		if(_currentToken->getString() == "=")
		{
			return true;
		}
		else
		{
			BackOne();
			return true;
		}
	} 
	else if(_currentToken->getString() == "=")
	{
		FetchNext();
		if(_currentToken->getString() == "=")
		{
			return true;
		}
		else _ruleTree.pop(); return false;
	}
	else if(_currentToken->getString() == "!")
	{
		FetchNext();
		if(_currentToken->getString() == "=")
		{
			return true;
		}
		else _ruleTree.pop(); return false;
	}
	else _ruleTree.pop(); return false;
}

//additive-expression -> term term-X
bool RecursiveParser_Old::AdditiveExpression()
{ 
	_ruleTree.push("AdditiveExpression");

	if(Term())
	{
		FetchNext();
		if(TermX())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false;
}

//term-X -> addop term term-X 
//			| null
bool RecursiveParser_Old::TermX()
{ 
	_ruleTree.push("TermX");
	if(AddOpp())
	{
		FetchNext();
		if(Term())
		{
			FetchNext();
			if(TermX())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else 
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

bool RecursiveParser_Old::AddOpp()
{
	return _currentToken->getString() == "+"
			|| _currentToken->getString() == "-";
}

//term -> unary-expression urn-X
bool RecursiveParser_Old::Term()
{ 
	_ruleTree.push("Term");
	if(UnaryExpression())
	{
		FetchNext();
		if(UrnX())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false;
}

//unary-expression -> unaryop unary-expression 
//					| factor
bool RecursiveParser_Old::UnaryExpression()
{ 
	_ruleTree.push("UnaryExpression");
	if(UnaryOpp())
	{
		FetchNext();
		if(UnaryExpression())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else return Factor();
}

bool RecursiveParser_Old::UnaryOpp()
{
	return _currentToken->getString() == "!"
		|| _currentToken->getString() == "-";

}

//urn-X -> mulop unary-expression urn-X 
//		| null
bool RecursiveParser_Old::UrnX()
{ 
	_ruleTree.push("UrnX");
	if(MulOpp())
	{
		FetchNext();
		if(UnaryExpression())
		{
			FetchNext();
			if(UrnX())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

bool RecursiveParser_Old::MulOpp()
{
	return _currentToken->getString() == "*"
			|| _currentToken->getString() == "/";
}

//factor -> ( simple-expression ) 
//			| ID ID-X 
//			| NUM
bool RecursiveParser_Old::Factor()
{ 
	_ruleTree.push("Factor");
	if(_currentToken->getString() == "(")
	{
		FetchNext();
		if(SimpleExpression())
		{
			FetchNext();
			if(_currentToken->getString() == ")")
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else if(_currentToken->getTokenType() == TokenType::VARIABLE)
	{
		FetchNext();
		if(IDX())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else return _currentToken->getTokenType() == TokenType::NUMCONSTANT;
}

//ID-X -> ( args ) 
//		| [ simple-expession ] 
//		| null
bool RecursiveParser_Old::IDX()
{ 
	_ruleTree.push("IDX");
	if(_currentToken->getString() == "(")
	{
		FetchNext();
		if(Args())
		{
			FetchNext();
			if(_currentToken->getString() == ")")
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else if(_currentToken->getString() == "[")
	{
		FetchNext();
		if(SimpleExpression())
		{
			FetchNext();
			if(_currentToken->getString() == "]")
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}

//args-> simple-expression arg-list
bool RecursiveParser_Old::Args()
{ 
	_ruleTree.push("Args");
	if(SimpleExpression())
	{
		FetchNext();
		if(ArgList())
		{
			return true;
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else _ruleTree.pop(); return false;
}

//arg-list -> , simple-expession arg-list 
//	| null
bool RecursiveParser_Old::ArgList()
{ 
	_ruleTree.push("ArgList");
	if(_currentToken->getString() == ",")
	{
		FetchNext();
		if(SimpleExpression())
		{
			FetchNext();
			if(ArgList())
			{
				return true;
			}
			else
			{
				BackOne();
				BackOne();
				_ruleTree.pop(); return false;
			}
		}
		else
		{
			BackOne();
			_ruleTree.pop(); return false;
		}
	}
	else
	{
		BackOne();
		return true; //lambda
	}
}