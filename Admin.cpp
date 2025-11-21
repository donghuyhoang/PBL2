#include "Admin.h"

Admin::Admin() : User("", "", nullptr, UserType::ADMIN) {}

void Admin::displayInfo() const {
    cout << "Admin - ";
    User::displayInfo();
}

void Admin::showMenu() {
    cout << "=== MENU ADMIN ===" << endl;
    cout << "1. Quan ly sinh vien" << endl;
    cout << "2. Quan ly giang vien" << endl;
    cout << "3. Quan ly mon hoc" << endl;
    cout << "4. Quan ly lop hoc phan" << endl;
    cout << "5. Quan ly dang ky" << endl;
    cout << "6. Quan ly tai khoan" << endl;
    cout << "7. Doi mat khau" << endl;
    cout << "8. Dang xuat" << endl;
}

Admin::~Admin() {}

