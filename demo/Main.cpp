#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <map>
#include <conio.h> 
#include "LoginSystem.h"

using namespace std;

// --- TIỆN ÍCH ---
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

bool smartCompareID(const string& id1, const string& id2) {
    long long n1 = 0, n2 = 0;
    try {
        string s1 = "", s2 = "";
        for(char c : id1) if(isdigit(c)) s1 += c;
        for(char c : id2) if(isdigit(c)) s2 += c;
        if(!s1.empty()) n1 = stoll(s1);
        if(!s2.empty()) n2 = stoll(s2);
    } catch(...) {}
    if (n1 != n2) return n1 < n2; 
    return id1 < id2; 
}

string getHiddenPassword() {
    string password; char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { if (!password.empty()) { password.pop_back(); cout << "\b \b"; } }
        else { password.push_back(ch); cout << '*'; }
    }
    cout << endl; return password;
}

int getValidIntInput(const string& prompt, int min, int max) {
    int choice;
    while (true) {
        cout << prompt; cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); cout << " >> Loi: Nhap so nguyen!" << endl; continue; }
        cin.ignore(10000, '\n'); 
        if (choice >= min && choice <= max) return choice;
        cout << " >> Loi: Chon tu " << min << " den " << max << "!" << endl;
    }
}

float getValidFloatInput(const string& prompt) {
    float value;
    while (true) {
        cout << prompt; cin >> value;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); cout << " >> Loi: Nhap so hop le!" << endl; continue; }
        cin.ignore(10000, '\n');
        if (value >= 0.0f && value <= 10.0f) return value;
        cout << " >> Loi: Diem tu 0.0 - 10.0!" << endl;
    }
}

// --- THỐNG KÊ ---
void showClassStatistics(LoginSystem& ls) {
    cout << "\n--- THONG KE PHO DIEM ---" << endl;
    cout << "Nhap Ma Lop can xem: "; string maLop; getline(cin, maLop);

    if(!ls.getLopHPTable().contains(maLop)) { cout << " >> Loi: Ma lop khong ton tai!" << endl; return; }

    int count = 0;
    int scores[5] = {0}; 
    auto allDK = ls.getDangKyTable().getAllElements();
    for(auto& dk : allDK) {
        if(dk.getMaLHP_FK() == maLop) {
            float d = dk.getDiem();
            if (d < 4.0) scores[0]++; else if (d < 5.5) scores[1]++; else if (d < 7.0) scores[2]++; else if (d < 8.5) scores[3]++; else scores[4]++;
            count++;
        }
    }

    if(count == 0) { cout << " >> Lop trong hoac chua nhap diem." << endl; return; }

    auto drawBar = [](string label, int val, int total) {
        cout << setw(15) << left << label << "| ";
        int len = (total == 0) ? 0 : (val * 30) / total; for(int i=0; i<len; i++) cout << "=";
        cout << " " << val << " (" << fixed << setprecision(1) << (total? (float)val*100/total : 0) << "%)" << endl;
    };

    cout << "\n=== PHO DIEM LOP: " << maLop << " (Si so: " << count << ") ===" << endl;
    drawBar("Kem (<4.0)",   scores[0], count);
    drawBar("Yeu (4.0-5.4)", scores[1], count);
    drawBar("TB  (5.5-6.9)", scores[2], count);
    drawBar("Kha (7.0-8.4)", scores[3], count);
    drawBar("Gioi(8.5-10)",  scores[4], count);
}

// --- HIỂN THỊ (FIX SETW) ---
void viewAllClassesList(LoginSystem& ls) {
    auto list = ls.getLopHPTable().getAllElements();
    sort(list.begin(), list.end(), [](const LopHocPhan& a, const LopHocPhan& b){ return smartCompareID(a.getMaLopHP(), b.getMaLopHP()); });
    
    // HEADER phải dùng setw giống hệt DATA
    cout << "--- DANH SACH LOP ---" << endl;
    cout << setw(12) << left << "Ma Lop" 
         << setw(35) << left << "Ten Mon Hoc" 
         << setw(20) << left << "Giang Vien" << endl;
    cout << string(67, '-') << endl;
    
    for(auto& l : list) {
        string gv = l.getGiangVien() ? l.getGiangVien()->getName() : "CHUA PHAN CONG";
        string mh = l.getMonHoc() ? l.getMonHoc()->getTenMon() : "Unknown";
        cout << setw(12) << left << l.getMaLopHP() 
             << setw(35) << left << mh 
             << setw(20) << left << gv << endl;
    }
}

void AdminViewAllUsers(LoginSystem& ls, bool viewStudents) {
    clearScreen();
    cout << (viewStudents ? "=== DANH SACH SINH VIEN ===" : "=== DANH SACH GIANG VIEN ===") << endl;
    
    // Định dạng rộng rãi
    cout << setw(15) << left << (viewStudents?"Ma SV":"Ma GV") 
         << setw(35) << left << "Ho va Ten" 
         << setw(10) << left << "Khoa" << endl;
    cout << string(60, '-') << endl;

    if(viewStudents) {
        auto list = ls.getStudentTable().getAllElements();
        sort(list.begin(), list.end(), [](const Student& a, const Student& b){ return smartCompareID(a.getId(), b.getId()); });
        for(auto& s:list) cout << setw(15) << left << s.getId() << setw(35) << left << s.getName() << setw(10) << left << (s.getKhoa()?s.getKhoa()->getMaKhoa():"N/A") << endl;
    } else {
        auto list = ls.getTeacherTable().getAllElements();
        sort(list.begin(), list.end(), [](const Teacher& a, const Teacher& b){ return smartCompareID(a.getId(), b.getId()); });
        for(auto& t:list) cout << setw(15) << left << t.getId() << setw(35) << left << t.getName() << setw(10) << left << (t.getKhoa()?t.getKhoa()->getMaKhoa():"N/A") << endl;
    }
}

// --- QUẢN LÝ LỚP ---
void AdminManageClassesMenu(LoginSystem& ls) {
    while(true) {
        clearScreen();
        cout << "=== QUAN LY LOP HOC PHAN ===" << endl;
        viewAllClassesList(ls);

        cout << "\n[1] Them SV vao lop | [2] Xem Thong ke | [3] Quay lai: ";
        int choice = getValidIntInput("", 1, 3);

        if (choice == 1) {
            string lhp, sv; cout << "\nNhap Ma Lop: "; getline(cin, lhp); cout << "Nhap Ma SV:  "; getline(cin, sv); 
            if(!ls.getStudentTable().contains(sv) || !ls.getLopHPTable().contains(lhp)) cout << " >> Loi ID!" << endl;
            else if(ls.getDangKyTable().contains(lhp+"-"+sv)) cout << " >> Da ton tai!" << endl;
            else { ls.getDangKyTable().insert(lhp+"-"+sv, DangKy(lhp, sv, 0.0f)); cout << " >> Them thanh cong!" << endl; }
            cout << "An Enter..."; cin.get();
        } 
        else if (choice == 2) {
            showClassStatistics(ls);
            cout << "An Enter..."; cin.get();
        } 
        else break; 
    }
}

// --- CÁC CHỨC NĂNG KHÁC ---
void AdminAddAccount(LoginSystem& ls) {
    clearScreen(); cout << "=== THEM TAI KHOAN MOI ===" << endl;
    cout << "1. Sinh Vien\n2. Giang Vien\nChon: "; int type = getValidIntInput("",1,2);
    string name; cout << "Ho Ten: "; getline(cin, name); for(auto& c:name) c=toupper(c);
    cout << "\nDanh sach Khoa: "; 
    auto ks = ls.getKhoaTable().getAllElements();
    for(auto& k:ks) cout << k.getMaKhoa() << " ";
    cout << "\nNhap Ma Khoa (VD: MK10): "; string mk; getline(cin, mk); for(auto& c:mk) c=toupper(c);
    if(!ls.getKhoaTable().contains(mk)) { cout << " >> Loi: Ma Khoa khong hop le!" << endl; return; }
    string id = "";
    if (type == 1) {    
        // NẾU LÀ SINH VIÊN -> HỎI THÊM NĂM
        int year = getValidIntInput("Nhap Khoa (Nam nhap hoc, VD 25): ", 0, 99);
        id = ls.addNewStudent(name, mk, year); // Gọi hàm mới cập nhật
    } else {
        // GIẢNG VIÊN KHÔNG CẦN NĂM
        id = ls.addNewTeacher(name, mk);
    }
    if(id!="") {
        cout << "\n[THANH CONG] ID: " << id << " | Pass: 123 | Ten: " << name << endl;
        ls.saveStudentsToFile("Student.txt"); ls.saveTeachersToFile("Teacher.txt"); ls.saveAccountsToFile("Account.txt");
    } else cout << " >> Loi tao ID!" << endl;
}

void AdminChangeDept(LoginSystem& ls) {
    clearScreen(); cout << "=== DOI KHOA ===" << endl;
    cout << "ID User: "; string id; getline(cin, id);
    cout << "Khoa Moi: "; string mk; getline(cin, mk); for(auto& c:mk) c=toupper(c);
    
    cout << "Xac nhan (y/n): "; char c; cin >> c; cin.ignore();
    if(tolower(c)!='y') return;
    
    bool ok = ls.getStudentTable().contains(id) ? ls.changeStudentDepartment(id, mk) : ls.changeTeacherDepartment(id, mk);
    if(ok) {
        cout << " >> Thanh cong! Vui long luu file va khoi dong lai." << endl;
        ls.saveStudentsToFile("Student.txt"); ls.saveTeachersToFile("Teacher.txt"); ls.saveAccountsToFile("Account.txt"); ls.saveDangKyToFile("DangKy.txt");
    } else cout << " >> That bai!" << endl;
}

void handleChangePassword(User* u, LoginSystem& ls) {
    clearScreen(); cout << "=== DOI MAT KHAU ===" << endl;
    cout << "Cu: "; string o = getHiddenPassword(); cout << "Moi: "; string n = getHiddenPassword(); cout << "Lai: "; string c = getHiddenPassword();
    if(n!=c) { cout << "Loi xac nhan!" << endl; return; }
    if(ls.changePassword(u->getId(), o, n)) cout << "Thanh cong!" << endl; else cout << "Sai mat khau!" << endl;
}

// --- MENU ĐIỀU HƯỚNG ---
void handleAdminMenu(LoginSystem& ls, User* u, int c, bool& logout) {
    switch(c) {
        case 1: AdminViewAllUsers(ls, true); break;
        case 2: AdminViewAllUsers(ls, false); break;
        case 3: AdminManageClassesMenu(ls); break; 
        case 4: AdminAddAccount(ls); break;
        case 5: AdminChangeDept(ls); break;
        case 6: handleChangePassword(u, ls); break;
        case 7: logout = true; break;
    }
    if(c!=3 && c!=7) { cout << "\nAn Enter..."; cin.get(); }
}

void handleTeacherMenu(Teacher* t, LoginSystem& ls, int c, bool& logout) {
    if(c==1) { clearScreen(); t->displayInfo(); cout<<"\nEnter..."; cin.get(); return; }
    if(c==4) { handleChangePassword(t, ls); cout<<"\nEnter..."; cin.get(); return; }
    if(c==5) { logout = true; return; }

    if(c==2) { 
        clearScreen(); cout << "=== LOP DANG DAY ===" << endl;
        auto list = ls.getLopHPTable().getAllElements();
        cout << setw(12) << left << "Ma Lop" << "Ten Mon Hoc" << endl; cout << string(40, '-') << endl;
        for(auto& l : list) if(l.getGiangVien() && l.getGiangVien()->getId() == t->getId()) cout << setw(12) << left << l.getMaLopHP() << l.getMonHoc()->getTenMon() << endl;
        cout<<"\nEnter..."; cin.get();
    }
    if(c==3) { 
        clearScreen();
        cout << "Nhap Ma Lop: "; string ml; getline(cin, ml);
        cout << "Nhap Ma SV: "; string msv; getline(cin, msv);
        DangKy* dk = ls.getDangKyTable().searchByString(ml+"-"+msv);
        if(dk) { float d = getValidFloatInput("Diem moi: "); dk->setDiem(d); cout << "Cap nhat xong." << endl; } 
        else cout << "Khong tim thay!" << endl;
        cout<<"\nEnter..."; cin.get();
    }
}

void handleStudentMenu(Student* s, LoginSystem& ls, int c, bool& logout) {
    if(c==1) { clearScreen(); s->displayInfo(); }
    else if(c==2) { 
        clearScreen(); auto list = ls.getDangKyTable().getAllElements();
        cout << "=== LOP DA DANG KY ===" << endl;
        cout << setw(12) << left << "Ma Lop" << setw(30) << left << "Mon Hoc" << "GV" << endl;
        for(auto& d:list) if(d.getMaSV_FK()==s->getId()) {
            LopHocPhan* l = ls.getLopHPTable().searchByString(d.getMaLHP_FK());
            cout << setw(12) << left << d.getMaLHP_FK() 
                 << setw(30) << left << (l?l->getMonHoc()->getTenMon():"") 
                 << (l && l->getGiangVien() ? l->getGiangVien()->getName():"") << endl;
        }
    }
    else if(c==3) {
        clearScreen(); auto list = ls.getDangKyTable().getAllElements();
        cout << "=== BANG DIEM ===" << endl;
        for(auto& d:list) if(d.getMaSV_FK()==s->getId()) cout << setw(12) << left << d.getMaLHP_FK() << ": " << d.getDiem() << endl;
    }
    else if(c==4) handleChangePassword(s, ls);
    else if(c==5) logout = true;
    if(c!=5) { cout<<"\nEnter..."; cin.get(); }
}

void saveExit(LoginSystem& ls) {
    cout << "Luu du lieu? (y/n): "; char c; cin >> c;
    if(tolower(c)=='y') {
        ls.saveStudentsToFile("Student.txt"); ls.saveTeachersToFile("Teacher.txt");
        ls.saveAccountsToFile("Account.txt"); ls.saveDangKyToFile("DangKy.txt");
        ls.saveLopHPToFile("LopHP.txt");
        cout << "Da luu." << endl;
    }
}

int main() {
    LoginSystem ls; if(!ls.loadAllData()) cout << "Loi data!" << endl;

    while(true) {
        clearScreen(); cout << "=== HE THONG QUAN LY ===" << endl << "1. Dang nhap\n2. Thoat\nChon: ";
        if(getValidIntInput("",1,2)==2) { saveExit(ls); break; }

        cout << "User: "; string u; getline(cin, u); cout << "Pass: "; string p = getHiddenPassword();
        User* user = ls.login(u, p);

        if(user) {
            bool logout = false;
            while(!logout) {
                clearScreen(); cout << "Xin chao: " << user->getName() << endl;
                user->showMenu(); 
                int max = (user->getUserType()==UserType::ADMIN) ? 7 : 5;
                int c = getValidIntInput("Chon: ", 1, max);
                if(user->getUserType()==UserType::STUDENT) handleStudentMenu((Student*)user, ls, c, logout);
                else if(user->getUserType()==UserType::TEACHER) handleTeacherMenu((Teacher*)user, ls, c, logout);
                else handleAdminMenu(ls, user, c, logout);
            }
        } else { cout << "Sai thong tin! Enter thu lai..."; cin.get(); }
    }
    return 0;
}