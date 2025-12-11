#include "Account.h"   
Account::Account() : Username(""), Password(""), user(nullptr) {}

Account::Account(string username, string password, User* userPtr) 
    : Username(username), Password(password), user(userPtr) {}

void Account::displayInfo() const {
    cout << "Tai khoan: " << Username;
    if (user) {
        cout << " - User: " << user->getName();
        switch (user->getUserType()) {
            case UserType::STUDENT: cout << " (Sinh vien)"; break;
            case UserType::TEACHER: cout << " (Giang vien)"; break;
            case UserType::ADMIN: cout << " (Admin)"; break;
        }
    }
    cout << endl;
}

string Account::toString() const {
    string userId = user ? user->getId() : "NULL";
    string userType;
    
    if (user) {
        switch (user->getUserType()) {
            case UserType::STUDENT: userType = "STUDENT"; break;
            case UserType::TEACHER: userType = "TEACHER"; break;
            case UserType::ADMIN: userType = "ADMIN"; break;
            default: userType = "UNKNOWN"; break;
        }
    } else {
        userType = "NULL";
    }
    
    return Username + "," + Password + "," + userId + "," + userType;
}

Account::~Account() {}