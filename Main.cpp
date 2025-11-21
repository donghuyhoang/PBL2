#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "LoginSystem.h"

using namespace std;

// --- CÁC HÀM TIỆN ÍCH (UTILITIES) ---

void clearScreen() {
    system("cls"); 
}

// Hàm hỗ trợ lấy phần số từ ID (VD: "SV10" -> 10) để sắp xếp đúng
int extractNumberFromID(const string& id) {
    try {
        // Bỏ qua 2 ký tự đầu (SV, GV, HP, MK...)
        if (id.length() > 2) {
            string numPart = id.substr(2);
            return stoi(numPart);
        }
    } catch (...) {
        return 0;
    }
    return 0;
}

string getHiddenPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' là phím Enter
        if (ch == '\b') { // Phím Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Xóa ký tự trên màn hình
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

int getValidIntInput(const string& prompt, int min, int max) {
    int choice;
    while (true) {
        cout << prompt;
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Loi: Vui long nhap so nguyen!" << endl;
            continue;
        }
        
        cin.ignore(10000, '\n');
        if (choice >= min && choice <= max) {
            return choice;
        }
        cout << "Loi: Vui long chon tu " << min << " den " << max << "!" << endl;
    }
}

float getValidFloatInput(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Loi: Vui long nhap so hop le!" << endl;
            continue;
        }
        
        cin.ignore(10000, '\n');
        if (value >= 0.0f && value <= 10.0f) {
            return value;
        }
        cout << "Loi: Diem phai nam trong khoang 0.0 den 10.0!" << endl;
    }
}

// --- CÁC CHỨC NĂNG CHUNG ---

void handleChangePassword(User* user, LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== DOI MAT KHAU ===" << endl;
    
    cout << "Nhap mat khau cu: ";
    string oldPass = getHiddenPassword();
    
    cout << "Nhap mat khau moi: ";
    string newPass = getHiddenPassword();
    
    if (newPass.empty()) {
        cout << "Loi: Mat khau khong duoc de trong!" << endl;
        return;
    }
    
    cout << "Xac nhan mat khau moi: ";
    string confirmPass = getHiddenPassword();

    if (newPass != confirmPass) {
        cout << "Loi: Mat khau xac nhan khong khop!" << endl;
        return;
    }

    if (loginSystem.changePassword(user->getId(), oldPass, newPass)) {
        cout << "Thanh cong: Doi mat khau thanh cong!" << endl;
    } else {
        cout << "Loi: Mat khau cu khong chinh xac!" << endl;
    }
}

bool saveAllData(LoginSystem& loginSystem) {
    try {
        cout << "\n--- DANG LUU VA SAP XEP DU LIEU ---" << endl;
        bool success = true;
        
        if (!loginSystem.saveDangKyToFile("DangKy.txt")) success = false;
        if (!loginSystem.saveAccountsToFile("Account.txt")) success = false;
        if (!loginSystem.saveStudentsToFile("Student.txt")) success = false;
        if (!loginSystem.saveTeachersToFile("Teacher.txt")) success = false;
        if (!loginSystem.saveKhoaToFile("Khoa.txt")) success = false;
        if (!loginSystem.saveMonHocToFile("MonHoc.txt")) success = false;
        if (!loginSystem.saveLopHPToFile("LopHP.txt")) success = false;
        
        return success;
    } catch (...) {
        return false;
    }
}

void askToSaveData(LoginSystem& loginSystem) {
    cout << "\nBan co muon luu lai cac thay doi truoc khi thoat? (y/n): ";
    char c;
    cin >> c;
    cin.ignore(10000, '\n');
    
    if (tolower(c) == 'y') {
        if (saveAllData(loginSystem)) {
            cout << "Da luu du lieu thanh cong." << endl;
        } else {
            cout << "Co loi xay ra khi luu file!" << endl;
        }
    }
}

// --- CHỨC NĂNG CỦA SINH VIEN ---

void studentViewScores(Student* student, LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== DIEM CUA BAN ===" << endl;
    student->displayInfo();
    cout << endl << endl;
    
    bool found = false;
    cout << setw(10) << left << "Ma LHP" 
         << setw(30) << left << "Ten Mon Hoc" 
         << setw(10) << right << "Diem" << endl;
    cout << string(50, '-') << endl;
    
    for (auto& dk : loginSystem.getDangKyTable().getAllElements()) {
        if (dk.getMaSV_FK() == student->getId()) {
            LopHocPhan* lhp = loginSystem.getLopHPTable().searchByString(dk.getMaLHP_FK());
            
            if (lhp && lhp->getMonHoc()) {
                cout << setw(10) << left << lhp->getMaLopHP()
                     << setw(30) << left << lhp->getMonHoc()->getTenMon()
                     << setw(10) << right << fixed << setprecision(2) << dk.getDiem() << endl;
                found = true;
            }
        }
    }
    
    if (!found) {
        cout << "Ban chua co diem mon hoc nao." << endl;
    }
}

void studentViewRegisteredClasses(Student* student, LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== LOP HOC PHAN DA DANG KY ===" << endl;
    student->displayInfo();
    cout << endl << endl;
    
    bool found = false;
    cout << setw(10) << left << "Ma LHP" 
         << setw(30) << left << "Ten Mon Hoc" 
         << setw(20) << left << "Giang Vien" << endl;
    cout << string(60, '-') << endl;
    
    for (auto& dk : loginSystem.getDangKyTable().getAllElements()) {
        if (dk.getMaSV_FK() == student->getId()) {
            LopHocPhan* lhp = loginSystem.getLopHPTable().searchByString(dk.getMaLHP_FK());
            
            if (lhp && lhp->getMonHoc() && lhp->getGiangVien()) {
                cout << setw(10) << left << lhp->getMaLopHP()
                     << setw(30) << left << lhp->getMonHoc()->getTenMon()
                     << setw(20) << left << lhp->getGiangVien()->getName() << endl;
                found = true;
            }
        }
    }
    
    if (!found) {
        cout << "Ban chua dang ky lop hoc phan nao." << endl;
    }
}

void handleStudentMenu(Student* student, LoginSystem& loginSystem, int choice, bool& logout) {
    switch (choice) {
        case 1:
            clearScreen();
            cout << "=== THONG TIN CA NHAN ===" << endl;
            student->displayInfo();
            break;
        case 2:
            studentViewRegisteredClasses(student, loginSystem);
            break;
        case 3:
            studentViewScores(student, loginSystem);
            break;
        case 4:
            handleChangePassword(student, loginSystem);
            break;
        case 5:
            logout = true;
            break;
    }
}

// --- CHỨC NĂNG CỦA GIANG VIEN ---

void teacherViewClasses(Teacher* teacher, LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== DANH SACH LOP DANG DAY ===" << endl;
    teacher->displayInfo();
    cout << endl << endl;
    
    bool found = false;
    cout << setw(10) << left << "Ma LHP" 
         << setw(30) << left << "Ten Mon Hoc" << endl;
    cout << string(40, '-') << endl;
    
    // Lấy danh sách và sắp xếp theo ID lớp
    auto listLopHP = loginSystem.getLopHPTable().getAllElements();
    sort(listLopHP.begin(), listLopHP.end(), [](const LopHocPhan& a, const LopHocPhan& b) {
        return extractNumberFromID(a.getMaLopHP()) < extractNumberFromID(b.getMaLopHP());
    });
    
    for (auto& lhp : listLopHP) {
        if (lhp.getGiangVien() && lhp.getGiangVien()->getId() == teacher->getId()) {
            cout << setw(10) << left << lhp.getMaLopHP()
                 << setw(30) << left << lhp.getMonHoc()->getTenMon() << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "Ban chua duoc phan cong day lop nao." << endl;
    }
}

void teacherViewAndEditScores(Teacher* teacher, LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== XEM VA SUA DIEM ===" << endl;
    
    // 1. Lấy danh sách lớp của giảng viên
    MyVector<string> myClasses;
    auto listLopHP = loginSystem.getLopHPTable().getAllElements();
    
    // Sắp xếp danh sách lớp cho dễ nhìn
    sort(listLopHP.begin(), listLopHP.end(), [](const LopHocPhan& a, const LopHocPhan& b) {
        return extractNumberFromID(a.getMaLopHP()) < extractNumberFromID(b.getMaLopHP());
    });

    for (auto& lhp : listLopHP) {
        if (lhp.getGiangVien() && lhp.getGiangVien()->getId() == teacher->getId()) {
            cout << "- " << lhp.getMaLopHP() << " (" << lhp.getMonHoc()->getTenMon() << ")" << endl;
            myClasses.push_back(lhp.getMaLopHP());
        }
    }
    
    if (myClasses.empty()) {
        cout << "Ban khong co lop hoc phan nao!" << endl;
        return;
    }

    cout << "\nNhap Ma Lop Hoc Phan de xem diem: ";
    string maLHP;
    getline(cin, maLHP);
    
    // Kiểm tra giảng viên có dạy lớp này không
    bool isMyClass = false;
    for (auto& c : myClasses) {
        if (c == maLHP) isMyClass = true;
    }
    
    if (!isMyClass) {
        cout << "Loi: Ma lop khong hop le hoac ban khong day lop nay!" << endl;
        return;
    }

    clearScreen();
    cout << "=== BANG DIEM LOP " << maLHP << " ===" << endl;
    
    // 2. Lấy danh sách đăng ký và LỌC ra sinh viên của lớp này
    MyVector<DangKy> classStudents;
    auto allDangKy = loginSystem.getDangKyTable().getAllElements();
    
    for (auto& dk : allDangKy) {
        if (dk.getMaLHP_FK() == maLHP) {
            classStudents.push_back(dk);
        }
    }

    if (classStudents.empty()) {
        cout << "Lop nay chua co sinh viên nao dang ky." << endl;
        return;
    }

    // 3. SẮP XẾP sinh viên theo Mã SV (SV1 -> SV2 -> SV10)
    sort(classStudents.begin(), classStudents.end(), [](const DangKy& a, const DangKy& b) {
        return extractNumberFromID(a.getMaSV_FK()) < extractNumberFromID(b.getMaSV_FK());
    });

    // 4. Hiển thị
    cout << setw(10) << left << "Ma SV" 
         << setw(25) << left << "Ten Sinh Vien" 
         << setw(10) << right << "Diem" << endl;
    cout << string(50, '-') << endl;
    
    for (auto& dk : classStudents) {
        Student* s = loginSystem.getStudentTable().searchByString(dk.getMaSV_FK());
        cout << setw(10) << left << dk.getMaSV_FK()
             << setw(25) << left << (s ? s->getName() : "Khong xac dinh")
             << setw(10) << right << fixed << setprecision(2) << dk.getDiem() << endl;
    }

    // 5. Sửa điểm
    cout << "\nNhap Ma SV de sua diem (Nhan Enter de bo qua): ";
    string maSV;
    getline(cin, maSV);
    
    if (!maSV.empty()) {
        string key = maLHP + "-" + maSV;
        DangKy* dk = loginSystem.getDangKyTable().searchByString(key);
        
        if (dk) {
            Student* s = loginSystem.getStudentTable().searchByString(maSV);
            cout << "Sinh vien: " << (s ? s->getName() : maSV) << endl;
            cout << "Diem hien tai: " << dk->getDiem() << endl;
            
            float diemMoi = getValidFloatInput("Nhap diem moi (0.0 - 10.0): ");
            dk->setDiem(diemMoi);
            cout << "Cap nhat diem thanh cong!" << endl;
        } else {
            cout << "Loi: Sinh vien nay khong co trong lop!" << endl;
        }
    }
}

void teacherGenerateClassReport(string LopHocPhan, LoginSystem& loginSystem) {
    ofstream outFile("ClassReport_" + LopHocPhan + ".txt");
    if (!outFile.is_open()) {
        cout << "Loi: Khong the tao file bao cao!" << endl;
        return;
    }
    
    int gioi = 0, kha = 0, tb = 0, yeu = 0;
    int count = 0;
    
    for (auto& dk : loginSystem.getDangKyTable().getAllElements()) {
        if (dk.getMaLHP_FK() == LopHocPhan) {
            float d = dk.getDiem();
            if (d >= 8.0) gioi++;
            else if (d >= 6.5) kha++;
            else if (d >= 5.0) tb++;
            else yeu++;
            count++;
        }
    }
    
    outFile << "BAO CAO THONG KE LOP HOC PHAN: " << LopHocPhan << endl;
    outFile << "Tong so sinh vien: " << count << endl;
    outFile << "----------------------------" << endl;
    outFile << "Gioi (>= 8.0):      " << gioi << endl;
    outFile << "Kha  (6.5 - 7.9):   " << kha << endl;
    outFile << "TB   (5.0 - 6.4):   " << tb << endl;
    outFile << "Yeu  (< 5.0):       " << yeu << endl;
    
    outFile.close();
    cout << "Da tao file bao cao thanh cong: ClassReport_" << LopHocPhan << ".txt" << endl;
}

void handleTeacherMenu(Teacher* t, LoginSystem& ls, int choice, bool& logout) {
    switch (choice) {
        case 1:
            clearScreen();
            t->displayInfo();
            break;
        case 2:
            teacherViewClasses(t, ls);
            break;
        case 3:
            teacherViewAndEditScores(t, ls);
            break;
        case 4:
            handleChangePassword(t, ls);
            break;
        case 5:
            logout = true;
            break;
    }
}

// --- CHỨC NĂNG CỦA ADMIN ---

void AdminviewAllStudents(LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== DANH SACH SINH VIEN TOAN TRUONG ===" << endl;
    
    auto list = loginSystem.getStudentTable().getAllElements();
    // Sắp xếp theo ID (SV1 < SV2 < SV10)
    sort(list.begin(), list.end(), [](const Student& a, const Student& b) {
        return extractNumberFromID(a.getId()) < extractNumberFromID(b.getId());
    });
    
    cout << setw(10) << left << "Ma SV" 
         << setw(25) << left << "Ho va Ten" 
         << setw(15) << left << "Ma Khoa" 
         << setw(25) << left << "Ten Khoa" << endl;
    cout << string(75, '-') << endl;
    
    for (auto& s : list) {
        cout << setw(10) << left << s.getId()
             << setw(25) << left << s.getName()
             << setw(15) << left << (s.getKhoa() ? s.getKhoa()->getMaKhoa() : "N/A")
             << setw(25) << left << (s.getKhoa() ? s.getKhoa()->getTenKhoa() : "N/A") << endl;
    }
}

void AdminviewAllTeachers(LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== DANH SACH GIANG VIEN TOAN TRUONG ===" << endl;
    
    auto list = loginSystem.getTeacherTable().getAllElements();
    sort(list.begin(), list.end(), [](const Teacher& a, const Teacher& b) {
        return extractNumberFromID(a.getId()) < extractNumberFromID(b.getId());
    });

    cout << setw(10) << left << "Ma GV" 
         << setw(25) << left << "Ho va Ten" 
         << setw(15) << left << "Ma Khoa" 
         << setw(25) << left << "Ten Khoa" << endl;
    cout << string(75, '-') << endl;

    for (auto& t : list) {
        cout << setw(10) << left << t.getId()
             << setw(25) << left << t.getName()
             << setw(15) << left << (t.getKhoa() ? t.getKhoa()->getMaKhoa() : "N/A")
             << setw(25) << left << (t.getKhoa() ? t.getKhoa()->getTenKhoa() : "N/A") << endl;
    }
}

void AdminviewAllClasses(LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== DANH SACH LOP HOC PHAN ===" << endl;
    
    auto list = loginSystem.getLopHPTable().getAllElements();
    sort(list.begin(), list.end(), [](const LopHocPhan& a, const LopHocPhan& b) {
        return extractNumberFromID(a.getMaLopHP()) < extractNumberFromID(b.getMaLopHP());
    });

    cout << setw(10) << left << "Ma LHP" 
         << setw(25) << left << "Mon Hoc" 
         << setw(20) << left << "Giang Vien" << endl;
    cout << string(60, '-') << endl;

    for (auto& l : list) {
        cout << setw(10) << left << l.getMaLopHP()
             << setw(25) << left << (l.getMonHoc() ? l.getMonHoc()->getTenMon() : "N/A")
             << setw(20) << left << (l.getGiangVien() ? l.getGiangVien()->getName() : "N/A") << endl;
    }
}

void AdminviewAllRegistrations(LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== DANH SACH DANG KY HOC ===" << endl;
    
    auto list = loginSystem.getDangKyTable().getAllElements();
    // Sắp xếp theo Lớp trước, sau đó đến SV
    sort(list.begin(), list.end(), [](const DangKy& a, const DangKy& b) {
        int lhpA = extractNumberFromID(a.getMaLHP_FK());
        int lhpB = extractNumberFromID(b.getMaLHP_FK());
        if (lhpA != lhpB) return lhpA < lhpB;
        return extractNumberFromID(a.getMaSV_FK()) < extractNumberFromID(b.getMaSV_FK());
    });

    cout << setw(10) << left << "Ma LHP" 
         << setw(10) << left << "Ma SV" 
         << setw(10) << right << "Diem" << endl;
    cout << string(40, '-') << endl;

    for (auto& dk : list) {
        cout << setw(10) << left << dk.getMaLHP_FK()
             << setw(10) << left << dk.getMaSV_FK()
             << setw(10) << right << fixed << setprecision(2) << dk.getDiem() << endl;
    }
}

void AdminManageAccounts(LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== QUAN LY TAI KHOAN ===" << endl;
    
    // 1. Hiển thị danh sách tài khoản hiện có
    ifstream file("Account.txt");
    if (file.is_open()) {
        cout << setw(5) << left << "STT" << "Username" << endl;
        cout << string(20, '-') << endl;
        string line;
        int i = 0;
        while (getline(file, line)) {
            if (line.length() > 3) {
                // Chỉ lấy phần username trước dấu phẩy
                cout << setw(5) << left << ++i << line.substr(0, line.find(',')) << endl;
            }
        }
        file.close();
    }
    
    cout << "\nBan co muon them tai khoan moi? (y/n): ";
    char c;
    cin >> c;
    cin.ignore(10000, '\n');
    
    if (tolower(c) != 'y') return;

    // --- BƯỚC 1: CHỌN LOẠI TÀI KHOẢN ---
    cout << "\nChon loai tai khoan can them:" << endl;
    cout << "1. Sinh vien (SV)" << endl;
    cout << "2. Giang vien (GV)" << endl;
    int type = getValidIntInput("Lua chon (1-2): ", 1, 2);
    
    string prefix = (type == 1) ? "SV" : "GV";
    
    // --- BƯỚC 2: TỰ ĐỘNG SINH MÃ & KIỂM TRA AN TOÀN ---
    string username = loginSystem.generateNextID(prefix);
    
    // Kiểm tra kỹ xem mã tự sinh này có bị trùng không (đề phòng file bị lỗi)
    if (loginSystem.isAccountExist(username) || 
        loginSystem.getStudentTable().contains(username) || 
        loginSystem.getTeacherTable().contains(username)) {
        cout << "Loi he thong: Ma tu dong sinh ra (" << username << ") da ton tai!" << endl;
        cout << "Vui long kiem tra lai file du lieu." << endl;
        return;
    }
    
    cout << "He thong tu dong cap ma: " << username << endl;
    
    // --- BƯỚC 3: NHẬP MẬT KHẨU ---
    cout << "Nhap mat khau: ";
    string password = getHiddenPassword();
    if (password.empty()) {
        password = "123";
        cout << "(Da thiet lap mat khau mac dinh: 123)" << endl;
    }
    
    // --- BƯỚC 4: NHẬP THÔNG TIN CÁ NHÂN ---
    string fullname;
    cout << "Nhap Ho va Ten: ";
    getline(cin, fullname);
    // Viết hoa tên
    for (auto& ch : fullname) ch = toupper(ch);
    
    cout << "\n--- DANH SACH KHOA HIEN CO ---" << endl;
    auto listKhoa = loginSystem.getKhoaTable().getAllElements();
    if (listKhoa.empty()) {
        cout << "Loi: He thong chua co Khoa nao! Vui long them Khoa vao file Khoa.txt truoc." << endl;
        return;
    }
    
    for (auto& k : listKhoa) {
        cout << k.getMaKhoa() << " - " << k.getTenKhoa() << endl;
    }
    
    string maKhoa;
    while (true) {
        cout << "Nhap Ma Khoa (VD: MK1): ";
        getline(cin, maKhoa);
        
        // Chuyển về chữ hoa để so sánh (vd: mk1 -> MK1)
        for (auto& ch : maKhoa) ch = toupper(ch);
        
        if (loginSystem.getKhoaTable().contains(maKhoa)) {
            break; // Mã khoa hợp lệ, thoát vòng lặp
        } else {
            cout << "Loi: Ma khoa '" << maKhoa << "' khong ton tai! Vui long nhap lai." << endl;
        }
    }
    
    // --- BƯỚC 6: LƯU VÀO FILE DỮ LIỆU ---
    string filename = (prefix == "SV") ? "Student.txt" : "Teacher.txt";
    ofstream fs(filename, ios::app);
    
    if (fs.is_open()) {
        // Thêm ký tự xuống dòng \n trước để tránh dính dòng cũ
        fs << "\n" << username << "," << fullname << "," << maKhoa;
        fs.close();
    } else {
        cout << "Loi: Khong the mo file " << filename << " de ghi!" << endl;
        return;
    }
    
    // --- BƯỚC 7: LOAD LẠI DỮ LIỆU VÀO RAM ---
    // (Quan trọng: Để hệ thống nhận diện được User mới vừa thêm)
    if (prefix == "SV") loginSystem.loadStudentFromFile("Student.txt");
    else loginSystem.loadTeacherFromFile("Teacher.txt");
    
    // --- BƯỚC 8: TẠO TÀI KHOẢN ĐĂNG NHẬP ---
    User* u = nullptr;
    if (prefix == "SV") u = loginSystem.getStudentTable().searchByString(username);
    else u = loginSystem.getTeacherTable().searchByString(username);
    
    if (u) {
        // Chỉ khi tìm thấy User trong RAM thì mới tạo Account
        ofstream fa("Account.txt", ios::app);
        if (fa.is_open()) {
            fa << "\n" << username << "," << password;
            fa.close();
            
            Account newAcc(username, password, u);
            loginSystem.getAccountTable().insert(username, newAcc);
            
            cout << "\n---> THEM TAI KHOAN THANH CONG! <---" << endl;
            cout << "Tai khoan: " << username << endl;
            cout << "Ho ten:    " << fullname << endl;
            cout << "Khoa:      " << maKhoa << endl;
        }
    } else {
        cout << "Loi nghiem trong: Khong the tao lien ket User trong bo nho!" << endl;
    }
}

void AdminviewClassReport(string LopHocPhan, LoginSystem& loginSystem) {
    string filename = "ClassReport_" + LopHocPhan + ".txt";
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Bao cao chua ton tai. Ban co muon tao moi? (y/n): ";
        char c;
        cin >> c;
        cin.ignore(10000, '\n');
        if (tolower(c) == 'y') {
            teacherGenerateClassReport(LopHocPhan, loginSystem);
            file.open(filename); // Mở lại sau khi tạo
        } else {
            return;
        }
    }
    
    if (file.is_open()) {
        clearScreen();
        cout << "=== NOI DUNG BAO CAO ===" << endl;
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
}

void AdminViewClassStats(string LopHocPhan, LoginSystem& loginSystem) {
    clearScreen();
    cout << "=== THONG KE LOP: " << LopHocPhan << " ===" << endl;
    
    float diemChuan = getValidFloatInput("Nhap muc diem de loc (>=): ");
    
    MyVector<DangKy> result;
    for (auto& dk : loginSystem.getDangKyTable().getAllElements()) {
        if (dk.getMaLHP_FK() == LopHocPhan && dk.getDiem() >= diemChuan) {
            result.push_back(dk);
        }
    }
    
    // Sắp xếp điểm giảm dần
    sort(result.begin(), result.end(), [](const DangKy& a, const DangKy& b) {
        return a.getDiem() > b.getDiem();
    });
    
    if (result.empty()) {
        cout << "Khong co sinh vien nao dat yeu cau." << endl;
    } else {
        cout << "\nTim thay " << result.size() << " sinh vien:" << endl;
        cout << setw(10) << left << "Ma SV" 
             << setw(25) << left << "Ten Sinh Vien" 
             << setw(10) << right << "Diem" << endl;
        cout << string(50, '-') << endl;
        
        for (auto& dk : result) {
            Student* s = loginSystem.getStudentTable().searchByString(dk.getMaSV_FK());
            cout << setw(10) << left << dk.getMaSV_FK()
                 << setw(25) << left << (s ? s->getName() : "N/A")
                 << setw(10) << right << fixed << setprecision(2) << dk.getDiem() << endl;
        }
    }
}

// Hàm thêm sinh viên vào lớp (Chỉ Thêm)
void addStudentToClass(string LopHocPhan, string MaSV, LoginSystem& loginSystem) {
    // 1. Kiểm tra Sinh viên có tồn tại không?
    if (!loginSystem.getStudentTable().contains(MaSV)) {
        cout << "Loi: Ma sinh vien " << MaSV << " khong ton tai trong he thong!" << endl;
        return;
    }
    
    // 2. Kiểm tra Lớp học phần có tồn tại không?
    if (!loginSystem.getLopHPTable().contains(LopHocPhan)) {
        cout << "Loi: Ma lop hoc phan " << LopHocPhan << " khong ton tai!" << endl;
        return;
    }
    
    // 3. Kiểm tra đã có trong lớp chưa
    string key = LopHocPhan + "-" + MaSV;
    if (loginSystem.getDangKyTable().contains(key)) {
        cout << "Thong bao: Sinh vien nay DA CO trong lop hoc phan roi." << endl;
    } else {
        // Thêm mới
        DangKy dk(LopHocPhan, MaSV, 0.0f);
        loginSystem.getDangKyTable().insert(key, dk);
        cout << "Thanh cong: Da them sinh vien " << MaSV << " vao lop " << LopHocPhan << "." << endl;
    }
}

void changeDepartmentforUser(string userId, string maKhoaMoi, LoginSystem& loginSystem) {
    Khoa* k = loginSystem.getKhoaTable().searchByString(maKhoaMoi);
    if (!k) {
        cout << "Loi: Ma khoa khong ton tai!" << endl;
        return;
    }
    
    if (loginSystem.getStudentTable().contains(userId)) {
        loginSystem.getStudentTable().searchByString(userId)->setKhoa(k);
        cout << "Da doi khoa cho Sinh vien thanh cong." << endl;
    } else if (loginSystem.getTeacherTable().contains(userId)) {
        loginSystem.getTeacherTable().searchByString(userId)->setKhoa(k);
        cout << "Da doi khoa cho Giang vien thanh cong." << endl;
    } else {
        cout << "Loi: Khong tim thay User ID!" << endl;
    }
}

void handleAdminMenu(LoginSystem& loginSystem, User* currentUser, int choice, bool& logout) {
    switch (choice) {
        case 1: // Xem Sinh Vien
            {
                AdminviewAllStudents(loginSystem);
                cout << "\n[Tuy chon] Doi khoa cho SV (Nhap ID hoac Enter de bo qua): ";
                string id;
                getline(cin, id);
                if (!id.empty()) {
                    cout << "Nhap Ma Khoa moi: ";
                    string k; getline(cin, k);
                    changeDepartmentforUser(id, k, loginSystem);
                }
            }
            break;
            
        case 2: // Xem Giang Vien
            {
                AdminviewAllTeachers(loginSystem);
                cout << "\n[Tuy chon] Doi khoa cho GV (Nhap ID hoac Enter de bo qua): ";
                string id;
                getline(cin, id);
                if (!id.empty()) {
                    cout << "Nhap Ma Khoa moi: ";
                    string k; getline(cin, k);
                    changeDepartmentforUser(id, k, loginSystem);
                }
            }
            break;
            
        case 3: // Xem Mon Hoc
            clearScreen();
            cout << "=== DANH SACH MON HOC ===" << endl;
            for (auto& m : loginSystem.getMonHocTable().getAllElements()) {
                cout << m.getMaMon() << " - " << m.getTenMon() << endl;
            }
            break;
            
        case 4: // Quan ly Lop Hoc Phan
            {
                AdminviewAllClasses(loginSystem);
                cout << "\n--- QUAN LY LOP HOC PHAN ---" << endl;
                cout << "1. Quay lai" << endl;
                cout << "2. Them sinh vien vao lop" << endl; // Đã sửa text theo yêu cầu
                cout << "3. Tao bao cao thong ke" << endl;
                cout << "4. Xem bao cao" << endl;
                cout << "5. Loc sinh vien theo diem" << endl;
                
                int sub = getValidIntInput("Chon chuc nang (1-5): ", 1, 5);
                
                if (sub == 1) break;
                
                cout << "Nhap Ma Lop Hoc Phan: ";
                string lhp;
                getline(cin, lhp);
                
                if (!loginSystem.getLopHPTable().contains(lhp)) {
                    cout << "Loi: Ma lop khong ton tai!" << endl;
                    break;
                }
                
                if (sub == 2) {
                    cout << "Nhap Ma Sinh Vien can them: ";
                    string sv;
                    getline(cin, sv);
                    addStudentToClass(lhp, sv, loginSystem); // Dùng hàm mới
                }
                else if (sub == 3) {
                    teacherGenerateClassReport(lhp, loginSystem);
                }
                else if (sub == 4) {
                    AdminviewClassReport(lhp, loginSystem);
                }
                else if (sub == 5) {
                    AdminViewClassStats(lhp, loginSystem);
                }
            }
            break;
            
        case 5: // Xem Dang Ky
            AdminviewAllRegistrations(loginSystem);
            break;
            
        case 6: // Quan ly Tai khoan
            AdminManageAccounts(loginSystem);
            break;
            
        case 7: // Doi mat khau
            handleChangePassword(currentUser, loginSystem);
            break;
            
        case 8: // Thoat
            logout = true;
            break;
            
        default:
            cout << "Lua chon khong hop le!" << endl;
    }
}

// --- CHUONG TRINH CHINH (MAIN) ---

int main() {
    LoginSystem loginSystem;
    
    if (!loginSystem.loadAllData()) {
        cout << "Loi nghiem trong: Khong the tai du lieu tu file!" << endl;
        return 1;
    }
    
    while (true) {
        clearScreen();
        cout << "=== HE THONG QUAN LY SINH VIEN ===" << endl;
        cout << "1. Dang nhap" << endl;
        cout << "2. Thoat chuong trinh" << endl;
        
        int choice = getValidIntInput("Lua chon: ", 1, 2);
        
        if (choice == 2) {
            askToSaveData(loginSystem);
            cout << "Tam biet!" << endl;
            break;
        }
        
        cout << "Tai khoan: ";
        string u;
        getline(cin, u);
        
        cout << "Mat khau: ";
        string p = getHiddenPassword();
        
        User* currentUser = loginSystem.login(u, p);
        
        if (currentUser) {
            bool logout = false;
            while (!logout) {
                clearScreen();
                cout << "Xin chao: " << currentUser->getName() << endl;
                currentUser->showMenu();
                
                int maxOpt = (currentUser->getUserType() == UserType::ADMIN) ? 8 : 5;
                int userChoice = getValidIntInput("Chon chuc nang: ", 1, maxOpt);
                
                if (currentUser->getUserType() == UserType::STUDENT) {
                    handleStudentMenu((Student*)currentUser, loginSystem, userChoice, logout);
                }
                else if (currentUser->getUserType() == UserType::TEACHER) {
                    handleTeacherMenu((Teacher*)currentUser, loginSystem, userChoice, logout);
                }
                else {
                    handleAdminMenu(loginSystem, currentUser, userChoice, logout);
                }
                
                if (!logout) {
                    cout << "\nNhan Enter de tiep tuc...";
                    cin.get();
                }
            }
        } else {
            cout << "\nNhan Enter de thu lai...";
            cin.get();
        }
    }
    
    return 0;
}