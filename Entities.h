#include "User.h"
class Student: public User{
    private:
        string Mk;
        string Status;
    public:
        Student();
        Student(const string&, const string&, const string&,const string&);
        virtual ~Student();
        void show_info();
};
class Teacher: public User{
    private:
        string Mk;
        string Status;
    public:
        Teacher();
        Teacher(const string&, const string&, const string&,const string&);
        virtual ~Teacher();
        void show_info();
};
class Admin: public User{
    public:
        Admin();
        Admin(const string&, const string&);
        virtual ~Admin();
        void show_info();
};
class Khoa{
    private:
        string Id;
        string Name;
    public:
        Khoa();
        Khoa(const string&, const string&);
        ~Khoa();
        void show_info();
        string getId() const { return Id; }
        string getName() const { return Name; }
};
class MonHoc{
    private:
        string Id;
        string Name;
        int SoTinChi;
    public:
        MonHoc();
        MonHoc(const string&, const string&, int);
        ~MonHoc();
        void show_info();
        string getId() const { return Id; }
        string getName() const { return Name; }
        int getSoTinChi() const { return SoTinChi; }
};
class LopHocPhan{
    private:
        string Id;
        string MaGiangVien;
        string MaMonHoc;
    public:
        LopHocPhan();
        LopHocPhan(const string&, const string&, const string&);
        ~LopHocPhan();
        void show_info();
        string getId() const { return Id; }
        string getMaGiangVien() const { return MaGiangVien; }
        string getMaMonHoc() const { return MaMonHoc; }
};
class DangKy{
    private:
        string MaLopHocPhan;
        string MaSinhVien;
        float Diem;
    public:
        DangKy();
        DangKy(const string&, const string&, float);
        ~DangKy();
        void show_info();
        string getMaLopHocPhan() const { return MaLopHocPhan; }
        string getMaSinhVien() const { return MaSinhVien; }
        float getDiem() const { return Diem; }
};