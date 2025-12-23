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
    cout << "1. Xem thong tin" << endl;
    cout << "2. Xem va sua diem" << endl;
    cout << "3. Thay doi mat khau" << endl;
    cout << "4. Dang xuat" << endl;
}

string Teacher::toString() const {
    return Id + "," + Name + "," + (khoa ? khoa->getMaKhoa() : "NULL");
}

Teacher::~Teacher() {}

