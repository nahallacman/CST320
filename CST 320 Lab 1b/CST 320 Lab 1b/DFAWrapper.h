#ifndef DFAWRAPPER_H
#define DFAWRAPPER_H

#include "DFA.h"
#include <iostream>
#include <fstream>
using std::ifstream;
using std::cout;
using std::endl;

class DFAWrapper
{
private:
	char m_inputfile[100];
	DFA m_dfa;

public:
	DFAWrapper(char * input_file);
	void ReadFile();
	bool TestString(char * input);
	int getLength();
};

#endif