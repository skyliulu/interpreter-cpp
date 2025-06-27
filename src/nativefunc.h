#pragma once
#include <chrono>
#include "callable.h"

class Clock : public Callable
{
private:
    /* data */
public:
    Clock(/* args */) {}
    ~Clock() {}
    int arity() override
    {
        return 0;
    }
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override
    {
        // 获取当前时间点
        auto now = std::chrono::system_clock::now();
        // 计算自 epoch (通常是 1970-01-01 00:00:00 UTC) 以来的持续时间
        auto duration_since_epoch = now.time_since_epoch();
        // 将持续时间转换为以毫秒为单位
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count();
        // 将毫秒数转换为以秒为单位的 double
        return static_cast<double>(millis) / 1000.0;
    }

    std::string to_string() const override
    {
        return "<fn clock>";
    }
};
