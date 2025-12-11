#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include "Khoa.h"

using namespace std;

enum class UserType {
    STUDENT,
    TEACHER,
    ADMIN
};

int to_int(string s);

class User{
    protected:
        string Id;
        string Name;
        Khoa* khoa;
        UserType type; 
    public:
        User();
        User(string id, string name, Khoa* khoaPtr, UserType userType = UserType::STUDENT);
        virtual void displayInfo() const;
        virtual ~User();
        string getId() const { return Id; }
        string getName() const { return Name; }
        Khoa* getKhoa() const { return khoa; }
        void setKhoa(Khoa* khoaPtr) { khoa = khoaPtr; }
        UserType getUserType() const { return type; }
        virtual void showMenu() = 0;
};

#endif // USER_H