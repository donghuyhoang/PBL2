#include <bits/stdc++.h>
using namespace std;

class SV{
	public:
		string MSV = "";
		string Name = "";
		string K = "";
		float Math = -1;
		float English = -1;
		float Tech = -1;
		float PBL = -1;
};
map<string,string> checkadmin;
map<string,string> checkuser;
vector<SV> sv;
void SaveData(){
	ifstream in("Data.txt");
	string ss;
	while(getline(in,ss)){
		SV a;
		stringstream s(ss);
		string sss = "";
		while(s >> sss){
			if(a.MSV.empty()) a.MSV = sss;
			else if(a.K.empty() && (sss[0] < '0' || sss[0] > '9')) a.Name += sss;
			else if(a.K.empty() && sss[0] >= '0' && sss[0] <= 9) a.K = sss;
			else if(a.Math == -1) a.Math = stof(sss);
			else if(a.English == -1) a.English = stof(sss);
			else if(a.Tech == -1) a.Tech = stof(sss);
			else if(a.PBL == -1) a.PBL = stof(sss);
		}
		sv.push_back(a);
	}
}
void SaveAccUser(){
	ifstream in("AccUser.txt");
	string user,pass;
	while(in >> user >> pass){
		checkuser[user] = pass;
	}
	in.close();
}
void SaveAccAdmin(){
	ifstream in("AccAdmin.txt");
	string user,pass;
	while(in >> user >> pass){
		checkadmin[user] = pass;
	}
	in.close();
}

int Login(){
	cout << "Ban dang nhap voi tu cach nao?" << endl << "1:Admin" << endl << "2:SinhVien" << endl;
	string s; cin >> s;
	if(s == "1"){
		string user,pass;
		cout << "UserName: "; cin >> user;
		cout << "PassWord: "; cin >> pass;
		if(checkadmin[user] == pass) {
			cout << "Successful!" << endl;
			return 1;
		}
		else {
			cout << "Not found!" << endl;
			return 0;
		}
	}
	else if(s == "2"){
		string user,pass;
		cout << "UserName: "; cin >> user;
		cout << "PassWord: "; cin >> pass;
		if(checkuser[user] == pass) {
			cout << "Successful!" << endl;
			return 2;			
		}
		else {
			cout << "Not found!" << endl;
			return 0;
		}
	}
	else if(s == "exit") return -1;
	else return 0;
}
void show(){
	int n = sv.size();
	for (int i = 0; i < n; i++){
		cout << "MSV: " << sv[i].MSV << ' ' << "Ten: " << sv[i].Name << ' ' << "Math " << sv[i].Math << ' ' << "English " <<  sv[i].English << ' ' << "Tech " << sv[i].Tech << ' ' << "PBL " << sv[i].PBL << endl;
	}
}
//void InputPass(){
//	
//}
int main(){
	SaveData();
	SaveAccUser();
	SaveAccAdmin();
	cout << "Nhap exit de thoat" << endl;
	while(1){
		int x = Login();
		while(!x && x != -1){
			x = Login();
		}
		if(x == -1) return 0;
		if(x == 1){
			cout << "Cac quyen cua ban: ShowAll ShowMSV DeleteMSV" << endl;	
		}
		else if(x == 2){
			cout << "Cac quyen cua ban: Show" << endl;
		}
		string req;
		do{
			cin >> req;
			if(x == 1 && req == "ShowAll") show();
			
		}while(req != "exit");
	}
}
