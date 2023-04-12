#ifndef CUSTOMER_H
#define CUSTOMER_H

/*  
    模拟顾客
*/
class Customer
{
private:
    /* data */

protected:

    // 服务时间
    size_t service_t {};

public:
    
    // 显式调用
    explicit Customer(size_t st = 10) : service_t{ st }{};

    // 顾客存在服务时间则进行处理
    Customer & time_decrement()
    {
        if (service_t > 0)
        {
            -- service_t;
        }
        return * this;
    }

    // 处理完服务时间
    bool done() const
    {
        return service_t == 0;
    }
};

#endif 