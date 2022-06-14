#include "Data.hpp"

int Data::read(string fileName) {

    ifstream file;
    file.open(fileName, ios::in);

    if(!file.is_open()) return 0;

    int n, m;

    file >> n;
    file >> m;
    file >> this->p;

    serversCapacity = vector<double>(m);
    for(int server = 0; server < m; server++) {
        file >> serversCapacity[server];
    }


    t = vector<vector<double>>(m);
    for(int server = 0; server < m; server++) {
        t[server] = vector<double>(n);
        for(int job = 0; job < n; job++) {
            file >> t[server][job];
        }    
    }
    
    c = vector<vector<double>>(m);
    for(int server = 0; server < m; server++) {
        c[server] = vector<double>(n);
        for(int job = 0; job < n; job++) {
            file >> c[server][job];
        }    
    }

    file.close();

    return 1;
}

int Data::getJobsCount() {
    return t[0].size();
}
int Data::getServersCount() {
    return t.size();
}

int Data::getPenalityCount() {
    return this->p;
}

void Data::printCost() {
    for (vector <double> v: this->c) {
        for (double i: v) {
            cout << i << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void Data::printTime() {
    for (vector <double> v: this->t) {
        for (double i: v) {
            cout << i << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void Data::printCapacity() {
    for (double i: this->serversCapacity) {
        cout << i << ' ';
    }

    cout << "\n\n";
}

void Data::printData() {

    cout << getJobsCount() << '\n' << getServersCount() << "\n\n";

    this->printCapacity();
    this->printTime();
    this->printCost();
}

vector<vector<double>> Data::getT() {
    return t;
}

vector<vector<double>> Data::getC() {
    return c;
}

vector<double> Data::getServersCapacity() {
    return this->serversCapacity;
}