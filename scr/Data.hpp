#ifndef DATA_HPP
#define DATA_HPP

#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

class Data {    
    private:
        vector<double> serversCapacity;
        vector<vector<double>> t;
        vector<vector<double>> c;
        void printCost();
        void printTime();
        void printCapacity();
    public:
        int read(string fileName);
        int getJobsCount();
        int getServersCount();
        void printData();
        vector<vector<double>> getT();
        vector<vector<double>> getC();
        vector<double> getServersCapacity();
};

#endif