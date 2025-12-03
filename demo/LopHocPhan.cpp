#include "LopHocPhan.h"

LopHocPhan::LopHocPhan() : Malophp(""), giangvien(nullptr), monhoc(nullptr) {}

LopHocPhan::LopHocPhan(string malophp, Teacher* gv, MonHoc* mh) 
    : Malophp(malophp), giangvien(gv), monhoc(mh) {}

void LopHocPhan::displayInfo() const {
    cout << "Lop HP: " << Malophp;
    if (monhoc) {
        cout << " - Mon: " << monhoc->getTenMon();
    }
    if (giangvien) {
        cout << " - GV: " << giangvien->getName();
    }
    cout << endl;
}
string LopHocPhan::toString() const {
    return Malophp + "," + 
           (monhoc ? monhoc->getMaMon() : "NULL") + "," +
           (giangvien ? giangvien->getKey() : "NULL");
}
string LopHocPhan::getMaLopHP() const {
    return Malophp;
}

Teacher* LopHocPhan::getGiangVien() const {
    return giangvien;
}

MonHoc* LopHocPhan::getMonHoc() const {
    return monhoc;
}

string LopHocPhan::getKey() const {
    return Malophp;
}

LopHocPhan::~LopHocPhan() {}