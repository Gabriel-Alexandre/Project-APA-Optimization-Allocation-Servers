#include "Scenario.hpp"
#include <iostream>
#include <vector>

using namespace std;

Scenario::Scenario() {
    
}

void Scenario::setServers(int servers) {
    this->servers = servers;
    
}
void Scenario::setJobs(int jobs) {
    this->jobs = jobs;
}

void Scenario::addItemSpend(vector <int> spend) {
    this->spend.push_back(spend);
}

void Scenario::addItemTime(vector <int> time) {
    this->time.push_back(time);
}

void Scenario::printServersJobs() {
    cout << "servers: " << this->servers << ", " << "jobs: " << this->jobs << endl;
    
}
void Scenario::printVectorSpend() {
    for (vector <int> v: this->spend) {
        for (int i: v) {
            cout << i;
        }
        cout << endl;
    }
}
void Scenario::printVectorTime() {
    for (vector <int> v: this->time) {
        for (int i: v) {
            cout << i;
        }
        cout << endl;
    }
}