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
        vector <double> capacity; // capacidade de cada servidor
        vector <vector<double>> finalSolutionSpend; // custo de acordo com servidor escolhido
        vector <vector<double>> finalSolutionTime; // tempo de acordo com servidor escolhido
        vector <vector<double>> spend; // vetor de custos de cada servidor
        vector <vector<double>> time; // vetor de tempo de cada servidor
        vector <vector <int>> solution; // vetor de 'time/spend' de cada servidor

	public:
		Scenario();         
       
        void printSolution();

        void generateSolution(Data *data); // gerar solucao -> escolher quais servidores serão alocados para cada job
		
};
#endif