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

    int           ret;
    unsigned long ul = 1;
    ret              = ioctlsocket(serverFd, FIONBIO, &ul);
    if (ret == SOCKET_ERROR) {
        LOGE("设置非阻塞失败%ld", ret);
        return -1;
    }

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

    char recvBuf[1000] = { 0 };
    int  recvBufLen    = 1000;
    int  recvLen       = 0;

    int    max_fd = 0;
    fd_set readfds; // 可读描述符
    FD_ZERO(&readfds);
    FD_SET(serverFd, &readfds); // 将sockFd添加进入集合内，并更新最大文件描述符
    max_fd = max_fd > serverFd ? max_fd : serverFd;

    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;
    char sendBuf[10000];
    int  sendBufLen = 10000;
    memset(sendBuf, 0, sendBufLen);
    while (true) {
        fd_set readfds_temp; // 设置临时容器
        FD_ZERO(&readfds_temp);
        readfds_temp = readfds;
        ret          = select(max_fd + 1, &readfds_temp, nullptr, nullptr,
                              &timeout); // 寻找可读描述符
        if (ret <= 0) {
            // LOGE("未检测到活跃fd");
        }
        else {
            // 每个进程默认打开3个文件描述符
            // 0,1,2 其中0代表标准输入流，1代表标准输出流，2代表标准错误流
            for (int fd = 3; fd < max_fd + 1; fd++) {
                if (FD_ISSET(fd, &readfds_temp)) {
                    LOGI("fd=%d，触发可读事件", fd);

                    if (fd == serverFd) { // 如果套接字就绪了则等待客户端连接
                        int clientFd;
                        if ((clientFd = accept(serverFd, NULL, NULL)) == -1) {
                            LOGE("accept error");
                        }
                        LOGI("发现新连接：clientFd=%d", clientFd);

                        // 如果有客户端连接将产生的新的文件描述符添加到集合中,并更新最大文件描述符
                        FD_SET(clientFd, &readfds);
                        max_fd = max_fd > clientFd ? max_fd : clientFd;
                    }
                    else { // 客户端发来消息
                        // memset(recvBuf, 0, recvBufLen);
                        recvLen = recv(fd, recvBuf, recvBufLen, 0);

                        if (recvLen <= 0) {
                            LOGE("fd=%d,recvLen=%d error", fd, recvLen);
                            closesocket(fd);
                            FD_CLR(fd, &readfds); // 从可读集合中删除
                            continue;
                        }
                        else {
                            LOGI("fd=%d,recvLen=%d success", fd, recvLen);
                        }
                    }
                }
            }
        }

        for (int i = 0; i < readfds.fd_count; i++) {
            int fd = readfds.fd_array[i];
            if (fd != serverFd) { // 客户端fd
                int size = send(fd, sendBuf, sendBufLen, 0);
                if (size < 0) {
                    LOGE("fd=%d,send error，错误码：%d", fd, WSAGetLastError());
                    continue;
                }
            }
        }
    }

    if (serverFd > -1) {
        closesocket(serverFd);
        serverFd = -1;
    }
    WSACleanup();

    return 0;
}