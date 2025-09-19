#include <iostream>
#include <string.h>
using namespace std;

class User{
    protected:
        string Id;
        string Name;
    public:
        User();
        User(const string&,const string &);
        virtual ~User();
        virtual void show_info();
        string getId() const { return Id; }
        string getName() const { return Name; }
};