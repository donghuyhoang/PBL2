#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>
#include <conio.h> 
#include "LoginSystem.h"
#include "MyVector.h" // Su dung MyVector

using namespace std;

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

// Comparator cho Student
bool cmpStudentByID(const Student& a, const Student& b) {
    return smartCompareID(a.getId(), b.getId());
}

// Comparator cho Teacher
bool cmpTeacherByID(const Teacher& a, const Teacher& b) {
    return smartCompareID(a.getId(), b.getId());
}

// Comparator cho LopHocPhan
bool cmpLopHPByID(const LopHocPhan& a, const LopHocPhan& b) {
    return smartCompareID(a.getMaLopHP(), b.getMaLopHP());
}

// --- TIỆN ÍCH ---
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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

// --- HÀM HỖ TRỢ VẼ ĐƯỜNG KẺ ---
void printDynamicLine(size_t w1, size_t w2, size_t w3, size_t w4 = 0, size_t w5 = 0) {
    cout << "+" << string(w1, '-') << "+" << string(w2, '-');
    if (w3 > 0) cout << "+" << string(w3, '-');
    if (w4 > 0) cout << "+" << string(w4, '-');
    if (w5 > 0) cout << "+" << string(w5, '-');
    cout << "+" << endl;
}

// --- HIỂN THỊ ADMIN (DYNAMIC TABLE) ---
void viewAllClassesList(LoginSystem& ls) {
    MyVector<LopHocPhan> list = ls.getLopHPTable().getAllElements();
    if (list.size() == 0) {
        cout << " >> Danh sach lop rong!" << endl;
        return;
    }

    sort(list.begin(), list.end(), cmpLopHPByID);
    
    // 1. Tính độ rộng cột
    size_t wMa = 8;
    size_t wTen = 15;
    size_t wGV = 12;

    for(size_t i = 0; i < list.size(); i++) {
        wMa = max(wMa, list[i].getMaLopHP().length());
        string tenMon = list[i].getMonHoc() ? list[i].getMonHoc()->getTenMon() : "Unknown";
        wTen = max(wTen, tenMon.length());
        string tenGV = list[i].getGiangVien() ? list[i].getGiangVien()->getName() : "CHUA PHAN CONG";
        wGV = max(wGV, tenGV.length());
    }
    wMa += 4; wTen += 4; wGV += 4;

    // 2. In bảng
    cout << "=== DANH SACH LOP HOC PHAN ===" << endl;
    printDynamicLine(wMa, wTen, wGV);
    cout << "|" << setw(wMa) << left << " Ma Lop" 
         << "|" << setw(wTen) << left << " Ten Mon Hoc" 
         << "|" << setw(wGV) << left << " Giang Vien" << "|" << endl;
    printDynamicLine(wMa, wTen, wGV);

    for(size_t i = 0; i < list.size(); i++) {
        string tenMon = list[i].getMonHoc() ? list[i].getMonHoc()->getTenMon() : "Unknown";
        string tenGV = list[i].getGiangVien() ? list[i].getGiangVien()->getName() : "CHUA PHAN CONG";
        cout << "|" << setw(wMa) << left << (" " + list[i].getMaLopHP())
             << "|" << setw(wTen) << left << (" " + tenMon)
             << "|" << setw(wGV) << left << (" " + tenGV) << "|" << endl;
    }
    printDynamicLine(wMa, wTen, wGV);
}

void AdminViewAllUsers(LoginSystem& ls, bool viewStudents) {
    clearScreen();
    cout << (viewStudents ? "=== DANH SACH SINH VIEN ===" : "=== DANH SACH GIANG VIEN ===") << endl;
    
    // Lấy dữ liệu
    MyVector<Student> listS;
    MyVector<Teacher> listT;

    size_t wID = 8; 
    size_t wName = 15;
    size_t wKhoa = 6;

    if (viewStudents) {
        listS = ls.getStudentTable().getAllElements();
        sort(listS.begin(), listS.end(), cmpStudentByID);
        for(size_t i = 0; i < listS.size(); i++) {
            wID = max(wID, listS[i].getId().length());
            wName = max(wName, listS[i].getName().length());
            string k = listS[i].getKhoa() ? listS[i].getKhoa()->getMaKhoa() : "N/A";
            wKhoa = max(wKhoa, k.length());
        }
    } else {
        listT = ls.getTeacherTable().getAllElements();
        sort(listT.begin(), listT.end(), cmpTeacherByID);
        for(size_t i = 0; i < listT.size(); i++) {
            wID = max(wID, listT[i].getId().length());
            wName = max(wName, listT[i].getName().length());
            string k = listT[i].getKhoa() ? listT[i].getKhoa()->getMaKhoa() : "N/A";
            wKhoa = max(wKhoa, k.length());
        }
    }
    
    wID += 4; wName += 4; wKhoa += 4;

    // In Bảng
    printDynamicLine(wID, wName, wKhoa);
    cout << "|" << setw(wID) << left << (viewStudents ? " Ma SV" : " Ma GV")
         << "|" << setw(wName) << left << " Ho va Ten"
         << "|" << setw(wKhoa) << left << " Khoa" << "|" << endl;
    printDynamicLine(wID, wName, wKhoa);

    if (viewStudents) {
        for(size_t i = 0; i < listS.size(); i++) {
            string k = listS[i].getKhoa() ? listS[i].getKhoa()->getMaKhoa() : "N/A";
            cout << "|" << setw(wID) << left << (" " + listS[i].getId())
                 << "|" << setw(wName) << left << (" " + listS[i].getName())
                 << "|" << setw(wKhoa) << left << (" " + k) << "|" << endl;
        }
    } else {
        for(size_t i = 0; i < listT.size(); i++) {
            string k = listT[i].getKhoa() ? listT[i].getKhoa()->getMaKhoa() : "N/A";
            cout << "|" << setw(wID) << left << (" " + listT[i].getId())
                 << "|" << setw(wName) << left << (" " + listT[i].getName())
                 << "|" << setw(wKhoa) << left << (" " + k) << "|" << endl;
        }
    }
    printDynamicLine(wID, wName, wKhoa);
}

// --- LOGIC XEM ĐIỂM CỦA ADMIN (CHỨC NĂNG MỚI) ---
struct StudentRecord {
    Student* sv;
    DangKy* dk;
    string khoaName;
};

// Comparator cho Struct StudentRecord
bool cmpRecordByID(const StudentRecord& a, const StudentRecord& b) {
    return smartCompareID(a.sv->getId(), b.sv->getId());
}

void AdminViewClassGrades(LoginSystem& ls) {
    cout << "\nNhap Ma Lop Hoc Phan can xem diem: ";
    string maLop; 
    getline(cin, maLop);

    // Kiểm tra lớp tồn tại
    LopHocPhan* lhp = ls.getLopHPTable().searchByString(maLop);
    if (!lhp) {
        cout << " >> Loi: Lop hoc phan '" << maLop << "' khong ton tai!" << endl;
        return;
    }

    // Lấy danh sách đăng ký
    MyVector<StudentRecord> listSV;
    MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
    
    for(size_t i = 0; i < allDK.size(); i++) {
        if (allDK[i].getMaLHP_FK() == maLop) {
            Student* s = ls.getStudentTable().searchByString(allDK[i].getMaSV_FK());
            DangKy* originalDK = ls.getDangKyTable().searchByString(allDK[i].getKey());
            
            if (s && originalDK) {
                string kName = (s->getKhoa()) ? s->getKhoa()->getMaKhoa() : "N/A";
                StudentRecord rec;
                rec.sv = s;
                rec.dk = originalDK;
                rec.khoaName = kName;
                listSV.push_back(rec);
            }
        }
    }

    if (listSV.size() == 0) {
        cout << " >> Lop nay chua co sinh vien nao dang ky!" << endl;
        return;
    }

    // Sắp xếp
    sort(listSV.begin(), listSV.end(), cmpRecordByID);

    // Tính độ rộng cột
    size_t wID = 5; size_t wName = 9; size_t wKhoa = 4; size_t wDiem = 4;
    for (size_t i = 0; i < listSV.size(); i++) {
        wID = max(wID, listSV[i].sv->getId().length());
        wName = max(wName, listSV[i].sv->getName().length());
        wKhoa = max(wKhoa, listSV[i].khoaName.length());
    }
    wID += 4; wName += 4; wKhoa += 4; wDiem += 4;

    // Hiển thị
    cout << "\n=== BANG DIEM LOP: " << maLop << " - " << lhp->getMonHoc()->getTenMon() << " ===" << endl;
    printDynamicLine(wID, wName, wKhoa, wDiem);
    cout << "|" << setw(wID) << left << " Ma SV"
            << "|" << setw(wName) << left << " Ho Va Ten"
            << "|" << setw(wKhoa) << left << " Khoa"
            << "|" << setw(wDiem) << left << " Diem" << "|" << endl;
    printDynamicLine(wID, wName, wKhoa, wDiem);

    for (size_t i = 0; i < listSV.size(); i++) {
        cout << "|" << setw(wID) << left << (" " + listSV[i].sv->getId())
                << "|" << setw(wName) << left << (" " + listSV[i].sv->getName())
                << "|" << setw(wKhoa) << left << (" " + listSV[i].khoaName)
                << "|" << " " << setw(wDiem - 2) << left << fixed << setprecision(2) << listSV[i].dk->getDiem() << " |" << endl;
    }
    printDynamicLine(wID, wName, wKhoa, wDiem);
}

// --- THỐNG KÊ (GIỮ NGUYÊN NHƯNG DÙNG MYVECTOR) ---
void showClassStatistics(LoginSystem& ls) {
    cout << "\n--- THONG KE PHO DIEM ---" << endl;
    cout << "Nhap Ma Lop can xem: "; string maLop; getline(cin, maLop);

    if(!ls.getLopHPTable().contains(maLop)) { cout << " >> Loi: Ma lop khong ton tai!" << endl; return; }

    int count = 0;
    int scores[5] = {0}; 
    MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
    
    for(size_t i = 0; i < allDK.size(); i++) {
        if(allDK[i].getMaLHP_FK() == maLop) {
            float d = allDK[i].getDiem();
            if (d < 4.0) scores[0]++; else if (d < 5.5) scores[1]++; else if (d < 7.0) scores[2]++; else if (d < 8.5) scores[3]++; else scores[4]++;
            count++;
        }
    }

    if(count == 0) { cout << " >> Lop trong hoac chua nhap diem." << endl; return; }

    // Không dùng lambda, viết vòng lặp thường
    cout << "\n=== PHO DIEM LOP: " << maLop << " (Si so: " << count << ") ===" << endl;
    
    string labels[5] = {"Kem (<4.0)", "Yeu (4.0-5.4)", "TB  (5.5-6.9)", "Kha (7.0-8.4)", "Gioi(8.5-10)"};
    
    for(int k=0; k<5; k++) {
        cout << setw(15) << left << labels[k] << "| ";
        int val = scores[k];
        int len = (count == 0) ? 0 : (val * 30) / count; 
        for(int j=0; j<len; j++) cout << "=";
        cout << " " << val << " (" << fixed << setprecision(1) << (count? (float)val*100/count : 0) << "%)" << endl;
    }
}

// --- MENU QUẢN LÝ LỚP CỦA ADMIN ---
void AdminManageClassesMenu(LoginSystem& ls) {
    while(true) {
        clearScreen();
        // Hiển thị danh sách lớp trước để Admin dễ chọn
        viewAllClassesList(ls); 

        cout << "\n[1] Them SV vao lop | [2] Xem Thong ke | [3] Xem Danh Sach & Diem | [4] Quay lai: ";
        int choice = getValidIntInput("", 1, 4);

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
        else if (choice == 3) { // Chức năng mới
            AdminViewClassGrades(ls);
            cout << "An Enter..."; cin.get();
        } 
        else break; 
    }
}

// --- CÁC CHỨC NĂNG KHÁC (AdminAddAccount, AdminChangeDept...) ---
// (Giữ nguyên logic, chỉ thay đổi std::vector nếu có dùng)

void AdminAddAccount(LoginSystem& ls) {
    clearScreen(); cout << "=== THEM TAI KHOAN MOI ===" << endl;
    cout << "1. Sinh Vien\n2. Giang Vien\nChon: "; int type = getValidIntInput("",1,2);
    string name; cout << "Ho Ten: "; getline(cin, name); for(auto& c:name) c=toupper(c);
    cout << "\nDanh sach Khoa: "; 
    MyVector<Khoa> ks = ls.getKhoaTable().getAllElements();
    for(size_t i=0; i<ks.size(); i++) cout << ks[i].getMaKhoa() << " ";
    cout << "\nNhap Ma Khoa (VD: MK10): "; string mk; getline(cin, mk); for(auto& c:mk) c=toupper(c);
    if(!ls.getKhoaTable().contains(mk)) { cout << " >> Loi: Ma Khoa khong hop le!" << endl; return; }
    string id = "";
    if (type == 1) {    
        int year = getValidIntInput("Nhap Khoa (Nam nhap hoc, VD 25): ", 0, 99);
        id = ls.addNewStudent(name, mk, year); 
    } else {
        id = ls.addNewTeacher(name, mk);
    }
    if(id!="") {
        cout << "\n[THANH CONG] ID: " << id << " | Pass: 123 | Ten: " << name << endl;
        ls.saveStudentsToFile("Student.txt"); ls.saveTeachersToFile("Teacher.txt"); ls.saveAccountsToFile("Account.txt");
    } else cout << " >> Loi tao ID!" << endl;
}

void AdminChangeDept(LoginSystem& ls) {
    while (true) {
        clearScreen();
        cout << "=== CHUYEN DOI KHOA ===" << endl;
        cout << "1. Chuyen Khoa Sinh Vien" << endl;
        cout << "2. Chuyen Khoa Giang Vien" << endl;
        cout << "3. Quay lai Menu Admin" << endl;
        
        int type = getValidIntInput("Chon chuc nang: ", 1, 3);
        if (type == 3) return;

        bool isStudent = (type == 1);

        // --- BƯỚC 1: HIỂN THỊ DANH SÁCH USER ĐỂ CHỌN ---
        cout << "\n--- DANH SACH " << (isStudent ? "SINH VIEN" : "GIANG VIEN") << " HIEN TAI ---" << endl;
        
        MyVector<Student> listS;
        MyVector<Teacher> listT;
        size_t wID = 8, wName = 15, wKhoa = 6;

        // Lấy dữ liệu và tính độ rộng cột
        if (isStudent) {
            listS = ls.getStudentTable().getAllElements();
            sort(listS.begin(), listS.end(), cmpStudentByID);
            if (listS.empty()) { cout << " >> Danh sach rong!" << endl; _getch(); continue; }
            for(size_t i = 0; i < listS.size(); i++) {
                wID = max(wID, listS[i].getId().length());
                wName = max(wName, listS[i].getName().length());
                string k = listS[i].getKhoa() ? listS[i].getKhoa()->getMaKhoa() : "N/A";
                wKhoa = max(wKhoa, k.length());
            }
        } else {
            listT = ls.getTeacherTable().getAllElements();
            sort(listT.begin(), listT.end(), cmpTeacherByID);
            if (listT.empty()) { cout << " >> Danh sach rong!" << endl; _getch(); continue; }
            for(size_t i = 0; i < listT.size(); i++) {
                wID = max(wID, listT[i].getId().length());
                wName = max(wName, listT[i].getName().length());
                string k = listT[i].getKhoa() ? listT[i].getKhoa()->getMaKhoa() : "N/A";
                wKhoa = max(wKhoa, k.length());
            }
        }
        wID += 4; wName += 4; wKhoa += 4;

        // In bảng danh sách
        printDynamicLine(wID, wName, wKhoa);
        cout << "|" << setw(wID) << left << (isStudent ? " Ma SV" : " Ma GV")
             << "|" << setw(wName) << left << " Ho va Ten"
             << "|" << setw(wKhoa) << left << " Khoa Hien Tai" << "|" << endl;
        printDynamicLine(wID, wName, wKhoa);

        if (isStudent) {
            for(size_t i = 0; i < listS.size(); i++) {
                string k = listS[i].getKhoa() ? listS[i].getKhoa()->getMaKhoa() : "N/A";
                cout << "|" << setw(wID) << left << (" " + listS[i].getId())
                     << "|" << setw(wName) << left << (" " + listS[i].getName())
                     << "|" << setw(wKhoa) << left << (" " + k) << "|" << endl;
            }
        } else {
            for(size_t i = 0; i < listT.size(); i++) {
                string k = listT[i].getKhoa() ? listT[i].getKhoa()->getMaKhoa() : "N/A";
                cout << "|" << setw(wID) << left << (" " + listT[i].getId())
                     << "|" << setw(wName) << left << (" " + listT[i].getName())
                     << "|" << setw(wKhoa) << left << (" " + k) << "|" << endl;
            }
        }
        printDynamicLine(wID, wName, wKhoa);

        // --- BƯỚC 2: NHẬP ID NGƯỜI CẦN CHUYỂN ---
        cout << "\nNhap ID can chuyen khoa (hoac Enter de quay lai): ";
        string id; 
        getline(cin, id);
        if (id.empty()) continue;

        // Check ID tồn tại
        bool exists = isStudent ? ls.getStudentTable().contains(id) : ls.getTeacherTable().contains(id);
        if (!exists) {
            cout << " >> LOI: ID '" << id << "' khong ton tai trong he thong!" << endl;
            cout << "An Enter thu lai..."; cin.get();
            continue;
        }

        // Lấy thông tin khoa hiện tại để so sánh
        string currentKhoaID = "";
        if (isStudent) {
            Student* s = ls.getStudentTable().searchByString(id);
            if(s && s->getKhoa()) currentKhoaID = s->getKhoa()->getMaKhoa();
        } else {
            Teacher* t = ls.getTeacherTable().searchByString(id);
            if(t && t->getKhoa()) currentKhoaID = t->getKhoa()->getMaKhoa();
        }

        // --- BƯỚC 3: CHỌN KHOA MỚI ---
        cout << "\n--- DANH SACH KHOA KHA DUNG ---" << endl;
        MyVector<Khoa> khoas = ls.getKhoaTable().getAllElements();
        for(size_t i = 0; i < khoas.size(); i++) {
            cout << "- " << setw(6) << left << khoas[i].getMaKhoa() << ": " << khoas[i].getTenKhoa() << endl;
        }

        cout << "\nNhap MA KHOA moi (VD: MK1): ";
        string mk; 
        getline(cin, mk);
        for(auto& c : mk) c = toupper(c); // Tự động viết hoa

        // Check Khoa tồn tại
        if (!ls.getKhoaTable().contains(mk)) {
            cout << " >> LOI: Ma Khoa '" << mk << "' khong ton tai!" << endl;
            cout << "An Enter thu lai..."; cin.get();
            continue;
        }
        if (mk == currentKhoaID) {
            cout << " >> LOI: User da thuoc khoa " << mk << " roi! Khong can chuyen." << endl;
            cout << "An Enter thu lai..."; cin.get();
            continue;
        }
        cout << "\n[XAC NHAN] Ban co chac muon chuyen " << id << " sang khoa " << mk << "? (y/n): ";
        char confirm; cin >> confirm; cin.ignore();

        if (tolower(confirm) == 'y') {
            bool ok = isStudent 
                    ? ls.changeStudentDepartment(id, mk) 
                    : ls.changeTeacherDepartment(id, mk);
            
            if (ok) {
                cout << "\n >> [THANH CONG] Da chuyen khoa!" << endl;
                cout << " >> LUY Y: ID cua nguoi dung da thay doi theo khoa moi." << endl;
                cout << " >> Vui long luu du lieu va khoi dong lai chuong trinh de cap nhat." << endl;
            } else {
                cout << "\n >> [THAT BAI] Co loi xay ra (Co the do ID moi bi trung lap)." << endl;
            }
        } else {
            cout << " >> Da huy thao tac." << endl;
        }
        cout << "An Enter de tiep tuc..."; cin.get();
    }
}

void handleChangePassword(User* u, LoginSystem& ls) {
    clearScreen(); cout << "=== DOI MAT KHAU ===" << endl;
    cout << "Cu: "; string o = getHiddenPassword(); cout << "Moi: "; string n = getHiddenPassword(); cout << "Lai: "; string c = getHiddenPassword();
    if(n!=c) { cout << "Loi xac nhan!" << endl; return; }
    if(ls.changePassword(u->getId(), o, n)) cout << "Thanh cong!" << endl; else cout << "Sai mat khau!" << endl;
}

// --- LOGIC GIẢNG VIÊN (SỬA LẠI DÙNG MYVECTOR & BỎ LAMBDA) ---
void TeacherViewAndEditScores(Teacher* t, LoginSystem& ls) {
    while (true) {
        clearScreen();
        cout << "=== XEM VA SUA DIEM SINH VIEN ===" << endl;

        // 1. Lọc lớp
        MyVector<LopHocPhan> myClasses;
        MyVector<LopHocPhan> allClasses = ls.getLopHPTable().getAllElements();
        for (size_t i = 0; i < allClasses.size(); i++) {
            if (allClasses[i].getGiangVien() && allClasses[i].getGiangVien()->getId() == t->getId()) {
                myClasses.push_back(allClasses[i]);
            }
        }

        if (myClasses.size() == 0) {
            cout << " >> Ban chua co lop hoc phan nao!" << endl;
            return;
        }

        // 2. Hiển thị danh sách lớp
        cout << "\nDANH SACH LOP DANG DAY:" << endl;
        for (size_t i = 0; i < myClasses.size(); i++) {
            cout << "[" << i + 1 << "] " << setw(10) << left << myClasses[i].getMaLopHP() 
                 << " - " << myClasses[i].getMonHoc()->getTenMon() << endl;
        }
        cout << "[" << myClasses.size() + 1 << "] Quay lai" << endl;

        int choice = getValidIntInput("\nChon Lop (Nhap STT): ", 1, myClasses.size() + 1);
        if (choice == (int)myClasses.size() + 1) return;

        LopHocPhan selectedClass = myClasses[choice - 1];
        string maLop = selectedClass.getMaLopHP();

        // 3. Lấy dữ liệu
        MyVector<StudentRecord> listSV;
        MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
        
        for (size_t i = 0; i < allDK.size(); i++) {
            if (allDK[i].getMaLHP_FK() == maLop) {
                Student* s = ls.getStudentTable().searchByString(allDK[i].getMaSV_FK());
                DangKy* originalDK = ls.getDangKyTable().searchByString(allDK[i].getKey());
                if (s && originalDK) {
                    string kName = (s->getKhoa()) ? s->getKhoa()->getMaKhoa() : "N/A";
                    StudentRecord rec;
                    rec.sv = s; rec.dk = originalDK; rec.khoaName = kName;
                    listSV.push_back(rec);
                }
            }
        }

        if (listSV.size() == 0) {
            cout << "\n >> Lop nay chua co sinh vien dang ky!" << endl;
            cout << "An Enter de tiep tuc..."; cin.get();
            continue;
        }

        sort(listSV.begin(), listSV.end(), cmpRecordByID);

        // 4. Tính toán độ rộng cột (Dynamic)
        size_t wID = 5; size_t wName = 9; size_t wKhoa = 4; size_t wDiem = 4;
        for (size_t i = 0; i < listSV.size(); i++) {
            wID = max(wID, listSV[i].sv->getId().length());
            wName = max(wName, listSV[i].sv->getName().length());
            wKhoa = max(wKhoa, listSV[i].khoaName.length());
        }
        wID += 4; wName += 4; wKhoa += 4; wDiem += 4;

        // 5. Hiển thị bảng
        while (true) {
            clearScreen();
            cout << "=== DIEM LOP: " << maLop << " - " << selectedClass.getMonHoc()->getTenMon() << " ===" << endl;
            
            printDynamicLine(wID, wName, wKhoa, wDiem);
            cout << "|" << setw(wID) << left << " Ma SV"
                 << "|" << setw(wName) << left << " Ho Va Ten"
                 << "|" << setw(wKhoa) << left << " Khoa"
                 << "|" << setw(wDiem) << left << " Diem" << "|" << endl;
            printDynamicLine(wID, wName, wKhoa, wDiem);

            for (size_t i = 0; i < listSV.size(); i++) {
                cout << "|" << setw(wID) << left << (" " + listSV[i].sv->getId())
                     << "|" << setw(wName) << left << (" " + listSV[i].sv->getName())
                     << "|" << setw(wKhoa) << left << (" " + listSV[i].khoaName)
                     << "|" << " " << setw(wDiem - 2) << left << fixed << setprecision(2) << listSV[i].dk->getDiem() << " |" << endl;
            }
            printDynamicLine(wID, wName, wKhoa, wDiem);

            cout << "\nNhap [MA SV] de sua diem (hoac Enter de quay lai): ";
            string inputID; getline(cin, inputID);
            if (inputID.empty()) break;

            bool found = false;
            for(size_t i = 0; i < listSV.size(); i++) {
                if(listSV[i].sv->getId() == inputID) {
                    found = true;
                    cout << "\n >> Dang sua diem cho: " << listSV[i].sv->getName() << " (" << listSV[i].sv->getId() << ")" << endl;
                    float newScore = getValidFloatInput("Nhap diem moi (0.0 - 10.0): ");
                    listSV[i].dk->setDiem(newScore);
                    cout << " >> Cap nhat thanh cong! An Enter..."; cin.get();
                    break;
                }
            }
            if (!found) { cout << " >> Khong tim thay SV! Enter thu lai..."; cin.get(); }
        }
    }
}

// --- LOGIC SINH VIÊN (SỬA LẠI DÙNG MYVECTOR & BỎ LAMBDA) ---
struct TranscriptRecord {
    string maLop;
    string tenMon;
    float diem;
};

// Comparator cho Transcript
bool cmpTranscript(const TranscriptRecord& a, const TranscriptRecord& b) {
    return a.maLop < b.maLop;
}

void StudentViewGrades(Student* s, LoginSystem& ls) {
    clearScreen();
    MyVector<TranscriptRecord> transcript;
    MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
    
    float totalScore = 0;
    int count = 0;

    for(size_t i = 0; i < allDK.size(); i++) {
        if(allDK[i].getMaSV_FK() == s->getId()) {
            string tenMon = "Unknown";
            LopHocPhan* lhp = ls.getLopHPTable().searchByString(allDK[i].getMaLHP_FK());
            if (lhp && lhp->getMonHoc()) {
                tenMon = lhp->getMonHoc()->getTenMon();
            }
            TranscriptRecord tr;
            tr.maLop = allDK[i].getMaLHP_FK();
            tr.tenMon = tenMon;
            tr.diem = allDK[i].getDiem();
            transcript.push_back(tr);
            
            totalScore += allDK[i].getDiem();
            count++;
        }
    }

    if(transcript.size() == 0) {
        cout << "=== BANG DIEM CA NHAN ===" << endl;
        cout << " >> Ban chua co diem mon nao!" << endl;
        return;
    }

    // Sắp xếp
    sort(transcript.begin(), transcript.end(), cmpTranscript);

    // Tính toán độ rộng cột (Dynamic Width)
    size_t wMaLop = 10; 
    size_t wTenMon = 15;
    size_t wDiem = 6;    

    for(size_t i = 0; i < transcript.size(); i++) {
        wMaLop = max(wMaLop, transcript[i].maLop.length());
        wTenMon = max(wTenMon, transcript[i].tenMon.length());
    }
    wMaLop += 4; wTenMon += 4; wDiem += 4;

    // Hiển thị
    cout << "=== BANG DIEM SINH VIEN: " << s->getName() << " (" << s->getId() << ") ===" << endl;
    printDynamicLine(wMaLop, wTenMon, wDiem);
    cout << "|" << setw(wMaLop) << left << " Ma Lop"
         << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
         << "|" << setw(wDiem) << left << " Diem" << "|" << endl;
    printDynamicLine(wMaLop, wTenMon, wDiem);

    for(size_t i = 0; i < transcript.size(); i++) {
        cout << "|" << setw(wMaLop) << left << (" " + transcript[i].maLop)
             << "|" << setw(wTenMon) << left << (" " + transcript[i].tenMon)
             << "|" << " " << setw(wDiem - 2) << left << fixed << setprecision(2) << transcript[i].diem << " |" << endl;
    }
    printDynamicLine(wMaLop, wTenMon, wDiem);

    cout << "\n >> Tong so mon: " << count << endl;
    if(count > 0) cout << " >> Diem trung binh (GPA): " << fixed << setprecision(2) << (totalScore / count) << endl;
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
        MyVector<LopHocPhan> list = ls.getLopHPTable().getAllElements();
        cout << setw(12) << left << "Ma Lop" << "Ten Mon Hoc" << endl; cout << string(40, '-') << endl;
        for(size_t i=0; i<list.size(); i++) {
            if(list[i].getGiangVien() && list[i].getGiangVien()->getId() == t->getId()) 
                cout << setw(12) << left << list[i].getMaLopHP() << list[i].getMonHoc()->getTenMon() << endl;
        }
        cout<<"\nEnter..."; cin.get();
    }
    if(c==3) { 
        TeacherViewAndEditScores(t, ls);
    }
}

void handleStudentMenu(Student* s, LoginSystem& ls, int c, bool& logout) {
    if(c==1) { clearScreen(); s->displayInfo(); }
    else if(c==2) { 
        clearScreen(); 
        MyVector<DangKy> list = ls.getDangKyTable().getAllElements();
        cout << "=== LOP DA DANG KY ===" << endl;
        cout << setw(12) << left << "Ma Lop" << setw(30) << left << "Mon Hoc" << "GV" << endl;
        for(size_t i=0; i<list.size(); i++) {
            if(list[i].getMaSV_FK()==s->getId()) {
                LopHocPhan* l = ls.getLopHPTable().searchByString(list[i].getMaLHP_FK());
                cout << setw(12) << left << list[i].getMaLHP_FK() 
                     << setw(30) << left << (l?l->getMonHoc()->getTenMon():"") 
                     << (l && l->getGiangVien() ? l->getGiangVien()->getName():"") << endl;
            }
        }
    }
    else if(c==3) {
        StudentViewGrades(s, ls);
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