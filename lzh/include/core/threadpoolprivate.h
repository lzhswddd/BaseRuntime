#ifndef THREADPOOL_PRIVATE_H
#define THREADPOOL_PRIVATE_H

#include <mutex>
#include "timer.h"
#include "pointer.h"
#include "thread.h"
#include "runnable.h"
#include "container.h"
#include "autoresetevent.h"

namespace lzh
{
    class QueuePage
    {
    public:
        enum {
            MaxPageSize = 256
        };

        QueuePage(Runnable *runnable, int pri) : m_priority(pri) { push(runnable); }

        bool isFull() { return m_lastIndex >= MaxPageSize - 1; }

        bool isFinished() { return m_firstIndex > m_lastIndex; }

        void push(Runnable *runnable)
        {
            LZH_ASSERT(runnable != nullptr);
            LZH_ASSERT(!isFull());
            m_lastIndex += 1;
            m_entries[m_lastIndex] = runnable;
        }

        void skipToNextOrEnd()
        {
            while (!isFinished() && m_entries[m_firstIndex] == nullptr) {
                m_firstIndex += 1;
            }
        }

        Runnable *first()
        {
            LZH_ASSERT(!isFinished());
            Runnable *runnable = m_entries[m_firstIndex];
            LZH_ASSERT(runnable);
            return runnable;
        }

        Runnable *pop()
        {
            LZH_ASSERT(!isFinished());
            Runnable *runnable = first();
            LZH_ASSERT(runnable);

            // clear the entry although this should not be necessary
            m_entries[m_firstIndex] = nullptr;
            m_firstIndex += 1;

            // make sure the next runnable returned by first() is not a nullptr
            skipToNextOrEnd();

            return runnable;
        }

        bool tryTake(Runnable *runnable)
        {
            LZH_ASSERT(!isFinished());
            for (int i = m_firstIndex; i <= m_lastIndex; i++) {
                if (m_entries[i] == runnable) {
                    m_entries[i] = nullptr;
                    if (i == m_firstIndex) {
                        // make sure first() does not return a nullptr
                        skipToNextOrEnd();
                    }
                    return true;
                }
            }
            return false;
        }

        int priority() const { return m_priority; }

    private:
        int m_priority = 0;
        int m_firstIndex = 0;
        int m_lastIndex = -1;
        Runnable *m_entries[MaxPageSize];
    };

    class ThreadPool;
    class ThreadPoolThread;
    class ThreadPoolPrivate
	{
        LZH_DECLARE_PUBLIC(ThreadPool)
        LZH_DISABLE_COPY(ThreadPoolPrivate)
    public:
        ThreadPoolPrivate(ThreadPool* = nullptr);
        ~ThreadPoolPrivate();

        bool tryStart(Runnable *task);
        void enqueueTask(Runnable *task, Thread::Priority priority = Thread::Priority::NORMAL);
        int activeThreadCount() const;

        void tryToStartMoreThreads();
        bool tooManyThreadsActive() const;

        int maxThreadCount() const
        { return std::max(requestedMaxThreadCount, 1); }    // documentation says we start at least one
        void startThread(Runnable *runnable = nullptr);
        void reset();
        bool waitForDone(int msecs);
        void clear();
        void stealAndRunRunnable(Runnable *runnable);
//        void deletePageIfFinished(QueuePage *page);

        mutable std::mutex mutex;
        Set<ThreadPoolThread *> allThreads;
        Queue<ThreadPoolThread *> waitingThreads;
        Queue<ThreadPoolThread *> expiredThreads;
        List<QueuePage *> queue;
        AutoResetEvent noActiveThreads;

        int expiryTimeout = 30000;
        int requestedMaxThreadCount;  // don't use this directly
        int reservedThreads = 0;
        int activeThreads = 0;
        uint32 stackSize = 0;
        Thread::Priority threadPriority = Thread::NORMAL;
    };
}
#endif // THREADPOOL_PRIVATE_H
