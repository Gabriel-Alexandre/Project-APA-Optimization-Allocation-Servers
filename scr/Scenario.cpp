#include "Scenario.hpp"
#include <iostream>
#include <vector>

using namespace std;

Scenario::Scenario() {
    
}

void Scenario::generateSolution(Data *data) {

    servers = data->getServersCount();
    jobs = data->getJobsCount();
    penality = data->getPenalityCount();
    spend = data->getC();
    time = data->getT();
    capacity = data->getServersCapacity();

    for(int i = 0; i < 3; i++) {
        
        int allocJobsCount = 0;
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
                    solution[server][job] = 1;
                    allocJobsCount++;
                    break;
                }

                sortedServersCost.pop();
            }
        }

        if(allocJobsCount == jobs) break;
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
        totalFinalC = totalFinalC+ (jobs - jobControl) * penality;
    }

    printf("Total tempo: %.2lf\n", totalFinalT);
    printf("Total custo: %.2lf\n", totalFinalC);
}

// Fiz isso só para teste
void Scenario::generateSolution2(Data *data) {

    servers = data->getServersCount();
    jobs = data->getJobsCount();
    penality = data->getPenalityCount();
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
        totalFinalC = totalFinalC+ (jobs - jobControl) * penality;
    }

    printf("Total tempo: %.2lf\n", totalFinalT);
    printf("Total custo: %.2lf\n", totalFinalC);
}

vector<int> Scenario::caculateTotalCoastAndTimeServer(vector<vector<int>> solution) {
    vector<int> timesAndTotalCoast(this->servers);

    for(int server = 0; server < servers; server++) {
        int total = 0;
        for(int job = 0; job < jobs; job++) {
            if(solution[server][job] == 1) {
                total += time[server][job];
            }
        }
        timesAndTotalCoast[server] = total;
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

    for(int server = 0; server < servers; server++) {
        for(int job = 0; job < jobs; job++) {
            int auxCoast = 0;
            int spendServerJobs = spend[server][job];
            int timeServerJobs = time[server][job];
            int currentTimeServer = currentTime[server];

            for(int serverSwap = server + 1; serverSwap < servers; serverSwap++) {
                int spendServerSwapJob = spend[serverSwap][job];
                int timeServerSwapJob = time[serverSwap][job];
                int currentTimeServerSwap = currentTime[serverSwap];

                for(int jobSwap = job + 1; jobSwap < jobs; jobSwap++) {
                    if(solution[server][job] == 1 && solution[serverSwap][jobSwap] == 1) {

                        auxCoast = - spendServerJobs - spend[serverSwap][jobSwap]
                                    + spend[server][jobSwap] + spendServerSwapJob;

                        int vCapacity1 = (currentTimeServer - timeServerJobs + time[server][jobSwap]);
                        int vCapacity2 = (currentTimeServerSwap - time[serverSwap][jobSwap] + timeServerSwapJob);

                        if ((auxCoast < minCoastTotal) && 
                        (vCapacity1 < capacity[server]) &&
                        (vCapacity2 < capacity[serverSwap])
                        ) {
                            minCoastTotal = auxCoast;

                            bestServer = server;
                            bestJob = job;
                            bestServerSwap = serverSwap;
                            bestJobSwap = jobSwap;
                        }
                    }
                }
            }
        }
    }

    if(minCoastTotal < 0) {
        printf("%d", solution[bestServer][bestJob]);
        solution[bestServer][bestJob] = 0;
        solution[bestServerSwap][bestJob] = 1;
        solution[bestServerSwap][bestJobSwap] = 0;
        solution[bestServer][bestJobSwap] = 1;
    }

    cout << "\n\nsolução depois do swap: \n"<< endl;

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
        totalFinalC = totalFinalC+ (jobs - jobControl) * penality;
    }

    printf("Total tempo: %.2lf\n", totalFinalT);
    printf("Total custo: %.2lf\n", totalFinalC);

    if(minCoastTotal < 0) {
        return true;
    } else {
        return false;
    }
    
}