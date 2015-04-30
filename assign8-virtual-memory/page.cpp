#include <iostream>
// #include <algorithm>
// #include <iterator>
// #include <exception>
// #include <stdexcept>
// #include <cerrno>
//#include "page.h"
#include "pageH.cpp"

using namespace std;

int main()
{
	short choice=0;
	PRL_simulate simulator;

	cout << "Which Page Replacement Algorithm do you want to simulate ?" << endl;
	cout << "1. FIFO" << endl;
	cout << "2. LFU" << endl;
	cout << "3. LRU" << endl;
	cout << "4. Second Chance Algorithm" << endl;
	cout << "Enter your choice here : ";
	cin >> choice;
	//cout << "You selected choice no. " << choice << endl;

	switch (choice)
	{
		case 1:
		{
			simulator.fifo();
			break;
		}
		case 2:
		{
			simulator.lfu();
			break;
		}
		case 3:
		{
			simulator.lru();
			break;
		}
		case 4:
		{
			simulator.sca();
			break;
		}
		default:
		{
			cout << "Default mode." << endl;
		}
	}

	return 1;
}