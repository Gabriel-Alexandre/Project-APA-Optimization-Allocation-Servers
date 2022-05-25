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
        vector <vector<int>> finalSolutionSpend;
        vector <vector<int>> finalSolutionTime;
        vector <vector<int>> spend;
        vector <vector<int>> time;
        vector <vector <float>> solution;

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
        void printSolution();

        vector <int> auxSplit(string word);

        void generateSolution();
		
};
#endif