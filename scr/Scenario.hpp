#ifndef SCENARIO_H
#define SCENARIO_H
#include <vector>
#include <string>
#include <queue>

#include "Data.hpp"

#define MAX_CONSTRUCTIONS 1

using namespace std;
	
class Scenario {
	private:
		int serversCount; // numero de servidores
		int jobsCount; // numero de jobs
        vector<int> jobsNotAlloc;
        vector<double> currentTimeByServer;
        int penality;
        vector<double> capacity; // capacidade de cada servidor
        vector<vector<double>> cost; // vetor de custos de cada servidor
        vector<vector<double>> time; // vetor de tempo de cada servidor
        vector<vector<int>> solution; // vetor de 'time/spend' de cada servidor
        double objectiveValue;
	public:
		Scenario();       

        void construction(Data *data);
        void generateSolution2(Data *data);
        // retorna -> Melhor solução encontrada depois de fazer todos movimentos possiveis de swap
        bool swap();
        bool reinsertion();
        void printAux();
        vector<int> caculateTotalCoastAndTimeServer(vector<vector<int>> solution);
		double getObjectiveValue();
        bool allocExternJobs();
        void printNotAlloc();
        void printServersTime();
        bool swapNotAllocJobs();
        void ILS();
        void printSolution();
};
#endif