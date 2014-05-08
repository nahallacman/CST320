#ifndef DFA_H
#define DFA_H

#include <list>
#include <map>
#include <iostream>
using std::cout;
using std::endl;
using std::pair;
using std::map;
using std::list;

class DFA
{
private:
	int m_num_states;
	int m_cur_state;
	map<pair<int, char>, int> m_map;
	list<int> m_end_states;

public:
	DFA();
	DFA(int states);
	void SetNumStates(int max);
	void AddEdge(int start, char value, int end);
	void AddEndState(int state);
	bool TryValue(char input);
	//int GetCurState();
	bool CheckIfCurrentStateIsAFinalState();
	void ResetMachineState();
	int GetLastFinalState();
};

#endif