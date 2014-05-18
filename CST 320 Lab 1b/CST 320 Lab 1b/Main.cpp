
#include <stdio.h>
#include "Token.h"
#include "LexicalCheckNode.h"
#include "Preprocessor.h"
#include "LL1Parser.h"
#include "RecursiveParser.h"
#include <fstream>
#include <list>
//using std::ifstream;
//using std::cin;
using namespace::std;

list<Token> LexicalAnalyzer(char * input_file);
list<Token> DefineReplacement(list<Token>, map<string, string>);

/*
conventions:
.ppc: PreProcessor Comments intermediate file
.ppd: PreProcessor Directive output file
.ltl: Lexically-analyzed Token List output file
*/
int main()
{
	list<Token> tokens;
	SymbolTable the_symbolTable;

	cout << " Starting Preprocessor " << endl;
	Preprocessor pp("Program13.txt", the_symbolTable);
	the_symbolTable = pp.getSymbolTable();

	cout << " Starting Lexical Analyzer " << endl;
	tokens = LexicalAnalyzer("Program.ppd");

	tokens = DefineReplacement(tokens, pp.getSymbols());
	LL1Parser LL1P(tokens);
	if (LL1P.Parse())
	{
		cout << "Parse succeeded" << endl;
	}
	else
	{
		cout << "Parse failed" << endl;
		LL1P.PrintErrors();
	}
	/*
	RecursiveParser Parser(tokens, the_symbolTable);
	if(Parser.Parse())
	{
		cout << "Parse succeeded" << endl;
		Parser.PrintSymbolTable();
	}
	else
	{
		cout << "Parse failed" << endl;
		Parser.PrintErrors();
	}
	*/
	system("pause");
	return 0;
};

list<Token> LexicalAnalyzer(char * input_file)
{
	list<Token> tokens;
	bool match = false;
	bool done = false;
	bool EOFbool = true;
	//int j=0;
	int i=0;
	int place=0;///*
	int length=0;
	char line1[1024];
	char temp1[1024];
	char * index;
	index = &line1[0];
	//char temp1;
	//char temp2, temp3, temp4, temp5, temp6;

	
	DFAWrapper LegalNumericalConstant("LegalNumericConstant.txt");
	DFAWrapper LegalLabel("LegalLabel.txt");
	DFAWrapper LegalCVariable("LegalCVariable.txt");

	//list<LexicalCheckNode> Word_List;
	list<LexicalCheckNode> Keywords;
	list<LexicalCheckNode> Operators;
	list<LexicalCheckNode> Symbols;

	//Load LexicalCheckers using the DFA text file and the first character of the DFA
		//Keywords
	/*new list*/
	LexicalCheckNode my_input("input.txt", 'i');
	LexicalCheckNode my_output("output.txt", 'o');

	/*old list*/
	LexicalCheckNode my_char( "char.txt" , 'c' );
	LexicalCheckNode my_int( "int.txt" , 'i' );
	LexicalCheckNode my_if( "if.txt" , 'i' );
	LexicalCheckNode my_main( "main.txt" , 'm' );
	LexicalCheckNode my_return( "return.txt" , 'r' );
	LexicalCheckNode my_goto( "goto.txt" , 'g' );
	LexicalCheckNode my_else( "else.txt" , 'e' );
	LexicalCheckNode my_scanf( "scanf.txt" , 's' );
	LexicalCheckNode my_printf( "printf.txt" , 'p' );
		//Operators
	LexicalCheckNode my_asterix( "asterix.txt" , '*' );
	LexicalCheckNode my_asterixasterix( "asterixasterix.txt" , '*' );
	LexicalCheckNode my_equals( "equals.txt" , '=' );
	LexicalCheckNode my_equalsequals( "equalsequals.txt" , '=' );
	LexicalCheckNode my_forwardslash( "forwardslash.txt" , '/' );
	LexicalCheckNode my_greaterthan( "greaterthan.txt" , '>' );
	LexicalCheckNode my_greaterthanorequalto( "greaterthanorequalto.txt" , '>' );
	LexicalCheckNode my_lessthan( "lessthan.txt" , '<' );
	LexicalCheckNode my_lessthanorequalto( "lessthanorequalto.txt" , '<' );
	LexicalCheckNode my_minus( "minus.txt" , '-' );
	LexicalCheckNode my_minusminus( "minusminus.txt" , '-' );
	LexicalCheckNode my_percent( "percent.txt" , '%' );
	LexicalCheckNode my_plus( "plus.txt" , '+' );
	LexicalCheckNode my_plusplus( "plusplus.txt" , '+' );
		//Symbols
	LexicalCheckNode my_ampersand( "ampersand.txt" , '&' );
	LexicalCheckNode my_closebracket( "closebracket.txt" , '}' );
	LexicalCheckNode my_closeparen( "closeparen.txt" , ')' );
	LexicalCheckNode my_comma( "comma.txt" , ',' );
	LexicalCheckNode my_openbracket( "openbracket.txt" , '{' );
	LexicalCheckNode my_openparen( "openparen.txt" , '(' );
	LexicalCheckNode my_pipe( "pipe.txt" , '|' );
	LexicalCheckNode my_semicolon( "semicolon.txt" , ';' );

	//Load LexicalCheckers into the list
		//Keywords
	/* new list */
	Keywords.push_front(my_input);
	Keywords.push_front(my_output);
	/* old list */
	Keywords.push_front(my_char);
	Keywords.push_front(my_int);
	Keywords.push_front(my_if);
	Keywords.push_front(my_main);
	Keywords.push_front(my_return);
	Keywords.push_front(my_goto);
	Keywords.push_front(my_else);
	Keywords.push_front(my_scanf);
	Keywords.push_front(my_printf);
	/*
	Word_List.push_front(my_char);
	Word_List.push_front(my_int);
	Word_List.push_front(my_if);
	Word_List.push_front(my_main);
	Word_List.push_front(my_return);
	Word_List.push_front(my_goto);
	Word_List.push_front(my_else);
	Word_List.push_front(my_scanf);
	Word_List.push_front(my_printf);
	*/
		//Operators
	Operators.push_front(my_asterix);
	Operators.push_front(my_asterixasterix);
	Operators.push_front(my_equals);
	Operators.push_front(my_equalsequals);
	Operators.push_front(my_forwardslash);
	Operators.push_front(my_greaterthan);
	Operators.push_front(my_greaterthanorequalto);
	Operators.push_front(my_lessthan);
	Operators.push_front(my_lessthanorequalto);
	Operators.push_front(my_minus);
	Operators.push_front(my_minusminus);
	Operators.push_front(my_percent);
	Operators.push_front(my_plus);
	Operators.push_front(my_plusplus);
	/*
	Word_List.push_front(my_asterix);
	Word_List.push_front(my_asterixasterix);
	Word_List.push_front(my_equals);
	Word_List.push_front(my_equalsequals);
	Word_List.push_front(my_forwardslash);
	Word_List.push_front(my_greaterthan);
	Word_List.push_front(my_greaterthanorequalto);
	Word_List.push_front(my_lessthan);
	Word_List.push_front(my_lessthanorequalto);
	Word_List.push_front(my_minus);
	Word_List.push_front(my_minusminus);
	Word_List.push_front(my_percent);
	Word_List.push_front(my_plus);
	Word_List.push_front(my_plusplus);
	*/
		//Symbols
	Symbols.push_front(my_ampersand);
	Symbols.push_front(my_closebracket);
	Symbols.push_front(my_closeparen);
	Symbols.push_front(my_comma);
	Symbols.push_front(my_openbracket);
	Symbols.push_front(my_openparen);
	Symbols.push_front(my_pipe);
	Symbols.push_front(my_semicolon);
	/*
	Word_List.push_front(my_ampersand);
	Word_List.push_front(my_closebracket);
	Word_List.push_front(my_closeparen);
	Word_List.push_front(my_comma);
	Word_List.push_front(my_openbracket);
	Word_List.push_front(my_openparen);
	Word_List.push_front(my_pipe);
	Word_List.push_front(my_semicolon);
	*/


	list<LexicalCheckNode>::iterator it = Symbols.begin(); // this word list contains all of the legal symbol and 



	//char test_string[100];
	//char try_again = 'y';


	ifstream input;
	input.open(input_file);
	if( input.is_open() )
	{
		while( !input.eof() )
		{
			input.getline(line1, 1023); // get file one line at a time
			index = &line1[0]; // set the index pointer to the beginning of the line
			while(index[0] != 0 ) // make sure the index pointer does not point at the end of the line (null)
			{
				match = false;
				done = false;
				// Check for white space
				if( index[0] == ' ' || index[0] == '\t' || index[0] == '\n' )
				{
					//cout << "White space" << endl;
					match = true;
					index++;
				}
				else if( index[0] == '"' )				// Handle quotations for constants
				{
					//Read until next "
					EOFbool = true;
					while(index[length++] != '"' )
					{
						if(index[length] == 0)
						{
							cout << "Improperly ended constant" << endl;
						}
					}
					strncpy(temp1, index, length);
					temp1[length] = '\0';

					//print string constant to screen
					cout << temp1 << "\t\tString Constant" << endl;
					match = true;
					index = index + length;

					//add string constant to token list
					Token temp( temp1, STRCONSTANT );
					tokens.push_back(temp);
				}
				else
				{
					// -------------------- Handle numerical constants ------------------------------
					if( index[0] <= '9' && index[0] >= '0')
					{
						//Read until space or end of line or a ; for endline
						length = 0;
						while(index[length] != ' ' && index[length] != 0 && index[length] != ';')
						{
							length++;
						}
						strncpy(temp1, index, length);
						temp1[length] = '\0';
						//Test against LegalNumeric DFA
						if( LegalNumericalConstant.TestString( temp1 ) )
						{
							//print numeric constant to screen
							cout << temp1 << "\t\tNumeric Constant" << endl;
							match = true;
							index = index + length;

							//add numeric constant to token list
							Token temp( temp1, NUMCONSTANT );
							tokens.push_back(temp);
						}
					}
					else 
					{
						//------------------- Handle symbols ----------------------------------
						//it = Word_List.begin();
						it = Symbols.begin(); 
						do
						{
							if( it == Symbols.end() ) //if the iterator isnt at the end of the list
							{
								done = true;
							}
							else
							{
								if( it->getFirstChar() == index[0] ) // check the first character at the pointer
								{
									length = it->getLength();				//only read in the number of characters you will need to check against the DFA
									strncpy( temp1, index, length );
									temp1[length] = '\0';
									if ( ! ( it->CheckString( temp1 ) ) )
									{
										//print symbol to screen
										cout << temp1 << "\t\tSymbol" << endl;
										done = true;
										index = index + length;

										//add symbol to token list
										Token temp( temp1, SYMBOL );
										tokens.push_back(temp);
										match = true;
									}
									else // if not the right DFA, move on to the next one in the list
									{
										it++;
									}
								}
								else //if not the first character try the next LexicalCheck in the list
								{
									it++;
								}
							}
						}
						while( !done );

						if( !match )
						{
							//------------------------- Handle Operators ----------------------------
							done = false;
							it = Operators.begin();
							do
							{
								if( it == Operators.end() ) //if the iterator isnt at the end of the list
								{
									done = true;
								}
								else
								{
									if( it->getFirstChar() == index[0] ) // check the first character at the pointer
									{
										length = it->getLength();				//only read in the number of characters you will need to check against the DFA
										strncpy( temp1, index, length );
										temp1[length] = '\0';
										if ( ! ( it->CheckString( temp1 ) ) )
										{
											//print operator to screen
											cout << temp1 << "\t\tOperator" << endl;
											done = true;
											index = index + length;

											//add operator to token list
											Token temp( temp1, OPERATOR );
											tokens.push_back(temp);

											match = true;
										}
										else // if not the right DFA, move on to the next one in the list
										{
											it++;
										}
									}
									else //if not the first character try the next LexicalCheck in the list
									{
										it++;
									}
								}
							}
							while( !done );
						}

						if( !match )
						{
							//------------------------- Handle Keywords ----------------------------
							done = false;
							it = Keywords.begin();
							do
							{
								if( it == Keywords.end() ) //if the iterator isnt at the end of the list
								{
									done = true;
								}
								else
								{
									if( it->getFirstChar() == index[0] ) // check the first character at the pointer
									{
										length = it->getLength();				//only read in the number of characters you will need to check against the DFA
										strncpy( temp1, index, length );
										temp1[length] = '\0';
										if ( ! ( it->CheckString( temp1 ) ) )
										{
											//print token to screen
											cout << temp1 << "\t\tKeyword" << endl;
											done = true;
											index = index + length;

											//add keyword to token list
											Token temp( temp1, KEYWORD );
											tokens.push_back(temp);

											match = true;
										}
										else // if not the right DFA, move on to the next one in the list
										{
											it++;
										}
									}
									else //if not the first character try the next LexicalCheck in the list
									{
										it++;
									}
								}
							}
							while( !done );
						}

						if( ! match ) //if a match is not found yet, continue.
						{
							//------------------------- Handle Labels ----------------------------
							//Read until space
							length = 0;
							while(index[length] != ' ' && index[length] != '\0' )
							{
								length++;
							}
							strncpy(temp1, index, length);
							temp1[length] = '\0';
						
							if( LegalLabel.TestString( temp1 ) )	//Handle Legal Labels
							{
								//print label to screen
								cout << temp1 << "\t\tLabel" << endl;
								index = index + length;

								//add Label to token list
								Token temp( temp1, KEYWORD );
								tokens.push_back(temp);

								match = true;
							}
							else //last test to see if a legal identifier
							{
								//------------------------- Handle Variables ----------------------------
								//Read until space or null or (
								// or ; because of goto labelname; case
								length = 0;
								while(index[length] != ' ' && index[length] != '\0' && index[length] != '(' && index[length] != ';' )
								{
									length++;
								}
								strncpy(temp1, index, length);
								temp1[length] = '\0';
								//Handle Legal Variable names
								if( LegalCVariable.TestString( temp1 ) )
								{
									//print Legal Identifier to screen
									cout << temp1 << "\t\tIdentifier" << endl;
									index = index + length;

									//add Legal Identifier to list
									Token temp( temp1, VARIABLE );
									tokens.push_back(temp);

									match = true;
								}
								else
								{
									//print Illegal Identifier to screen
									cout << temp1 << "\t\tIllegal identifier" << endl;
									index = index + length;
									match = true;
								}
							}
						}
					}

				}
				if( ! match ) // if not found already, incriment to the next place in the buffer
				{
					cout << temp1 << "\t\tIllegal identifier" << endl;
					index = index + length;
				}
			}
		}
	}

	system("pause");
	return tokens;
}

list<Token> DefineReplacement(list<Token> tokens, map<string, string> symbols)
{
	string placeholder;
	//iterate through token list
	for(auto itr = tokens.begin(); itr != tokens.end(); ++itr)
	{
		//look for tokens of type variable
		if( itr->getTokenType() == VARIABLE )
		{
			//when a token of type variable is found, see if a corresponding value exists in the map
			placeholder = symbols[itr->getString()];
			//if it does, replace the value
			if( ! placeholder.empty() )
			{
				itr->setString(placeholder);
			}
			//if it does not, leave the value alone
		}
	}
	return tokens;
}