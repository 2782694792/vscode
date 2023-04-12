#include <iostream>

#include <ctime>

const std::string currentDateTime();

int main(int argc, char const *argv[])
{
    // 1、std::chrono::system_clock::now()

    // 2、std::time(0)              #include <ctime>
    std::time_t now = std::time(0);
    std::tm *now2 = std::localtime(&now);
    std::cout << (now2->tm_year + 1900)
                << (now2->tm_mon + 1)
                << (now2->tm_mday)
                << std::endl;

    // 3、跨平台代码获取            #include <time.h>    const std::string currentDateTime()
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;

    // 5、C++继承了C语言中日期和时间操作的结构和函数，以及考虑本地化的几个日期/时间输入和输出函数
    std::time_t now5 = std::time(0);
    tm *localtm5 = localtime(&now5);
    std::cout << "The local date and time is: " << asctime(localtm5) << std::endl;
    tm *gmtm5 = gmtime(&now5);
    if (gmtm5 != NULL)
    {
        std::cout << "The UTC date and time is: " << asctime(gmtm5) << std::endl;
    }
    else
    {
        std::cerr << "Failed to get the UTC date and time" << std::endl;
        return EXIT_FAILURE;
    }

    // 6、





    getchar();

    return 0;
}

const std::string currentDateTime()
{
    char buf[80];
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    // strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); //2021-09-22.21:47:59
    return buf;
}
