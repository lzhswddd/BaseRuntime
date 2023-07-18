#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <queue>
#include "thread.h"
#include "runnable.h"

namespace lzh
{
    class Thread;
	class ThreadPoolPrivate;
	class LZHAPI ThreadPool
    {
        LZH_DECLARE_PRIVATE(ThreadPool)
        LZH_DISABLE_COPY(ThreadPool)
    public:
        ThreadPool();
        ~ThreadPool();

        static ThreadPool *globalInstance();

        void start(Runnable *runnable, Thread::Priority priority = Thread::NORMAL);
        bool tryStart(Runnable *runnable);

        void start(std::function<void()> functionToRun, Thread::Priority priority = Thread::NORMAL);
        bool tryStart(std::function<void()> functionToRun);

        int expiryTimeout() const;
        void setExpiryTimeout(int expiryTimeout);

        int maxThreadCount() const;
        void setMaxThreadCount(int maxThreadCount);

        int activeThreadCount() const;

        void setStackSize(uint32 stackSize);
        uint32 stackSize() const;

        void setThreadPriority(Thread::Priority priority);
        Thread::Priority threadPriority() const;

        void reserveThread();
        void releaseThread();

        bool waitForDone(int msecs = -1);

        void clear();

        bool contains(const Thread *thread) const;

//        void cancel(Runnable *runnable);
        bool tryTake(Runnable *runnable);
    };
}
#endif // THREADPOOL_H
