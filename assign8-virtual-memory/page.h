class PRL_simulate
{

public:
	PRL_simulate(); //the constructor
	int fifo(); /* function to simulate FIFO page replacement algorithm */
	int lfu(); /* function to simulate LFU page replacement algorithm */
	int lru(); /* function to simulate LRU page replacement algorithm */
	int sca(); /* function to simulate Second chance algorithm page replacement algorithm */
};