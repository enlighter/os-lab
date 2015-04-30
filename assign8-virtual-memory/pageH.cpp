#include <iostream>
#include <algorithm>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <cerrno>
#include "page.h"

using namespace std;

PRL_simulate::PRL_simulate()
/* Public Constructor definition */
{

}

int PRL_simulate::fifo()
/* Public function FIFO definition */
{
	cout << "Starting FIFO simulation" << endl;
}

int PRL_simulate::lfu()
/* Public function LFU definition */
{
	cout << "Starting LFU simulation" << endl;
}

int PRL_simulate::lru()
/* Public function LRU definition */
{
	cout << "Starting LRU simulation" << endl;
}

int PRL_simulate::sca()
/* Public function sca definition */
{
	cout << "Starting Second Chance Algorithm simulation" << endl;
}