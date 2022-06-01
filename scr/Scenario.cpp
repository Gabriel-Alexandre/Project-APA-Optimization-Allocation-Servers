#include "Scenario.hpp"
#include <iostream>
#include <vector>

using namespace std;

Scenario::Scenario() {
    
}


void Scenario::printSolution() {
    cout << "Final solution:" << endl;

    int total[this->servers];
    int sum = 0;

    for(int i = 0; i < this->servers; i++) {
        total[i] = 0;
    }

    cout << "\nSpend:\n" << endl;

    for(vector <double> i: this->finalSolutionSpend) {
        int indice = 0;

        for (int j = 0; j < 2; j++) {
            if (j == 0) {
                indice = i[j];
            }
            if (j == 1) {
                total[indice] += i[j];
                sum += i[j];
            }

            cout << i[j] << ' ';
        }
        cout << endl;
    }

    for (int i = 0; i < this->servers; i++) {
        cout << "Total spend[" << i << "] : " << total[i] << endl;
    }
    cout << "Sum: " << sum << endl;

    for(int i = 0; i < this->servers; i++) {
        total[i] = 0;
    }

    sum = 0;

    cout << "\nTime:\n" << endl;

    for(vector <double> i: this->finalSolutionTime) {
        int indice = 0;

        for (int j = 0; j < 2; j++) {
            if (j == 0) {
                indice = i[j];
            }
            if (j == 1) {
                total[indice] += i[j];
                sum += i[j];
            }

            cout << i[j] << ' ';
        }
        cout << endl;
    }
    for (int i = 0; i < this->servers; i++) {
        cout << "Total time[" << i << "] : " << total[i] << endl;
    }
    cout << "Sum: " << sum << endl;
}



void Scenario::generateSolution(Data *data) {

    servers = data->getServersCount();
    jobs = data->getJobsCount();
    spend = data->getC();
    time = data->getT();
    capacity = data->getServersCapacity();

    for(int i = 0; i < 3; i++) {
        
        int allocJobsCount = 0;
        solution = vector<vector<int>>(servers);

        vector<double> auxCapacity = capacity;
        for (int job = 0; job < this->jobs; job++) {
            double aux;
            priority_queue<pair<double, int>> sortedServersCost;
            for (int server = 0; server < this->servers; server++) {

                switch(i) {
                    case 0:
                        aux = spend[server][job];
                        break;
                    case 1:
                        aux = spend[server][job]/time[server][job];
                        break;
                    case 2:
                        aux = time[server][job];
                        break;
                }


                sortedServersCost.push(make_pair(-aux, server));
            }
            
            while(!sortedServersCost.empty()) {
                
                pair<double, int> values = sortedServersCost.top();
                double cost = values.first;
                int server = values.second;

                if(auxCapacity[server] - time[server][job] >= 0) {
                    auxCapacity[server] -= time[server][job];
                    solution[server].push_back(job);
                    allocJobsCount++;
                    break;
                }

                sortedServersCost.pop();
            }
        }

        if(allocJobsCount == jobs) break;

    }

    for(int server = 0; server < solution.size(); server++) {
        printf("Server %d\n", server+1);
        double total = 0;
        for(int jobs = 0; jobs < solution[server].size(); jobs++) {
            printf("%d ", solution[server][jobs]+1);
            total += time[server][solution[server][jobs]];
        }
        printf("Custo: %.2lf", total);
        putchar('\n');
    }
    
}