#include <iostream>
#include "head.hpp"

using namespace std;

//undefined rooms are set as Standard by default
bool write_to_file(list<room>& data);

const int floors = 5;
const int rpf = 10;
int vacant_count = 0;
int alt_count = 0; //availiable later today

bool read_from_file(list<room>& data)
{
    ifstream fs;
    fs.open("data.txt", ios::in); //文件流打开并读取酒店房间信息
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
        if (buffer == "")
        {
            break;
        }
        //cout << buffer << endl;
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
        switch (temp.getState())
        {
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
    if (floors >= i || (floors == i && rpf >= j))
    {
        int diff = (floors - i) * rpf + (rpf - j) + 1;
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
    fs.close();
    return true;
}

bool write_to_file(list<room>& data)
{
    ofstream fs("data.txt", ios::out);
    list<room>::iterator it = data.begin();
    int ct = 0;
    while (it != data.end())
    {
        fs << it->floor << endl
            << it->number << endl;
        fs << it->type << endl;
        ct = it->records_count();
        fs << ct << endl;
        if (ct == 0)
        { //0条记录，则当前房间信息写入结束
            fs << endl;
            it++;
            continue;
        }
        it->write_records_to_file(fs);
        fs << endl;
        it++;
    }
    fs.close();
    return true;
}

bool checkin(list<room>& data)
{
    system("cls");
    string name;
    string tel;
    cout << "Please fill in your following information" << endl
        << endl;
    cout << "Name: ";
    cin >> name;
    cout << "Telephone number: ";
    cin >> tel;
    list<room>::iterator it = data.begin();

    pair<int, int> in_out;
    while (it != data.end())
    {
        if (it->isCheckedin)
        {
            it++;
            continue;
        }
        in_out = it->checkinAvailibility(name, tel);
        if (in_out.first != 0)
        {
            if (it->isCheckedin)
            {
                return false; //顾客今天已入住
            }
            return true; //找到顾客预约记录
        }
        it++;
    }
    return false; //没有预约记录
}

//办理入住
//预定
//查询空房
//查询消费记录
//管理员登陆

void menu()
{
    system("cls");
    cout << "Welcome to Buji Island Hotel!\t\t\tDay " << cur_date << endl
        << endl;
    cout << "Press 1 to Check-in\t\t\t\t"
        << "Press 2 to make a reservation" << endl;
    cout << "Press 3 to view the availiable rooms\t\t"
        << "Press 4 to view your purchase history" << endl;
    cout << "Press 5 to manage the hotel system (admin only)" << endl
        << endl;
    cout << "Currently availiable rooms: " << vacant_count << "\t\tRooms availiable after 12:00 PM: " << alt_count << endl;
    int option;
    cin >> option;
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

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(unsigned short forecolor = 4, unsigned short backgroudcolor = 0)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); //获取缓冲区句柄
    SetConsoleTextAttribute(hCon, forecolor | backgroudcolor); //设置文本及背景色
}

void grid_view(list<room>& data)
{
    list<room>::reverse_iterator r_it = data.rbegin();
    string divide_line;
    divide_line.push_back(' ');
    for (int i = 1; i <= rpf * 14 - 1; i++)
    {
        divide_line.push_back('-');
    }
    divide_line.push_back(' ');
    int i;

    for (i = 1; i <= floors; i++)
    {
        gotoxy(0, (i - 1) * 3);
        cout << divide_line;


        for (int j = 1; j <= rpf && r_it != data.rend(); j++)
        {
            gotoxy(14 * (rpf - j), (i - 1) * 3 + 1);
            cout << "| " << r_it->floor;
            if (r_it->number < 10) {
                cout << 0 << r_it->number << "\t";
            }
            else {
                cout << r_it->number << "\t    ";
            }
            if (j == 1) {
                cout << "|";
            }

            gotoxy(14 * (rpf - j), (i - 1) * 3 + 2);
            cout << "| ";
            if (r_it->isCheckedin) {
                SetColor(15, BACKGROUND_RED);
                cout << "Occupied" << "\t\t";
                SetColor(15, 0);
            }
            else {
                SetColor(15, BACKGROUND_GREEN);
                cout << "Availiable";
                SetColor(15, 0);
                cout << "  ";
            }
            if (j == 1) {
                cout << "|";
            }
            r_it++;
        }
    }
    gotoxy(0, (i - 1) * 3);
    cout << divide_line;
}

int main()
{
    list<room> data;
    read_from_file(data);
    //cout << "test" << endl;
    write_to_file(data);
    data.begin()->isCheckedin = true;
    //gotoxy(0,0);
    //cout << "project test output..." << endl;
    //SetColor(40,30);
    //std::cout <<"Colored hello world for windows!\n";
    //SetColor(15,BACKGROUND_GREEN);
    //std::cout <<"Colored hello world for windows!\n";
    //SetColor(15,BACKGROUND_RED);
    //std::cout <<"Colored hello world for windows!\n"<<endl;
    char t;
    cin >> t;
    grid_view(data);
    return 0;
}
