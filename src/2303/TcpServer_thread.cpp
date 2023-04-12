#include <WS2tcpip.h>
#include <WinSock2.h>
#include <iostream>
#include <map>
#include <mutex>
#include <stdint.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

#define LOGI(format, ...)                                                   \
    fprintf(stderr, "[INFO] [%s:%d]:%s() " format "\n", __FILE__, __LINE__, \
            __func__, ##__VA_ARGS__)
#define LOGE(format, ...)                                                    \
    fprintf(stderr, "[ERROR] [%s:%d]:%s() " format "\n", __FILE__, __LINE__, \
            __func__, ##__VA_ARGS__)

class Server;
class Connection {
public:
    Connection(Server* server, int clientFd)
        : m_server(server), m_clientFd(clientFd) {
        LOGI("");
    }
    ~Connection() {
        LOGI("");
        closesocket(m_clientFd);
        if (th) {
            th->join();
            delete th;
            th = nullptr;
        }
    }

public:
    typedef void (*DisconnectionCallback)(void*, int); //(server, sockFd)

    void setDisconnectionCallback(DisconnectionCallback cb, void* arg) {
        m_disconnectionCallback = cb;
        m_arg                   = arg;
    }

    int start() {
        th = new std::thread(
            [](Connection* conn) {
                int      size;
                uint64_t totalSize = 0;
                time_t   t1        = time(NULL);

                while (true) {
                    char buf[1024];
                    memset(buf, 1, sizeof(buf));
                    size = ::send(conn->m_clientFd, buf, sizeof(buf), 0);
                    if (size < 0) {
                        printf("clientFd=%d,send error，错误码：%d\n",
                               conn->m_clientFd, WSAGetLastError());

                        conn->m_disconnectionCallback(conn->m_arg,
                                                      conn->m_clientFd);
                        break;
                    }
                    totalSize += size;

                    if (totalSize > 62914560) /* 62914560=60*1024*1024=60mb*/
                    {
                        time_t t2 = time(NULL);
                        if (t2 - t1 > 0) {
                            uint64_t speed =
                                totalSize / 1024 / 1024 / (t2 - t1);

                            printf("clientFd=%d,size=%d,totalSize=%llu,speed=%"
                                   "lluMbps\n",
                                   conn->m_clientFd, size, totalSize, speed);

                            totalSize = 0;
                            t1        = time(NULL);
                        }
                    }
                }
            },
            this);
        return 0;
    }
    int getClientFd() {
        return m_clientFd;
    }

private:
    Server*      m_server;
    int          m_clientFd;
    std::thread* th = nullptr;
    ;

    DisconnectionCallback m_disconnectionCallback = nullptr;
    void*                 m_arg                   = nullptr; // server *
};
class Server {
public:
    Server(const char* ip, uint16_t port);
    ~Server();

public:
    int start() {
        LOGI("TcpServer2 tcp://%s:%d", m_ip, m_port);
        // 创建描述符
        m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_sockFd < 0) {
            LOGI("create socket error");
            return -1;
        }
        int on = 1;
        setsockopt(m_sockFd, SOL_SOCKET, SO_REUSEADDR, ( const char* )&on,
                   sizeof(on));

        // bind
        SOCKADDR_IN server_addr;
        server_addr.sin_family           = AF_INET;
        server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        // server_addr.sin_addr.s_addr = inet_addr("192.168.2.61");
        server_addr.sin_port = htons(m_port);

        if (bind(m_sockFd, ( SOCKADDR* )&server_addr, sizeof(SOCKADDR))
            == SOCKET_ERROR) {
            LOGI("socket bind error");
            return -1;
        }

        if (listen(m_sockFd, 10) < 0) {
            LOGE("socket listen error");
            return -1;
        }

        while (true) {
			// 判断连接集合是否存在对应客户端 fd，存在即开始发送数据

            LOGI("阻塞监听新连接...");
            // 阻塞接收请求 start
            int      clientFd;
            char     clientIp[40] = { 0 };
            uint16_t clientPort;

            socklen_t          len = 0;
            struct sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            len = sizeof(addr);

            clientFd = accept(m_sockFd, ( struct sockaddr* )&addr, &len);
            if (clientFd < 0) {
                LOGE("socket accept error");
                return -1;
            }
            strcpy(clientIp, inet_ntoa(addr.sin_addr));
            clientPort = ntohs(addr.sin_port);
            // 阻塞接收请求 end
            LOGI("发现新连接：clientIp=%s,clientPort=%d,clientFd=%d", clientIp,
                 clientPort, clientFd);

            Connection* conn = new Connection(this, clientFd);
            conn->setDisconnectionCallback(Server::cbDisconnection,
                                           this); // 断开连接回调
            this->addConnection(conn); // 存入对应客户端套接字的连接类
            conn->start(); // 非阻塞在子线程中启动，完成发送
        }
        return 0;
    }

    void handleDisconnection(int clientFd) {
        LOGI("clientFd=%d", clientFd);
        this->removeConnection(clientFd);
    }

    static void cbDisconnection(void* arg, int clientFd) {
        LOGI("clientFd=%d", clientFd);
        Server* server = ( Server* )arg;
        server->handleDisconnection(clientFd);
    }

private:
    const char* m_ip;
    uint16_t    m_port;
    int         m_sockFd; // 服务器描述符

    std::map< int, Connection* >
               m_connMap; // <sockFd,conn> 维护所有被创建的连接
    std::mutex m_connMap_mtx;

    bool       addConnection(Connection* conn) {
        m_connMap_mtx.lock();
        if (m_connMap.find(conn->getClientFd()) != m_connMap.end()) {
            m_connMap_mtx.unlock();
            return false;
        }
        else {
            m_connMap.insert(std::make_pair(conn->getClientFd(), conn));
            m_connMap_mtx.unlock();
            return true;
        }
    }

    Connection* getConnection(int clientFd) {
        m_connMap_mtx.lock();
        std::map< int, Connection* >::iterator it = m_connMap.find(clientFd);
        if (it != m_connMap.end()) {
            m_connMap_mtx.unlock();
            return it->second;
        }
        else {
            m_connMap_mtx.unlock();
            return nullptr;
        }
    }

    bool removeConnection(int clientFd) {
        m_connMap_mtx.lock();
        std::map< int, Connection* >::iterator it = m_connMap.find(clientFd);
        if (it != m_connMap.end()) {
            m_connMap.erase(it);
            m_connMap_mtx.unlock();
            return true;
        }
        else {
            m_connMap_mtx.unlock();
            return false;
        }
    }
};

// 初始化套接字
Server::Server(const char* ip, uint16_t port)
    : m_ip(ip), m_port(port), m_sockFd(-1) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        LOGE("WSAStartup Error");
        return;
    }
}

// 关闭连接
Server::~Server() {
    if (m_sockFd > -1) {
        closesocket(m_sockFd);
        m_sockFd = -1;
    }
    WSACleanup();
}

int main() {
    const char* ip   = "127.0.0.1";
    uint16_t    port = 8080;

    Server server(ip, port);
    server.start();

    return 0;
}