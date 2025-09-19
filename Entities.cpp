#include "Entities.h"
// Student class
Student::Student() : User(){
    this->Mk = "NULL";
    this->Status = "NULL";
}
Student::Student(const string& id, const string &name, const string &mk,const string &status)
: User(id,name),Mk(mk),Status(status){}
Student::~Student(){};
void Student::show_info(){
    cout << "ID: " << this->Id << ", Name: " << this->Name << ", Ma Khoa:" << this->Mk << ", Status: " << this->Status << endl;
}

// Teacher class
Teacher::Teacher() : User(){
    this->Mk = "NULL";
    this->Status = "NULL";
}
Teacher::Teacher(const string& id, const string &name, const string &mk,const string &status)
: User(id,name),Mk(mk),Status(status){}
Teacher::~Teacher(){};
void Teacher::show_info(){
    cout << "ID: " << this->Id << ", Name: " << this->Name << ", Ma Khoa:" << this->Mk << ", Status: " << this->Status << endl;
}

// Admin class
Admin::Admin() : User(){}
Admin::Admin(const string& id, const string &name) : User(id,name){}
Admin::~Admin(){};
void Admin::show_info(){
    cout << "ID: " << this->Id << ", Name: " << this->Name << endl;
}
// Khoa class
Khoa::Khoa(){
    this->Id = "NULL";
    this->Name = "NULL";
}
Khoa::Khoa(const string& id, const string &name)
: Id(id),Name(name){}
Khoa::~Khoa(){};
void Khoa::show_info(){
    cout << "ID: " << this->Id << ", Name: " << this->Name << endl;
}
// MonHoc class
MonHoc::MonHoc(){
    this->Id = "NULL";
    this->Name = "NULL";
    this->SoTinChi = 0;
}
MonHoc::MonHoc(const string& id, const string &name, int so_tin_chi)
: Id(id),Name(name),SoTinChi(so_tin_chi){}
MonHoc::~MonHoc(){};
void MonHoc::show_info(){
    cout << "ID: " << this->Id << ", Name: " << this->Name << ", So Tin Chi: " << this->SoTinChi << endl;
}
// LopHocPhan class
LopHocPhan::LopHocPhan(){
    this->Id = "NULL";
    this->MaGiangVien = "NULL";
    this->MaMonHoc = "NULL";
}
LopHocPhan::LopHocPhan(const string& id, const string &ma_giang_vien, const string &ma_mon_hoc)
: Id(id),MaGiangVien(ma_giang_vien),MaMonHoc(ma_mon_hoc){}
LopHocPhan::~LopHocPhan(){};
void LopHocPhan::show_info(){
    cout << "ID: " << this->Id << ", Ma Giang Vien: " << this->MaGiangVien << ", Ma Mon Hoc: " << this->MaMonHoc << endl;
}
// DangKy class
DangKy::DangKy(){
    this->MaLopHocPhan = "NULL";
    this->MaSinhVien = "NULL";
    this->Diem = -1.0;
}
DangKy::DangKy(const string& ma_lop_hoc_phan, const string &ma_sinh_vien, float diem)
: MaLopHocPhan(ma_lop_hoc_phan),MaSinhVien(ma_sinh_vien),Diem(diem){}
DangKy::~DangKy(){};
void DangKy::show_info(){
    cout << "Ma Lop Hoc Phan: " << this->MaLopHocPhan << ", Ma Sinh Vien: " << this->MaSinhVien << ", Diem: " << this->Diem << endl;
}