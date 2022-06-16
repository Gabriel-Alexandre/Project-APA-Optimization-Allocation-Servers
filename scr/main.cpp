#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "ServerJobOpt.hpp"
#include "Data.hpp"
#include <cfloat>
#include <chrono>
#include <string.h>

using namespace std;

void setup(int jobsCount);
double VND(ServerJobOpt &solution);
void solve_problem(ServerJobOpt &solution);

int maxIterations, maxILS;
vector<int> neighborhoods;

int main(int argc, char *argv[]) {
    
    time_t t;
    srand(time(&t));
    
    Data data;
    data.read(argv[1]);

    setup(data.getJobsCount());        
    
    ServerJobOpt bestSolution, auxSolution, solution;
    bestSolution = auxSolution = solution = ServerJobOpt(&data);

    double bestObjectiveValue = DBL_MAX;

    for(int total = 0; total < maxIterations; total++) {

        solution.construction();
        auxSolution.updateSolution(&solution);

        for(int ils = 0; ils < maxILS; ils++) {

            VND(auxSolution);

            if(auxSolution.getObjectiveValue() < solution.getObjectiveValue()) {
                solution.updateSolution(&auxSolution);
                ils = -1;
            } else {
                auxSolution.updateSolution(&solution);
            }

            auxSolution.ILS();

        }
        
        if(solution.getObjectiveValue() < bestObjectiveValue) {
            bestObjectiveValue = solution.getObjectiveValue();
            bestSolution.updateSolution(&solution);
        }

    
    }

    cout << "Objective Value: " << bestSolution.getObjectiveValue() << endl;
     
   return 0;
}

void setup(int jobsCount) {

    if(jobsCount <= 30) {
        maxIterations = 300;
        maxILS = 100;
        neighborhoods = {REINSERTION_NOT_ALLOC, REINSERTION, SWAP, SWAP_NOT_ALLOC};
    } else {
        maxIterations = 100;
        maxILS = 10;
        neighborhoods = {REINSERTION, REINSERTION_NOT_ALLOC, SWAP, SWAP_NOT_ALLOC};
    }

}

double VND(ServerJobOpt &solution) {

    for(int neighIndex = 0; neighIndex < neighborhoods.size(); neighIndex++) {
            
        bool improvement = false;
        if(neighborhoods[neighIndex] == REINSERTION_NOT_ALLOC) {
            improvement = solution.allocExternJobs();
        } else if (neighborhoods[neighIndex] == REINSERTION) {
            improvement = solution.reinsertion();
    
        } else if(neighborhoods[neighIndex] == SWAP) {
            improvement = solution.swap();
        } else if(neighborhoods[neighIndex] == SWAP_NOT_ALLOC) {
            improvement = solution.swapNotAllocJobs();
        }
        if(improvement) {
            neighIndex = -1;
        } 
    }

    return solution.getObjectiveValue();
}

