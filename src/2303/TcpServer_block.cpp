#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")

static std::string getTime() {
    const char* time_fmt = "%Y-%m-%d %H:%M:%S";
    time_t      t        = time(nullptr);
    char        time_str[64];
    strftime(time_str, sizeof(time_str), time_fmt, localtime(&t));

    return time_str;
}

#define LOGI(format, ...)                                                   \
    fprintf(stderr, "[INFO]%s [%s:%d %s()] " format "\n", getTime().data(), \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define LOGE(format, ...)                                                    \
    fprintf(stderr, "[ERROR]%s [%s:%d %s()] " format "\n", getTime().data(), \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define block_accept

int main() {
    const char* ip   = "127.0.0.1";
    uint16_t    port = 8080;

    LOGI("TcpServer_select tcp://%s:%d", ip, port);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // 加载2.2版本套接字
        LOGE("WSAStartup Error");
        return -1;
    }

    SOCKET serverFd = socket(AF_INET, SOCK_STREAM, 0); // 创建默认TCP IP4套接字
    if (serverFd < 0) {
        LOGI("create socket error");
        return -1;
    }

    int ret;

    int on = 1; // TCP不支持多播，UDP 可以
    ret    = setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, ( const char* )&on,
                        sizeof(on)); // 设置地址可复用、通用代码的套接字属性选项
    if (ret != 0) {
        LOGE("设置套接字属性失败%ld", ret);
        return -1;
    }

    SOCKADDR_IN server_addr;
    server_addr.sin_family           = AF_INET;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    // server_addr.sin_addr.s_addr = inet_addr("192.168.2.61");
    // //将字符串转换为in_addr类型
    server_addr.sin_port = htons(port);
    if (ret = bind(serverFd, ( SOCKADDR* )&server_addr, sizeof(SOCKADDR))
              == SOCKET_ERROR) {
        LOGI("socket bind error%ld", ret);
        return -1;
    }

    if (ret = listen(serverFd, 10) < 0) { // 监听可排队的最大连接个数 10
        LOGE("socket listen error%ld", ret);
        return -1;
    }

    while (true) {
        LOGI("阻塞监听新连接...");
        // 阻塞接收请求 start
        int         len = sizeof(SOCKADDR);
        SOCKADDR_IN accept_addr;
        int clientFd = accept(serverFd, ( SOCKADDR* )&accept_addr, &len);
        // const char* clientIp = inet_ntoa(accept_addr.sin_addr);

        if (clientFd == SOCKET_ERROR) {
            LOGI("accept connection error");
            break;
        }
        // 阻塞接收请求 end
        LOGI("发现新连接：clientFd=%d", clientFd);
        int      size;
        uint64_t totalSize = 0;
        time_t   t1        = time(NULL);

        while (true) {
            char buf[1024];
            memset(buf, 1, sizeof(buf));
            size = ::send(clientFd, buf, sizeof(buf), 0);
            if (size < 0) {
                printf("clientFd=%d,send error，错误码：%d\n", clientFd,
                       WSAGetLastError());
                break;
            }
            totalSize += size;
            if (totalSize > 62914560) /* 62914560=60*1024*1024=60mb*/
            {
                time_t t2 = time(NULL);
                if (t2 - t1 > 0) {
                    uint64_t speed = totalSize / 1024 / 1024 / (t2 - t1);
                    printf(
                        "clientFd=%d,size=%d,totalSize=%llu,speed=%lluMbps\n",
                        clientFd, size, totalSize, speed);
                    totalSize = 0;
                    t1        = time(NULL);
                }
                break;
            }
        }

        closesocket(serverFd);
        serverFd = -1;
        LOGI("关闭连接 clientFd=%d", clientFd);
    }
}