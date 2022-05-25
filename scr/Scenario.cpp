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

void Scenario::printSolution() {
    cout << "Final solution:" << endl;

    int total[this->servers];
    int sum = 0;

    for(int i = 0; i < this->servers; i++) {
        total[i] = 0;
    }

    cout << "\nSpend:\n" << endl;

    for(vector <int> i: this->finalSolutionSpend) {
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

    for(vector <int> i: this->finalSolutionTime) {
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

    // Preenchendo vetor solution
    for (int i = 0; i < this->servers; i++) {
        vector <float> aux;

        for (int j = 0; j < this->jobs; j++) {
            aux.push_back(this->time[i][j]/(this->spend[i][j]*1.0));
        }

        this->solution.push_back(aux);
    }

    // DEBUG para visualizar vetor solution
    // for (vector<float> x: this->solution) {
    //     for(float i: x) {
    //         cout << i << ' ';
    //     }
    //     cout << endl;
    // }
    
    int totalTime[this->servers]; // var que auxilia no crontrole da capacidade dos serv

    for(int i = 0; i < this->servers; i++) {
        totalTime[i] = 0;
    }

    // alocando serv levando em consideracao a capacidade de cada um e a relacao 'time/spend' armazenada em solution
    for (int i = 0; i < this->jobs; i++) {
        float min = 1000000; // relacao 'time/spend' minima
        int indice; // indice do servidor com relacao 'time/spend' minima
        int control = 0; // controle caso nenhum servidor tenha capacidade de alocar o job
        vector<int> aux;

        // VERIFICAR: essas condições podem estar redundantes
        for (int j = 0; j < this->servers; j++) {
            // iniciando a contagem da capacidade do primeiro job
            // apos passar por todos os serv a variavel "min" tera a relacao 'time/spend' minima para o primeiro job, e a
            // variavel "indice" tera o indice do servidor com relacao 'time/spend' minima
            if ((i == 0) && (this->time[j][i] <= this->capacity[j])) {
                if (this->solution[j][i] < min) {
                    min = this->solution[j][i];
                    indice = j;
                }
            // caso nenhum servidor tenha capacidade suficiente para alocar o job "control" sera igual a numero de serv
            } else if (i == 0) {
                control++;
            }
            
            // para o restante dos jobs a contagem da capacidade já foi iniciada
            // apos passar por todos os serv a variavel "min" tera a relacao 'time/spend' minima para o primeiro job, e a
            // variavel "indice" tera o indice do servidor com relacao 'time/spend' minima
            if ((i > 0) && (this->solution[j][i] < min) && ((totalTime[j] + this->time[j][i]) <= this->capacity[j])) {
                min = this->solution[j][i];
                indice = j;
            // caso nenhum servidor tenha capacidade suficiente para alocar o job "control" sera igual a numero de serv
            } else if (i > 0) {
                control++;
            }
        }
        
        // "control < this->servers" significa que pelo menos 1 serv tem capacidade de alocar o job
        // caso nenhum serv tenha capacidade de alocar o job, o job deixa de ser alocador
        if (control < this->servers) {
            // somando o tempo do serv escolhido a soma que ajuda no controle de capacidade de cada servidor
            totalTime[indice] += this->time[indice][i];

            // adicionando servidor (indice) e custo (this->spend[indice][i]) ao vetor "finalSolutionSpend" que
            // armazena o custo de acordo com servidor escolhido
            aux.push_back(indice);
            aux.push_back(this->spend[indice][i]);

            this->finalSolutionSpend.push_back(aux);

            // adicionando servidor (indice) e tempo (this->time[indice][i]) ao vetor "finalSolutionTime" que
            // armazena o tempo de acordo com servidor escolhido

            aux.pop_back();
            aux.push_back(this->time[indice][i]);

            this->finalSolutionTime.push_back(aux);

            // A solucao final esta nos vetores "finalSolutionSpend" e "finalSolutionTime", pois eles armazenam os servidores
            // que foram escolhidos para tratar cada job.

            // Armazenei em dois vetores diferentes para na funcao "printSolution" conseguir mostrar as estatisticas dos dados
            // de uma maneira mais efetiva.
        }
    }
}