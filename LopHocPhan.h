#include <iostream>
#include <string>
#include <sstream>
#include "teacher.h"
#include "Monhoc.h"
#pragma once
using namespace std;


class LopHocPhan{
    private:
        string Malophp;
        Teacher* giangvien;
        MonHoc* monhoc;
    public:
        LopHocPhan();
        LopHocPhan(string , Teacher* , MonHoc* );
        void displayInfo() const;
        string getMaLopHP() const;
        Teacher* getGiangVien() const;
        MonHoc* getMonHoc() const;
        string getKey() const ;
        string toString() const;
        virtual ~LopHocPhan();
};
