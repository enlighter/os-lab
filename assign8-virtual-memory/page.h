#include <vector>

using namespace std;

class PRL_simulate
{
	int page_faults;
	vector<int> frame_list; /* stores the page no. corresponding to a frame */

	class PageRequests
	{
	public:
		bool PageReuest();
		virtual int PageFaults();
	};

	class fifoService :public PageRequests
	{
	public:
		int PageFaults(int, int);
	};
	class scaService :public PageRequests
	{
	public:
		int PageFaults(int, int);
	};
	class lfuService :public PageRequests
	{
	public:
		int PageFaults(int, int);
	};
	class lruService :public PageRequests
	{
	public:
		int PageFaults(int, int);
	};

public:
	PRL_simulate(); //the constructor
	bool fifo(int, int); /* function to simulate FIFO page replacement algorithm */
	bool lfu(int, int); /* function to simulate LFU page replacement algorithm */
	bool lru(int, int); /* function to simulate LRU page replacement algorithm */
	bool sca(int, int); /* function to simulate Second chance algorithm page replacement algorithm */
};