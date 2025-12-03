#include "Teacher.h"

Teacher::Teacher() : User("", "", nullptr, UserType::TEACHER) {}

Teacher::Teacher(string id, string name, Khoa* khoa) 
    : User(id, name, khoa, UserType::TEACHER) {}

void Teacher::displayInfo() const {
    cout << "Giang vien - ";
    User::displayInfo();
}

void Teacher::showMenu() {
    cout << "=== MENU GIANG VIEN ===" << endl;
    cout << "1. Xem thong tin ca nhan" << endl;
    cout << "2. Xem lop hoc phan dang day" << endl;
    cout << "3. Xem va sua diem sinh vien" << endl;
    cout << "4. Thay doi mat khau" << endl;
    cout << "5. Dang xuat" << endl;
}

string Teacher::toString() const {
    return Id + "," + Name + "," + (khoa ? khoa->getMaKhoa() : "NULL");
}

Teacher::~Teacher() {}