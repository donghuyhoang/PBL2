#include "User.h"
#include <iostream>
#include <string>
using namespace std;

User::User(){
    Id = "";
    Name = "";
    khoa = nullptr;
    type = UserType::STUDENT;
}

User::User(string id, string name, Khoa* khoaPtr, UserType userType) 
    : Id(id), Name(name), khoa(khoaPtr), type(userType) {}

User::~User() {};

void User::displayInfo() const {
    cout << "ID: " << Id << " - Name: " << Name;
    if (khoa != nullptr) {
        cout << " - Khoa: " << khoa->getTenKhoa();
    }
}