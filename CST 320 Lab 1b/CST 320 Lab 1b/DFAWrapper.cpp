
#include "DFAWrapper.h"

DFAWrapper::DFAWrapper(char * input_file)
{
	strcpy(m_inputfile, input_file);
	ReadFile();
}

bool DFAWrapper::TestString(char * input)
{
	int i = 0;
	int inputlength = strlen(input);
	bool loop = true;
	bool ret = false;
	m_dfa.ResetMachineState();

	while(loop)
	{
		if( !m_dfa.TryValue(input[i]) )
		{
			loop = false;
			if( i < inputlength )
			{
				//cout << "Invalid input for state (no matching value in edge list)" << endl;
			}
			else
			{
				//cout << "End of input string." << endl;
				if( m_dfa.CheckIfCurrentStateIsAFinalState() )
				{
					//cout << "This is a legal string" << endl;
					ret = true;
				}
				else
				{
					//cout << "Input string ended but the machine is not in a final state." << endl;
				}
			}
		}
		i++;
	}
	return ret;
}

void DFAWrapper::ReadFile()
{
	ifstream input;
	input.open(m_inputfile);
	int temp2, temp4;
	char temp1, temp3;

	if( input.is_open() )
	{
		input >> temp1;
		if(temp1 == 'N')
		{
			input >> temp2;
			m_dfa.SetNumStates(temp2);
		}
		else
		{
			cout << "Error in reading number of states" << endl;
		}

		input >> temp1;
		if(temp1 == 'E')
		{
			input >> temp2 >> temp3 >> temp4;
			m_dfa.AddEdge(temp2, temp3, temp4);
		}
		else
		{
			cout << "Error in reading first edge" << endl;
		}

		while( !input.eof() )
		{
			input >> temp1;
			if(temp1 == 'E')
			{
				input >> temp2 >> temp3 >> temp4;
				m_dfa.AddEdge(temp2, temp3, temp4);
			}
			else if(temp1 == 'F')
			{
				//read all final states
				while( !input.eof() )
				{
					input >> temp2;
					m_dfa.AddEndState(temp2);
				}
			}
			else
			{
				cout << "Error in reading file" << endl;
			}
		}
		input.close();
	}
	else
	{
		cout << "File was not opened" << endl;
	}
}

//Note: this only works if there is only one parse tree per DFA. Don't rely on this.
int DFAWrapper::getLength()
{
	return m_dfa.GetLastFinalState() - 1;
}