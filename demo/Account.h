#include <iostream>
#include <string>
#include <sstream>
#include "User.h"
using namespace std;

#pragma once
class Account{          
    private:
        string Username;
        string Password;
        User* user;
    public:
        Account();
        Account(string username, string password, User* userPtr = nullptr);
        string getUsername() const { return Username; }
        string getPassword() const { return Password; }
        string getKey() const { return Username; }
        User* getUser() const { return user; }
        void setUser(User* userPtr) { user = userPtr; }
        void displayInfo() const;
        void setPassword(const string& newPass) {this->Password = newPass;}
        string toString() const;
        virtual ~Account();
};