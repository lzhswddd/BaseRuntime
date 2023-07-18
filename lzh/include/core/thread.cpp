#define LZH_EXPORTS
#include "threadprivate.h"
#include <iostream>
#include <windows.h>
using namespace std;

lzh::ThreadPrivate::ThreadPrivate(Thread *ptr)
    : q_ptr(ptr),
      _thread(nullptr),
      _pauseFlag(false),
      _stopFlag(false),
      _state(Thread::Stoped)
{

}

lzh::ThreadPrivate::~ThreadPrivate()
{

}

void lzh::ThreadPrivate::run()
{
//    cout << "enter thread:" << this_thread::get_id() << endl;

    while (!_stopFlag)
    {
        q_func()->process();
        if (_pauseFlag)
        {
            unique_lock<mutex> locker(_mutex);
            while (_pauseFlag)
            {
                _condition.wait(locker); // Unlock _mutex and wait to be notified
            }
            locker.unlock();
        }
    }
    _pauseFlag = false;
    _stopFlag = false;
    _state = Thread::Stoped;

    cout << "exit thread:" << this_thread::get_id() << endl;
}

/*********************************************************************/

lzh::Thread::Thread()
    :d_ptr(new ThreadPrivate(this))
{
}

lzh::Thread::~Thread()
{
    waitForDone();

}

lzh::Thread::State lzh::Thread::state() const
{
    LZH_D(const Thread);
    return d->_state;
}

void lzh::Thread::start(lzh::Thread::Priority priority)
{
    LZH_D(Thread);
    if (d->_thread == nullptr)
    {
        d->_thread = new std::thread(&ThreadPrivate::run, d);
        d->_pauseFlag = false;
        d->_stopFlag = false;
        d->_state = Thread::Running;
        setPriority(priority);
    }
}

void lzh::Thread::stop()
{
    LZH_D(Thread);
    if (d->_thread != nullptr)
    {
        d->_pauseFlag = false;
        d->_stopFlag = true;
        d->_condition.notify_all();  // Notify one waiting thread, if there is one.
    }
}

void lzh::Thread::pause()
{
    LZH_D(Thread);
    if (d->_thread != nullptr)
    {
        d->_pauseFlag = true;
        d->_state = Thread::Paused;
    }
}

void lzh::Thread::resume()
{
    LZH_D(Thread);
    if (d->_thread != nullptr)
    {
        d->_pauseFlag = false;
        d->_condition.notify_all();
        d->_state = Thread::Running;
    }
}

void lzh::Thread::wait()
{
    LZH_D(Thread);
    if (d->_thread != nullptr){
        if(d->_thread->joinable()){
            d->_thread->join(); // wait for thread finished
        }
        delete d->_thread;
        d->_thread = nullptr;
        d->_state = Thread::Stoped;
    }
}

void lzh::Thread::waitForDone()
{
    stop();
    wait();
}

lzh::Thread::Priority lzh::Thread::priority()
{
    LZH_D(Thread);
    return d->_priority;
}

void lzh::Thread::setPriority(Priority priority)
{
    LZH_D(Thread);
    if(state() != Thread::Stoped){
        auto handle = d->_thread->native_handle();
        if(SetThreadPriority(handle, (int)d->_priority))
            d->_priority = priority;
    }
}

bool lzh::Thread::isRunning() const noexcept
{
    return state() != Thread::Stoped;
}

bool lzh::Thread::isFinished() const noexcept
{
    return state() == Thread::Stoped;
}

void lzh::Thread::sleep(lzh::uint32 wait_sec)
{
    std::this_thread::sleep_for(std::chrono::seconds(wait_sec));
}

void lzh::Thread::msleep(lzh::uint32 wait_msec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_msec));
}

void lzh::Thread::usleep(lzh::uint32 wait_usec)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::microseconds(wait_usec);
    do {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}
