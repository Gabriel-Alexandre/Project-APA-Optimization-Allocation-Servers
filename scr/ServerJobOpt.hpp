#ifndef SERVER_JOB_OPT_H
#define SERVER_JOB_OPT_H

#include <vector>
#include <string>
#include <queue>
#include <numeric>

#include "Data.hpp"

#define REINSERTION_NOT_ALLOC 0
#define REINSERTION 1
#define SWAP 2
#define SWAP_NOT_ALLOC 3

#define MAX_CONSTRUCTIONS 2

using namespace std;
	
class ServerJobOpt {
	private:
		int serversCount; // numero de servidores
		int jobsCount; // numero de jobs
        vector<int> jobsNotAlloc;
        vector<double> currentTimeByServer; // tempo consumido de cada servidor
        int penality;
        vector<double> capacity; // capacidade de cada servidor
        vector<vector<double>> cost; // matriz de custos
        vector<vector<double>> time; // matriz de tempoS
        vector<vector<int>> solution; // solução
        double objectiveValue; // custo total da solução
	public:
		ServerJobOpt();       
        ServerJobOpt(Data *data);
        double construction();
        bool swap();
        bool reinsertion();
		double getObjectiveValue();
        bool allocExternJobs();
        bool swapNotAllocJobs();
        void ILS();
        void updateSolution(ServerJobOpt *opt);
        vector<int> getNotAllocJobs();
        vector<double> getCurrentTimeByServer();
        vector<vector<int>> getSolution();
        int getJobsCoutn();
};
#endif