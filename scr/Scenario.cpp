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

    vector<int> vectorJobs(jobs);
    solution = vector<vector<int>>(servers, vectorJobs);

    for(int i = 0; i < servers; i++) {
        for (int j = 0; j < jobs; j++) {
            solution[i][j] = 0;
        }
    }

    vector<double> auxCapacity = capacity;
    for (int job = 0; job < this->jobs; job++) {
        double aux;
        priority_queue<pair<double, int>> sortedServersCost;
        for (int server = 0; server < this->servers; server++) {

            aux = spend[server][job];

            sortedServersCost.push(make_pair(-aux, server));
        }
        
        while(!sortedServersCost.empty()) {
            
            pair<double, int> values = sortedServersCost.top();
            double cost = values.first;
            int server = values.second;

            if(auxCapacity[server] - time[server][job] >= 0) {
                auxCapacity[server] -= time[server][job];
                solution[server][job] = 1;
                break;
            }

            sortedServersCost.pop();
        }
    }

    // for(int i = 0; i < servers; i++) {
    //     for (int j = 0; j < jobs; j++) {
    //         cout << solution[i][j] << ' ';
    //     }
    //     cout << endl;
    // }

    // cout << i << endl;

    double totalFinalT = 0;
    double totalFinalC = 0;
    int jobControl = 0;
    for(int server = 0; server < servers; server++) {
        printf("Server %d\n", server+1);
        double totaltime = 0;
        double totalCoast = 0;
        for(int job = 0; job < jobs; job++) {
            if(solution[server][job] == 1) {
                printf("%d ", job+1);
                totaltime += time[server][job];
                totalCoast += spend[server][job];
                jobControl++;
            }
        }
        printf("Tempo: %.2lf", totaltime);
        printf("  Custo: %.2lf", totalCoast);
        putchar('\n');
        totalFinalT += totaltime;
        totalFinalC += totalCoast;
    }

    if (jobControl < jobs) {
        totalFinalC = totalFinalC+ (jobs - jobControl) * 1000;
    }

    printf("Total tempo: %.2lf\n", totalFinalT);
    printf("Total custo: %.2lf\n", totalFinalC);
}

vector<int> Scenario::caculateTotalCoastAndTimeServer(vector<vector<int>> solution) {
    vector<int> timesAndTotalCoast(this->servers);
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

    return timesAndTotalCoast;
}

bool Scenario::swap() {
    vector<int> currentTime = caculateTotalCoastAndTimeServer(solution);
    int minCoastTotal = 0;
    int bestServer;
    int bestJob;
    int bestServerSwap;
    int bestJobSwap;

    for(int server = 0; server < solution.size(); server++) {
        for(int jobs = 0; jobs < solution[server].size(); jobs++) {
            int auxCoast = 0;
            int spendServerJobs = spend[server][solution[server][jobs]];
            int timeServerJobs = time[server][solution[server][jobs]];
            int currentTimeServer = currentTime[server];

            for(int serverSwap = server; serverSwap < solution.size(); serverSwap++) {
                int spendServerSwapJob = spend[serverSwap][solution[server][jobs]];
                int timeServerSwapJob = time[serverSwap][solution[server][jobs]];
                int currentTimeServerSwap = currentTime[serverSwap];

                for(int jobSwap = jobs + 1; jobSwap < solution[serverSwap].size(); jobSwap++) {
                    auxCoast = - spendServerJobs - spend[serverSwap][solution[serverSwap][jobSwap]]
                                + spend[server][solution[serverSwap][jobSwap]] + spendServerSwapJob;

                    int vCapacity1 = (currentTimeServer - timeServerJobs + time[server][solution[serverSwap][jobSwap]]);
                    int vCapacity2 = (currentTimeServerSwap - time[serverSwap][solution[serverSwap][jobSwap]] + timeServerSwapJob);

                    if ((auxCoast < minCoastTotal) && 
                    (vCapacity1 < capacity[server]) &&
                    (vCapacity2 < capacity[serverSwap])
                    ) {
                        minCoastTotal = auxCoast;

                        bestServer = server;
                        bestJob = jobs;
                        bestServerSwap = serverSwap;
                        bestJobSwap = jobSwap;
                    }
                }
            }
        }
    }

    if(minCoastTotal < 0) {
        int aux = solution[bestServer][bestJob];
        solution[bestServer][bestJob] = solution[bestServerSwap][bestJobSwap];
        solution[bestServerSwap][bestJobSwap] = aux;
    }

    cout << "\n\nsolução depois do swap: \n"<< endl;

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

    if(minCoastTotal < 0) {
        return true;
    } else {
        return false;
    }
    
}