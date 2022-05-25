#ifndef SCENARIO_H
#define SCENARIO_H
#include <vector>
#include <string>

using namespace std;
	
class Scenario {
	private:
		int servers;
		int jobs;
        vector <int> capacity;
        vector <vector<int>> spend;
        vector <vector<int>> time;

	public:
		Scenario();
		void setServers(int servers);
		void setJobs(int jobs);
        int getServers();
        int getJobs();

        void addItemSpend(string spend);
        void addItemTime(string time);
        void addItemCapacity(string capacity);
        
        void printServersJobs();
        void printVectorSpend();
        void printVectorTime();
        void printCapacity();

        vector <int> auxSplit(string word);
		
};
#endif