#include <iostream>
#include <string>
#include <iomanip>

#include <vector>
#include <algorithm>

#include "220913_queue_customer.h"
#include "220913_queue_checkout.h"

#include <random>
#include <numeric>

using std::string;
using distribution = std::uniform_int_distribution<>;

/*
 * Description : 输出服务时间直方图
 * Param1 : vector<int> & v  服务时间向量
 * Param2 : min  最短服务时长
 * Description2 : 以固定数量的结账柜台
 */
void histogram( const std::vector<int> & v, int min )
{
    string bar( 60, '*' );
    for (size_t i {}; i < v.size(); ++i)
    {
        std::cout 
            << std::setw(3) << i+min << " "
            << std::setw(4) << v[i] << " "
            << bar.substr(0, v[i])
            << (v[i] > static_cast<int>(bar.size()) ? "..." : "")
            << std::endl;
    }
}


int main(int argc, char const *argv[])
{
    /* code */

    // 随机数生成器
    std::random_device random_n;

    // 服务时长
    int service_t_min { 2 }, service_t_max { 15 };
    distribution service_t_d { service_t_min, service_t_max };

    // 顾客数量
    int customers_min { 15 }, customers_max { 20 };
    distribution customers_t_d { customers_min, customers_max };

    // 顾客到达数量
    int interval_min { 1 }, interval_max { 5 };
    distribution arrival_interval_d { interval_min, interval_max };

    // 结账数量
    size_t n_checkouts { }; // 默认为 0
    std::cout << "超市可结账的数量：";
    std::cin >> n_checkouts;
    if (!n_checkouts)
    {
        std::cout << "Number of checkouts must be greater than 0. Setting to 1." << std::endl;
        n_checkouts = 1;
    }
    
    // 结账数量——向量
    std::vector<Checkout> checkouts { n_checkouts };
    // 服务时长——向量
    std::vector<int> service_times(service_t_max - service_t_min + 1);

    // 当前等待开店的顾客总数
    int count {customers_t_d(random_n)};
    std::cout << "等待开店的顾客：" << count << std::endl;
    // 添加后的顾客数量
    int added { };
    // 顾客结账需要的时长
    int service_t { };
    while (added++ < count) 
    {
        service_t = service_t_d(random_n);
        // 寻找最短的 Checkout 对象队列，将顾客分配到最短的结账队列中
        std::min_element(std::begin(checkouts), std::end(checkouts))->add(Customer(service_t));
        ++service_times[service_t - service_t_min];
    }
    
    // 商店经过的时间（存储过去的时间）
    size_t time { };
    // 模拟持续的时间—— 1 分钟
    const size_t total_time { 600 };
    // 商店最长的结账队列长度
    size_t longest_q{ };

    // 直到下一个客户到达的时间
    int new_cust_interval { arrival_interval_d(random_n) };

    // 在 total_time 分钟期间内运行商店模拟
    while (time < total_time) 
    {
        ++time;

        // 到达间隔时间为 0 时新顾客到达
        if ( --new_cust_interval == 0 )
        {
            // 顾客服务时长
            service_t = service_t_d(random_n);
            // 寻找最短队列，将顾客分配到最短的结账队列中
            std::min_element(std::begin(checkouts), std::end(checkouts))->add(Customer(service_t));
            // 记录服务时长
            ++service_times[service_t - service_t_min];

            // 更新最长结账队列的记录
            for (auto & checkout : checkouts)
            {
                longest_q = std::max(longest_q, checkout.qlength());
            }
            
            // 新客户到达
            new_cust_interval = arrival_interval_d(random_n);
        }
        
        // 更新结账时间 - 为每个队列中的第一个客户提供服务
        for (auto & checkout : checkouts)
        {
            // 减少剩余的服务时间
            checkout.time_increment();
        }
    }

    // 最长结账队列
    std::cout << "最长的结账队列长度：" << longest_q << std::endl;
    std::cout << "\n服务时长直方图：\n";
    histogram(service_times, service_t_min);

    std::cout 
            << "\n当天顾客总数："
            << std::accumulate(std::begin(service_times), std::end(service_times), 0)
            << std::endl;

    return 0;
}
