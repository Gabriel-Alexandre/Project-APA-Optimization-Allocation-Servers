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

int Scenario::getServers() {
    return this->servers;
}

int Scenario::getJobs() {
    return this->jobs;
}


void Scenario::addItemSpend(string spend) {
    this->spend.push_back(this->auxSplit(spend));
}

void Scenario::addItemTime(string time) {
    this->time.push_back(this->auxSplit(time));
}

void Scenario::addItemCapacity(string capacity) {
    for (int i: this->auxSplit(capacity)) {
        this->capacity.push_back(i);
    }
}

void Scenario::printServersJobs() {
    cout << "servers: " << this->servers << ", " << "jobs: " << this->jobs << endl;
    
}
void Scenario::printVectorSpend() {
    for (vector <int> v: this->spend) {
        for (int i: v) {
            cout << i << ' ';
        }
        cout << endl;
    }
}
void Scenario::printVectorTime() {
    for (vector <int> v: this->time) {
        for (int i: v) {
            cout << i << ' ';
        }
        cout << endl;
    }
}

void Scenario::printCapacity() {
    for (int i: this->capacity) {
        cout << i << endl;
    }
}

vector <int> Scenario::auxSplit(string word) {
    string aux;
    vector <int> v;

    for(int i = 0; i <= word.size(); i++) {
        if (word[i] == ' ') {
            v.push_back(stoi(aux));
            aux = " ";
            continue;
        }

        aux += word[i];

        if (i == word.size()) {
            v.push_back(stoi(aux));
        }
    }

    return v;
}