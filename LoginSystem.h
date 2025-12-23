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
#include <cctype>
//#include <vector>
#include "MyVector.h"

class LoginSystem {
private:
    HashTable<Account> accountTable;
    HashTable<Khoa> khoaTable;
    HashTable<MonHoc> monHocTable;
    HashTable<Student> studentTable;
    HashTable<Teacher> teacherTable;
    HashTable<LopHocPhan> lopHPTable;
    HashTable<DangKy> dangKyTable;

    // --- HÀM HỖ TRỢ XỬ LÝ CHUỖI ---
    string cleanString(string str) {
        if (str.empty()) return "";
        if (str.length() >= 3 && (unsigned char)str[0] == 0xEF && (unsigned char)str[1] == 0xBB && (unsigned char)str[2] == 0xBF) {
            str = str.substr(3);
        }
        const string whitespace = " \t\n\r\f\v";
        size_t start = str.find_first_not_of(whitespace);
        if (start == string::npos) return ""; 
        size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

    string extractKhoaNumber(string maKhoa) {
        string num = "";
        for (char c : maKhoa) if (isdigit(c)) num += c;
        return num;
    }

    string createAbbreviation(string fullname) {
        stringstream ss(fullname);
        string word, abbr = "";
//        vector<string> words;
        MyVector<string> words;
        while (ss >> word) words.push_back(word);
        if (words.empty()) return "user";
        for (size_t i = 0; i < words.size() - 1; i++) abbr += tolower(words[i][0]);
        string lastName = words.back();
        for (char c : lastName) abbr += tolower(c);
        return abbr;
    }

    static long long extractNum(const string& id) {
        try {
            string n = ""; 
            for(char c : id) if(isdigit(c)) n += c;
            return n.empty() ? 0 : stoll(n);
        } catch(...) { return 0; }
    }

public:
    LoginSystem() : accountTable(2000), khoaTable(100), monHocTable(500), 
                   studentTable(1000), teacherTable(500), lopHPTable(1000), dangKyTable(5000) {}

    // GETTERS
    HashTable<Account>& getAccountTable() { return accountTable; }
    HashTable<LopHocPhan>& getLopHPTable() { return lopHPTable; }
    HashTable<DangKy>& getDangKyTable() { return dangKyTable; }
    HashTable<Student>& getStudentTable() { return studentTable; }
    HashTable<Teacher>& getTeacherTable() { return teacherTable; }
    HashTable<MonHoc>& getMonHocTable() { return monHocTable; }
    HashTable<Khoa>& getKhoaTable() { return khoaTable; }

    User* login(const string& username, const string& password) {
        string cleanUser = cleanString(username);
        Account* account = accountTable.searchByString(cleanUser);
        if (account == nullptr) return nullptr;
        if (account->getPassword() != password) return nullptr;
        return account->getUser();
    }

    bool isAccountExist(const string& u) { return accountTable.contains(cleanString(u)); }
    
    bool changePassword(const string& u, const string& o, const string& n) {
        Account* acc = accountTable.searchByString(cleanString(u));
        if(acc && acc->getPassword() == o) { acc->setPassword(n); return true; }
        return false;
    }

    // --- LOGIC SINH ID ---
    string generateStudentID(string maKhoa, int year) {
        string khoaNum = extractKhoaNumber(maKhoa);
        
        // Đảm bảo năm luôn có 2 chữ số (VD: 5 -> 05)
        string sYear = to_string(year);
        if (sYear.length() < 2) sYear = "0" + sYear;
        if (sYear.length() > 2) sYear = sYear.substr(sYear.length() - 2); // Nếu nhập 2025 -> lấy 25

        string prefix = khoaNum + sYear;  
        int maxSeq = 0;
        auto students = studentTable.getAllElements();
        for (auto& s : students) {
            string id = s.getId();
            if (id.find(prefix) == 0 && id.length() == prefix.length() + 3) {
                try {
                    int seq = stoi(id.substr(prefix.length()));
                    if (seq > maxSeq) maxSeq = seq;
                } catch(...) {}
            }
        }
        string seqStr = to_string(maxSeq + 1);
        while (seqStr.length() < 3) seqStr = "0" + seqStr;
        return prefix + seqStr;
    }

    string generateTeacherID(string fullname, string maKhoa) {
        string abbr = createAbbreviation(fullname);
        string khoaNum = extractKhoaNumber(maKhoa);
        string baseID = abbr + khoaNum; 
        if (!teacherTable.contains(baseID)) return baseID;
        int suffix = 1;
        while (true) {
            string newID = baseID + to_string(suffix);
            if (!teacherTable.contains(newID)) return newID;
            suffix++;
        }
    }

    // --- LOGIC THÊM MỚI (ĐÃ CẬP NHẬT THAM SỐ YEAR) ---
    
    string addNewStudent(string fullname, string maKhoa, int year) { // <--- Thêm int year
        string cleanMK = cleanString(maKhoa);
        Khoa* k = khoaTable.searchByString(cleanMK);
        if (!k) return "";
        
        string newID = generateStudentID(cleanMK, year); // <--- Truyền year vào đây
        
        Student newStudent(newID, fullname, k);
        studentTable.insert(newID, newStudent);
        Student* sPtr = studentTable.searchByString(newID);
        if (sPtr) {
            accountTable.insert(newID, Account(newID, "123", sPtr));
            return newID;
        }
        return "";
    }

    string addNewTeacher(string fullname, string maKhoa) {
        string cleanMK = cleanString(maKhoa);
        Khoa* k = khoaTable.searchByString(cleanMK);
        if (!k) return "";
        string newID = generateTeacherID(fullname, cleanMK);
        Teacher newTeacher(newID, fullname, k);
        teacherTable.insert(newID, newTeacher);
        Teacher* tPtr = teacherTable.searchByString(newID);
        if (tPtr) {
            accountTable.insert(newID, Account(newID, "123", tPtr));
            return newID;
        }
        return "";
    }

    // --- LOGIC ĐỔI KHOA ---
    // (Giữ nguyên logic cũ nhưng copy lại cho đầy đủ)
    bool changeStudentDepartment(string oldID, string newMaKhoa) {
        oldID = cleanString(oldID);
        Student* s = studentTable.searchByString(oldID);
        Khoa* newKhoa = khoaTable.searchByString(cleanString(newMaKhoa));
        if (!s || !newKhoa) return false;

        string oldName = s->getName();
        string oldPassword = "123"; 
        Account* oldAcc = accountTable.searchByString(oldID);
        if (oldAcc) oldPassword = oldAcc->getPassword();

        string khoaNumNew = extractKhoaNumber(newMaKhoa);
        if (oldID.length() < 2) return false;
        string newID = khoaNumNew + oldID.substr(2); 

        if (studentTable.contains(newID)) return false;

        Student newStudent(newID, oldName, newKhoa);
        
        auto allDK = dangKyTable.getAllElements();
        for (auto& dk : allDK) {
            if (dk.getMaSV_FK() == oldID) {
                string oldKey = dk.getMaLHP_FK() + "-" + oldID;
                string newKey = dk.getMaLHP_FK() + "-" + newID;
                DangKy newDK(dk.getMaLHP_FK(), newID, dk.getDiem());
                dangKyTable.remove(oldKey);
                dangKyTable.insert(newKey, newDK);
            }
        }
        studentTable.remove(oldID);
        studentTable.insert(newID, newStudent);
        accountTable.remove(oldID);
        Student* sPtr = studentTable.searchByString(newID);
        if (sPtr) accountTable.insert(newID, Account(newID, oldPassword, sPtr));
        return true;
    }

    bool changeTeacherDepartment(string oldID, string newMaKhoa) {
        oldID = cleanString(oldID);
        Teacher* t = teacherTable.searchByString(oldID);
        Khoa* newKhoa = khoaTable.searchByString(cleanString(newMaKhoa));
        if (!t || !newKhoa) return false;

        string oldName = t->getName();
        string oldPassword = "123";
        Account* oldAcc = accountTable.searchByString(oldID);
        if (oldAcc) oldPassword = oldAcc->getPassword();

        // Xử lý Lớp học phần dangling pointer
//        vector<string> classesToUpdate;
				MyVector<string> classesToUpdate;
        auto allLops = lopHPTable.getAllElements();
        for(auto& l : allLops) {
            if(l.getGiangVien() && l.getGiangVien()->getId() == oldID) classesToUpdate.push_back(l.getMaLopHP());
        }

        string newID = oldID;
        string khoaNumNew = extractKhoaNumber(newMaKhoa);
        bool replaced = false;
        for (size_t i = 0; i < newID.length() - 1; i++) {
            if (isdigit(newID[i]) && isdigit(newID[i+1])) {
                newID.replace(i, 2, khoaNumNew);
                replaced = true;
                break;
            }
        }
        if (!replaced) newID += khoaNumNew; 
        if (teacherTable.contains(newID)) {
             int suffix = 1; string base = newID;
             while(teacherTable.contains(newID)) newID = base + to_string(suffix++);
        }

        teacherTable.remove(oldID); 
        Teacher newTeacher(newID, oldName, newKhoa);
        teacherTable.insert(newID, newTeacher);
        Teacher* newTPtr = teacherTable.searchByString(newID);

        if (newTPtr) {
            for (const string& maLop : classesToUpdate) {
                LopHocPhan* lop = lopHPTable.searchByString(maLop);
                if (lop) {
                    LopHocPhan lpMoi(lop->getMaLopHP(), newTPtr, lop->getMonHoc());
                    lopHPTable.insert(maLop, lpMoi); 
                }
            }
        }

        accountTable.remove(oldID);
        if (newTPtr) accountTable.insert(newID, Account(newID, oldPassword, newTPtr));
        return true;
    }

    // --- CÁC HÀM LOAD FILE ---
    bool loadKhoaFromFile(const string& f) {
        ifstream file(f); if(!file.is_open()) return false;
        string line; while(getline(file, line)) {
            stringstream ss(line); string mk, tk; if(getline(ss, mk, ',') && getline(ss, tk)) {
                mk = cleanString(mk); tk = cleanString(tk); if(!mk.empty()) khoaTable.insert(mk, Khoa(mk, tk));
            }
        } file.close(); return true;
    }
    bool loadMonHocFromFile(const string& f) {
        ifstream file(f); if(!file.is_open()) return false;
        string line; while(getline(file, line)) {
            stringstream ss(line); string mm, tm; if(getline(ss, mm, ',') && getline(ss, tm)) {
                mm = cleanString(mm); tm = cleanString(tm); if(!mm.empty()) monHocTable.insert(mm, MonHoc(mm, tm));
            }
        } file.close(); return true;
    }
    bool loadStudentFromFile(const string& f) {
        ifstream file(f); if(!file.is_open()) return false;
        string line; while(getline(file, line)) {
            if(line.length()<2) continue; stringstream ss(line); string id, ten;
            if(getline(ss, id, ',') && getline(ss, ten)) {
                id = cleanString(id); ten = cleanString(ten); Khoa* k = nullptr;
                if(id.length() >= 2) { string ma = "MK" + id.substr(0, 2); k = khoaTable.searchByString(ma); }
                studentTable.insert(id, Student(id, ten, k)); 
            }
        } file.close(); return true;
    }
    bool loadTeacherFromFile(const string& f) {
        ifstream file(f); if(!file.is_open()) return false;
        string line; while(getline(file, line)) {
            if(line.length()<2) continue; stringstream ss(line); string id, ten;
            if(getline(ss, id, ',') && getline(ss, ten)) {
                id = cleanString(id); ten = cleanString(ten); Khoa* k = nullptr;
                for(size_t i=0; i<id.length()-1; i++) { if(isdigit(id[i]) && isdigit(id[i+1])) {
                    k = khoaTable.searchByString("MK" + id.substr(i, 2)); break; }}
                teacherTable.insert(id, Teacher(id, ten, k));
            }
        } file.close(); return true;
    }
    bool loadLopHPFromFile(const string& f) {
        ifstream file(f); if(!file.is_open()) return false;
        string line; while(getline(file, line)) {
            stringstream ss(line); string l, g, m; if(getline(ss, l, ',') && getline(ss, m, ',') && getline(ss, g)) {
                l=cleanString(l); g=cleanString(g); m=cleanString(m);
                Teacher* t = teacherTable.searchByString(g); MonHoc* mh = monHocTable.searchByString(m);
                if(t && mh) lopHPTable.insert(l, LopHocPhan(l, t, mh));
            }
        } file.close(); return true;
    }
    bool loadDangKyFromFile(const string& f) {
        ifstream file(f); if(!file.is_open()) return false;
        string line; while(getline(file, line)) {
            stringstream ss(line); string l, s, d; if(getline(ss, l, ',') && getline(ss, s, ',') && getline(ss, d)) {
                l=cleanString(l); s=cleanString(s); d=cleanString(d);
                try { dangKyTable.insert(l + "-" + s, DangKy(l, s, stof(d))); } catch(...) {}
            }
        } file.close(); return true;
    }
    bool loadAccountsFromFile(const string& f) {
        ifstream file(f); if(!file.is_open()) return false;
        string line; while(getline(file, line)) {
            stringstream ss(line); string u, p; if(getline(ss, u, ',') && getline(ss, p)) {
                u = cleanString(u); p = cleanString(p); User* ptr = nullptr;
                Student* s = studentTable.searchByString(u);
                if(s) ptr = s;
                else { Teacher* t = teacherTable.searchByString(u);
                       if(t) ptr = t;
                       else if(u.length()>=2 && u.substr(0,2)=="AD") ptr = UserFactory::createUser("Admin", u, "Admin System"); }
                if(ptr) accountTable.insert(u, Account(u, p, ptr));
            }
        } file.close(); return true;
    }
    bool loadAllData() {
        bool s = true; s&=loadKhoaFromFile("data/Khoa.txt"); s&=loadMonHocFromFile("data/MonHoc.txt");
        s&=loadStudentFromFile("data/Student.txt"); s&=loadTeacherFromFile("data/Teacher.txt");
        s&=loadLopHPFromFile("data/LopHP.txt"); s&=loadDangKyFromFile("data/DangKy.txt"); s&=loadAccountsFromFile("data/Account.txt");
        return s;
    }

    // --- CÁC HÀM SAVE FILE ---
    bool saveStudentsToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = studentTable.getAllElements();
        sort(list.begin(), list.end(), [](const Student& a, const Student& b) { return extractNum(a.getId()) < extractNum(b.getId()); });
        for (auto& item : list) file << item.getId() << "," << item.getName() << endl;
        file.close(); return true;
    }
    bool saveTeachersToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = teacherTable.getAllElements();
        sort(list.begin(), list.end(), [](const Teacher& a, const Teacher& b) { return a.getId() < b.getId(); });
        for (auto& item : list) file << item.getId() << "," << item.getName() << endl;
        file.close(); return true;
    }
    bool saveAccountsToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto accounts = accountTable.getAllElements();
//        vector<Account> svList, gvList, adminList;
        MyVector<Account> svList, gvList, adminList;
        for(auto& acc : accounts) {
            if(acc.getUser() && acc.getUser()->getUserType() == UserType::STUDENT) svList.push_back(acc);
            else if(acc.getUser() && acc.getUser()->getUserType() == UserType::TEACHER) gvList.push_back(acc);
            else adminList.push_back(acc);
        }
        sort(svList.begin(), svList.end(), [](const Account& a, const Account& b) { return extractNum(a.getUsername()) < extractNum(b.getUsername()); });
        sort(gvList.begin(), gvList.end(), [](const Account& a, const Account& b) { return a.getUsername() < b.getUsername(); });
        sort(adminList.begin(), adminList.end(), [](const Account& a, const Account& b) { return a.getUsername() < b.getUsername(); });

        for (auto& acc : svList) file << acc.getUsername() << "," << acc.getPassword() << endl;
        for (auto& acc : gvList) file << acc.getUsername() << "," << acc.getPassword() << endl;
        for (auto& acc : adminList) file << acc.getUsername() << "," << acc.getPassword() << endl;
        file.close(); return true;
    }
    bool saveDangKyToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = dangKyTable.getAllElements();
        sort(list.begin(), list.end(), [](const DangKy& a, const DangKy& b) {
            if (a.getMaLHP_FK() != b.getMaLHP_FK()) return a.getMaLHP_FK() < b.getMaLHP_FK();
            return extractNum(a.getMaSV_FK()) < extractNum(b.getMaSV_FK());
        });
        for (auto& item : list) file << item.toString() << endl;
        file.close(); return true;
    }
    bool saveLopHPToFile(const string& filename) {
        ofstream file(filename); if (!file.is_open()) return false;
        auto list = lopHPTable.getAllElements();
        sort(list.begin(), list.end(), [](const LopHocPhan& a, const LopHocPhan& b) { return a.getMaLopHP() < b.getMaLopHP(); });
        for (auto& item : list) {
            string gvID = (item.getGiangVien()) ? item.getGiangVien()->getId() : "NULL";
            string mhID = (item.getMonHoc()) ? item.getMonHoc()->getMaMon() : "NULL";
            file << item.getMaLopHP() << "," << mhID << "," << gvID << endl;
        }
        file.close(); return true;
    }
};
#endif
