#ifndef SCENARIO_H
#define SCENARIO_H
#include <vector>

using namespace std;
	
class Scenario {
	private:
		int servers;
		int jobs;
        vector <vector<int>> spend;
        vector <vector<int>> time;

	public:
		Scenario();
		void setServers(int servers);
		void setJobs(int jobs);

        void addItemSpend(vector <int> spend);
        void addItemTime(vector <int> time);
        
        void printServersJobs();
        void printVectorSpend();
        void printVectorTime();
		
};
#endif