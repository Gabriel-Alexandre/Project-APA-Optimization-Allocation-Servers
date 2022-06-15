#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Scenario.hpp"
#include "Data.hpp"
#include <cfloat>

#define MAX_ITE 100
#define MAX_ILS 200

using namespace std;

int main(int argc, char *argv[]) {

    time_t t;
    srand(time(&t));

    Data data;
    data.read(argv[1]);
    

    Scenario bestScenario;
    double bestObjectiveValue = DBL_MAX;

    for(int total = 0; total < MAX_ITE; total++) {

        Scenario A;
        A.construction(&data);
        //A.printNotAlloc();
        //getchar();


        Scenario B = A;

        for(int iter = 0; iter < MAX_ILS; iter++) {
            
            for(int i = 0; i < 4; i++) {
                
                bool improvement = false;

                if(i == 0) {
                    improvement = A.reinsertion();
                } else if (i == 1) {
                    improvement = A.allocExternJobs();
                } else if(i == 2) {
                    improvement = A.swapNotAllocJobs();
                } else if(i == 3) {
                    improvement = A.swap();

                }

                if(improvement) {
                    i = -1;
                } 

            }
            
            if(A.getObjectiveValue() < B.getObjectiveValue()) {
                //printf("Funcionou: %.4lf\n", A.getObjectiveValue());
                B = A;
                iter = -1;
            } else {
                A = B;
                //printf("%.2lf\n", A.getObjectiveValue());
            }

            
            A.ILS();
            


        }

        if(B.getObjectiveValue() < bestObjectiveValue) {
            bestObjectiveValue = B.getObjectiveValue();
            bestScenario = B;
        }


    }



    //printf("\n\n\n");
    bestScenario.printAux();
    bestScenario.printServersTime();
    printf("Custo Final: %.4lf\n", bestScenario.getObjectiveValue());
    bestScenario.printNotAlloc();
    bestScenario.printSolution();

   return 0;
}