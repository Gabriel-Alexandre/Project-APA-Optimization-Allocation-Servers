#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Scenario.hpp"

using namespace std;

int main() {
    // Scenario first;

    // first.setJobs(4);
    // first.setServers(4);

    // for(int i = 0; i < 4; i++) {
    //     vector <int> aux;

    //     aux.push_back(i);
    //     aux.push_back(i);
    //     aux.push_back(i);
    //     aux.push_back(i);

    //     first.addItemSpend(aux);
    //     first.addItemTime(aux);
    // }

    // first.printServersJobs();

    // first.printVectorSpend();
    // first.printVectorTime();

    string line;
    ifstream data ("adjacency_matrix.txt"); 

    string servers, jobs; 

    getline(data, servers); // transformar em int
    getline(data, jobs); // transformar em int

    cout << servers << endl;
    cout << jobs << endl;

    // Lê a outras linha segundo esse mesmo princípio



    // if (data.is_open())
    // {
    //     while(!data.eof())
    //     {
    //         getline(data,line);
    //         cout << line << endl;
    //     }
    //     data.close();
    // }





}