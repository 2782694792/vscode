#include <iostream>
#include <list>
#include <string>

using namespace std;

// 过滤器抽象接口
class Filter {
public:
    Filter(){};
    virtual ~Filter(){};
    virtual void execute(string request) = 0;
};

// 过滤器AuthenticationFilter
class AuthenticationFilter : public Filter {
public:
    AuthenticationFilter(){};
    ~AuthenticationFilter(){};
    void execute(string request) {
        cout << "AuthenticationFilter request: " << request << endl;
    }
};

// 过滤器DebugFilter
class DebugFilter : public Filter {
public:
    DebugFilter(){};
    ~DebugFilter(){};
    void execute(string request) {
        cout << "DebugFilter request: " << request << endl;
    }
};

// 请求处理程序
class Target {
public:
    Target(){};
    ~Target(){};
    void execute(string request) {
        cout << "Target request: " << request << endl;
    }
};

// 过滤器链，将多个过滤器实体对象连接起来，实现更强大的过滤功能
class FilterChain {
public:
    FilterChain(){};
    ~FilterChain() {
        list< Filter* >::iterator it;
        for (it = this->filterList.begin(); it != this->filterList.end();
             ++it) {
            delete (*it);
            (*it) = NULL;
        }
        delete this->target;
        this->target = NULL;
    }
    
    void addFilter(Filter* filter) {
        this->filterList.push_back(filter);
    }

    void execute(string request) {
        list< Filter* >::iterator it;
        for (it = this->filterList.begin(); it != this->filterList.end();
             ++it) {
            (*it)->execute(request);
        }
        this->target->execute(request);
    }
    
    void setTarget(Target* target) {
        this->target = target;
    }

private:
    list< Filter* > filterList;
    Target*         target;
};

// 过滤器管理类
class FilterManager {
public:
    FilterManager(Target* target) {
        this->filterchain = new FilterChain();
        this->filterchain->setTarget(target);
    }
    ~FilterManager() {
        delete this->filterchain;
        this->filterchain = NULL;
    }
    void setFilter(Filter* filter) {
        this->filterchain->addFilter(filter);
    }
    void filterRequest(string request) {
        this->filterchain->execute(request);
    }

private:
    FilterChain* filterchain;
};

// 客户端
class Client {
public:
    Client(){};
    ~Client(){};
    void setFilterManager(FilterManager* filterManager) {
        this->filtermanager = filterManager;
    }
    void sendRequest(string request) {
        this->filtermanager->filterRequest(request);
    }

private:
    FilterManager* filtermanager;
};

int main() {
    FilterManager* filtermanager = new FilterManager(new Target());
    filtermanager->setFilter(new AuthenticationFilter());
    filtermanager->setFilter(new DebugFilter());

    Client* client = new Client();
    client->setFilterManager(filtermanager);
    client->sendRequest("Home");

    delete filtermanager;
    filtermanager = NULL;

    delete client;
    client = NULL;

    system("pause");
    return 0;
}