#include "Monhoc.h"

MonHoc::MonHoc(){
    Mamon = "";
    Tenmon = "";
}
MonHoc::MonHoc(string mamon, string tenmon) 
    : Mamon(mamon), Tenmon(tenmon) {}
void MonHoc::displayInfo() const {
    cout << "Ma mon: " << Mamon << " - Ten mon: " <<
    Tenmon << endl;
}
string MonHoc::getMaMon() const {
    return Mamon;
}
string MonHoc::getTenMon() const {
    return Tenmon;
}
string MonHoc::getKey() const {
    return Mamon;
}
string MonHoc::toString() const {
    return Mamon + "," + Tenmon;
}
MonHoc::~MonHoc() {}

