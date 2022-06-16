#include "ServerJobOpt.hpp"
#include <iostream>
#include <vector>

using namespace std;

ServerJobOpt::ServerJobOpt() {}

ServerJobOpt::ServerJobOpt(Data *data) {
    serversCount = data->getServersCount();
    jobsCount = data->getJobsCount();
    penality = data->getPenalityCount();
    cost = data->getC();
    time = data->getT();
    capacity = data->getServersCapacity();
}

double ServerJobOpt::construction() {

    currentTimeByServer = vector<double>(serversCount);
    jobsNotAlloc =  vector<int>();
    objectiveValue = 0;
    int allocJobsCount = 0;
    vector<int> vectorJobs(jobsCount);
    solution = vector<vector<int>>(serversCount, vectorJobs);

    for(int i = 0; i < serversCount; i++) {
        for (int j = 0; j < jobsCount; j++) {
            solution[i][j] = 0;
        }
    }

    // inclusão dos jobs no vetor de candidatos
    vector<int> candidates(jobsCount);
    std::iota(begin(candidates), end(candidates), 0);


    vector<double> auxCapacity = capacity;
    // alocação dos candidatos
    while(!candidates.empty()) {
        // um dos jobs é escolhido aleatoriamente

        int i = rand() % candidates.size();
        int job = candidates[i];

        // os servidores são adicionados à fila de prioridade em ordem crescente de custo para o job escolhido
        priority_queue<pair<double, int>> sortedServersCost;
        for (int server = 0; server < this->serversCount; server++) {
            double aux = cost[server][job];
            sortedServersCost.push(make_pair(-aux, server));
        }
        
        bool isJobAlloc = false;

        // um dos servidores é escolhido aleatoriamente na fila de prioridade
        int choose = rand() % sortedServersCost.size() ;
        while(choose--) {
            sortedServersCost.pop();
        }

        pair<double, int> values = sortedServersCost.top();
        double auxCost = values.first;
        int server = values.second;

        //tenta-se alocar o job ao servidor escolhido
        if(auxCapacity[server] - time[server][job] >= 0) {
            auxCapacity[server] -= time[server][job];
            solution[server][job] = 1;
            allocJobsCount++;
            currentTimeByServer[server] += time[server][job];
            objectiveValue += cost[server][job];
            isJobAlloc = true;
        } else {
            // caso não seja possível alocar o job ao servidor, ele não é alocado
            jobsNotAlloc.push_back(job);
        }
        candidates.erase(candidates.begin() + i);
    }
    
    // a penalidade dos jobs que ficaram fora da solução é adicionada ao custo total
    objectiveValue += jobsNotAlloc.size() *penality;
   
   return objectiveValue;
}

// swap realizado somente entre os jobs alocados
bool ServerJobOpt::swap() {
    
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
            double cost1 = cost[server][job];
            double timeServerJobs = time[server][job];
            double currentTimeServer = currentTimeByServer[server];

            if(solution[server][job]) {

                for(int serverSwap = 0; serverSwap < serversCount; serverSwap++) {
                    if (server == serverSwap) {
                        continue;
                    }

                    double cost2 = cost[serverSwap][job];
                    double timeServerSwapJob = time[serverSwap][job];
                    double currentTimeServerSwap = currentTimeByServer[serverSwap];

                    for(int jobSwap = 0; jobSwap < jobsCount; jobSwap++) {
                        if (job == jobSwap) {
                            continue;
                        }

                        if(solution[serverSwap][jobSwap]) {

                            auxCost = - cost1 - cost[serverSwap][jobSwap]
                                        + cost[server][jobSwap] + cost2;

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

//reinsertion realizado somente nos jobs alocados
bool ServerJobOpt::reinsertion() {
    
    double bestCost = 0;
    int bestServer;
    int bestJob;
    int bestServerR;

    for(int server = 0; server < serversCount; server++) {
        for(int job = 0; job < jobsCount; job++) {
            double cost1 = cost[server][job];
            
            if(solution[server][job]) {
                for(int serverR = 0; serverR < serversCount; serverR++) {
                    
                    if(serverR == server) continue;

                    double auxCost = - cost1 + cost[serverR][job];

                    double newTime = currentTimeByServer[serverR] + time[serverR][job];

                    if ((auxCost < bestCost) && 
                        (newTime <= capacity[serverR]))
                    {
                        bestCost = auxCost;
                        bestServer = server;
                        bestServerR = serverR;
                        bestJob = job;
                    }
                }
            }
        }
    }

    if(bestCost < 0) {
        solution[bestServer][bestJob] = 0;
        solution[bestServerR][bestJob] = 1;
        objectiveValue += bestCost;
        currentTimeByServer[bestServer] -= time[bestServer][bestJob];
        currentTimeByServer[bestServerR] += time[bestServerR][bestJob];
        return true;
    }

    return false;
}

// reinsertion realizado com os jobs que não estão alocados a nenhum servidor
bool ServerJobOpt::allocExternJobs() {

    int bestJob, bestServer, bestI;
    double bestCost = 0;
    for(int i = 0; i < jobsNotAlloc.size(); i++) {
        int job = jobsNotAlloc[i];
        for(int server = 0; server < serversCount; server++) {
            double auxCost = 0;
            double currentTime = currentTimeByServer[server];
            double newTime = currentTime + time[server][job];

            if(newTime <= capacity[server]) {
                auxCost = cost[server][job] - penality;

                if(auxCost < bestCost) {
                    bestJob = job;
                    bestServer = server;
                    bestCost = auxCost;
                    bestI = i;
                    
                }
            }
        }
    }

    if(bestCost < 0) {
       
        solution[bestServer][bestJob] = 1;
        
        objectiveValue += bestCost;
        
        currentTimeByServer[bestServer] += time[bestServer][bestJob];
        jobsNotAlloc.erase(jobsNotAlloc.begin() + bestI);
        
        
       
        return true;
    }
    
    return false;
}

// swap realizado entre um job não alocado com um job alocado
bool ServerJobOpt::swapNotAllocJobs() {

    int bestJob1, bestJob2, bestserver, bestI;
    double bestCost = 0, bestTime;

    for(int i = 0; i < jobsNotAlloc.size(); i++) {

        int job1 = jobsNotAlloc[i];

        for(int server = 0; server < serversCount; server++) {
            double currentTime = currentTimeByServer[server];
            double serverCapacity = capacity[server];
            double cost1 = cost[server][job1];
            double time1 = time[server][job1];
            for(int job2 = 0; job2 < jobsCount; job2++) {
                
                if(job1 == job2) continue;

                if(solution[server][job2]) {

                    double newTime = currentTime + (time1 - time[server][job2]);

                    if(newTime <= serverCapacity) {
                        double auxCost = cost1 - cost[server][job2];

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

double ServerJobOpt::getObjectiveValue() {
    return this->objectiveValue;
}

// um dos servidores é escolhido aleatoriamente e seus jobs são removidos
void ServerJobOpt::ILS() {

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

void ServerJobOpt::updateSolution(ServerJobOpt *opt) {
    
        this->jobsNotAlloc = opt->getNotAllocJobs();
        this->currentTimeByServer = opt->getCurrentTimeByServer();
        this->solution = opt->getSolution();
        this->objectiveValue = opt->getObjectiveValue();

}

vector<int> ServerJobOpt::getNotAllocJobs() {
    return jobsNotAlloc;
}

vector<double> ServerJobOpt::getCurrentTimeByServer() {
    return currentTimeByServer;
}

vector<vector<int>> ServerJobOpt::getSolution() {
    return this->solution;
}

int ServerJobOpt::getJobsCoutn() {
    return this->jobsCount;
}