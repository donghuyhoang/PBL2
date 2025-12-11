#include <iostream>
#include "User.h"
#pragma once
using namespace std;


class Admin : public User {
    public:
        Admin();
        Admin(string id, string name) : User(id, name, nullptr, UserType::ADMIN) {}
        void displayInfo() const override;
        void showMenu() override;
        string getKey() const { return Id; }
        
        string toString() const {
            return Id + "," + Name + ",ADMIN";
        }
        
        virtual ~Admin();
};