#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Scenario.hpp"
#include "Data.hpp"

using namespace std;

int main(int argc, char *argv[]) {

    Data data;
    data.read(argv[1]);
    // data.printData();

    
    Scenario A;

    A.generateSolution(&data);
    // A.swap(); 
    // A.reinsertion();
    int n =5;

    while(n--) {

        A.reinsertion();
        A.swap();  
    }

    A.printAux();

   return 0;
}