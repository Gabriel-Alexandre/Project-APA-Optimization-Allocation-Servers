#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "ServerJobOpt.hpp"
#include "Data.hpp"
#include <cfloat>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {
    
    time_t t;
    srand(time(&t));

    int n = 10;
    while(n--){
        

    Data data;
    data.read(argv[1]);
    
    
    int maxIterations = 100, maxILS = 10;
    vector<int> neighborhoods;
    if(data.getJobsCount() <= 30) {
        maxIterations = 200;
        maxILS = 20;
        neighborhoods = {0, 1, 2, 3};
    } else {
        neighborhoods = {1, 0, 2, 3};
    }

    ServerJobOpt bestSolution, auxSolution, solution;
    bestSolution = solution = auxSolution = ServerJobOpt(&data);

    double bestObjectiveValue = DBL_MAX;

    for(int total = 0; total < maxIterations; total++) {

        //auxSolution = ServerJobOpt(&data);
        
       // auto begin = chrono::system_clock::now();
        auxSolution.construction();
        //printf("Objective Value: %.4lf\n", A.getObjectiveValue());
      //  auto end = chrono::system_clock::now();
       // chrono::duration<double> algTime = end - begin;
       // printf("Tempo: %.4lf ms\n", algTime.count()*1000);


        solution.updateSolution(&auxSolution);
        //ServerJobOpt solution = auxSolution;

        for(int iter = 0; iter < maxILS; iter++) {
            
            for(int i = 0; i < 4; i++) {
                
                bool improvement = false;

                if(neighborhoods[i] == 0) {
                    improvement = auxSolution.allocExternJobs();
                } else if (neighborhoods[i] == 1) {
                    improvement = auxSolution.reinsertion();
                } else if(neighborhoods[i] == 2) {
                    improvement = auxSolution.swap();
                } else if(neighborhoods[i] == 3) {
                    improvement = auxSolution.swapNotAllocJobs();

                }

                if(improvement) {
                    i = -1;
                } 

            }
            
            if(auxSolution.getObjectiveValue() < solution.getObjectiveValue()) {
                //printf("Funcionou: %.4lf\n", A.getObjectiveValue());
                solution.updateSolution(&auxSolution);
                //solution = auxSolution;
                iter = -1;
            } else {
                auxSolution.updateSolution(&solution);
                // auxSolution = solution;
                //printf("%.2lf\n", A.getObjectiveValue());
            }

            
            auxSolution.ILS();
            


        }

        
        if(solution.getObjectiveValue() < bestObjectiveValue) {
            bestObjectiveValue = solution.getObjectiveValue();
            bestSolution.updateSolution(&solution);
            //bestSolution = solution;
        }


    }



    //printf("\n\n\n");
   // bestScenario.printAux();
    //bestScenario.printServersTime();
    printf("Custo Final: %.4lf\n", bestSolution.getObjectiveValue());
    bestSolution.printNotAlloc();
    //bestScenario.printSolution();
    }
        
   return 0;
}

