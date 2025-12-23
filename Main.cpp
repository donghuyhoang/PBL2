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
        if (ch == '\b') { if (!password.empty()) { password.pop_back(); cout <<"\b \b"; } }
        else { password.push_back(ch); cout <<'*'; }
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

// Overload cho 2 cột
void printDynamicLine(size_t w1, size_t w2) {
    cout << "+" << string(w1, '-') << "+" << string(w2, '-') << "+" << endl;
}

// --- STRUCTS TOÀN CỤC ---
struct StudentRecord {
    Student* sv;
    DangKy* dk;
    string khoaName;
};

struct TranscriptRecord {
    string maLop;
    string tenMon;
    float diem;
};

struct DepartmentStatRecord {
    string maKhoa;
    string tenKhoa;
    int soSV;
    int soGV;
};

// Comparator cho StudentRecord
bool cmpRecordByID(const StudentRecord& a, const StudentRecord& b) {
    return smartCompareID(a.sv->getId(), b.sv->getId());
}

// Comparator cho TranscriptRecord
bool cmpTranscript(const TranscriptRecord& a, const TranscriptRecord& b) {
    return a.maLop < b.maLop;
}

// Comparator cho DepartmentStatRecord
bool cmpDeptByID(const DepartmentStatRecord& a, const DepartmentStatRecord& b) {
    return a.maKhoa < b.maKhoa;
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
    MyVector<LopHocPhan> listLopHP = ls.getLopHPTable().getAllElements();
    MyVector<DangKy> listDK = ls.getDangKyTable().getAllElements();

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

    // Menu thống kê cho sinh viên (hiển thị sau bảng)
    if (viewStudents) {
        while (true) {
            int choice;
            cout << "\n" << "=== CHUC NANG BO SUNG ===" << endl;
            cout << "[1] Xem thong ke chi tiet" << endl;
            cout << "[2] Xem chi tiet ve 1 sinh vien bat ki" << endl;
            cout << "[0] Quay lai" << endl;
            cout << "\n" << "Chon chuc nang: ";

            choice = getValidIntInput("", 0, 2);
            
            if (choice == 0) {
                break; // Thoát về menu Admin
            }
            else if (choice == 1) {
                // Thu thập dữ liệu thống kê
                struct StatRecord {
                    string maSV;
                    string hoTen;
                    int soLop;
                    float diemTB;
                };
                MyVector<StatRecord> stats;
                
                for (size_t i = 0; i < listS.size(); i++) {
                    int lopCount = 0;
                    float totalDiem = 0.0f;
                    for (size_t j = 0; j < listDK.size(); j++) {
                        if (listDK[j].getMaSV_FK() == listS[i].getId()) {
                            lopCount++;
                            totalDiem += listDK[j].getDiem();
                        }
                    }
                    float avgDiem = (lopCount > 0) ? (totalDiem / lopCount) : 0.0f;
                    
                    StatRecord sr;
                    sr.maSV = listS[i].getId();
                    sr.hoTen = listS[i].getName();
                    sr.soLop = lopCount;
                    sr.diemTB = avgDiem;
                    stats.push_back(sr);
                }
                
                // Tính độ rộng cột
                size_t wMaSV = 8, wHoTen = 15, wSoLop = 10, wDiemTB = 12;
                for (size_t i = 0; i < stats.size(); i++) {
                    wMaSV = max(wMaSV, stats[i].maSV.length());
                    wHoTen = max(wHoTen, stats[i].hoTen.length());
                }
                wMaSV += 4; wHoTen += 4; wSoLop += 4; wDiemTB += 4;
                
                // In bảng thống kê
                cout << "\n" << "=== THONG KE CHI TIET VE SINH VIEN ===" << endl;
                printDynamicLine(wMaSV, wHoTen, wSoLop, wDiemTB);
                cout << "|" << setw(wMaSV) << left << " Ma SV"
                     << "|" << setw(wHoTen) << left << " Ho va Ten"
                     << "|" << setw(wSoLop) << left << " So Lop DK"
                     << "|" << setw(wDiemTB) << left << " Diem TB" << "|" << endl;
                printDynamicLine(wMaSV, wHoTen, wSoLop, wDiemTB);
                
                for (size_t i = 0; i < stats.size(); i++) {
                    cout << "|" << setw(wMaSV) << left << (" " + stats[i].maSV)
                         << "|" << setw(wHoTen) << left << (" " + stats[i].hoTen)
                         << "|" << " " << setw(wSoLop - 2) << right << stats[i].soLop << " "
                         << "|" << " " << setw(wDiemTB - 2) << right << fixed << setprecision(2) << stats[i].diemTB << " |" << endl;
                }
                printDynamicLine(wMaSV, wHoTen, wSoLop, wDiemTB);
                
                cout << "\n" << "An Enter de tiep tuc..."; 
                cin.get();
            }
            else if (choice == 2) {
                cout << "\n" << "Nhap Ma SV: ";
                string maSV; 
                getline(cin, maSV);
                Student* sv = ls.getStudentTable().searchByString(maSV);
                if (!sv) {
                    cout << " >> Loi: Sinh vien khong ton tai!" << endl;
                    cout << "An Enter de thu lai..."; 
                    cin.get();
                    continue; // Quay lại menu chức năng bổ sung
                }
                cout << "\n" << "--- Thong Tin Chi Tiet Sinh Vien ---" << endl;
                sv->displayInfo();
                
                // Thu thập danh sách lớp đã đăng ký
                MyVector<TranscriptRecord> svTranscript;
                for (size_t i = 0; i < listLopHP.size(); i++) {
                    for (size_t j = 0; j < listDK.size(); j++) {
                        if (listDK[j].getMaSV_FK() == maSV && listDK[j].getMaLHP_FK() == listLopHP[i].getMaLopHP()) {
                            TranscriptRecord tr;
                            tr.maLop = listLopHP[i].getMaLopHP();
                            tr.tenMon = listLopHP[i].getMonHoc()->getTenMon();
                            tr.diem = listDK[j].getDiem();
                            svTranscript.push_back(tr);
                        }
                    }
                }
                
                if (svTranscript.size() > 0) {
                    // Tính độ rộng cột
                    size_t wMaLop = 8, wTenMon = 15, wDiem = 6;
                    for (size_t k = 0; k < svTranscript.size(); k++) {
                        wMaLop = max(wMaLop, svTranscript[k].maLop.length());
                        wTenMon = max(wTenMon, svTranscript[k].tenMon.length());
                    }
                    wMaLop += 4; wTenMon += 4; wDiem += 4;
                    
                    cout << "\n" << "=== DANH SACH LOP HOC PHAN DA DANG KY ===" << endl;
                    printDynamicLine(wMaLop, wTenMon, wDiem);
                    cout << "|" << setw(wMaLop) << left << " Ma Lop"
                         << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
                         << "|" << setw(wDiem) << left << " Diem" << "|" << endl;
                    printDynamicLine(wMaLop, wTenMon, wDiem);
                    
                    for (size_t k = 0; k < svTranscript.size(); k++) {
                        cout << "|" << setw(wMaLop) << left << (" " + svTranscript[k].maLop)
                             << "|" << setw(wTenMon) << left << (" " + svTranscript[k].tenMon)
                             << "|" << " " << setw(wDiem - 2) << left << fixed << setprecision(2) << svTranscript[k].diem << " |" << endl;
                    }
                    printDynamicLine(wMaLop, wTenMon, wDiem);
                } else {
                    cout << "\n" << " >> Sinh vien chua dang ky lop nao!" << endl;
                }
                
                cout << "\n" << "An Enter de tiep tuc..."; 
                cin.get();
            }
        }
    }
    
    // Menu thống kê cho giảng viên (hiển thị sau bảng)
    if (!viewStudents) {
        while (true) {
            int choice;
            cout << "\n" << "=== CHUC NANG BO SUNG ===" << endl;
            cout << "[1] Xem thong ke chi tiet" << endl;
            cout << "[2] Xem chi tiet ve 1 giang vien bat ki" << endl;
            cout << "[0] Quay lai" << endl;
            cout << "\n" << "Chon chuc nang: ";

            choice = getValidIntInput("", 0, 2);
            
            if (choice == 0) {
                break; // Thoát về menu Admin
            }
            else if (choice == 1) {
                // Thu thập dữ liệu thống kê giảng viên
                struct TeacherStatRecord {
                    string maGV;
                    string hoTen;
                    int soLop;
                    float diemTBChung;
                };
                MyVector<TeacherStatRecord> statsT;
                MyVector<LopHocPhan> allLHP = ls.getLopHPTable().getAllElements();
                
                for (size_t i = 0; i < listT.size(); i++) {
                    int lopCount = 0;
                    float totalAvgDiem = 0.0f;
                    
                    // Đếm số lớp và tính điểm TB chung
                    for (size_t j = 0; j < allLHP.size(); j++) {
                        if (allLHP[j].getGiangVien() && allLHP[j].getGiangVien()->getId() == listT[i].getId()) {
                            lopCount++;
                            
                            // Tính điểm TB của lớp này
                            float lopAvg = 0.0f;
                            int svCount = 0;
                            for (size_t k = 0; k < listDK.size(); k++) {
                                if (listDK[k].getMaLHP_FK() == allLHP[j].getMaLopHP()) {
                                    lopAvg += listDK[k].getDiem();
                                    svCount++;
                                }
                            }
                            if (svCount > 0) totalAvgDiem += (lopAvg / svCount);
                        }
                    }
                    
                    float avgDiem = (lopCount > 0) ? (totalAvgDiem / lopCount) : 0.0f;
                    
                    TeacherStatRecord tsr;
                    tsr.maGV = listT[i].getId();
                    tsr.hoTen = listT[i].getName();
                    tsr.soLop = lopCount;
                    tsr.diemTBChung = avgDiem;
                    statsT.push_back(tsr);
                }
                
                // Tính độ rộng cột
                size_t wMaGV = 8, wHoTen = 15, wSoLop = 10, wDiemTB = 12;
                for (size_t i = 0; i < statsT.size(); i++) {
                    wMaGV = max(wMaGV, statsT[i].maGV.length());
                    wHoTen = max(wHoTen, statsT[i].hoTen.length());
                }
                wMaGV += 4; wHoTen += 4; wSoLop += 4; wDiemTB += 4;
                
                // In bảng thống kê
                cout << "\n" << "=== THONG KE CHI TIET VE GIANG VIEN ===" << endl;
                printDynamicLine(wMaGV, wHoTen, wSoLop, wDiemTB);
                cout << "|" << setw(wMaGV) << left << " Ma GV"
                     << "|" << setw(wHoTen) << left << " Ho va Ten"
                     << "|" << setw(wSoLop) << left << " So Lop Day"
                     << "|" << setw(wDiemTB) << left << " Diem TB" << "|" << endl;
                printDynamicLine(wMaGV, wHoTen, wSoLop, wDiemTB);
                
                for (size_t i = 0; i < statsT.size(); i++) {
                    cout << "|" << setw(wMaGV) << left << (" " + statsT[i].maGV)
                         << "|" << setw(wHoTen) << left << (" " + statsT[i].hoTen)
                         << "|" << " " << setw(wSoLop - 2) << right << statsT[i].soLop << " "
                         << "|" << " " << setw(wDiemTB - 2) << right << fixed << setprecision(2) << statsT[i].diemTBChung << " |" << endl;
                }
                printDynamicLine(wMaGV, wHoTen, wSoLop, wDiemTB);
                
                cout << "\n" << "An Enter de tiep tuc..."; 
                cin.get();
            }
            else if (choice == 2) {
                cout << "\n" << "Nhap Ma GV: ";
                string maGV; 
                getline(cin, maGV);
                Teacher* gv = ls.getTeacherTable().searchByString(maGV);
                if (!gv) {
                    cout << " >> Loi: Giang vien khong ton tai!" << endl;
                    cout << "An Enter de thu lai..."; 
                    cin.get();
                    continue;
                }
                cout << "\n" << "--- Thong Tin Chi Tiet Giang Vien ---" << endl;
                gv->displayInfo();
                
                // Thu thập danh sách lớp đang dạy
                struct TeacherClassRecord {
                    string maLop;
                    string tenMon;
                    int siSo;
                    float diemTB;
                };
                MyVector<TeacherClassRecord> gvClasses;
                MyVector<LopHocPhan> allLHP = ls.getLopHPTable().getAllElements();
                
                for (size_t i = 0; i < allLHP.size(); i++) {
                    if (allLHP[i].getGiangVien() && allLHP[i].getGiangVien()->getId() == maGV) {
                        TeacherClassRecord tcr;
                        tcr.maLop = allLHP[i].getMaLopHP();
                        tcr.tenMon = allLHP[i].getMonHoc()->getTenMon();
                        
                        // Tính sĩ số và điểm TB của lớp
                        int count = 0;
                        float totalDiem = 0.0f;
                        for (size_t j = 0; j < listDK.size(); j++) {
                            if (listDK[j].getMaLHP_FK() == tcr.maLop) {
                                count++;
                                totalDiem += listDK[j].getDiem();
                            }
                        }
                        tcr.siSo = count;
                        tcr.diemTB = (count > 0) ? (totalDiem / count) : 0.0f;
                        gvClasses.push_back(tcr);
                    }
                }
                
                if (gvClasses.size() > 0) {
                    // Tính độ rộng cột
                    size_t wMaLop = 8, wTenMon = 15, wSiSo = 8, wDiem = 10;
                    for (size_t k = 0; k < gvClasses.size(); k++) {
                        wMaLop = max(wMaLop, gvClasses[k].maLop.length());
                        wTenMon = max(wTenMon, gvClasses[k].tenMon.length());
                    }
                    wMaLop += 4; wTenMon += 4; wSiSo += 4; wDiem += 4;
                    
                    cout << "\n" << "=== DANH SACH LOP HOC PHAN DANG DAY ===" << endl;
                    printDynamicLine(wMaLop, wTenMon, wSiSo, wDiem);
                    cout << "|" << setw(wMaLop) << left << " Ma Lop"
                         << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
                         << "|" << setw(wSiSo) << left << " Si So"
                         << "|" << setw(wDiem) << left << " Diem TB" << "|" << endl;
                    printDynamicLine(wMaLop, wTenMon, wSiSo, wDiem);
                    
                    for (size_t k = 0; k < gvClasses.size(); k++) {
                        cout << "|" << setw(wMaLop) << left << (" " + gvClasses[k].maLop)
                             << "|" << setw(wTenMon) << left << (" " + gvClasses[k].tenMon)
                             << "|" << " " << setw(wSiSo - 2) << right << gvClasses[k].siSo << " "
                             << "|" << " " << setw(wDiem - 2) << right << fixed << setprecision(2) << gvClasses[k].diemTB << " |" << endl;
                    }
                    printDynamicLine(wMaLop, wTenMon, wSiSo, wDiem);
                } else {
                    cout << "\n" << " >> Giang vien chua duoc phan cong lop nao!" << endl;
                }
                
                cout << "\n" << "An Enter de tiep tuc..."; 
                cin.get();
            }
        }
    }
}

// --- LOGIC XEM ĐIỂM CỦA ADMIN (CHỨC NĂNG MỚI) ---
void AdminViewClassGrades(LoginSystem& ls) {
    cout << "\n" << "Nhap Ma Lop Hoc Phan can xem diem: ";
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
    cout << "\n" << "=== BANG DIEM LOP: " << maLop << " - " << lhp->getMonHoc()->getTenMon() << " ===" << endl;
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
    cout << "\n" << "--- THONG KE PHO DIEM ---" << endl;
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
    cout << "\n" << "=== PHO DIEM LOP: " << maLop << " (Si so: " << count << ") ===" << endl;
    
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

        cout << "\n" << "[1] Them SV vao lop | [2] Xem Thong ke | [3] Xem Danh Sach & Diem | [4] Quay lai: ";
        int choice = getValidIntInput("", 1, 4);

        if (choice == 1) {
            string lhp, sv; cout << "\n" << "Nhap Ma Lop: "; getline(cin, lhp); cout << "Nhap Ma SV:  "; getline(cin, sv); 
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

void AdminManageAccounts(LoginSystem& ls) {
    while (true) {
        clearScreen();
        cout << "=== QUAN LY TAI KHOAN ===" << endl;
        
        // Hiển thị tổng quan
        MyVector<Student> allS = ls.getStudentTable().getAllElements();
        MyVector<Teacher> allT = ls.getTeacherTable().getAllElements();
        cout << "\n" << " >> Tong so Sinh Vien: " << allS.size() << endl;
        cout << " >> Tong so Giang Vien: " << allT.size() << endl;
        cout << " >> Tong so Tai Khoan: " << (allS.size() + allT.size()) << endl;
        
        cout << "\n" << "=== MENU ===" << endl;
        cout << "[1] Xem danh sach Sinh Vien" << endl;
        cout << "[2] Xem danh sach Giang Vien" << endl;
        cout << "[3] Them Sinh Vien moi" << endl;
        cout << "[4] Them Giang Vien moi" << endl;
        cout << "[5] Xoa Sinh Vien" << endl;
        cout << "[6] Xoa Giang Vien" << endl;
        cout << "[7] Quay lai Menu Admin" << endl;
        
        int choice = getValidIntInput("\nChon chuc nang: ", 1, 7);
        
        if (choice == 7) break;
        
        if (choice == 1) {
            // Xem danh sách tài khoản sinh viên
            clearScreen();
            cout << "=== DANH SACH TAI KHOAN SINH VIEN ===" << endl;
            
            struct AccountInfo {
                string id;
                string name;
                string password;
                string khoa;
                string userType;
            };
            MyVector<AccountInfo> accountList;
            
            MyVector<Student> listS = ls.getStudentTable().getAllElements();
            sort(listS.begin(), listS.end(), cmpStudentByID);
            
            for (size_t i = 0; i < listS.size(); i++) {
                AccountInfo acc;
                acc.id = listS[i].getId();
                acc.name = listS[i].getName();
                
                // Lấy mật khẩu từ Account table
                Account* account = ls.getAccountTable().searchByString(acc.id);
                acc.password = account ? account->getPassword() : "N/A";
                
                acc.khoa = listS[i].getKhoa() ? listS[i].getKhoa()->getMaKhoa() : "N/A";
                acc.userType = "SINH VIEN";
                accountList.push_back(acc);
            }
            
            size_t wID = 10, wName = 20, wPass = 10, wKhoa = 8, wType = 12;
            for (size_t i = 0; i < accountList.size(); i++) {
                wID = max(wID, accountList[i].id.length());
                wName = max(wName, accountList[i].name.length());
                wPass = max(wPass, accountList[i].password.length());
                wKhoa = max(wKhoa, accountList[i].khoa.length());
            }
            wID += 4; wName += 4; wPass += 4; wKhoa += 4; wType += 4;
            
            printDynamicLine(wID, wName, wPass, wKhoa, wType);
            cout << "|" << setw(wID) << left << " Ma TK"
                 << "|" << setw(wName) << left << " Ho va Ten"
                 << "|" << setw(wPass) << left << " Mat Khau"
                 << "|" << setw(wKhoa) << left << " Khoa"
                 << "|" << setw(wType) << left << " Loai TK" << "|" << endl;
            printDynamicLine(wID, wName, wPass, wKhoa, wType);
            
            for (size_t i = 0; i < accountList.size(); i++) {
                cout << "|" << setw(wID) << left << (" " + accountList[i].id)
                     << "|" << setw(wName) << left << (" " + accountList[i].name)
                     << "|" << setw(wPass) << left << (" " + accountList[i].password)
                     << "|" << setw(wKhoa) << left << (" " + accountList[i].khoa)
                     << "|" << setw(wType) << left << (" " + accountList[i].userType) << "|" << endl;
            }
            printDynamicLine(wID, wName, wPass, wKhoa, wType);
            
            cout << "\n" << " >> Tong so tai khoan: " << accountList.size() << endl;
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
        else if (choice == 2) {
            // Xem danh sách tài khoản giảng viên
            clearScreen();
            cout << "=== DANH SACH TAI KHOAN GIANG VIEN ===" << endl;
            
            struct AccountInfo {
                string id;
                string name;
                string password;
                string khoa;
                string userType;
            };
            MyVector<AccountInfo> accountList;
            
            MyVector<Teacher> listT = ls.getTeacherTable().getAllElements();
            sort(listT.begin(), listT.end(), cmpTeacherByID);
            
            for (size_t i = 0; i < listT.size(); i++) {
                AccountInfo acc;
                acc.id = listT[i].getId();
                acc.name = listT[i].getName();
                
                // Lấy mật khẩu từ Account table
                Account* account = ls.getAccountTable().searchByString(acc.id);
                acc.password = account ? account->getPassword() : "N/A";
                
                acc.khoa = listT[i].getKhoa() ? listT[i].getKhoa()->getMaKhoa() : "N/A";
                acc.userType = "GIANG VIEN";
                accountList.push_back(acc);
            }
            
            size_t wID = 12, wName = 20, wPass = 10, wKhoa = 8, wType = 12;
            for (size_t i = 0; i < accountList.size(); i++) {
                wID = max(wID, accountList[i].id.length());
                wName = max(wName, accountList[i].name.length());
                wPass = max(wPass, accountList[i].password.length());
                wKhoa = max(wKhoa, accountList[i].khoa.length());
            }
            wID += 4; wName += 4; wPass += 4; wKhoa += 4; wType += 4;
            
            printDynamicLine(wID, wName, wPass, wKhoa, wType);
            cout << "|" << setw(wID) << left << " Ma TK"
                 << "|" << setw(wName) << left << " Ho va Ten"
                 << "|" << setw(wPass) << left << " Mat Khau"
                 << "|" << setw(wKhoa) << left << " Khoa"
                 << "|" << setw(wType) << left << " Loai TK" << "|" << endl;
            printDynamicLine(wID, wName, wPass, wKhoa, wType);
            
            for (size_t i = 0; i < accountList.size(); i++) {
                cout << "|" << setw(wID) << left << (" " + accountList[i].id)
                     << "|" << setw(wName) << left << (" " + accountList[i].name)
                     << "|" << setw(wPass) << left << (" " + accountList[i].password)
                     << "|" << setw(wKhoa) << left << (" " + accountList[i].khoa)
                     << "|" << setw(wType) << left << (" " + accountList[i].userType) << "|" << endl;
            }
            printDynamicLine(wID, wName, wPass, wKhoa, wType);
            
            cout << "\n" << " >> Tong so tai khoan: " << accountList.size() << endl;
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
        else if (choice == 3) {
            // Thêm sinh viên
            clearScreen();
            cout << "=== THEM SINH VIEN MOI ===" << endl;
            string name; 
            cout << "Ho Ten: "; getline(cin, name); 
            for(auto& c:name) c=toupper(c);
            
            cout << "\n" << "Danh sach Khoa: "; 
            MyVector<Khoa> ks = ls.getKhoaTable().getAllElements();
            for(size_t i=0; i<ks.size(); i++) cout << ks[i].getMaKhoa() << " ";
            
            cout << "\n" << "Nhap Ma Khoa (VD: MK10): "; 
            string mk; getline(cin, mk); 
            for(auto& c:mk) c=toupper(c);
            
            if(!ls.getKhoaTable().contains(mk)) { 
                cout << " >> Loi: Ma Khoa khong hop le!" << endl; 
                cout << "An Enter..."; cin.get();
                continue;
            }
            
            int year = getValidIntInput("Nhap Khoa (Nam nhap hoc, VD 25): ", 0, 99);
            string id = ls.addNewStudent(name, mk, year);
            
            if(id != "") {
                cout << "\n" << "[THANH CONG] ID: " << id << " | Pass: 123 | Ten: " << name << endl;
                ls.saveStudentsToFile("data/Student.txt");
                ls.saveAccountsToFile("data/Account.txt");
            } else {
                cout << " >> Loi tao ID!" << endl;
            }
            cout << "An Enter..."; cin.get();
        }
        else if (choice == 4) {
            // Thêm giảng viên
            clearScreen();
            cout << "=== THEM GIANG VIEN MOI ===" << endl;
            string name; 
            cout << "Ho Ten: "; getline(cin, name); 
            for(auto& c:name) c=toupper(c);
            
            cout << "\n" << "Danh sach Khoa: "; 
            MyVector<Khoa> ks = ls.getKhoaTable().getAllElements();
            for(size_t i=0; i<ks.size(); i++) cout << ks[i].getMaKhoa() << " ";
            
            cout << "\n" << "Nhap Ma Khoa (VD: MK10): "; 
            string mk; getline(cin, mk); 
            for(auto& c:mk) c=toupper(c);
            
            if(!ls.getKhoaTable().contains(mk)) { 
                cout << " >> Loi: Ma Khoa khong hop le!" << endl; 
                cout << "An Enter..."; cin.get();
                continue;
            }
            
            string id = ls.addNewTeacher(name, mk);
            
            if(id != "") {
                cout << "\n" << "[THANH CONG] ID: " << id << " | Pass: 123 | Ten: " << name << endl;
                ls.saveTeachersToFile("data/Teacher.txt");
                ls.saveAccountsToFile("data/Account.txt");
            } else {
                cout << " >> Loi tao ID!" << endl;
            }
            cout << "An Enter..."; cin.get();
        }
        else if (choice == 5) {
            // Xóa sinh viên
            clearScreen();
            cout << "=== XOA SINH VIEN ===" << endl;
            
            // Hiển thị danh sách
            MyVector<Student> listS = ls.getStudentTable().getAllElements();
            sort(listS.begin(), listS.end(), cmpStudentByID);
            
            size_t wID = 10, wName = 20, wKhoa = 8;
            for (size_t i = 0; i < listS.size(); i++) {
                wID = max(wID, listS[i].getId().length());
                wName = max(wName, listS[i].getName().length());
                string k = listS[i].getKhoa() ? listS[i].getKhoa()->getMaKhoa() : "N/A";
                wKhoa = max(wKhoa, k.length());
            }
            wID += 4; wName += 4; wKhoa += 4;
            
            printDynamicLine(wID, wName, wKhoa);
            cout << "|" << setw(wID) << left << " Ma SV"
                 << "|" << setw(wName) << left << " Ho va Ten"
                 << "|" << setw(wKhoa) << left << " Khoa" << "|" << endl;
            printDynamicLine(wID, wName, wKhoa);
            
            for (size_t i = 0; i < listS.size(); i++) {
                string k = listS[i].getKhoa() ? listS[i].getKhoa()->getMaKhoa() : "N/A";
                cout << "|" << setw(wID) << left << (" " + listS[i].getId())
                     << "|" << setw(wName) << left << (" " + listS[i].getName())
                     << "|" << setw(wKhoa) << left << (" " + k) << "|" << endl;
            }
            printDynamicLine(wID, wName, wKhoa);
            
            cout << "\n" << "Nhap Ma SV can xoa (hoac Enter de huy): ";
            string maSV; getline(cin, maSV);
            if (maSV.empty()) continue;
            
            if (!ls.getStudentTable().contains(maSV)) {
                cout << " >> Loi: Sinh vien khong ton tai!" << endl;
                cout << "An Enter..."; cin.get();
                continue;
            }
            
            cout << "\n" << "[CANH BAO] Ban co chac muon xoa sinh vien " << maSV << "? (y/n): ";
            char confirm; cin >> confirm; cin.ignore();
            
            if (tolower(confirm) == 'y') {
                // Xóa tất cả đăng ký của sinh viên
                MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
                for (size_t i = 0; i < allDK.size(); i++) {
                    if (allDK[i].getMaSV_FK() == maSV) {
                        ls.getDangKyTable().remove(allDK[i].getKey());
                    }
                }
                
                // Xóa sinh viên
                ls.getStudentTable().remove(maSV);
                
                // Xóa tài khoản
                ls.getAccountTable().remove(maSV);
                
                cout << "\n" << " >> [THANH CONG] Da xoa sinh vien!" << endl;
                ls.saveStudentsToFile("data/Student.txt");
                ls.saveAccountsToFile("data/Account.txt");
                ls.saveDangKyToFile("data/DangKy.txt");
            } else {
                cout << " >> Da huy thao tac." << endl;
            }
            cout << "An Enter..."; cin.get();
        }
        else if (choice == 6) {
            // Xóa giảng viên
            clearScreen();
            cout << "=== XOA GIANG VIEN ===" << endl;
            
            // Hiển thị danh sách
            MyVector<Teacher> listT = ls.getTeacherTable().getAllElements();
            sort(listT.begin(), listT.end(), cmpTeacherByID);
            
            size_t wID = 12, wName = 20, wKhoa = 8;
            for (size_t i = 0; i < listT.size(); i++) {
                wID = max(wID, listT[i].getId().length());
                wName = max(wName, listT[i].getName().length());
                string k = listT[i].getKhoa() ? listT[i].getKhoa()->getMaKhoa() : "N/A";
                wKhoa = max(wKhoa, k.length());
            }
            wID += 4; wName += 4; wKhoa += 4;
            
            printDynamicLine(wID, wName, wKhoa);
            cout << "|" << setw(wID) << left << " Ma GV"
                 << "|" << setw(wName) << left << " Ho va Ten"
                 << "|" << setw(wKhoa) << left << " Khoa" << "|" << endl;
            printDynamicLine(wID, wName, wKhoa);
            
            for (size_t i = 0; i < listT.size(); i++) {
                string k = listT[i].getKhoa() ? listT[i].getKhoa()->getMaKhoa() : "N/A";
                cout << "|" << setw(wID) << left << (" " + listT[i].getId())
                     << "|" << setw(wName) << left << (" " + listT[i].getName())
                     << "|" << setw(wKhoa) << left << (" " + k) << "|" << endl;
            }
            printDynamicLine(wID, wName, wKhoa);
            
            cout << "\n" << "Nhap Ma GV can xoa (hoac Enter de huy): ";
            string maGV; getline(cin, maGV);
            if (maGV.empty()) continue;
            
            if (!ls.getTeacherTable().contains(maGV)) {
                cout << " >> Loi: Giang vien khong ton tai!" << endl;
                cout << "An Enter..."; cin.get();
                continue;
            }
            
            // Kiểm tra xem giảng viên có đang dạy lớp nào không
            MyVector<LopHocPhan> allLHP = ls.getLopHPTable().getAllElements();
            int classCount = 0;
            for (size_t i = 0; i < allLHP.size(); i++) {
                if (allLHP[i].getGiangVien() && allLHP[i].getGiangVien()->getId() == maGV) {
                    classCount++;
                }
            }
            
            if (classCount > 0) {
                cout << "\n" << "[CANH BAO] Giang vien dang phan cong " << classCount << " lop hoc phan!" << endl;
                cout << "Ban phai huy phan cong truoc khi xoa." << endl;
                cout << "An Enter..."; cin.get();
                continue;
            }
            
            cout << "\n" << "[CANH BAO] Ban co chac muon xoa giang vien " << maGV << "? (y/n): ";
            char confirm; cin >> confirm; cin.ignore();
            
            if (tolower(confirm) == 'y') {
                // Xóa giảng viên
                ls.getTeacherTable().remove(maGV);
                
                // Xóa tài khoản
                ls.getAccountTable().remove(maGV);
                
                cout << "\n" << " >> [THANH CONG] Da xoa giang vien!" << endl;
                ls.saveTeachersToFile("data/Teacher.txt");
                ls.saveAccountsToFile("data/Account.txt");
            } else {
                cout << " >> Da huy thao tac." << endl;
            }
            cout << "An Enter..."; cin.get();
        }
    }
}

void AdminAddAccount(LoginSystem& ls) {
    clearScreen(); cout << "=== THEM TAI KHOAN MOI ===" << endl;
    cout << "1. Sinh Vien\n2. Giang Vien\nChon: "; int type = getValidIntInput("",1,2);
    string name; cout << "Ho Ten: "; getline(cin, name); for(auto& c:name) c=toupper(c);
    cout << "\n" << "Danh sach Khoa: "; 
    MyVector<Khoa> ks = ls.getKhoaTable().getAllElements();
    for(size_t i=0; i<ks.size(); i++) cout << ks[i].getMaKhoa() << " ";
    cout << "\n" << "Nhap Ma Khoa (VD: MK10): "; string mk; getline(cin, mk); for(auto& c:mk) c=toupper(c);
    if(!ls.getKhoaTable().contains(mk)) { cout << " >> Loi: Ma Khoa khong hop le!" << endl; return; }
    string id = "";
    if (type == 1) {    
        int year = getValidIntInput("Nhap Khoa (Nam nhap hoc, VD 25): ", 0, 99);
        id = ls.addNewStudent(name, mk, year); 
    } else {
        id = ls.addNewTeacher(name, mk);
    }
    if(id!="") {
        cout << "\n" << "[THANH CONG] ID: " << id << " | Pass: 123 | Ten: " << name << endl;
        ls.saveStudentsToFile("data/Student.txt"); ls.saveTeachersToFile("data/Teacher.txt"); ls.saveAccountsToFile("data/Account.txt");
    } else cout << " >> Loi tao ID!" << endl;
}

void AdminChangeDept(LoginSystem& ls) {
    while (true) {
        clearScreen();
        cout << "=== CHUYEN DOI KHOA ===" << endl;
        cout << "1. Chuyen Khoa Sinh Vien" << endl;
        cout << "2. Chuyen Khoa Giang Vien" << endl;
        cout << "3. Quay lai" << endl;
        
        int type = getValidIntInput("Chon chuc nang: ", 1, 3);
        if (type == 3) return;

        bool isStudent = (type == 1);

        // --- BƯỚC 1: HIỂN THỊ DANH SÁCH USER ĐỂ CHỌN ---
        cout << "\n" << "--- DANH SACH " << (isStudent ? "SINH VIEN" : "GIANG VIEN") << " HIEN TAI ---" << endl;
        
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
        cout << "\n" << "Nhap ID can chuyen khoa (hoac Enter de quay lai): ";
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
        cout << "\n" << "--- DANH SACH KHOA KHA DUNG ---" << endl;
        MyVector<Khoa> khoas = ls.getKhoaTable().getAllElements();
        for(size_t i = 0; i < khoas.size(); i++) {
            cout << "- " << setw(6) << left << khoas[i].getMaKhoa() << ": " << khoas[i].getTenKhoa() << endl;
        }

        cout << "\n" << "Nhap MA KHOA moi (VD: MK1): ";
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
        cout << "\n" << "[XAC NHAN] Ban co chac muon chuyen " << id << " sang khoa " << mk << "? (y/n): ";
        char confirm; cin >> confirm; cin.ignore();

        if (tolower(confirm) == 'y') {
            bool ok = isStudent 
                    ? ls.changeStudentDepartment(id, mk) 
                    : ls.changeTeacherDepartment(id, mk);
            
            if (ok) {
                cout << "\n" << " >> [THANH CONG] Da chuyen khoa!" << endl;
                cout << " >> LUY Y: ID cua nguoi dung da thay doi theo khoa moi." << endl;
                cout << " >> Vui long luu du lieu va khoi dong lai chuong trinh de cap nhat." << endl;
            } else {
                cout << "\n" << " >> [THAT BAI] Co loi xay ra (Co the do ID moi bi trung lap)." << endl;
            }
        } else {
            cout << " >> Da huy thao tac." << endl;
        }
        cout << "An Enter de tiep tuc..."; cin.get();
    }
}

void AdminManageDepartments(LoginSystem& ls) {
    while (true) {
        clearScreen();
        cout << "============================================" << endl;
        cout << "       QUAN LY KHOA" << endl;
        cout << "============================================" << endl;
        
        // --- HIỂN THỊ DANH SÁCH KHOA (MẶC ĐỊNH) ---
        MyVector<Khoa> khoas = ls.getKhoaTable().getAllElements();
        
        if (khoas.empty()) {
            cout << "\n" << " >> Danh sach khoa rong!" << endl;
        } else {
            // Tính độ rộng cột
            size_t wMa = 8, wTen = 15;
            for (size_t i = 0; i < khoas.size(); i++) {
                wMa = max(wMa, khoas[i].getMaKhoa().length());
                wTen = max(wTen, khoas[i].getTenKhoa().length());
            }
            wMa += 4; wTen += 4;
            
            // In bảng
            printDynamicLine(wMa, wTen);
            cout << "|" << setw(wMa) << left << " Ma Khoa"
                 << "|" << setw(wTen) << left << " Ten Khoa" << "|" << endl;
            printDynamicLine(wMa, wTen);
            
            for (size_t i = 0; i < khoas.size(); i++) {
                cout << "|" << setw(wMa) << left << (" " + khoas[i].getMaKhoa())
                     << "|" << setw(wTen) << left << (" " + khoas[i].getTenKhoa()) << "|" << endl;
            }
            printDynamicLine(wMa, wTen);
        }
        
        // --- MENU CÁC CHỨC NĂNG PHỤ ---
        cout << "\n" << "============================================" << endl;
        cout << "[1] Chuyen khoa sinh vien / giang vien" << endl;
        cout << "[2] Xem thong ke tung khoa" << endl;
        cout << "[3] Xem chi tiet mot khoa" << endl;
        cout << "[0] Quay lai Menu Admin" << endl;
        cout << "============================================" << endl;
        
        int choice = getValidIntInput("Chon chuc nang: ", 0, 3);
        
        if (choice == 0) break;
        
        if (choice == 1) {
            // --- CHUYỂN KHOA SV/GV ---
            AdminChangeDept(ls);
        }
        else if (choice == 2) {
            // --- THỐNG KÊ TỪNG KHOA ---
            clearScreen();
            cout << "=== THONG KE TUNG KHOA ===" << endl;
            
            MyVector<Khoa> khoas = ls.getKhoaTable().getAllElements();
            if (khoas.empty()) {
                cout << " >> Khong co khoa nao!" << endl;
                cout << "An Enter de tiep tuc..."; cin.get();
                continue;
            }
            
            MyVector<Student> allSV = ls.getStudentTable().getAllElements();
            MyVector<Teacher> allGV = ls.getTeacherTable().getAllElements();
            MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
            
            MyVector<DepartmentStatRecord> stats;
            
            // Tính thống kê cho từng khoa
            for (size_t i = 0; i < khoas.size(); i++) {
                DepartmentStatRecord rec;
                rec.maKhoa = khoas[i].getMaKhoa();
                rec.tenKhoa = khoas[i].getTenKhoa();
                rec.soSV = 0;
                rec.soGV = 0;
                
                // Đếm số sinh viên thuộc khoa
                for (size_t j = 0; j < allSV.size(); j++) {
                    if (allSV[j].getKhoa() && allSV[j].getKhoa()->getMaKhoa() == rec.maKhoa) {
                        rec.soSV++;
                    }
                }
                
                // Đếm số giảng viên thuộc khoa
                for (size_t j = 0; j < allGV.size(); j++) {
                    if (allGV[j].getKhoa() && allGV[j].getKhoa()->getMaKhoa() == rec.maKhoa) {
                        rec.soGV++;
                    }
                }
                
                stats.push_back(rec);
            }
            
            // Sắp xếp theo mã khoa
            sort(stats.begin(), stats.end(), cmpDeptByID);
            
            // Tính độ rộng cột
            size_t wMa = 8, wTen = 15, wSV = 6, wGV = 6;
            for (size_t i = 0; i < stats.size(); i++) {
                wMa = max(wMa, stats[i].maKhoa.length());
                wTen = max(wTen, stats[i].tenKhoa.length());
            }
            wMa += 4; wTen += 4; wSV += 4; wGV += 4;
            
            // In bảng thống kê
            printDynamicLine(wMa, wTen, wSV, wGV);
            cout << "|" << setw(wMa) << left << " Ma Khoa"
                 << "|" << setw(wTen) << left << " Ten Khoa"
                 << "|" << setw(wSV) << left << " So SV"
                 << "|" << setw(wGV) << left << " So GV" << "|" << endl;
            printDynamicLine(wMa, wTen, wSV, wGV);
            
            for (size_t i = 0; i < stats.size(); i++) {
                cout << "|" << setw(wMa) << left << (" " + stats[i].maKhoa)
                     << "|" << setw(wTen) << left << (" " + stats[i].tenKhoa)
                     << "|" << " " << setw(wSV - 2) << right << stats[i].soSV << " |"
                     << "|" << " " << setw(wGV - 2) << right << stats[i].soGV << " |" << endl;
            }
            printDynamicLine(wMa, wTen, wSV, wGV);
            
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
        else if (choice == 3) {
            // --- XEM CHI TIẾT MỘT KHOA ---
            clearScreen();
            cout << "=== XEM CHI TIET KHOA ===" << endl;
            
            // Hiển thị danh sách khoa để chọn
            MyVector<Khoa> khoas = ls.getKhoaTable().getAllElements();
            if (khoas.empty()) {
                cout << " >> Khong co khoa nao!" << endl;
                cout << "An Enter de tiep tuc..."; cin.get();
                continue;
            }
            
            cout << "\n" << "--- DANH SACH KHOA ---" << endl;
            for (size_t i = 0; i < khoas.size(); i++) {
                cout << "- " << setw(6) << left << khoas[i].getMaKhoa() << ": " << khoas[i].getTenKhoa() << endl;
            }
            
            cout << "\n" << "Nhap MA KHOA can xem chi tiet (hoac Enter de quay lai): ";
            string maKhoa;
            getline(cin, maKhoa);
            if (maKhoa.empty()) continue;
            
            for(auto& c : maKhoa) c = toupper(c);
            
            // Kiểm tra khoa tồn tại
            Khoa* khoa = ls.getKhoaTable().searchByString(maKhoa);
            if (!khoa) {
                cout << " >> LOI: Ma khoa '" << maKhoa << "' khong ton tai!" << endl;
                cout << "An Enter thu lai..."; cin.get();
                continue;
            }
            
            clearScreen();
            cout << "=== CHI TIET KHOA: " << khoa->getTenKhoa() << " (" << khoa->getMaKhoa() << ") ===" << endl;
            
            // --- DANH SÁCH SINH VIÊN THUỘC KHOA ---
            cout << "\n" << "--- DANH SACH SINH VIEN ---" << endl;
            MyVector<Student> allSV = ls.getStudentTable().getAllElements();
            MyVector<Student> svKhoa;
            
            for (size_t i = 0; i < allSV.size(); i++) {
                if (allSV[i].getKhoa() && allSV[i].getKhoa()->getMaKhoa() == maKhoa) {
                    svKhoa.push_back(allSV[i]);
                }
            }
            
            if (svKhoa.empty()) {
                cout << " >> Khong co sinh vien nao!" << endl;
            } else {
                sort(svKhoa.begin(), svKhoa.end(), cmpStudentByID);
                
                size_t wID = 8, wName = 15;
                for (size_t i = 0; i < svKhoa.size(); i++) {
                    wID = max(wID, svKhoa[i].getId().length());
                    wName = max(wName, svKhoa[i].getName().length());
                }
                wID += 4; wName += 4;
                
                printDynamicLine(wID, wName);
                cout << "|" << setw(wID) << left << " Ma SV"
                     << "|" << setw(wName) << left << " Ho va Ten" << "|" << endl;
                printDynamicLine(wID, wName);
                
                for (size_t i = 0; i < svKhoa.size(); i++) {
                    cout << "|" << setw(wID) << left << (" " + svKhoa[i].getId())
                         << "|" << setw(wName) << left << (" " + svKhoa[i].getName()) << "|" << endl;
                }
                printDynamicLine(wID, wName);
            }
            
            // --- DANH SÁCH GIẢNG VIÊN THUỘC KHOA ---
            cout << "\n" << "--- DANH SACH GIANG VIEN ---" << endl;
            MyVector<Teacher> allGV = ls.getTeacherTable().getAllElements();
            MyVector<Teacher> gvKhoa;
            
            for (size_t i = 0; i < allGV.size(); i++) {
                if (allGV[i].getKhoa() && allGV[i].getKhoa()->getMaKhoa() == maKhoa) {
                    gvKhoa.push_back(allGV[i]);
                }
            }
            
            if (gvKhoa.empty()) {
                cout << " >> Khong co giang vien nao!" << endl;
            } else {
                sort(gvKhoa.begin(), gvKhoa.end(), cmpTeacherByID);
                
                size_t wID = 8, wName = 15;
                for (size_t i = 0; i < gvKhoa.size(); i++) {
                    wID = max(wID, gvKhoa[i].getId().length());
                    wName = max(wName, gvKhoa[i].getName().length());
                }
                wID += 4; wName += 4;
                
                printDynamicLine(wID, wName);
                cout << "|" << setw(wID) << left << " Ma GV"
                     << "|" << setw(wName) << left << " Ho va Ten" << "|" << endl;
                printDynamicLine(wID, wName);
                
                for (size_t i = 0; i < gvKhoa.size(); i++) {
                    cout << "|" << setw(wID) << left << (" " + gvKhoa[i].getId())
                         << "|" << setw(wName) << left << (" " + gvKhoa[i].getName()) << "|" << endl;
                }
                printDynamicLine(wID, wName);
            }
            
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
    }
}

void AdminManageRegistrations(LoginSystem& ls) {
    while (true) {
        clearScreen();
        cout << "============================================" << endl;
        cout << "       QUAN LY DANG KY" << endl;
        cout << "============================================" << endl;
        
        // --- HIỂN THỊ TẤT CẢ ĐĂNG KÝ (MẶC ĐỊNH) ---
        MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
        
        if (allDK.empty()) {
            cout << "\n" << " >> Danh sach dang ky rong!" << endl;
        } else {
            // Tính độ rộng cột
            size_t wMaLHP = 10, wTenMon = 20, wMaSV = 10, wTenSV = 20, wDiem = 8;
            
            for (size_t i = 0; i < allDK.size(); i++) {
                wMaLHP = max(wMaLHP, allDK[i].getMaLHP_FK().length());
                wMaSV = max(wMaSV, allDK[i].getMaSV_FK().length());
                
                LopHocPhan* lhp = ls.getLopHPTable().searchByString(allDK[i].getMaLHP_FK());
                if (lhp && lhp->getMonHoc()) {
                    wTenMon = max(wTenMon, lhp->getMonHoc()->getTenMon().length());
                }
                
                Student* sv = ls.getStudentTable().searchByString(allDK[i].getMaSV_FK());
                if (sv) {
                    wTenSV = max(wTenSV, sv->getName().length());
                }
            }
            wMaLHP += 4; wTenMon += 4; wMaSV += 4; wTenSV += 4; wDiem += 4;
            
            // In bảng
            printDynamicLine(wMaLHP, wTenMon, wMaSV, wTenSV, wDiem);
            cout << "|" << setw(wMaLHP) << left << " Ma LHP"
                 << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
                 << "|" << setw(wMaSV) << left << " Ma SV"
                 << "|" << setw(wTenSV) << left << " Ten Sinh Vien"
                 << "|" << setw(wDiem) << left << " Diem" << "|" << endl;
            printDynamicLine(wMaLHP, wTenMon, wMaSV, wTenSV, wDiem);
            
            for (size_t i = 0; i < allDK.size(); i++) {
                LopHocPhan* lhp = ls.getLopHPTable().searchByString(allDK[i].getMaLHP_FK());
                Student* sv = ls.getStudentTable().searchByString(allDK[i].getMaSV_FK());
                
                string tenMon = (lhp && lhp->getMonHoc()) ? lhp->getMonHoc()->getTenMon() : "N/A";
                string tenSV = sv ? sv->getName() : "N/A";
                
                cout << "|" << setw(wMaLHP) << left << (" " + allDK[i].getMaLHP_FK())
                     << "|" << setw(wTenMon) << left << (" " + tenMon)
                     << "|" << setw(wMaSV) << left << (" " + allDK[i].getMaSV_FK())
                     << "|" << setw(wTenSV) << left << (" " + tenSV)
                     << "|" << " " << setw(wDiem - 2) << left << fixed << setprecision(2) << allDK[i].getDiem() << " |" << endl;
            }
            printDynamicLine(wMaLHP, wTenMon, wMaSV, wTenSV, wDiem);
            
            cout << "\n" << " >> Tong so dang ky: " << allDK.size() << endl;
        }
        
        // --- MENU CÁC CHỨC NĂNG PHỤ ---
        cout << "\n" << "============================================" << endl;
        cout << "[1] thong ke chi tiet theo 1 mon hoc bat ki" << endl;
        cout << "[2] So luong dang ky tung mon hoc" << endl;
        cout << "[3] Xem chi tiet 1 dang ky" << endl;
        cout << "[0] Quay lai Menu Admin" << endl;
        cout << "============================================" << endl;
        
        int choice = getValidIntInput("Chon chuc nang: ", 0, 3);
        
        if (choice == 0) break;
        
        if (choice == 1) {
            // --- THỐNG KÊ THEO MÔN HỌC ---
            clearScreen();
            cout << "=== THONG KE THEO MON HOC ===" << endl;
            
            cin.ignore(10000, '\n');
            cout << "Nhap ten mon hoc (hoac mot phan): ";
            string keyword;
            getline(cin, keyword);
            
            // Chuyển thành chữ thường để tìm kiếm không phân biệt hoa/thường
            string keywordLower = keyword;
            transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);
            
            MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
            MyVector<DangKy> filteredDK;
            
            for (size_t i = 0; i < allDK.size(); i++) {
                LopHocPhan* lhp = ls.getLopHPTable().searchByString(allDK[i].getMaLHP_FK());
                if (lhp && lhp->getMonHoc()) {
                    string tenMon = lhp->getMonHoc()->getTenMon();
                    string tenMonLower = tenMon;
                    transform(tenMonLower.begin(), tenMonLower.end(), tenMonLower.begin(), ::tolower);
                    
                    if (tenMonLower.find(keywordLower) != string::npos) {
                        filteredDK.push_back(allDK[i]);
                    }
                }
            }
            
            if (filteredDK.empty()) {
                cout << "\n" << " >> Khong tim thay dang ky nao cho mon hoc '" << keyword << "'!" << endl;
            } else {
                cout << "\n" << " >> Tim thay " << filteredDK.size() << " dang ky cho mon hoc '" << keyword << "':" << endl << endl;
                
                size_t wMaLHP = 10, wMaSV = 10, wTenSV = 20, wDiem = 8;
                
                for (size_t i = 0; i < filteredDK.size(); i++) {
                    wMaLHP = max(wMaLHP, filteredDK[i].getMaLHP_FK().length());
                    wMaSV = max(wMaSV, filteredDK[i].getMaSV_FK().length());
                    
                    Student* sv = ls.getStudentTable().searchByString(filteredDK[i].getMaSV_FK());
                    if (sv) {
                        wTenSV = max(wTenSV, sv->getName().length());
                    }
                }
                wMaLHP += 4; wMaSV += 4; wTenSV += 4; wDiem += 4;
                
                printDynamicLine(wMaLHP, wMaSV, wTenSV, wDiem);
                cout << "|" << setw(wMaLHP) << left << " Ma LHP"
                     << "|" << setw(wMaSV) << left << " Ma SV"
                     << "|" << setw(wTenSV) << left << " Ten Sinh Vien"
                     << "|" << setw(wDiem) << left << " Diem" << "|" << endl;
                printDynamicLine(wMaLHP, wMaSV, wTenSV, wDiem);
                
                for (size_t i = 0; i < filteredDK.size(); i++) {
                    Student* sv = ls.getStudentTable().searchByString(filteredDK[i].getMaSV_FK());
                    string tenSV = sv ? sv->getName() : "N/A";
                    
                    cout << "|" << setw(wMaLHP) << left << (" " + filteredDK[i].getMaLHP_FK())
                         << "|" << setw(wMaSV) << left << (" " + filteredDK[i].getMaSV_FK())
                         << "|" << setw(wTenSV) << left << (" " + tenSV)
                         << "|" << " " << setw(wDiem - 2) << left << fixed << setprecision(2) << filteredDK[i].getDiem() << " |" << endl;
                }
                printDynamicLine(wMaLHP, wMaSV, wTenSV, wDiem);
            }
            
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
        else if (choice == 2) {
            // --- SỐ LƯỢNG ĐĂNG KÝ TỪNG MÔN HỌC ---
            clearScreen();
            cout << "=== SO LUONG DANG KY TUNG MON HOC ===" << endl;
            
            MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
            map<string, int> monHocCount; // key: Mã môn học, value: Số lượng đăng ký
            map<string, string> monHocName; // key: Mã môn học, value: Tên môn học
            
            for (size_t i = 0; i < allDK.size(); i++) {
                LopHocPhan* lhp = ls.getLopHPTable().searchByString(allDK[i].getMaLHP_FK());
                if (lhp && lhp->getMonHoc()) {
                    string maMon = lhp->getMonHoc()->getMaMon();
                    string tenMon = lhp->getMonHoc()->getTenMon();
                    
                    monHocCount[maMon]++;
                    monHocName[maMon] = tenMon;
                }
            }
            
            if (monHocCount.empty()) {
                cout << "\n" << " >> Khong co du lieu dang ky nao!" << endl;
            } else {
                size_t wMaMon = 10, wTenMon = 20, wCount = 10;
                
                for (auto& pair : monHocName) {
                    wMaMon = max(wMaMon, pair.first.length());
                    wTenMon = max(wTenMon, pair.second.length());
                }
                wMaMon += 4; wTenMon += 4; wCount += 4;
                
                printDynamicLine(wMaMon, wTenMon, wCount);
                cout << "|" << setw(wMaMon) << left << " Ma Mon"
                     << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
                     << "|" << setw(wCount) << left << " So DK" << "|" << endl;
                printDynamicLine(wMaMon, wTenMon, wCount);
                
                for (auto& pair : monHocCount) {
                    cout << "|" << setw(wMaMon) << left << (" " + pair.first)
                         << "|" << setw(wTenMon) << left << (" " + monHocName[pair.first])
                         << "|" << " " << setw(wCount - 2) << right << pair.second << " |" << endl;
                }
                printDynamicLine(wMaMon, wTenMon, wCount);
                
                cout << "\n" << " >> Tong so mon hoc co dang ky: " << monHocCount.size() << endl;
            }
            
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
        else if (choice == 3) {
            // --- XEM CHI TIẾT 1 ĐĂNG KÝ ---
            clearScreen();
            cout << "=== XEM CHI TIET DANG KY ===" << endl;
            
            cin.ignore(10000, '\n');
            cout << "Nhap ma lop hoc phan: ";
            string maLHP;
            getline(cin, maLHP);
            
            cout << "Nhap ma sinh vien: ";
            string maSV;
            getline(cin, maSV);
            
            string key = maLHP + "-" + maSV;
            DangKy* dk = ls.getDangKyTable().searchByString(key);
            
            if (!dk) {
                cout << "\n" << " >> Khong tim thay dang ky voi Ma LHP: '" << maLHP << "' va Ma SV: '" << maSV << "'!" << endl;
            } else {
                cout << "\n" << "=== THONG TIN CHI TIET DANG KY ===" << endl;
                cout << "------------------------------------" << endl;
                
                // Thông tin lớp học phần
                LopHocPhan* lhp = ls.getLopHPTable().searchByString(dk->getMaLHP_FK());
                if (lhp) {
                    cout << "Ma lop hoc phan: " << lhp->getMaLopHP() << endl;
                    if (lhp->getMonHoc()) {
                        cout << "Ma mon hoc: " << lhp->getMonHoc()->getMaMon() << endl;
                        cout << "Ten mon hoc: " << lhp->getMonHoc()->getTenMon() << endl;
                    }
                    if (lhp->getGiangVien()) {
                        cout << "Giang vien: " << lhp->getGiangVien()->getName() 
                             << " (" << lhp->getGiangVien()->getId() << ")" << endl;
                    }
                } else {
                    cout << "Ma lop hoc phan: " << dk->getMaLHP_FK() << " (Khong tim thay)" << endl;
                }
                
                cout << "------------------------------------" << endl;
                
                // Thông tin sinh viên
                Student* sv = ls.getStudentTable().searchByString(dk->getMaSV_FK());
                if (sv) {
                    cout << "Ma sinh vien: " << sv->getId() << endl;
                    cout << "Ten sinh vien: " << sv->getName() << endl;
                    if (sv->getKhoa()) {
                        cout << "Khoa: " << sv->getKhoa()->getTenKhoa() << endl;
                    }
                } else {
                    cout << "Ma sinh vien: " << dk->getMaSV_FK() << " (Khong tim thay)" << endl;
                }
                
                cout << "------------------------------------" << endl;
                cout << "Diem: " << fixed << setprecision(2) << dk->getDiem() << endl;
                cout << "------------------------------------" << endl;
            }
            
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
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
        cout << "============================================" << endl;
        cout << "       QUAN LY LOP HOC PHAN" << endl;
        cout << "============================================" << endl;

        // 1. Lọc lớp
        MyVector<LopHocPhan> myClasses;
        MyVector<LopHocPhan> allClasses = ls.getLopHPTable().getAllElements();
        for (size_t i = 0; i < allClasses.size(); i++) {
            if (allClasses[i].getGiangVien() && allClasses[i].getGiangVien()->getId() == t->getId()) {
                myClasses.push_back(allClasses[i]);
            }
        }

        if (myClasses.size() == 0) {
            cout << "\n" << " >> Ban chua co lop hoc phan nao!" << endl;
            cout << "An Enter..."; cin.get();
            return;
        }

        // 2. Hiển thị danh sách lớp với thông tin chi tiết
        cout << "\n" << "--- DANH SACH LOP HOC PHAN DANG DAY ---" << endl;
        
        // Tính độ rộng cột
        size_t wMaLop = 10, wTenMon = 20, wSoSV = 8;
        MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
        
        for (size_t i = 0; i < myClasses.size(); i++) {
            wMaLop = max(wMaLop, myClasses[i].getMaLopHP().length());
            if (myClasses[i].getMonHoc()) {
                wTenMon = max(wTenMon, myClasses[i].getMonHoc()->getTenMon().length());
            }
        }
        wMaLop += 4; wTenMon += 4; wSoSV += 4;
        
        printDynamicLine(wMaLop, wTenMon, wSoSV);
        cout << "|" << setw(wMaLop) << left << " Ma LHP"
             << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
             << "|" << setw(wSoSV) << left << " So SV" << "|" << endl;
        printDynamicLine(wMaLop, wTenMon, wSoSV);
        
        for (size_t i = 0; i < myClasses.size(); i++) {
            // Đếm số sinh viên trong lớp
            int countSV = 0;
            for (size_t j = 0; j < allDK.size(); j++) {
                if (allDK[j].getMaLHP_FK() == myClasses[i].getMaLopHP()) {
                    countSV++;
                }
            }
            
            string tenMon = myClasses[i].getMonHoc() ? myClasses[i].getMonHoc()->getTenMon() : "N/A";
            cout << "|" << setw(wMaLop) << left << (" " + myClasses[i].getMaLopHP())
                 << "|" << setw(wTenMon) << left << (" " + tenMon)
                 << "|" << " " << setw(wSoSV - 2) << right << countSV << " |" << endl;
        }
        printDynamicLine(wMaLop, wTenMon, wSoSV);
        
        cout << "\n" << " >> Tong so lop hoc phan: " << myClasses.size() << endl;

        // 3. Menu chức năng phụ
        cout << "\n" << "============================================" << endl;
        cout << "[1] Sua diem sinh vien" << endl;
        cout << "[2] Xem thong ke chi tiet lop hoc phan" << endl;
        cout << "[0] Quay lai" << endl;
        cout << "============================================" << endl;
        
        int choice = getValidIntInput("Chon chuc nang: ", 0, 2);
        
        if (choice == 0) return;
        
        if (choice == 1) {
            // --- SỬA ĐIỂM ---
            clearScreen();
            cout << "=== SUA DIEM SINH VIEN ===" << endl;
            cout << "\n" << "Nhap Ma Lop Hoc Phan: ";
            string maLop;
            getline(cin, maLop);
            
            // Kiểm tra lớp có thuộc giảng viên không
            LopHocPhan* selectedClass = nullptr;
            for (size_t i = 0; i < myClasses.size(); i++) {
                if (myClasses[i].getMaLopHP() == maLop) {
                    selectedClass = &myClasses[i];
                    break;
                }
            }
            
            if (!selectedClass) {
                cout << " >> Loi: Ban khong phu trach lop '" << maLop << "'!" << endl;
                cout << "An Enter..."; cin.get();
                continue;
            }
            
            // Lấy danh sách sinh viên
            MyVector<StudentRecord> listSV;
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
                cout << "\n" << " >> Lop nay chua co sinh vien dang ky!" << endl;
                cout << "An Enter de tiep tuc..."; cin.get();
                continue;
            }
            
            sort(listSV.begin(), listSV.end(), cmpRecordByID);
            
            // Tính độ rộng cột
            size_t wID = 5, wName = 9, wKhoa = 4, wDiem = 4;
            for (size_t i = 0; i < listSV.size(); i++) {
                wID = max(wID, listSV[i].sv->getId().length());
                wName = max(wName, listSV[i].sv->getName().length());
                wKhoa = max(wKhoa, listSV[i].khoaName.length());
            }
            wID += 4; wName += 4; wKhoa += 4; wDiem += 4;
            
            // Vòng lặp sửa điểm
            while (true) {
                clearScreen();
                cout << "=== BANG DIEM LOP: " << maLop << " - " << selectedClass->getMonHoc()->getTenMon() << " ===" << endl;
                
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
                
                cout << "\n" << "Nhap [MA SV] de sua diem (hoac Enter de quay lai): ";
                string inputID;
                getline(cin, inputID);
                if (inputID.empty()) break;
                
                bool found = false;
                for (size_t i = 0; i < listSV.size(); i++) {
                    if (listSV[i].sv->getId() == inputID) {
                        found = true;
                        cout << "\n" << " >> Dang sua diem cho: " << listSV[i].sv->getName() << " (" << listSV[i].sv->getId() << ")" << endl;
                        float newScore = getValidFloatInput("Nhap diem moi (0.0 - 10.0): ");
                        listSV[i].dk->setDiem(newScore);
                        cout << " >> Cap nhat thanh cong! An Enter..."; cin.get();
                        break;
                    }
                }
                if (!found) {
                    cout << " >> Khong tim thay SV! Enter thu lai..."; cin.get();
                }
            }
        }
        else if (choice == 2) {
            // --- XEM THỐNG KÊ CHI TIẾT ---
            clearScreen();
            cout << "=== THONG KE CHI TIET LOP HOC PHAN ===" << endl;
            cout << "\n" << "Nhap Ma Lop Hoc Phan: ";
            string maLop;
            getline(cin, maLop);
            
            // Kiểm tra lớp có thuộc giảng viên không
            LopHocPhan* selectedClass = nullptr;
            for (size_t i = 0; i < myClasses.size(); i++) {
                if (myClasses[i].getMaLopHP() == maLop) {
                    selectedClass = &myClasses[i];
                    break;
                }
            }
            
            if (!selectedClass) {
                cout << " >> Loi: Ban khong phu trach lop '" << maLop << "'!" << endl;
                cout << "An Enter..."; cin.get();
                continue;
            }
            
            // Lấy danh sách sinh viên và tính thống kê
            MyVector<StudentRecord> listSV;
            float totalScore = 0;
            int countWithScore = 0;
            int scores[5] = {0}; // Kem, Yeu, TB, Kha, Gioi
            
            for (size_t i = 0; i < allDK.size(); i++) {
                if (allDK[i].getMaLHP_FK() == maLop) {
                    Student* s = ls.getStudentTable().searchByString(allDK[i].getMaSV_FK());
                    DangKy* originalDK = ls.getDangKyTable().searchByString(allDK[i].getKey());
                    if (s && originalDK) {
                        string kName = (s->getKhoa()) ? s->getKhoa()->getMaKhoa() : "N/A";
                        StudentRecord rec;
                        rec.sv = s; rec.dk = originalDK; rec.khoaName = kName;
                        listSV.push_back(rec);
                        
                        float d = originalDK->getDiem();
                        if (d > 0) {
                            totalScore += d;
                            countWithScore++;
                            
                            // Phân loại điểm
                            if (d < 4.0) scores[0]++;
                            else if (d < 5.5) scores[1]++;
                            else if (d < 7.0) scores[2]++;
                            else if (d < 8.5) scores[3]++;
                            else scores[4]++;
                        }
                    }
                }
            }
            
            if (listSV.size() == 0) {
                cout << "\n" << " >> Lop nay chua co sinh vien dang ky!" << endl;
                cout << "An Enter de tiep tuc..."; cin.get();
                continue;
            }
            
            cout << "\n" << "=== THONG TIN CHUNG ===" << endl;
            cout << "Ma lop: " << maLop << endl;
            cout << "Ten mon hoc: " << selectedClass->getMonHoc()->getTenMon() << endl;
            cout << "Tong so sinh vien: " << listSV.size() << endl;
            
            if (countWithScore > 0) {
                cout << "Diem trung binh lop: " << fixed << setprecision(2) << (totalScore / countWithScore) << endl;
                
                cout << "\n" << "=== PHO DIEM ===" << endl;
                string labels[5] = {"Kem (<4.0)   ", "Yeu (4.0-5.4)", "TB  (5.5-6.9)", "Kha (7.0-8.4)", "Gioi(8.5-10)"};
                
                for (int k = 0; k < 5; k++) {
                    cout << labels[k] << " | ";
                    int len = (countWithScore == 0) ? 0 : (scores[k] * 30) / countWithScore;
                    for (int j = 0; j < len; j++) cout << "=";
                    cout << " " << scores[k] << " (" << fixed << setprecision(1) 
                         << (countWithScore ? (float)scores[k] * 100 / countWithScore : 0) << "%)" << endl;
                }
            } else {
                cout << "Chua co diem nao duoc nhap!" << endl;
            }
            
            cout << "\n" << "An Enter de tiep tuc..."; cin.get();
        }
    }
}

// --- XEM THÔNG TIN CHI TIẾT SINH VIÊN ---
void StudentViewDetailedInfo(Student* s, LoginSystem& ls) {
    clearScreen();
    cout << "============================================" << endl;
    cout << "       THONG TIN CHI TIET SINH VIEN" << endl;
    cout << "============================================" << endl;
    
    // --- THÔNG TIN CƠ BẢN ---
    cout << "\n" << "--- THONG TIN CA NHAN ---" << endl;
    cout << "Ma sinh vien: " << s->getId() << endl;
    cout << "Ho va ten: " << s->getName() << endl;
    cout << "Khoa: " << (s->getKhoa() ? s->getKhoa()->getTenKhoa() : "Chua xac dinh") << endl;
    cout << "Ma khoa: " << (s->getKhoa() ? s->getKhoa()->getMaKhoa() : "N/A") << endl;
    
    // --- DANH SÁCH LỚP HỌC PHẦN ĐÃ ĐĂNG KÝ ---
    cout << "\n" << "--- LOP HOC PHAN DA DANG KY ---" << endl;
    MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
    MyVector<DangKy> myDK;
    
    for (size_t i = 0; i < allDK.size(); i++) {
        if (allDK[i].getMaSV_FK() == s->getId()) {
            myDK.push_back(allDK[i]);
        }
    }
    
    if (myDK.empty()) {
        cout << " >> Chua dang ky lop hoc phan nao!" << endl;
    } else {
        // Tính độ rộng cột
        size_t wMaLop = 10, wMaMon = 8, wTenMon = 20, wGV = 20, wDiem = 8;
        
        for (size_t i = 0; i < myDK.size(); i++) {
            wMaLop = max(wMaLop, myDK[i].getMaLHP_FK().length());
            
            LopHocPhan* lhp = ls.getLopHPTable().searchByString(myDK[i].getMaLHP_FK());
            if (lhp) {
                if (lhp->getMonHoc()) {
                    wMaMon = max(wMaMon, lhp->getMonHoc()->getMaMon().length());
                    wTenMon = max(wTenMon, lhp->getMonHoc()->getTenMon().length());
                }
                if (lhp->getGiangVien()) {
                    wGV = max(wGV, lhp->getGiangVien()->getName().length());
                }
            }
        }
        wMaLop += 4; wMaMon += 4; wTenMon += 4; wGV += 4; wDiem += 4;
        
        // In bảng
        printDynamicLine(wMaLop, wMaMon, wTenMon, wGV, wDiem);
        cout << "|" << setw(wMaLop) << left << " Ma LHP"
             << "|" << setw(wMaMon) << left << " Ma Mon"
             << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
             << "|" << setw(wGV) << left << " Giang Vien"
             << "|" << setw(wDiem) << left << " Diem" << "|" << endl;
        printDynamicLine(wMaLop, wMaMon, wTenMon, wGV, wDiem);
        
        float totalScore = 0;
        int countWithScore = 0;
        
        for (size_t i = 0; i < myDK.size(); i++) {
            LopHocPhan* lhp = ls.getLopHPTable().searchByString(myDK[i].getMaLHP_FK());
            
            string maMon = "N/A";
            string tenMon = "N/A";
            string giangVien = "N/A";
            
            if (lhp) {
                if (lhp->getMonHoc()) {
                    maMon = lhp->getMonHoc()->getMaMon();
                    tenMon = lhp->getMonHoc()->getTenMon();
                }
                if (lhp->getGiangVien()) {
                    giangVien = lhp->getGiangVien()->getName();
                }
            }
            
            float diem = myDK[i].getDiem();
            if (diem > 0) {
                totalScore += diem;
                countWithScore++;
            }
            
            cout << "|" << setw(wMaLop) << left << (" " + myDK[i].getMaLHP_FK())
                 << "|" << setw(wMaMon) << left << (" " + maMon)
                 << "|" << setw(wTenMon) << left << (" " + tenMon)
                 << "|" << setw(wGV) << left << (" " + giangVien)
                 << "|" << " " << setw(wDiem - 2) << left << fixed << setprecision(2) << diem << " |" << endl;
        }
        printDynamicLine(wMaLop, wMaMon, wTenMon, wGV, wDiem);
        
        cout << "\n" << " >> Tong so lop da dang ky: " << myDK.size() << endl;
        if (countWithScore > 0) {
            cout << " >> Diem trung binh (GPA): " << fixed << setprecision(2) << (totalScore / countWithScore) << endl;
        }
    }
    
    cout << "\n" << "============================================" << endl;
}

// --- LOGIC SINH VIÊN (SỬA LẠI DÙNG MYVECTOR & BỎ LAMBDA) ---
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

    cout << "\n" << " >> Tong so mon: " << count << endl;
    if(count > 0) cout << " >> Diem trung binh (GPA): " << fixed << setprecision(2) << (totalScore / count) << endl;
}

// --- XEM THÔNG TIN CHI TIẾT GIẢNG VIÊN ---
void TeacherViewDetailedInfo(Teacher* t, LoginSystem& ls) {
    clearScreen();
    cout << "============================================" << endl;
    cout << "       THONG TIN CHI TIET GIANG VIEN" << endl;
    cout << "============================================" << endl;
    
    // --- THÔNG TIN CƠ BẢN ---
    cout << "\n" << "--- THONG TIN CA NHAN ---" << endl;
    cout << "Ma giang vien: " << t->getId() << endl;
    cout << "Ho va ten: " << t->getName() << endl;
    cout << "Khoa: " << (t->getKhoa() ? t->getKhoa()->getTenKhoa() : "Chua xac dinh") << endl;
    cout << "Ma khoa: " << (t->getKhoa() ? t->getKhoa()->getMaKhoa() : "N/A") << endl;
    
    // --- DANH SÁCH LỚP HỌC PHẦN ĐANG DẠY ---
    cout << "\n" << "--- LOP HOC PHAN DANG DAY ---" << endl;
    MyVector<LopHocPhan> allClasses = ls.getLopHPTable().getAllElements();
    MyVector<LopHocPhan> myClasses;
    
    for (size_t i = 0; i < allClasses.size(); i++) {
        if (allClasses[i].getGiangVien() && allClasses[i].getGiangVien()->getId() == t->getId()) {
            myClasses.push_back(allClasses[i]);
        }
    }
    
    if (myClasses.empty()) {
        cout << " >> Chua co lop hoc phan nao!" << endl;
    } else {
        // Tính độ rộng cột
        size_t wMaLop = 10, wMaMon = 8, wTenMon = 20, wSoSV = 8;
        
        for (size_t i = 0; i < myClasses.size(); i++) {
            wMaLop = max(wMaLop, myClasses[i].getMaLopHP().length());
            if (myClasses[i].getMonHoc()) {
                wMaMon = max(wMaMon, myClasses[i].getMonHoc()->getMaMon().length());
                wTenMon = max(wTenMon, myClasses[i].getMonHoc()->getTenMon().length());
            }
        }
        wMaLop += 4; wMaMon += 4; wTenMon += 4; wSoSV += 4;
        
        // In bảng
        printDynamicLine(wMaLop, wMaMon, wTenMon, wSoSV);
        cout << "|" << setw(wMaLop) << left << " Ma LHP"
             << "|" << setw(wMaMon) << left << " Ma Mon"
             << "|" << setw(wTenMon) << left << " Ten Mon Hoc"
             << "|" << setw(wSoSV) << left << " So SV" << "|" << endl;
        printDynamicLine(wMaLop, wMaMon, wTenMon, wSoSV);
        
        int totalStudents = 0;
        MyVector<DangKy> allDK = ls.getDangKyTable().getAllElements();
        
        for (size_t i = 0; i < myClasses.size(); i++) {
            // Đếm số sinh viên trong lớp
            int countSV = 0;
            for (size_t j = 0; j < allDK.size(); j++) {
                if (allDK[j].getMaLHP_FK() == myClasses[i].getMaLopHP()) {
                    countSV++;
                }
            }
            totalStudents += countSV;
            
            string maMon = myClasses[i].getMonHoc() ? myClasses[i].getMonHoc()->getMaMon() : "N/A";
            string tenMon = myClasses[i].getMonHoc() ? myClasses[i].getMonHoc()->getTenMon() : "N/A";
            
            cout << "|" << setw(wMaLop) << left << (" " + myClasses[i].getMaLopHP())
                 << "|" << setw(wMaMon) << left << (" " + maMon)
                 << "|" << setw(wTenMon) << left << (" " + tenMon)
                 << "|" << " " << setw(wSoSV - 2) << right << countSV << " |" << endl;
        }
        printDynamicLine(wMaLop, wMaMon, wTenMon, wSoSV);
        
        cout << "\n" << " >> Tong so lop hoc phan: " << myClasses.size() << endl;
        cout << " >> Tong so sinh vien: " << totalStudents << endl;
    }
    
    cout << "\n" << "============================================" << endl;
}

// --- MENU ĐIỀU HƯỚNG ---
void handleAdminMenu(LoginSystem& ls, User* u, int c, bool& logout) {
    switch(c) {
        case 1: AdminViewAllUsers(ls, true); break;
        case 2: AdminViewAllUsers(ls, false); break;
        case 3: AdminManageClassesMenu(ls); break; 
        case 4: AdminManageAccounts(ls); break;
        case 5: AdminManageDepartments(ls); break;
        case 6: AdminManageRegistrations(ls); break;
        case 7: handleChangePassword(u, ls); break;
        case 8: logout = true; break;
    }
    if(c!=3 && c!=4 && c!=5 && c!=6 && c!=8) { cout << "\n" << "An Enter..."; cin.get(); }
}

void handleTeacherMenu(Teacher* t, LoginSystem& ls, int c, bool& logout) {
    if(c==1) { TeacherViewDetailedInfo(t, ls); cout<<"\n" << "An Enter..."; cin.get(); return; }
    if(c==3) { handleChangePassword(t, ls); cout<<"\n" << "Enter..."; cin.get(); return; }
    if(c==4) { logout = true; return; }

    if(c==2) { 
        TeacherViewAndEditScores(t, ls);
    }
}

void handleStudentMenu(Student* s, LoginSystem& ls, int c, bool& logout) {
    if(c==1) { StudentViewDetailedInfo(s, ls); }
    else if(c==2) handleChangePassword(s, ls);
    else if(c==3) logout = true;
    if(c!=3) { cout<<"\n" << "Enter..."; cin.get(); }
}

void saveExit(LoginSystem& ls) {
    cout << "Luu du lieu? (y/n): "; char c; cin >> c;
    if(tolower(c)=='y') {
        ls.saveStudentsToFile("data/Student.txt"); ls.saveTeachersToFile("data/Teacher.txt");
        ls.saveAccountsToFile("data/Account.txt"); ls.saveDangKyToFile("data/DangKy.txt");
        ls.saveLopHPToFile("data/LopHP.txt");
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
                int max = (user->getUserType()==UserType::ADMIN) ? 8 : ((user->getUserType()==UserType::TEACHER) ? 4 : 3);
                int c = getValidIntInput("Chon: ", 1, max);
                if(user->getUserType()==UserType::STUDENT) handleStudentMenu((Student*)user, ls, c, logout);
                else if(user->getUserType()==UserType::TEACHER) handleTeacherMenu((Teacher*)user, ls, c, logout);
                else handleAdminMenu(ls, user, c, logout);
            }
        } else { cout << "Sai thong tin! Enter thu lai..."; cin.get(); }
    }
    return 0;
}













