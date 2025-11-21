#include <iostream>
#include <string>
#include <sstream>
#pragma once
using namespace std;

class MonHoc{
    private:
        string Mamon;
        string Tenmon;
    public:
        MonHoc();
        MonHoc(string , string ) ;
        void displayInfo() const;
        string getMaMon() const ;
        string getTenMon() const ;
        string getKey() const ;
        string toString() const ;
        virtual ~MonHoc();
};