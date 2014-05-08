#ifndef LEXICALCHECKNODE_H
#define LEXICALCHECKNODE_H
#include "DFAWrapper.h"



class LexicalCheckNode
{
private:
	char m_FirstChar;
	DFAWrapper m_LexicalChecker;
public:
	LexicalCheckNode( char * input_file , char m_FirstChar );
	int CheckString( char * checked );
	char getFirstChar();
	int getLength();
};



#endif