#ifndef CHECKOUT_H
#define CHECKOUT_H

#include "queue"
#include "220913_queue_customer.h"

class Checkout
{
private:
    std::queue<Customer> customers;

public:
    
    // 新增顾客
    void add(const Customer & customer)
    {
        customers.push(customer);
    }

    // 顾客数量（队列长度）
    size_t qlength() const
    {
        return customers.size();
    }

    // 顾客服务时长处理完成进行顾客出队
    void time_increment()
    {
        if (!customers.empty())
        {
            // 调用第一个顾客来减少服务时间，同时调用 done 完成顾客的服务，
            // 以此将第一个顾客出队列
            if (customers.front().time_decrement().done())
            {
                // 顾客服务时间为 0 时移出队列
                customers.pop();
            }
        }
    }

    // 运算符重载
    bool operator > (const Checkout & other)
    {
        return qlength() > other.qlength();
    }
    bool operator < (const Checkout & other)
    {
        return qlength() < other.qlength();
    }
};

#endif