
#ifndef EVENT_GENERATOR_TEST
#define EVENT_GENERATOR_TEST

#include "event_generator.h"

#include <functional>
#include <thread>
#include <iostream>
#include <string>
#include <cassert>

class event_generator_test
{
public:
    void scenario1()
    {
        std::cout << "checking scenario1...";
        e.set_period_milli(10);
        e.enable();
        std::thread t([this]() {
            e.run(std::bind(&event_generator_test::test1_func, this));
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        e.disable();
        t.join();
        assert_true("scenario1 failed", true == called);
        std::cout << "done" << std::endl;
    }

    void scenario2()
    {
        std::cout << "checking scenario2...";
        called = false;
        e.set_period_milli(50);
        e.enable();
        std::thread t([this]() {
            e.run(std::bind(&event_generator_test::test1_func, this));
        });
        e.disable();
        t.join();
        assert_true("scenario2 failed", false == called);
        std::cout << "done" << std::endl;
    }

    void scenario3()
    {
        std::cout << "checking scenario3...";
        count = 0;
        e.set_period_milli(10);
        e.enable();
        std::thread t([this]() {
            e.run(std::bind(&event_generator_test::test2_func, this));
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        e.disable();
        t.join();
        assert_true("scenario3 failed", 1 == count);
        std::cout << "done" << std::endl;
    }

    void scenario4()
    {
        std::cout << "checking scenario4...";
        count = 0;
        e.set_period_milli(30);
        e.enable();
        e.run(std::bind(&event_generator_test::test3_func, this));
        assert_true("scenario4 failed", 1 == count);
        std::cout << "done" << std::endl;
    }

    void scenario5()
    {
        std::cout << "checking scenario5...";
        count = 0;
        e.set_period_milli(30);
        e.enable();
        e.disable();
        e.run(std::bind(&event_generator_test::test3_func, this));
        assert_true("scenario5 failed", 0 == count);
        std::cout << "done" << std::endl;
    }

private:
    /// could throw instead
    void assert_true(const std::string& m, bool cond) 
    {
        if (!cond) {
            std::cerr << std::endl << m << std::endl;
            assert(false);
        }
    }

private:
    event_generator e;

    void test1_func()
    {
        called = true;
    }
    bool called = false;

    void test2_func()
    {
        ++count;
    }
    int count = 0;

    void test3_func()
    {
        if (count == 0) {
            e.disable();
        }
        ++count;
    }

public:
    void run()
    {
        scenario1();
        scenario2();
        scenario3();
        scenario4();
        scenario5();
    }
};

#endif // EVENT_GENERATOR_TEST
