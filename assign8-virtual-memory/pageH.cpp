#include <iostream>
#include <algorithm>
#include <vector>
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

bool PRL_simulate::fifo(int frames, int pages)
/* Public function FIFO definition */
{
	cout << "Starting FIFO simulation" << endl;
}

bool PRL_simulate::lfu(int frames, int pages)
/* Public function LFU definition */
{
	cout << "Starting LFU simulation" << endl;
}

bool PRL_simulate::lru(int frames, int pages)
/* Public function LRU definition */
{
	cout << "Starting LRU simulation" << endl;
}

bool PRL_simulate::sca(int frames, int pages)
/* Public function sca definition */
{
	cout << "Starting Second Chance Algorithm simulation" << endl;
}