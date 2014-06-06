#include <iostream>
#include "Preprocessor.h"
#include "Token.h"


Preprocessor::Preprocessor(string inFile, SymbolTable _symboltable) :_ifDepth(0), m_SymbolTable(_symboltable)
{
	_outputFile.open("Program.ppd", ofstream::out);
	_outputFile.clear();
	_unhandledIncludes.push_back(inFile);
	_intermediateFile.open("Program.ppc", fstream::in | fstream::out);

	Parse();
	m_SymbolTable.PrintTable();

	_outputFile.close();
	_intermediateFile.close();
}

Preprocessor::~Preprocessor()
{}

SymbolTable Preprocessor::getSymbolTable()
{
	return m_SymbolTable;
}

void Preprocessor::Parse()
{
	while(_unhandledIncludes.begin() != _unhandledIncludes.end())
	{
		_inFile.open(*_unhandledIncludes.begin(),ios::in);

		_intermediateFile.close();
		_intermediateFile.open("Program.ppc", fstream::out);
		_intermediateFile << 0;
		_intermediateFile.close();
		_intermediateFile.open("Program.ppc", fstream::out);

		cout << "Removing Comments -- " << endl;

		ParseComments();
		_intermediateFile.close();

		_intermediateFile.open("Program.ppc", fstream::in | fstream::beg);

		cout << "Handling preprocessor directives -- " << endl;
		ParseDirectives();

		_inFile.close();
		_includedFiles.push_back(*_unhandledIncludes.begin());
		_unhandledIncludes.pop_front();
	}
}

void Preprocessor::ParseComments()
{
	char buffer[1024];
	bool blockComment = false;
	do
	{
		bool lineDone = false;

		_inFile.getline(buffer,1023);

		char * index = buffer;

		//itterate until null, checking for inline comments
		while(*index != '\0')
		{			
			if(!blockComment)
			{
				//Start comment?
				if(*index == '/')
				{
					//Inline comment check
					if(*(index+1) == '/')
					{
						lineDone = true;
					}
					//Block comment check
					else if(*(index+1) == '*')
					{
						blockComment = true;
					}
				}
			}
			else
			{
				//Check for */
				if(*index == '*')
				{
					if(*(index + 1) == '/')
					{
						//incrament index to after */
						//continue normally
						index += 2;
						blockComment = false;
						continue;
					}
				}
			}

			if(lineDone) 
				break;
			if(!blockComment)
			{
				cout << *index;
				_intermediateFile << *index;
			}
			++index;
			if(*index == '\0')
			{
				cout << std::endl;
				_intermediateFile << std::endl;
			}
		}

	}
	while(!_inFile.eof());
}

//void Preprocessor::ParseDirectives()
//{
//	char buffer[1024];
//	bool endIfWait = false;
//	bool findStart = true;
//	bool error = false;
//
//	do
//	{
//		_intermediateFile.getline(buffer,1024);
//
//		char * index = buffer;
//		string statement;
//		string firstValue;
//		string secondValue;
//
//		while(*index != '\0')
//		{
//			//parse as preprocessor?
//			if(*index == '#')
//			{
//				//get statement
//				while(*index != '\0' && *index != ' ')
//				{
//					statement += *index++;
//				}
//
//				//Not all statements have values after
//				//#else / #endif
//				if(*index != '\0')
//					while(*index++ != ' ');
//				
//				if(statement == "#define")
//				{
//					while(*index != '\0' && *index != ' ')
//					{
//						firstValue += *index++;
//					}
//					if(*index == '\0')
//					{
//						//check defines list to see if they exist
//
//						//check symbol table to see if value exists for macro redefiniton
//						if(_symbols[firstValue] == "")
//							_symbols[firstValue] = "";
//						else
//						{
//							cout << firstValue << " already defined -- Ignored " << endl;
//							break;
//						}
//					}
//					while(*index++ != ' ');
//					
//					while(*index != '\0' && *index != ' ')
//					{
//						secondValue += *index++;
//					}
//
//					if(_symbols[firstValue] == "")
//						_symbols[firstValue] = secondValue;
//					else cout << firstValue << " already defined. -- Ignored -- Value is " << _symbols[firstValue] << endl;
//
//					//after both statements are collected, we are done.
//					//index++;
//					break;
//				}
//				else if( statement == "#ifdef" || statement == "#ifndef" )
//				{
//					_ifDepth++;
//					
//				}
//				else
//				{
//					cout << statement << " unknown preprocessor directive" << endl;
//					*index = '\0';
//				}
//			}
//			//if it's not a #statement just write to the output file
//			else
//			{
//				cout << *index;
//				_outputFile << *index;
//
//				++index;
//
//				if(*index == '\0') 
//				{
//					cout << endl;
//					_outputFile << endl;
//				}
//			}
//		}
//	}
//	while(!_intermediateFile.eof());
//}

void Preprocessor::GetValues(string line, string & directive, string & firstValue, string & secondValue) 
{
	char * value = 0;
	char buffer[1024];
	strcpy(buffer, line.c_str());
	value = strtok(const_cast<char*>(line.c_str()), " ");
	char * index = 0;
	directive = value;

	//if there are no passed values
	if(!value)
		return;

	value = strtok(NULL, " ");
	if(!value)
		return;

	firstValue = value;
	//case involving string literals
	if(firstValue[0] == '\"')
	{
		//index = one past the quote
		index = buffer;
		firstValue = string();	
		while(*index != '\"')
		{
			index++;
		}
		++index;

		try
		{
			while(*index != '\"')
			{
				firstValue += *index++;
			}
		}
		catch(exception e)
		{
			_errors.push_back("Error processing " + line + " -- Ignored");
			firstValue = string();
			return;
		}
	}
	else firstValue = value;

	if(index)
	{
		++index;
		if(*index != '\0')
			++index;
		else return;

		if(*index == '\"')
		{
			while(*index != '\"' && *index != '\0')
				secondValue = *index;
			if(*index == '\0')
			{
				_errors.push_back("Missing \"\"\" at end of string literal " + secondValue + " -- Ignored");
				secondValue = string();
			}
		}
	}
	else if( value = strtok(NULL, " ") )
	{
		secondValue = value;
	}
}

void Preprocessor::ParseDirective(string directive, string firstValue, string secondValue, bool skipForIf = false) 
{
	if( skipForIf )
	{
		if( directive == "#else" || directive == "#endif" )
		{
			if(_ifDepth && directive == "#endif")
				--_ifDepth;
			else if( directive == "#else" )
				ParseDirectives();
			else
			{
				_errors.push_back(directive + " without matching #if -- Ignored");
			}
		}
		return;
	}

	if( directive == "#include" )
	{
		if(firstValue != "")
		{
			ParseInclude(firstValue);
		}
		else
			_errors.push_back("No filepath for #include -- Ignored");
	} 
	else if( directive == "#define" )
	{
		if( secondValue != "" )
		{
			ParseDefine(firstValue, secondValue);
		}
		else if( firstValue != "" )
		{
			ParseDefine(firstValue);
		}
		else
		{
			_errors.push_back("#define with no key / value -- Ignored");
		}
	}	//#define
	else if( directive == "#ifdef" )
	{
		if(firstValue != "")
			ParseIfDef(firstValue);
		else
		{
			_errors.push_back("missing value for #ifdef -- Ignored");
		}
	}	//#ifdef
	else if( directive == "#ifndef" )
	{
		if(firstValue != "")
			ParseIfnDef(firstValue);
		else
		{
			_errors.push_back("missing value for #ifndef -- Ignored");
		}
	}	//#ifndef
	else if( directive == "#else" || directive == "#endif" )
	{
		if(directive == "#else" && _ifDepth)
		{
			SkipForIf();
		}
		else if(directive == "#endif" && _ifDepth)
		{
			--_ifDepth;
		}
		else
		{
			_errors.push_back(directive + " without matching #if -- Ignored");
		}
	}	//#else / #endif
	else
	{
		_errors.push_back(directive + " - unrecognized statement -- Ignored");
	}
}


void Preprocessor::ParseInclude(string fileName)
{
	for(auto itr = _unhandledIncludes.begin(); itr != _unhandledIncludes.end(); ++itr)
	{
		if( *itr == fileName )
		{
			return;
		}
	}

	for(auto itr = _includedFiles.begin(); itr != _includedFiles.end(); ++itr)
	{
		if( *itr == fileName )
		{
			return;
		}
	}

	_unhandledIncludes.push_back(fileName);
}

void Preprocessor::ParseDefine(string key)
{
	for(auto itr = _defines.begin(); itr != _defines.end(); ++itr)
	{
		if( *itr == key )
		{
			_errors.push_back(key + " -- Symbol alraedy defined - Ignored");
			return;
		}
	}

	for(auto itr = _symbols.begin(); itr != _symbols.end(); ++itr)
	{
		if( (*itr).first == key )
		{
			_errors.push_back(key + " -- Symbol alraedy defined - Ignored");
			return;
		}
	}

	_defines.push_back(key);
}

void Preprocessor::ParseDefine(string key, string value)
{
	for(auto itr = _defines.begin(); itr != _defines.end(); ++itr)		// check if the key is in the _defines list
	{
		if( *itr == key )
		{
			_errors.push_back(key + " -- Symbol alraedy defined - Ignored - Value is " + *itr);
			return;
		}
	}

	for(auto itr = _symbols.begin(); itr != _symbols.end(); ++itr)		//check if the key is in the symbols list
	{
		if( (*itr).first == key )
		{
			_errors.push_back(key + " -- Symbol alraedy defined - Ignored - Value is " + (*itr).second);
			return;
		}
	}

	_symbols[key] = value;
	Token a(value, TokenType::DEFINE);
	//m_SymbolTable.addSymbol(key, a);
	m_SymbolTable.addSymbolLevel(key, 0, a);
}

void Preprocessor::ParseIfDef(string key)
{
	for(auto itr = _defines.begin(); itr != _defines.end(); ++itr)
	{
		if( *itr == key )
		{
			++_ifDepth;
			ParseDirectives();
			return;
		}
	}

	for(auto itr = _symbols.begin(); itr != _symbols.end(); ++itr)
	{
		if( (*itr).first == key )
		{
			++_ifDepth;
			ParseDirectives();
			return;
		}
	}

	SkipForIf();
}

void Preprocessor::ParseIfnDef(string key)
{
	for(auto itr = _defines.begin(); itr != _defines.end(); ++itr)
	{
		if( *itr == key )
		{
			SkipForIf();
			return;
		}
	}

	for(auto itr = _symbols.begin(); itr != _symbols.end(); ++itr)
	{
		if( (*itr).first == key )
		{
			SkipForIf();
			return;
		}
	}

	++_ifDepth;
	ParseDirectives();
}

void Preprocessor::ParseDirectives()
{
	char buffer[1024];
	do
	{
		string directive;
		string firstValue;
		string secondValue;

		_intermediateFile.getline(buffer, 1023);
		char * index = buffer;

		if(*index != '#')
		{
			cout << buffer << endl;
			_outputFile << buffer << endl;
			continue;
		}
		else
		{
			GetValues(buffer, directive, firstValue, secondValue);
			ParseDirective(directive, firstValue, secondValue);
		}
	}while(!_intermediateFile.eof());
}

void Preprocessor::SkipForIf()
{
	char buffer[1024];
	do
	{
		if(!_ifDepth)
			return;
		string directive;
		string firstValue;
		string secondValue;

		_intermediateFile.getline(buffer, 1023);
		char * index = buffer;

		if(*index != '#')
		{
			continue;
		}
		else
		{
			GetValues(buffer, directive, firstValue, secondValue);
			ParseDirective(directive, firstValue, secondValue, true);
		}
	}while(!_intermediateFile.eof());
}

//void Preprocessor::ParseFromIf()
//{
//
//}

map<string,string> Preprocessor::getSymbols()
{
	return _symbols;
}