#include<iostream>
#include <string>
#include<cstring>
#include<fstream>
#include<list>
#include<iterator>
#include <windows.h>
#define PRICE_SD 210
#define PRICE_SG 180
#define PRICE_ST 300
using namespace std;



//Rooms types: Standard, Single, Suite
//Charging: Sd:210/night; Sg:180/night; St:300/night
int cur_date = 2;//1 by default, editable

class record {
public:
    string guest_name;
    string guest_tel;
    int in_date;
    int out_date;
    double fee;
    record() {};
};

bool compare_in_date(record& a, record& b);

class room {
public:
    int floor = 0;
    int number = 0;
    int type = 0;
    bool isCheckedin = false;
    //int state=0;//1: occupied; 2: reserved; 0: vacant
    // room(int f,int n,int t,string nm,string tel,int in,int out){        //有房客信息调用的构造函数
    //     floor=f;
    //     number=n;
    //     type=t;
    //     //state=s;

    //     guest_name=nm;
    //     guest_tel=tel;
    //     in_date=in;
    //     out_date=out;
    // }
    // room(int f,int n,int t){                                            //无房客信息调用的构造函数
    //     floor=f;
    //     number=n;
    //     type=t;
    //     //state=s;
    // }
    room() {};
    int getState() {     //0:Vacant; 1:Occupied; 2:Reserved; 3:Availiable later today
        // if(in_date==out_date==0){
        //     return 0;
        // }
        // else if(in_date<=cur_date<out_date){
        //     return 1;
        // }
        // else if(out_date==cur_date){
        //     return 3;
        // }
        // else if(in_date>cur_date){
        //     return 2;
        // }
        //record* cur=rlist.begin();
        if (rlist.empty())
            return 0;
        bool availiability = false;
        bool future_record_availibility = false;
        list<record>::iterator it = rlist.begin();
        while (it != rlist.end()) {
            if (it->in_date <= cur_date && cur_date < it->out_date) {
                return 1;
            }
            if (it->out_date == cur_date) {
                availiability = true;
            }
            if (it->in_date > cur_date) {
                future_record_availibility = true;
            }
            it++;
        }
        if (availiability) {
            return 3;
        }
        if (future_record_availibility) {
            return 2;
        }
        else {
            return 0;
        }
    }
    bool isAvailiable(int in, int out) {
        int state = getState();
        if (state == 0) {
            //异常
            return true;
        }
        if (state == 1) {
            //异常
            return false;
        }
        //record *cur=rlist;
        list<record>::iterator it = rlist.begin();
        if (rlist.empty()) {
            //异常
            return true;
        }
        while (it != rlist.end()) {
            if (!(in >= it->out_date || out <= it->in_date)) {
                MessageBox(NULL, TEXT("Room is not availiable in this period!"), TEXT("Time Conflicts"), MB_ICONWARNING | MB_OK);
                return false;
            }
            it++;
        }
        return true;
    }
    bool book() {
        string name, tel;
        int in, out;
    Fill:
        cout << "Please fill the following information to complete the reservation of the room!" << endl << endl;

        cout << "Your name: ";
        getline(cin, name);
        cout << "Telephone number: ";
        getline(cin, name);
        cout << "Check-in date: ";
        cin >> in;
        cout << "Check-out date: ";
        cin >> out;
        if (isAvailiable(in, out)) {
            cout << "Confirm your information" << endl << endl;
            cout << "Name: " << name << endl << "Telephone: " << tel << endl << "Reservation: Room" << floor << number << ", from Day" << in << " to Day " << out << ", " << out - in << " days" << endl;
            cout << "You should pay ";
            if (type == 1)
                cout << (out - in) * PRICE_SD;
            else if (type == 2)
                cout << (out - in) * PRICE_SG;
            else if (type == 3)
                cout << (out - in) * PRICE_ST;
            cout << " dollars" << endl << endl;
            cout << "Press 1 to confirm your order" << endl << "Press 0 to cancel the order" << endl << "Press other keys to redo your reservation" << endl;
            int option;
            cin >> option;
            if (option == 0) {
                cout << "Reservation canceled." << endl;
                return false;
            }
            if (option != 1) {
                goto Fill;
            }
            record temp;
            temp.in_date = in; temp.out_date = out; temp.guest_tel = tel; temp.guest_name = name;
            if (type == 1)
                temp.fee = (out - in) * PRICE_SD;
            else if (type == 2)
                temp.fee = (out - in) * PRICE_SG;
            else if (type == 3)
                temp.fee = (out - in) * PRICE_ST;
            rlist.push_back(temp);
            //rlist.sort(compare_in_date);
        }
        return true;
    }

    bool push_record(record d) {
        rlist.push_back(d);
        //rlist.sort(compare_in_date);
        return true;
    }
    void clear_records() {
        rlist.clear();
    }

    pair<int, int> checkinAvailibility(string name, string tel) {
        list<record>::iterator it = rlist.begin();
        pair<int, int> return_val(0, 0);
        while (it != rlist.end()) {
            if (name == it->guest_name && tel == it->guest_tel) {
                if (it->in_date <= cur_date && it->out_date > cur_date) {
                    return_val.first = it->in_date; return_val.second = it->out_date;
                    return return_val;
                }
            }
            it++;
        }
        return return_val;
    }

    int records_count() {
        return rlist.size();
    }
    bool write_records_to_file(ofstream& fs) {
        list<record>::iterator it = rlist.begin();
        while (it != rlist.end()) {
            fs << it->guest_name << endl;
            fs << it->guest_tel << endl;
            fs << it->in_date << endl;
            fs << it->out_date << endl;
            it++;
        }
        return true;
    }
protected:
    // string guest_name=0;
    // string guest_tel=0;
    // int in_date=0;
    // int out_date=0;
    // double fee=0;
    list<record> rlist;
    //record* rlist;
};

bool compare_in_date(const record& a, const record& b) {
    if (a.in_date < b.in_date) {
        return true;
    }
    scanf("%d");
    return false;
}