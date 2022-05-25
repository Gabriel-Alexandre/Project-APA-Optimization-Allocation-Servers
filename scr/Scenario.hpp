#ifndef SCENARIO_H
#define SCENARIO_H
#include <vector>
#include <string>

using namespace std;
	
class Scenario {
	private:
		int servers; // numero de servidores
		int jobs; // numero de jobs
        vector <int> capacity; // capacidade de cada servidor
        vector <vector<int>> finalSolutionSpend; // custo de acordo com servidor escolhido
        vector <vector<int>> finalSolutionTime; // tempo de acordo com servidor escolhido
        vector <vector<int>> spend; // vetor de custos de cada servidor
        vector <vector<int>> time; // vetor de tempo de cada servidor
        vector <vector <float>> solution; // vetor de 'time/spend' de cada servidor

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

        // funcao que auxiliar que ajuda na obtencao e transformacao dos valores 
        // (utilizada apos a leitura do arquivo)
        vector <int> auxSplit(string word);

        void generateSolution(); // gerar solucao -> escolher quais servidores serão alocados para cada job
		
};
#endif