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

   return 0;
}