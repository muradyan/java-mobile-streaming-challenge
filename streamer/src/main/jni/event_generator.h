
#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H

#include <mutex>
#include <condition_variable>
#include <functional>

/**
 @brief simple abstraction of event generator for the given period
*/
class event_generator
{
public:
    event_generator()
      : _stopped(true)
      , _period(500)
    {}

    /// stops the generator
    void stop()
    {
        std::unique_lock<std::mutex> lck(_mutex);
        _stopped = true;
        _condition_variable.notify_one();
    }

    /// starts the generator
    void start()
    {
        std::unique_lock<std::mutex> lck(_mutex);
        _stopped = false;
    }

    /// sets period in milliseconds; how often the event should be triggered
    void set_period_milli(int period)
    {
        _period = period;
    }

    /**
     calls the callback in a loop with defined period
     Loop stops if the application is put to background or is closed.
    */
    void run(std::function<void()>&& f)
    {
        do {
            f();
        } while (!wait_until());
    }

private:
    /*
     blocks the thread until either _period elapses or _stopped becomes true
     returns true if _stopped became true

     @todo - check the performance if switched to a solution based on
     std::this_thread::yield and/or std::this_thread::sleep_for
     */
    bool wait_until()
    {
        std::unique_lock<std::mutex> lck(_mutex);
        auto a = std::chrono::duration<int,std::milli>(_period);
        return _condition_variable.wait_for(lck, a, [this]{return _stopped;});
    }

    /// disabling the move & copy constructors, assignment operators
private:
    event_generator(const event_generator&) = delete;
    event_generator& operator=(const event_generator&) = delete;
    event_generator(event_generator&&) = delete;
    event_generator& operator=(event_generator&&) = delete;

private:
    /// indicates whether event generator is stopped or not
    bool _stopped;

    // the default value is 0.5 seconds
    int _period;

    std::mutex _mutex;

    std::condition_variable _condition_variable;
};

#endif // EVENT_GENERATOR_H
