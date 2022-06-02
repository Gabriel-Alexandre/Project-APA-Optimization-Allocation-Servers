#include "Scenario.hpp"
#include <iostream>
#include <vector>

using namespace std;

Scenario::Scenario() {
    
}


void Scenario::generateSolution(Data *data) {

    servers = data->getServersCount();
    jobs = data->getJobsCount();
    spend = data->getC();
    time = data->getT();
    capacity = data->getServersCapacity();

    int i;

    for(i = 0; i < 3; i++) {
        
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

    // cout << i << endl;
    double totalFinalT = 0;
    double totalFinalC = 0;
    for(int server = 0; server < solution.size(); server++) {
        printf("Server %d\n", server+1);
        double total = 0;
        double totalCoast = 0;
        for(int jobs = 0; jobs < solution[server].size(); jobs++) {
            printf("%d ", solution[server][jobs]+1);
            total += time[server][solution[server][jobs]];
            totalCoast += spend[server][solution[server][jobs]];
        }
        printf("Tempo: %.2lf", total);
        printf("  Custo: %.2lf", totalCoast);
        putchar('\n');
        totalFinalT += total;
        totalFinalC += totalCoast;
    }

    printf("Total tempo: %.2lf\n", totalFinalT);
    printf("Total custo: %.2lf\n", totalFinalC);
    
}

vector<int> Scenario::caculateTotalCoastAndTimeServer(vector<vector<int>> solution) {
    vector<int> timesAndTotalCoast(this->servers+1);
    int totalFinal = 0;
    for(int server = 0; server < solution.size(); server++) {
        double total = 0;
        double totalCoast = 0;
        for(int jobs = 0; jobs < solution[server].size(); jobs++) {
            total += time[server][solution[server][jobs]];
            totalCoast += spend[server][solution[server][jobs]];
            timesAndTotalCoast[server] = total;
        }

        totalFinal += totalCoast;
    }

    timesAndTotalCoast[this->servers+1] = totalFinal;

    return timesAndTotalCoast;
}

vector<vector<int>> Scenario::swap(vector<vector<int>> solution) {
    vector<vector<int>> bestSolution = solution;

    vector<int> currentTime = caculateTotalCoastAndTimeServer(bestSolution);
    int minCoast = currentTime[this->servers+1];
    int minCoastTotal = minCoast;
    int bestServer = -1;
    int bestJob = -1;
    int bestServerSwap = -1;
    int bestJobSwap = -1;
    currentTime.erase(currentTime.begin()+this->servers);

    for(int server = 0; server < solution.size(); server++) {
        for(int jobs = 0; jobs < solution[server].size(); jobs++) {
            int auxCoast = 0;
            for(int serverSwap = server; serverSwap < solution.size(); serverSwap++) {
                for(int jobSwap = jobs + 1; jobSwap < solution[serverSwap].size(); jobSwap++) {
                    auxCoast = minCoast - spend[server][solution[server][jobs]] - spend[serverSwap][solution[serverSwap][jobSwap]]
                                + spend[server][solution[serverSwap][jobSwap]] + spend[serverSwap][solution[server][jobs]];

                    int vCapacity1 = (currentTime[server] - time[server][solution[server][jobs]] + time[server][solution[serverSwap][jobSwap]]);
                    int vCapacity2 = (currentTime[serverSwap] - time[serverSwap][solution[serverSwap][jobSwap]] + time[serverSwap][solution[server][jobs]]);

                    if ((auxCoast < minCoastTotal) && 
                    (vCapacity1 < capacity[server]) &&
                    (vCapacity2 < capacity[serverSwap])
                    ) {
                        minCoastTotal = auxCoast;

                        currentTime[server] = vCapacity1;
                        currentTime[serverSwap] = vCapacity2;

                        // cout << minCoastTotal << endl;
                        // cout << server << ' ' << jobs << endl;
                        // cout << serverSwap << ' ' << jobSwap << endl;
                        // cout << time[server][jobs] << ' ' << time[serverSwap][jobSwap] << endl;
                        // cout << time[server][jobSwap] << ' ' << time[serverSwap][jobs] << '\n' << endl;

                        // cout << vCapacity1 << ' ' << vCapacity2 << '\n' << endl;

                        bestServer = server;
                        bestJob = jobs;
                        bestServerSwap = serverSwap;
                        bestJobSwap = jobSwap;
                    }
                }
            }
        }
    }

    // cout << bestJob << ' ' << bestServer << ' ' <<  bestJobSwap << ' ' << bestServerSwap << ' ' << endl;

    if(bestServer != -1) {
        bestSolution[bestServer][bestJob] = solution[bestServerSwap][bestJobSwap];
        bestSolution[bestServerSwap][bestJobSwap] = solution[bestServer][bestJob];
    }

    cout << "\n\nsolução depois do swap: \n"<< endl;

    double totalFinalT = 0;
    double totalFinalC = 0;
    for(int server = 0; server < bestSolution.size(); server++) {
        printf("Server %d\n", server+1);
        double total = 0;
        double totalCoast = 0;
        for(int jobs = 0; jobs < bestSolution[server].size(); jobs++) {
            printf("%d ", bestSolution[server][jobs]+1);
            total += time[server][bestSolution[server][jobs]];
            totalCoast += spend[server][bestSolution[server][jobs]];
        }
        printf("Tempo: %.2lf", total);
        printf("  Custo: %.2lf", totalCoast);
        putchar('\n');
        totalFinalT += total;
        totalFinalC += totalCoast;
    }

    printf("Total tempo: %.2lf\n", totalFinalT);
    printf("Total custo: %.2lf\n", totalFinalC);

    return bestSolution;
    
}

void Scenario::printAux() {
    swap(this->solution);
}