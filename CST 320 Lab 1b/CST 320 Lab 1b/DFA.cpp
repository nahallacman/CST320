#include "DFA.h"


DFA::DFA()
{
	m_num_states = 0;
	m_cur_state = 1;
}

DFA::DFA(int states):m_num_states(states)
{
	m_cur_state = 1;
}

void DFA::SetNumStates(int max)
{
	m_num_states = max;
}

void DFA::AddEdge(int start, char value, int end)
{
	if(start <= m_num_states)
	{
		if( end <= m_num_states )
		{
			pair<int, char> _pair;
			_pair.first = start;
			_pair.second = value;
			m_map[_pair] = end;
		}
		else
		{
			cout << "End state is above the max number of states" << endl;
		}
	}
	else
	{
		cout << "Start state is above the max number of states" << endl;
	}
}

bool DFA::TryValue(char input)
{
	bool ret = false;
	pair<int, char> _pair;
	int a;
	_pair.first = m_cur_state;
	_pair.second = input;
	a = m_map[_pair];
	if(a != 0 && a <= m_num_states)
	{
		ret = true;
		m_cur_state = a;
	}
	return ret;
}
/*
int DFA::GetCurState()
{
	return m_cur_state;
}
*/
void DFA::AddEndState(int state)
{
	if(state <= m_num_states)
	{
		m_end_states.push_back(state);
		m_end_states.sort();
	}
	else
	{
		cout << "Final state value is greater then the number of states" << endl;
	}
}

bool DFA::CheckIfCurrentStateIsAFinalState()
{
	bool ret=false;
	list<int>::iterator i;
	for( i = m_end_states.begin() ; i != m_end_states.end() ; ++i )
	{
		if(*i == m_cur_state)
		{
			ret = true;
		}
	}
	return ret;
}

void DFA::ResetMachineState()
{
	m_cur_state = 1;
}

int DFA::GetLastFinalState()
{
	list<int>::iterator i = m_end_states.end();
	--i;
	return *i;
}