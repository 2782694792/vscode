#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    // 1.写文件
    ofstream ofs;
    ofs.open("220813file.dat");

    // 2.写入输入的数据
    char data[5];
    cout << "please enter 5 characters data" << endl;
    // 2.1 cin.getline()
    cin.getline(data, 5);
    // 3.向文件写入数据
    ofs << data << endl;

    // 2.2 cin.ignore()
    cout << "Ignore enter data" << endl;
    cin >> data;
    cin.ignore(); // 跳过忽略指定字符
    // 3.向文件写入数据
    ofs << data << endl;

    // 4.关闭文件
    ofs.close();

    // 5.读文件
    ifstream ifs;
    ifs.open("220813file.dat");

    // 6.打印数据
    /* while (1)
    {
        ifs >> data;
        cout << data << endl;
    } */
    
    cout << ifs.cur << endl;
    ifs.seekg(1, ios::cur);
    cout << ifs.cur << endl;
    ifs >> data;
    cout << data << endl;
    
    ifs.close();
    return 0;
}
