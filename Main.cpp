#include "Entities.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
void load_data_account(const string& filename,map<string, string>& check_account) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string role, id, password;
        getline(ss, role, ',');
        getline(ss, id, ',');
        getline(ss, password, ',');
        string key = role + id;
        check_account[key] = password;
    }
    file.close();
}
void load_data_khoa(const string& filename,map<string,Khoa>& khoa) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name;
        getline(ss, id, ',');  
        getline(ss, name, ',');
        khoa[id] = Khoa(id,name);
    }
    file.close();
}

void load_data_user(const map<string, string>& check_account, map<string,Student>& students, map<string,Teacher>& teachers, map<string,Admin>& admins) {
    for(const auto& account : check_account){
        string role_id = account.first;
        string role = role_id.substr(0,2);
        // string id = role_id.substr(2);
        if(role == "SV"){
            ifstream file("SinhVien.txt");
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string sv_id, sv_name, mk,status;
                getline(ss, sv_id, ',');
                getline(ss, sv_name, ',');
                getline(ss, mk, ',');
                getline(ss,status, ',');
                if(status == "1") status = "Dang hoc";
                else status = "Nghi hoc";
                if(sv_id == role_id){
                    students[sv_id] = Student(sv_id,sv_name,mk,status);
                    break;
                }
            }
            file.close();
        } 
        else if(role == "GV"){
            ifstream file("GiangVien.txt");
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string gv_id, gv_name, mk,status;
                getline(ss, gv_id, ',');
                getline(ss, gv_name, ',');
                getline(ss, mk, ',');
                getline(ss,status, ',');
                if(status == "1") status = "Dang day";
                else status = "Nghi day";
                if(gv_id == role_id){
                    teachers[gv_id] = Teacher(gv_id,gv_name,mk,status);
                    break;
                }
            }
            file.close();
        } 
        else if(role == "AD"){
            ifstream file("Admin.txt");
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string ad_id, ad_name;
                getline(ss, ad_id, ',');
                getline(ss, ad_name, ',');
                if(ad_id == role_id){
                    admins[ad_id] = Admin(ad_id,ad_name);
                    break;
                }
            }
            file.close();
        }
    }
}

void load_data_lop_hp(const string& filename,map<string,LopHocPhan>& lop_hp) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, ma_giang_vien, ma_mon_hoc;
        getline(ss, id, ',');  
        getline(ss, ma_giang_vien, ',');
        getline(ss, ma_mon_hoc, ',');
        lop_hp[id] = LopHocPhan(id,ma_giang_vien,ma_mon_hoc);
    }
    file.close();
}
void load_data_mon_hoc(const string& filename,map<string,MonHoc>& mon_hoc) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, so_tin_chi_str;
        getline(ss, id, ',');  
        getline(ss, name, ',');
        getline(ss, so_tin_chi_str, ',');
        int so_tin_chi = stoi(so_tin_chi_str);
        mon_hoc[id] = MonHoc(id,name,so_tin_chi);
    }
    file.close();
}

void load_data_dang_ky(const string& filename,map<pair<string,string>,DangKy>& dang_ky) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string ma_lop_hp, ma_sv, diem_str;
        getline(ss, ma_lop_hp, ',');  
        getline(ss, ma_sv, ',');
        getline(ss, diem_str, ',');
        float diem = stof(diem_str);
        dang_ky[{ma_lop_hp, ma_sv}] = DangKy(ma_lop_hp,ma_sv,diem);
    }
    file.close();
}

void show_my_classes_point(User* user, const map<string,LopHocPhan>& lop_hp, const map<string,MonHoc>& mon_hoc, const map<pair<string,string>,DangKy>& dang_ky) {
    Student* student = dynamic_cast<Student*>(user);
    if (student == nullptr) {
        cout << "User is not a student." << endl;
        return;
    }
    string student_id = student->getId();
    // cout << student_id << ' ';
    for(auto& entry : dang_ky) {
        auto& key = entry.first; // pair<MALHP, MSV>
        auto& registration = entry.second; // DangKy object
        // cout << key.first << ' ' << key.second << endl;
        if (key.second == student_id) {
            string lop_hp_id = registration.getMaLopHocPhan();
            auto lop_hp_it = lop_hp.find(lop_hp_id);
            if (lop_hp_it != lop_hp.end()) {
                string ma_mon_hoc = lop_hp_it->second.getMaMonHoc();
                auto mon_hoc_it = mon_hoc.find(ma_mon_hoc);
                if (mon_hoc_it != mon_hoc.end()) {
                    cout << "Course: " << mon_hoc_it->second.getName() << ", Credits: " << mon_hoc_it->second.getSoTinChi() << ", Grade: " << registration.getDiem() << endl;
                }
            }
        }
    }
}

void show_all( map<string,Student>& students,map<string,Teacher>& teachers, map<string,Admin>& admins){
    cout << "Students:" << endl;
    for(auto& pair : students) {
        pair.second.show_info();
    }
    cout << "Teachers:" << endl;
    for(auto& pair : teachers) {
        pair.second.show_info();
    }
    cout << "Admins:" << endl;
    for(auto& pair : admins) {
        pair.second.show_info();
    }
}

User* log_in(const map<string, string>& check_account, map<string,Student>& students, map<string,Teacher>& teachers, map<string,Admin>& admins) {
    string role, id, password;
    cout << "Enter role (SV/GV/AD): ";
    cin >> role;
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter Password: ";
    cin >> password;
    string key = role + id;
    auto it = check_account.find(key);
    if (it != check_account.end() && it->second == password) {
        cout << "Login successful!" << endl;
        if(role == "SV"){
            return &students[key];
        } 
        else if(role == "GV"){
            return &teachers[key];
        } 
        else if(role == "AD"){
            return &admins[key];
        }
    } else {
        cout << "Invalid credentials!" << endl;
        return nullptr;
    }
    return nullptr;
}
int main(){
    map<string,string> check_account;
    map<string,Khoa> khoa;
    map<string,LopHocPhan> lop_hp;
    map<string,MonHoc> mon_hoc;
    map<pair<string,string>,DangKy> dang_ky;
    map<string,Student> students;
    map<string,Teacher> teachers;
    map<string,Admin> admins;
    load_data_account("users.txt",check_account);
    load_data_khoa("Khoa.txt",khoa);
    load_data_user(check_account,students,teachers,admins);
    load_data_lop_hp("LopHP.txt",lop_hp);
    load_data_mon_hoc("MonHoc.txt",mon_hoc);
    load_data_dang_ky("DangKy.txt",dang_ky);
    // show_all(students,teachers,admins);
    // User* current_user = nullptr;
    // current_user = log_in(check_account,students,teachers,admins);
    // if(current_user != nullptr){
    //     current_user->show_info();
    // }
    // else{
    //     cout << "Failed to log in." << endl;
    // }
    while(true){
        User* current_user = nullptr;
        current_user = log_in(check_account,students,teachers,admins);
        if(current_user != nullptr){
            current_user->show_info();
        }
        else{
            cout << "Failed to log in." << endl;
            continue;
        }
        while(current_user != nullptr){
            // Use dynamic_cast to check the actual type of current_user
            if(dynamic_cast<Student*>(current_user) != nullptr){
                cout << "Student Menu: " << endl;
                cout << "1. View Courses" << endl;
                cout << "2. Register Course" << endl;
                cout << "3. Logout" << endl;
                int choice;
                cin >> choice;
                if(choice == 1){
                    show_my_classes_point(current_user,lop_hp,mon_hoc,dang_ky);
                }
                if(choice == 3){
                    current_user = nullptr;
                    cout << "Logged out." << endl;
                }
            }
            else if(dynamic_cast<Teacher*>(current_user) != nullptr){
                cout << "Teacher Menu: " << endl;
                cout << "1. View Classes" << endl;
                cout << "2. Manage Grades" << endl;
                cout << "3. Logout" << endl;
                int choice;
                cin >> choice;
                if(choice == 1){
                    // show_my_classes_point(current_user,lop_hp,mon_hoc,dang_ky);
                }   
                if(choice == 3){
                    current_user = nullptr;
                    cout << "Logged out." << endl;
                }
            }
            else if(dynamic_cast<Admin*>(current_user) != nullptr){
                cout << "Admin Menu: " << endl;
                cout << "1. Manage Users" << endl;
                cout << "2. Manage Courses" << endl;
                cout << "3. Logout" << endl;
                int choice;
                cin >> choice;
                if(choice == 3){
                    current_user = nullptr;
                    cout << "Logged out." << endl;
                }
            }
         }
    }   
    return 0;
}