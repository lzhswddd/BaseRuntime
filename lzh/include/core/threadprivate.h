#ifndef THREADPRIVATE_H
#define THREADPRIVATE_H

#include "thread.h"
#include "pointer.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace lzh {
class ThreadPrivate
{
    LZH_DECLARE_PUBLIC(Thread)
    LZH_DISABLE_COPY(ThreadPrivate)
public:
    ThreadPrivate(Thread* = nullptr);
    ~ThreadPrivate();

    std::thread* _thread;
    std::mutex _mutex;
    std::condition_variable _condition;
    std::atomic_bool _pauseFlag;   ///<暂停标识
    std::atomic_bool _stopFlag;   ///<停止标识
    Thread::State _state;
    Thread::Priority _priority;
private:
    void run();
};
}
#endif // THREADPRIVATE_H
