#include <iostream>
#include "head.hpp"

using namespace std;

//undefined rooms are set as Standard by default

const int floors = 5;
const int rpf = 10;
int vacant_count = 0;
int alt_count = 0;    //availiable later today

bool read_from_file(list<room>& data)
{
    ifstream fs;
    fs.open("data.txt"); //文件流打开并读取酒店房间信息
    room temp;
    room d_room;
    d_room.type = 1;
    record temp_r;
    string buffer;
    int rcount;
    int i = 1, j = 1; //i<=5, j<=10
    while (!fs.eof())
    {
        //fs.getline(buffer, 100);
        getline(fs, buffer);
        cout << buffer << endl;
        temp.floor = stoi(buffer);
        getline(fs, buffer);
        temp.number = stoi(buffer);
        getline(fs, buffer);
        temp.type = stoi(buffer);
        //需要处理异常，即floor或number超出限制

        if (temp.floor > i || (temp.floor == i && temp.number > j))
        {
            int diff = (temp.floor - i) * rpf + (temp.number - j);
            for (int ct = 1; ct <= diff; ct++)
            {
                d_room.floor = i;
                d_room.number = j;
                data.push_back(d_room);
                vacant_count++;
                if (j < 10)
                {
                    j++;
                }
                else if (j == 10)
                {
                    i++;
                    j = 1;
                }
            }
        }

        temp.type = stoi(buffer);
        
        getline(fs, buffer);
        rcount = stoi(buffer);

        for (int k = 1; k <= rcount; k++)
        {
            getline(fs, buffer);
            temp_r.guest_name = buffer;
            getline(fs, buffer);
            temp_r.guest_tel = buffer;
            getline(fs, buffer);
            temp_r.in_date = stoi(buffer);
            getline(fs, buffer);
            temp_r.out_date = stoi(buffer);
            switch (temp.type)
            {
            case 1:
                temp_r.fee = (temp_r.out_date - temp_r.in_date) * PRICE_SD;
                break;
            case 2:
                temp_r.fee = (temp_r.out_date - temp_r.in_date) * PRICE_SG;
                break;
            case 3:
                temp_r.fee = (temp_r.out_date - temp_r.in_date) * PRICE_ST;
                break;
            }
            temp.push_record(temp_r);
        }

        data.push_back(temp);
        switch (temp.getState()) {
        case 0:
            vacant_count++;
            break;
        case 2:
            vacant_count++;
            break;
        case 3:
            alt_count++;
            break;
        }
        if (j < 10)
        {
            j++;
        }
        else if (j == 10)
        {
            i++;
            j = 1;
        }
        temp.clear_records();
        getline(fs, buffer);
    }
    fs.close();
    return true;
}

bool checkin(list<room> &data){
    system("cls");
    string name; string tel;
    cout<<"Please fill in your following information"<<endl<<endl;
    cout<<"Name: "; cin>>name;
    cout<<"Telephone number: ";cin>>tel;
    list<room>::iterator it=data.begin();

    pair<int,int> in_out;
    while(it!=data.end()){
        if(it->isCheckedin){
            it++;
            continue;
        }
        in_out=it->checkinAvailibility(name,tel);
        if(in_out.first!=0){
            if(it->isCheckedin){
                return false;       //顾客今天已入住
            }
            return true;            //找到顾客预约记录
        }
        it++;
    }
    return false;                   //没有预约记录
}


//办理入住
//预定
//查询空房
//查询消费记录
//管理员登陆

void menu() {
    system("cls");
    cout << "Welcome to Buji Island Hotel!\t\t\tDay " << cur_date << endl << endl;
    cout << "Press 1 to Check-in\t\t\t\t" << "Press 2 to make a reservation" << endl;
    cout << "Press 3 to view the availiable rooms\t\t" << "Press 4 to view your purchase history" << endl;
    cout << "Press 5 to manage the hotel system (admin only)" << endl << endl;
    cout << "Currently availiable rooms: " << vacant_count << "\t\tRooms availiable after 12:00 PM: " << alt_count << endl;
    int option;
    cin>>option;
    switch (option)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    default:
        break;
    }
}

int main()
{
    list<room> data;
    if (!read_from_file(data))
    {
        //显示读取信息错误提示
    }
    menu();
    cout << "project test output..." << endl;
    return 0;
}
