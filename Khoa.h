#ifndef KHOA_H
#define KHOA_H

#include <iostream>
#include <string>
using namespace std;

class Khoa {
private:
    string Makhoa;
    string Tenkhoa;
    
public:
    Khoa() : Makhoa(""), Tenkhoa("") {}
    Khoa(string makhoa, string tenkhoa) : Makhoa(makhoa), Tenkhoa(tenkhoa) {}
    
    string getMaKhoa() const { return Makhoa; }
    string getTenKhoa() const { return Tenkhoa; }
    void setTenKhoa(const string& tenkhoa) { Tenkhoa = tenkhoa; }
    
    void displayInfo() const {
        cout << "Ma khoa: " << Makhoa << " - Ten khoa: " << Tenkhoa;
    }
    
    string toString() const {
        return Makhoa + "," + Tenkhoa;
    }
    
    string getKey() const {
        return Makhoa;
    }
    
    virtual ~Khoa() {}
};

#endif // KHOA_H