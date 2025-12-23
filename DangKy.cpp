
#include "DangKy.h"

DangKy::DangKy() : MaLHP_fk(""), MaSV_fk(""), Diem(0.0f) {}

DangKy::DangKy(string malhp, string masv, float diem) 
    : MaLHP_fk(malhp), MaSV_fk(masv), Diem(diem) {}

void DangKy::displayInfo() const {
    cout << "Dang ky - LHP: " << MaLHP_fk << " - SV: " << MaSV_fk 
         << " - Diem: " << fixed << setprecision(2) << Diem << endl;
}

DangKy::~DangKy() {}

