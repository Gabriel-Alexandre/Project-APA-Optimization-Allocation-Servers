#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Scenario.hpp"

using namespace std;

int main() {
    Scenario A;
    string line;
    ifstream data ("adjacency_matrix.txt");

    getline(data, line); 
    A.setJobs(stoi(line));
    getline(data, line); 
    A.setServers(stoi(line));

    getline(data, line);
    getline(data, line);

    A.addItemCapacity(line);

    // A.printServersJobs();
    // A.printCapacity();

    for (int i = 0; i < 2; i++) {
        getline(data, line);
        if(i == 0) {
            for (int i = 0; i < A.getServers(); i++) {
                getline(data, line);
                A.addItemTime(line);
            }
        }
        if(i == 1) {
            for (int i = 0; i < A.getServers(); i++) {
                getline(data, line);
                A.addItemSpend(line);
            }
        }
    }

    // A.printVectorTime();
    // A.printVectorSpend();
    A.generateSolution();
}