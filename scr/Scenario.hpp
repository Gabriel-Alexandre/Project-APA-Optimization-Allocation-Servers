#ifndef SCENARIO_H
#define SCENARIO_H
#include <vector>
#include <string>
#include <queue>

#include "Data.hpp"

using namespace std;
	
class Scenario {
	private:
		int servers; // numero de servidores
		int jobs; // numero de jobs
        vector<double> capacity; // capacidade de cada servidor
        vector<vector<double>> spend; // vetor de custos de cada servidor
        vector<vector<double>> time; // vetor de tempo de cada servidor
        vector<vector<int>> solution; // vetor de 'time/spend' de cada servidor

	public:
		Scenario();       

        void generateSolution(Data *data);
        // retorna -> Melhor solução encontrada depois de fazer todos movimentos possiveis de swap
        bool swap();
        vector<int> caculateTotalCoastAndTimeServer(vector<vector<int>> solution);
		
};
#endif