#include "LexicalCheckNode.h"

LexicalCheckNode::LexicalCheckNode (char * input_file , char FirstChar ):m_LexicalChecker(input_file),m_FirstChar(FirstChar)
{
}

int LexicalCheckNode::CheckString( char * checked )
{
	int ret = 1;
	if ( m_LexicalChecker.TestString(checked) )
	{
		ret = 0;
	}
	return ret;
}

char LexicalCheckNode::getFirstChar()
{
	return m_FirstChar;
}

int LexicalCheckNode::getLength()
{
	return m_LexicalChecker.getLength();
}