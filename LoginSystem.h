#ifndef LOGIN_SYSTEM_H
#define LOGIN_SYSTEM_H

#include "HashTable.h"
#include "Account.h"
#include "Khoa.h"
#include "MonHoc.h"
#include "Student.h"
#include "Teacher.h"
#include "LopHocPhan.h"
#include "DangKy.h"
#include "UserFactory.h"
#include <fstream>
#include <sstream>
#include <conio.h>
#include <algorithm>

class LoginSystem {
private:
    HashTable<Account> accountTable;
    HashTable<Khoa> khoaTable;
    HashTable<MonHoc> monHocTable;
    HashTable<Student> studentTable;
    HashTable<Teacher> teacherTable;
    HashTable<LopHocPhan> lopHPTable;
    HashTable<DangKy> dangKyTable;

public:
    LoginSystem() : accountTable(1000), khoaTable(500), monHocTable(500), 
                   studentTable(500), teacherTable(500), lopHPTable(500), dangKyTable(1000) {}

    // --- GETTERS ---
    HashTable<Account>& getAccountTable() { return accountTable; }
    HashTable<LopHocPhan>& getLopHPTable() { return lopHPTable; }
    HashTable<DangKy>& getDangKyTable() { return dangKyTable; }
    HashTable<Student>& getStudentTable() { return studentTable; }
    HashTable<Teacher>& getTeacherTable() { return teacherTable; }
    HashTable<MonHoc>& getMonHocTable() { return monHocTable; }
    HashTable<Khoa>& getKhoaTable() { return khoaTable; }

    // --- LOGIC CHỨC NĂNG ---

    User* login(const string& username, const string& password) {
        Account* account = accountTable.searchByString(username);
        if (account == nullptr) { 
            cout << "\n[!] Tai khoan khong ton tai!" << endl; 
            return nullptr; 
        }
        if (account->getPassword() != password) { 
            cout << "\n[!] Sai mat khau!" << endl; 
            return nullptr; 
        }
        cout << "\n[OK] Dang nhap thanh cong!" << endl;
        return account->getUser();
    }

    string generateNextID(const string& prefix) {
        int maxID = 0;
        MyVector<string> keys;
        if (prefix == "SV") {
            auto students = studentTable.getAllElements();
            for(auto& s : students) keys.push_back(s.getId());
        } else {
            auto teachers = teacherTable.getAllElements();
            for(auto& t : teachers) keys.push_back(t.getId());
        }

        for (size_t i = 0; i < keys.size(); i++) {
            string key = keys[i];
            if (key.substr(0, 2) == prefix) {
                try {
                    int num = stoi(key.substr(2));
                    if (num > maxID) maxID = num;
                } catch (...) {}
            }
        }
        return prefix + to_string(maxID + 1);
    }

    bool changePassword(const string& username, const string& oldPass, const string& newPass) {
        Account* acc = accountTable.searchByString(username);
        if (!acc) return false;
        if (acc->getPassword() == oldPass) {
            acc->setPassword(newPass);
            saveAccountsToFile("Account.txt");
            return true;
        }
        return false;
    }
    bool isAccountExist(const string& username) {
        return accountTable.contains(username);
    }

    // --- LOAD FILE (Rất gọn, chỉ báo lỗi nếu cần) ---

    bool loadKhoaFromFile(const string& filename) {
        ifstream file(filename); if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            stringstream ss(line); string mk, tk;
            if (getline(ss, mk, ',') && getline(ss, tk, ',')) 
                khoaTable.insert(mk, Khoa(mk, tk));
        }
        file.close(); return true;
    }

    bool loadMonHocFromFile(const string& filename) {
        ifstream file(filename); if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            stringstream ss(line); string mm, tm;
            if (getline(ss, mm, ',') && getline(ss, tm)) 
                monHocTable.insert(mm, MonHoc(mm, tm));
        }
        file.close(); return true;
    }

    bool loadStudentFromFile(const string& filename) {
        ifstream file(filename); if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            if (line.length() < 5) continue;
            stringstream ss(line); string id, ten, mk;
            if (getline(ss, id, ',') && getline(ss, ten, ',') && getline(ss, mk)) {
                if (!mk.empty() && mk.back() == '\r') mk.pop_back();
                Khoa* k = khoaTable.searchByString(mk);
                if (k) studentTable.insert(id, Student(id, ten, k));
            }
        }
        file.close(); return true;
    }

    bool loadTeacherFromFile(const string& filename) {
        ifstream file(filename); if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            if (line.length() < 5) continue;
            stringstream ss(line); string id, ten, mk;
            if (getline(ss, id, ',') && getline(ss, ten, ',') && getline(ss, mk)) {
                if (!mk.empty() && mk.back() == '\r') mk.pop_back();
                Khoa* k = khoaTable.searchByString(mk);
                if (k) teacherTable.insert(id, Teacher(id, ten, k));
            }
        }
        file.close(); return true;
    }

    bool loadLopHPFromFile(const string& filename) {
        ifstream file(filename); if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line); string lhp, gv, mh;
            // Fix thứ tự: Mã Lớp, Mã Môn, Mã GV
            if (getline(ss, lhp, ',') && getline(ss, mh, ',') && getline(ss, gv)) {
                if (!gv.empty() && gv.back() == '\r') gv.pop_back();
                Teacher* t = teacherTable.searchByString(gv);
                MonHoc* m = monHocTable.searchByString(mh);
                if (t && m) lopHPTable.insert(lhp, LopHocPhan(lhp, t, m));
            }
        }
        file.close(); return true;
    }

    bool loadDangKyFromFile(const string& filename) {
        ifstream file(filename); if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            stringstream ss(line); string lhp, sv, d;
            if (getline(ss, lhp, ',') && getline(ss, sv, ',') && getline(ss, d)) {
                try { dangKyTable.insert(lhp + "-" + sv, DangKy(lhp, sv, stof(d))); } catch (...) {}
            }
        }
        file.close(); return true;
    }

    bool loadAccountsFromFile(const string& filename) {
        ifstream file(filename); if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            stringstream ss(line); string u, p;
            if (getline(ss, u, ',') && getline(ss, p)) {
                if (!p.empty() && p.back() == '\r') p.pop_back();
                string pre = u.substr(0, 2);
                User* userPtr = nullptr;
                if (pre == "SV") {
                    Student* s = studentTable.searchByString(u);
                    if (s) userPtr = s;
                } else if (pre == "GV") {
                    Teacher* t = teacherTable.searchByString(u);
                    if (t) userPtr = t;
                } else if (pre == "AD") {
                    userPtr = UserFactory::createUser("Admin", u, "Admin " + u);
                }
                if (userPtr) accountTable.insert(u, Account(u, p, userPtr));
            }
        }
        file.close(); return true;
    }

    bool loadAllData() {
        cout << "--- DANG TAI DU LIEU ---" << endl;
        bool s = true;
        s &= loadKhoaFromFile("Khoa.txt");
        s &= loadMonHocFromFile("MonHoc.txt");
        s &= loadStudentFromFile("Student.txt");
        s &= loadTeacherFromFile("Teacher.txt");
        s &= loadLopHPFromFile("LopHP.txt");
        s &= loadDangKyFromFile("DangKy.txt");
        s &= loadAccountsFromFile("Account.txt");
        
        if(s) cout << "[OK] Tai du lieu hoan tat.\n" << endl;
        else cout << "[!] Co loi khi tai mot so file.\n" << endl;
        return s;
    }

    // --- SAVE FILE (Giữ nguyên logic Sort) ---
    // (Đã lược bớt code sort ở đây cho gọn, logic giữ nguyên như phiên bản trước của bạn)
    
    bool saveStudentsToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = studentTable.getAllElements();
        sort(list.begin(), list.end(), [](const Student& a, const Student& b) {
            return a.getId().length() == b.getId().length() ? a.getId() < b.getId() : a.getId().length() < b.getId().length();
        });
        for (auto& item : list) file << item.toString() << endl;
        file.close(); return true;
    }
    bool saveTeachersToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = teacherTable.getAllElements();
        sort(list.begin(), list.end(), [](const Teacher& a, const Teacher& b) {
            return a.getId().length() == b.getId().length() ? a.getId() < b.getId() : a.getId().length() < b.getId().length();
        });
        for (auto& item : list) file << item.toString() << endl;
        file.close(); return true;
    }
    bool saveKhoaToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = khoaTable.getAllElements();
        sort(list.begin(), list.end(), [](const Khoa& a, const Khoa& b) { return a.getMaKhoa() < b.getMaKhoa(); });
        for (auto& item : list) file << item.toString() << endl;
        file.close(); return true;
    }
    bool saveMonHocToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = monHocTable.getAllElements();
        sort(list.begin(), list.end(), [](const MonHoc& a, const MonHoc& b) { return a.getMaMon() < b.getMaMon(); });
        for (auto& item : list) file << item.toString() << endl;
        file.close(); return true;
    }
    bool saveLopHPToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = lopHPTable.getAllElements();
        sort(list.begin(), list.end(), [](const LopHocPhan& a, const LopHocPhan& b) { return a.getMaLopHP() < b.getMaLopHP(); });
        for (auto& item : list) file << item.toString() << endl;
        file.close(); return true;
    }
    bool saveDangKyToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = dangKyTable.getAllElements();
        sort(list.begin(), list.end(), [](const DangKy& a, const DangKy& b) {
            if (a.getMaLHP_FK() != b.getMaLHP_FK()) return a.getMaLHP_FK() < b.getMaLHP_FK();
            return a.getMaSV_FK().length() == b.getMaSV_FK().length() ? a.getMaSV_FK() < b.getMaSV_FK() : a.getMaSV_FK().length() < b.getMaSV_FK().length();
        });
        for (auto& item : list) file << item.toString() << endl;
        file.close(); return true;
    }
    bool saveAccountsToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto accounts = accountTable.getAllElements();
        sort(accounts.begin(), accounts.end(), [](const Account& a, const Account& b) {
            string idA = a.getUsername(); string idB = b.getUsername();
            auto getRank = [](const string& id) {
                string p = id.substr(0, 2);
                if (p == "SV") return 1; if (p == "GV") return 2; if (p == "AD") return 3; return 4;
            };
            int rankA = getRank(idA); int rankB = getRank(idB);
            if (rankA != rankB) return rankA < rankB;
            return idA.length() == idB.length() ? idA < idB : idA.length() < idB.length();
        });
        for (auto& acc : accounts) file << acc.getUsername() << "," << acc.getPassword() << endl;
        file.close(); return true;
    }
};
#endif