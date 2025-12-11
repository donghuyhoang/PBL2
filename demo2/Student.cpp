#include "Student.h"

Student::Student() : User("", "", nullptr, UserType::STUDENT) {}

Student::Student(string id, string name, Khoa* khoa) 
    : User(id, name, khoa, UserType::STUDENT) {}

void Student::displayInfo() const {
    cout << "Sinh vien - ";
    User::displayInfo();
}

void Student::showMenu() {
    cout << "=== MENU SINH VIEN ===" << endl;
    cout << "1. Xem thong tin" << endl;
    cout << "2. Xem lop hoc phan" << endl;
    cout << "3. Xem diem" << endl;
    cout << "4. Thay doi mat khau" << endl;
    cout << "5. Dang xuat" << endl;
}

string Student::toString() const {
    return Id + "," + Name + "," + (khoa ? khoa->getMaKhoa() : "NULL");
}

Student::~Student() {}