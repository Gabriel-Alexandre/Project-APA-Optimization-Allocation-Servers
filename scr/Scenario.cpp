#include "Scenario.hpp"
#include <iostream>
#include <vector>



using namespace std;

Scenario::Scenario() {
    
}

void Scenario::construction(Data *data) {

    serversCount = data->getServersCount();
    jobsCount = data->getJobsCount();
    penality = data->getPenalityCount();
    cost = data->getC();
    time = data->getT();
    capacity = data->getServersCapacity();
    

    for(int i = 0; i < MAX_CONSTRUCTIONS; i++) {
        currentTimeByServer = vector<double>(serversCount);
        jobsNotAlloc =  vector<int>();
        this->objectiveValue = 0;
        
        int allocJobsCount = 0;
        vector<int> vectorJobs(jobsCount);
        solution = vector<vector<int>>(serversCount, vectorJobs);

        for(int i = 0; i < serversCount; i++) {
            for (int j = 0; j < jobsCount; j++) {
                solution[i][j] = 0;
            }
        }

        vector<int> candidates;
        for(int i = 0; i < jobsCount; i++) {
            candidates.push_back(i);
        }
        
        vector<double> auxCapacity = capacity;
        while(!candidates.empty()) {
            int i = rand() % candidates.size();
            int job = candidates[i];

            double aux;
            priority_queue<pair<double, int>> sortedServersCost;
            for (int server = 0; server < this->serversCount; server++) {
                switch(i) {
                    case 0:
                        aux = cost[server][job];
                        break;
                    case 1:
                        aux = cost[server][job]/time[server][job];
                        break;
                    case 2:
                        aux = time[server][job];
                        break;
                }

                sortedServersCost.push(make_pair(-aux, server));
            }
            
            bool serverAlloc = false;
            while(!sortedServersCost.empty()) {
                
                pair<double, int> values = sortedServersCost.top();
                double auxCost = values.first;
                int server = values.second;

                if(auxCapacity[server] - time[server][job] >= 0) {
                    auxCapacity[server] -= time[server][job];
                    solution[server][job] = 1;
                    allocJobsCount++;
                    currentTimeByServer[server] += time[server][job];
                    objectiveValue += cost[server][job];
                    serverAlloc = true;
                    break;
                }

                sortedServersCost.pop();
            }

            if(!serverAlloc) {
                //printf("Not alloc: %d\n", job+1);
                //getchar();
                jobsNotAlloc.push_back(job);
            }

            candidates.erase(candidates.begin() + i);
        }
        /*

        vector<double> auxCapacity = capacity;
        for (int job = 0; job < this->jobsCount; job++) {
            double aux;
            priority_queue<pair<double, int>> sortedServersCost;
            for (int server = 0; server < this->serversCount; server++) {
                switch(i) {
                    case 0:
                        aux = cost[server][job];
                        break;
                    case 1:
                        aux = cost[server][job]/time[server][job];
                        break;
                    case 2:
                        aux = time[server][job];
                        break;
                }

                sortedServersCost.push(make_pair(-aux, server));
            }
            
            bool serverAlloc = false;
            while(!sortedServersCost.empty()) {
                
                pair<double, int> values = sortedServersCost.top();
                double auxCost = values.first;
                int server = values.second;

                if(auxCapacity[server] - time[server][job] >= 0) {
                    auxCapacity[server] -= time[server][job];
                    solution[server][job] = 1;
                    allocJobsCount++;
                    currentTimeByServer[server] += time[server][job];
                    objectiveValue += cost[server][job];
                    serverAlloc = true;
                    break;
                }

                sortedServersCost.pop();
            }

            if(!serverAlloc) {
                //printf("Not alloc: %d\n", job+1);
                //getchar();
                jobsNotAlloc.push_back(job);
            }
        }
        */
        if(allocJobsCount == jobsCount) break;
    }

    

    objectiveValue += jobsNotAlloc.size() *penality;
    //printf("Custo: %.4lf\n", objectiveValue);
    //printAux();
}

bool Scenario::swap() {
    
    double bestCost = 0;
    int bestServer;
    int bestJob;
    int bestServerSwap;
    int bestJobSwap;
    double bestTime1;
    double bestTime2;

    for(int server = 0; server < serversCount; server++) {
        for(int job = 0; job < jobsCount; job++) {
            double auxCost = 0;
            double spendServerJobs = cost[server][job];
            double timeServerJobs = time[server][job];
            double currentTimeServer = currentTimeByServer[server];

            if(solution[server][job] == 1) {

                for(int serverSwap = 0; serverSwap < serversCount; serverSwap++) {
                    if (server == serverSwap) {
                        continue;
                    }

                    double spendServerSwapJob = cost[serverSwap][job];
                    double timeServerSwapJob = time[serverSwap][job];
                    double currentTimeServerSwap = currentTimeByServer[serverSwap];

                    for(int jobSwap = 0; jobSwap < jobsCount; jobSwap++) {
                        if (job == jobSwap) {
                            continue;
                        }

                        if(solution[serverSwap][jobSwap] == 1) {

                            auxCost = - spendServerJobs - cost[serverSwap][jobSwap]
                                        + cost[server][jobSwap] + spendServerSwapJob;

                            double newTime1 = (currentTimeServer - timeServerJobs + time[server][jobSwap]);
                            double newTime2 = (currentTimeServerSwap - time[serverSwap][jobSwap] + timeServerSwapJob);

                            if ((auxCost < bestCost) && 
                            (newTime1 <= capacity[server]) &&
                            (newTime2 <= capacity[serverSwap])
                            ) {
                                bestCost = auxCost;

                                bestServer = server;
                                bestJob = job;
                                bestServerSwap = serverSwap;
                                bestJobSwap = jobSwap;
                                bestTime1 = newTime1;
                                bestTime2 = newTime2;
                            }
                        }
                    }
                }
            }
        }
    }

    if(bestCost < 0) {
        solution[bestServer][bestJob] = 0;
        solution[bestServerSwap][bestJob] = 1;
        solution[bestServerSwap][bestJobSwap] = 0;
        solution[bestServer][bestJobSwap] = 1;
        objectiveValue += bestCost;
        currentTimeByServer[bestServer] = bestTime1;
        currentTimeByServer[bestServerSwap] = bestTime2;
        return true;
    }

    

    return false;
}

bool Scenario::reinsertion() {
    
    int minCoastTotal = 0;
    int bestServer;
    int bestJob;
    int bestServerR;

    for(int server = 0; server < serversCount; server++) {
        for(int job = 0; job < jobsCount; job++) {
            int spendServerJob = cost[server][job];
            
            if(solution[server][job] == 1) {
                for(int serverR = 0; serverR < serversCount; serverR++) {
                    if(serverR == server) {
                        continue;
                    }
                    double auxCost = - spendServerJob + cost[serverR][job];

                    double newTime = currentTimeByServer[serverR] + time[serverR][job];

                    if ((auxCost < minCoastTotal) && 
                        (newTime <= capacity[serverR]))
                    {
                        minCoastTotal = auxCost;
                        bestServer = server;
                        bestServerR = serverR;
                        bestJob = job;
                    }
                }

            }

        }
    }

    

    if(minCoastTotal < 0) {
        solution[bestServer][bestJob] = 0;
        solution[bestServerR][bestJob] = 1;
        objectiveValue += minCoastTotal;
        currentTimeByServer[bestServer] -= time[bestServer][bestJob];
        currentTimeByServer[bestServerR] += time[bestServerR][bestJob];
        return true;
    }

    return false;
}

bool Scenario::allocExternJobs() {

    int bestJob, bestServer, bestI;
    double bestCost = 0;
    for(int i = 0; i < jobsNotAlloc.size(); i++) {
        int job = jobsNotAlloc[i];
        for(int server = 0; server < serversCount; server++) {
            double auxCost = 0;
            double currentTime = currentTimeByServer[server];
            double newTime = currentTime + time[server][job];

           // printf("Capacity server %d: %.2lf\n", server+1, capacity[server]);
           // printf("NewTime: %.2lf\n", time[server][job]);
            if(newTime <= capacity[server]) {
                //printf("\n\nEntrou\n\n");
                auxCost = cost[server][job] - penality;

                if(auxCost < bestCost) {
                    bestJob = job;
                    bestServer = server;
                    bestCost = auxCost;
                    bestI = i;
                    
                }
            } else {
                //printf("\n\nNem entrou\n\n");
            }

        }
    }

    if(bestCost < 0) {
       // printf("\n\nFuncionou\n\n");
        solution[bestServer][bestJob] = 1;
        
        objectiveValue += bestCost;
        
        currentTimeByServer[bestServer] += time[bestServer][bestJob];
        jobsNotAlloc.erase(jobsNotAlloc.begin() + bestI);
        
        
       
        return true;
    }
   // printf("\n\nNao Funcionou\n\n");
    return false;
}

bool Scenario::swapNotAllocJobs() {

    int bestJob1, bestJob2, bestserver, bestI;
    double bestCost = 0, bestTime;

    for(int i = 0; i < jobsNotAlloc.size(); i++) {

        int job1 = jobsNotAlloc[i];

        for(int server = 0; server < serversCount; server++) {
            for(int job2 = 0; job2 < jobsCount; job2++) {
                
                if(job1 == job2) continue;

                if(solution[server][job2] == 1) {

                    double newTime = currentTimeByServer[server] + (time[server][job1] - time[server][job2]);

                    if(newTime < capacity[server]) {
                        double auxCost = cost[server][job1] - cost[server][job2];

                        if(auxCost < bestCost) {

                            bestCost = auxCost;
                            bestJob1 = job1;
                            bestJob2 = job2;
                            bestserver = server;
                            bestTime = newTime;
                            bestI = i;

                        }
                    }

                }


            }
        }
    }

    if(bestCost < 0) {
        objectiveValue += bestCost;
        solution[bestserver][bestJob1] = 1;
        solution[bestserver][bestJob2] = 0;
        jobsNotAlloc.erase(jobsNotAlloc.begin() + bestI);
        jobsNotAlloc.push_back(bestJob2);
        currentTimeByServer[bestserver] = bestTime;
        
        return true;
    }

    return false;
}

void Scenario::printAux() {
    double totalFinalT = 0;
    double totalFinalC = 0;
    int jobControl = 0;
    for(int server = 0; server < serversCount; server++) {
        //printf("Server %d\n", server+1);
        double totaltime = 0;
        double totalCoast = 0;
        for(int job = 0; job < jobsCount; job++) {
            if(solution[server][job] == 1) {
                //printf("%d ", job+1);
                totaltime += time[server][job];
                totalCoast += cost[server][job];
                jobControl++;
            }
        }
        printf("Tempo: %.2lf", totaltime);
        printf("  Custo: %.2lf", totalCoast);
        putchar('\n');
        totalFinalT += totaltime;
        totalFinalC += totalCoast;
    }

    if (jobControl < jobsCount) {
        totalFinalC = totalFinalC+ (jobsCount - jobControl) * penality;
    }

    printf("Total tempo: %.2lf\n", totalFinalT);
    printf("Total custo: %.2lf\n", totalFinalC);

    

}

double Scenario::getObjectiveValue() {
    return this->objectiveValue;
}

void Scenario::printNotAlloc() {
    for(int i = 0; i < jobsNotAlloc.size(); i++) {
        printf("Job %d\n", jobsNotAlloc[i]);
    }
}

void Scenario::printServersTime() {
    for(int i = 0; i < currentTimeByServer.size(); i++) {
        printf("Server %d: %.2lf\n", i+1, currentTimeByServer[i]);
    }
}

void Scenario::ILS() {
    int n = 1;
    while(n--) {

        int server = rand() % serversCount;

        for(int job = 0; job < jobsCount; job++) {

            if(solution[server][job]) {

                solution[server][job] = 0;
                objectiveValue += (penality - cost[server][job]);
                jobsNotAlloc.push_back(job);
                currentTimeByServer[server] -= time[server][job];

            }

        }
    }
}

void Scenario::printSolution() {

    for(int server = 0; server < solution.size(); server++) {
        for(int job = 0; job < solution[server].size(); job++) {
            printf("%d ", solution[server][job]);
        }
        putchar('\n');
    }


}