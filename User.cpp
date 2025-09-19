#include "User.h"

User::User(){
    this->Id = "NULL";
    this->Name = "NULL";
}

User::User(const string& id, const string &name) : Id(id),Name(name){
}
User::~User(){};
void User::show_info(){
    cout << "ID: " << this->Id << " Name: " << this->Name << endl;
}
