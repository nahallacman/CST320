#ifndef PP_H
#define PP_H
#include <string>
#include <fstream>
#include <map>
#include <list>
#include "SymbolTable.h"
using namespace std;

class Preprocessor
{
public:
	Preprocessor(string, SymbolTable);
	~Preprocessor();

	map<string,string> getSymbols();
	SymbolTable getSymbolTable();

private:
	void Parse();
	void ParseComments();
	void ParseDirectives();
	void ParseDirective(string,string,string,bool);
	void ParseInclude(string);
	void ParseDefine(string);
	void ParseDefine(string,string);
	void ParseIfDef(string);
	void ParseIfnDef(string);
	void SkipForIf();
	//void ParseFromIf();

	void GetValues(string,string&,string&,string&);

	ifstream			_inFile;
	fstream				_intermediateFile;
	ofstream			_outputFile;

	map<string,string>	_symbols;

	SymbolTable			m_SymbolTable;

	list<string>		_unhandledIncludes;
	list<string>		_includedFiles;
	list<string>		_defines;
	list<string>		_errors;

	int					_ifDepth;
};
#endif

