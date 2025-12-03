#include <iostream>
#include <string>
#include <sstream>
#include "User.h"
#pragma once
using namespace std;


class Student : public User {
    public:
        Student();
        Student(string id, string name, Khoa* khoa);
        void displayInfo() const override;
        void showMenu() override;
        string getKey() const { return Id; }
        string toString() const;
        virtual ~Student();
};