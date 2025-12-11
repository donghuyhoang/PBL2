#include <iostream>
#include <sstream>
#include <iomanip>
#pragma once
using namespace std;


class DangKy{
    private:
        string MaLHP_fk;
        string MaSV_fk;
        float Diem;
    public:
        DangKy();
        DangKy(string malhp, string masv, float diem);
        string getMaLHP_FK() const { return MaLHP_fk; }
        string getMaSV_FK() const { return MaSV_fk; }
        void setDiem(float diem) { 
            if (diem >= 0.0f && diem <= 10.0f) {
                Diem = diem; 
            }
        }
        float getDiem() const { return Diem; }     
        void displayInfo() const;
        string getKey() const { return MaLHP_fk + "-" + MaSV_fk; }
        
        string toString() const {
            stringstream ss;
            ss << MaLHP_fk << "," << MaSV_fk << "," << fixed << setprecision(2) << Diem;
            return ss.str();
        }
        
        virtual ~DangKy();
};