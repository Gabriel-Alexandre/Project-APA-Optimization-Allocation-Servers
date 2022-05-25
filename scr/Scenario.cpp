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

void Scenario::generateSolution() {

    for (int i = 0; i < this->servers; i++) {
        vector <float> aux;

        for (int j = 0; j < this->jobs; j++) {
            aux.push_back(this->time[i][j]/(this->spend[i][j]*1.0));
        }

        this->solution.push_back(aux);
    }

    // for (vector<float> x: this->solution) {
    //     for(float i: x) {
    //         cout << i << ' ';
    //     }
    //     cout << endl;
    // }
    
    int totalTime[this->servers];

    for(int i = 0; i < servers; i++) {
            totalTime[i] = 0;
        }

    for (int i = 0; i < this->jobs; i++) {
        float min = 1000000;
        
        int indice;
        vector<int> aux;

        for (int j = 0; j < this->servers; j++) {
            if ((i == 0) && (this->time[j][i] <= this->capacity[j])) {
                if (this->solution[j][i] < min) {
                    min = this->solution[j][i];
                    indice = j;
                }
            }
            
            if ((i > 0) && (this->solution[j][i] < min) && ((totalTime[j]+ this->time[j][i]) <= this->capacity[j])) {
                min = this->solution[j][i];
                indice = j;
            }
        }

        totalTime[indice] += this->time[indice][i];

        aux.push_back(indice+1);
        aux.push_back(this->spend[indice][i]);

        this->finalSolutionSpend.push_back(aux);

        aux.pop_back();
        aux.push_back(this->time[indice][i]);

        this->finalSolutionTime.push_back(aux);
    }

    cout << "Final solution:\n" << endl;

    cout << "Spend:\n" << endl;

    for(vector <int> i: this->finalSolutionSpend) {
        for (int x: i) {
            cout << x << ' ';
        }
        cout << endl;
    }

    cout << "Time:\n" << endl;

    for(vector <int> i: this->finalSolutionTime) {
        for (int x: i) {
            cout << x << ' ';
        }
        cout << endl;
    }
}