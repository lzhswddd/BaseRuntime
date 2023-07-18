#define LZH_EXPORTS
#include "threadpoolprivate.h"
#include "threadpool.h"
#include "thread.h"
#include "threadlock.h"

namespace lzh {
class ThreadPoolThread : public Thread
{
public:
    ThreadPoolThread(ThreadPoolPrivate *manager);
    void process() override;
    void registerThreadInactive();

    AutoResetEvent runnableReady;
    ThreadPoolPrivate *manager;
    Runnable *runnable;
};

ThreadPoolThread::ThreadPoolThread(ThreadPoolPrivate *manager)
    :runnableReady(true), manager(manager), runnable(nullptr)
{
}

void ThreadPoolThread::process()
{
    MutexLocker locker(&manager->mutex);
    for(;;) {
        Runnable *r = runnable;
        runnable = nullptr;

        do {
            if (r) {
                // If autoDelete() is false, r might already be deleted after run(), so check status now.
                const bool del = r->autoDelete();

                // run the task
                locker.unlock();
                try {
                    r->run();
                } catch (...) {
                    registerThreadInactive();
                    throw;
                }

                if (del)
                    delete r;
                locker.relock();
            }

            // if too many threads are active, expire this thread
            if (manager->tooManyThreadsActive())
                break;

            if (manager->queue.isEmpty()) {
                r = nullptr;
                break;
            }

            QueuePage *page = manager->queue.first();
            r = page->pop();

            if (page->isFinished()) {
                manager->queue.removeFirst();
                delete page;
            }
        } while (true);

        // if too many threads are active, expire this thread
        bool expired = manager->tooManyThreadsActive();
        if (!expired) {
            manager->waitingThreads.enqueue(this);
            registerThreadInactive();
            // wait for work, exiting after the expiry timeout is reached
            locker.unlock();
            runnableReady.WaitOne(manager->expiryTimeout);
            //runnableReady.Reset();
            locker.relock();
            ++manager->activeThreads;
            if (manager->waitingThreads.removeOne(this))
                expired = true;
            if (!manager->allThreads.contains(this)) {
                registerThreadInactive();
                break;
            }
        }
        if (expired) {
            manager->expiredThreads.enqueue(this);
            registerThreadInactive();
            break;
        }
    }
//    std::cout << "finish thread:" << std::this_thread::get_id() << std::endl;
    stop();
}

void ThreadPoolThread::registerThreadInactive()
{
    if (--manager->activeThreads == 0)
        manager->noActiveThreads.Set();
}
/*************************************************************************************/
lzh::ThreadPoolPrivate::ThreadPoolPrivate(lzh::ThreadPool *d_ptr)
    :q_ptr(d_ptr), noActiveThreads(true)
{
    requestedMaxThreadCount = std::thread::hardware_concurrency();
}

lzh::ThreadPoolPrivate::~ThreadPoolPrivate()
{
    for (ThreadPoolThread *thread : allThreads) {
        if (!thread->isFinished()) {
            thread->runnableReady.Set();
            thread->wait();
        }
        delete thread;
    }
    clear();
    q_ptr=nullptr;
}

bool ThreadPoolPrivate::tryStart(Runnable *task)
{
    LZH_ASSERT(task != nullptr);
    if (allThreads.empty()) {
        // always create at least one thread
        startThread(task);
        return true;
    }

    // can't do anything if we're over the limit
    if (activeThreadCount() >= maxThreadCount())
        return false;

    if (waitingThreads.size() > 0) {
        // recycle an available thread
        enqueueTask(task);
        waitingThreads.front()->runnableReady.Set();
        return true;
    }

    if (!expiredThreads.empty()) {
        // restart an expired thread
        ThreadPoolThread *thread = expiredThreads.dequeue();
        LZH_ASSERT(thread->runnable == nullptr);

        ++activeThreads;

        thread->runnable = task;
        thread->start();
        return true;
    }

    // start a new thread
    startThread(task);
    return true;
}
inline bool comparePriority(Thread::Priority priority, const QueuePage *p)
{
    return p->priority() < priority;
}
void ThreadPoolPrivate::enqueueTask(Runnable *runnable, Thread::Priority priority)
{
    LZH_ASSERT(runnable != nullptr);
    for (QueuePage *page : queue) {
        if (!page->isFull()) {
            page->push(runnable);
            return;
        }
    }
    auto it = std::upper_bound(queue.begin(), queue.end(), priority, comparePriority);
    queue.insert(std::distance(queue.begin(), it), new QueuePage(runnable, priority));
}

int ThreadPoolPrivate::activeThreadCount() const
{
    return int(allThreads.count()
            - expiredThreads.count()
            - waitingThreads.count()
            + reservedThreads);
}

void ThreadPoolPrivate::tryToStartMoreThreads()
{
    while (!queue.isEmpty()) {
        QueuePage *page = queue.first();
        if (!tryStart(page->first()))
            break;

        page->pop();

        if (page->isFinished()) {
            queue.removeFirst();
            delete page;
        }
    }
}

bool ThreadPoolPrivate::tooManyThreadsActive() const
{
    const int activeThreadCount = this->activeThreadCount();
    return activeThreadCount > maxThreadCount() && (activeThreadCount - reservedThreads) > 1;
}

void ThreadPoolPrivate::startThread(Runnable *runnable)
{
    LZH_ASSERT(runnable != nullptr);
    ScopedPointer<ThreadPoolThread> thread(new ThreadPoolThread(this));
    LZH_ASSERT(!allThreads.contains(thread.data())); // if this assert hits, we have an ABA problem (deleted threads don't get removed here)
    allThreads.insert(thread.data());
    ++activeThreads;

    thread->runnable = runnable;
    thread.take()->start(threadPriority);
}

void ThreadPoolPrivate::reset()
{
    Set<ThreadPoolThread *> allThreadsCopy;
    allThreadsCopy.swap(allThreads);
    expiredThreads.clear();
    waitingThreads.clear();
    mutex.unlock();

    for (ThreadPoolThread *thread : allThreadsCopy) {
        if (!thread->isFinished()) {
            thread->runnableReady.Set();
            thread->wait();
        }
        delete thread;
    }

    mutex.lock();
}

bool ThreadPoolPrivate::waitForDone(int msecs)
{
    while (!(queue.isEmpty() && activeThreads == 0))
        noActiveThreads.WaitOne(msecs);

    return queue.isEmpty() && activeThreads == 0;
}

void ThreadPoolPrivate::clear()
{
    MutexLocker locker(&mutex);
    while (!queue.isEmpty()) {
        auto *page = queue.last();
        while (!page->isFinished()) {
            Runnable *r = page->pop();
            if (r && r->autoDelete()) {
                locker.unlock();
                delete r;
                locker.relock();
            }
        }
        delete page;
    }
}

void ThreadPoolPrivate::stealAndRunRunnable(Runnable *runnable)
{
    LZH_Q(ThreadPool);
    if (!q->tryTake(runnable))
        return;
    // If autoDelete() is false, runnable might already be deleted after run(), so check status now.
    const bool del = runnable->autoDelete();

    runnable->run();

    if (del)
        delete runnable;
}

//void ThreadPoolPrivate::deletePageIfFinished(QueuePage *page)
//{

//}

/*************************************************************************************/
lzh::ThreadPool::ThreadPool()
    :d_ptr(new ThreadPoolPrivate(this))
{

}

lzh::ThreadPool::~ThreadPool()
{
    waitForDone();
}

lzh::ThreadPool *lzh::ThreadPool::globalInstance()
{
    static Pointer<ThreadPool> theInstance;
    static std::mutex theMutex;

    const MutexLocker locker(&theMutex);
    if (theInstance.isNull())
        theInstance.create();
    return theInstance;
}

void ThreadPool::start(Runnable *runnable, Thread::Priority priority)
{
    if (!runnable)
        return;

    LZH_D(ThreadPool);
    MutexLocker locker(&d->mutex);

    if (!d->tryStart(runnable)) {
        d->enqueueTask(runnable, priority);

        if (!d->waitingThreads.empty())
            d->waitingThreads.front()->runnableReady.Set();
    }
}

void ThreadPool::start(std::function<void ()> functionToRun, Thread::Priority priority)
{
    if (!functionToRun)
        return;
    start(Runnable::create(std::move(functionToRun)), priority);
}

bool lzh::ThreadPool::tryStart(lzh::Runnable *runnable)
{
    if (!runnable)
        return false;

    LZH_D(ThreadPool);
    MutexLocker locker(&d->mutex);
    if (d->tryStart(runnable))
        return true;

    return false;
}

bool ThreadPool::tryStart(std::function<void ()> functionToRun)
{
    if (!functionToRun)
        return false;

    LZH_D(ThreadPool);
    MutexLocker locker(&d->mutex);
    if (!d->allThreads.empty() && d->activeThreadCount() >= d->maxThreadCount())
        return false;

    Runnable *runnable = Runnable::create(std::move(functionToRun));
    if (d->tryStart(runnable))
        return true;
    delete runnable;
    return false;
}

int lzh::ThreadPool::expiryTimeout() const
{
    LZH_D(const ThreadPool);
    return d->expiryTimeout;
}

void lzh::ThreadPool::setExpiryTimeout(int expiryTimeout)
{
    LZH_D(ThreadPool);
    if (d->expiryTimeout == expiryTimeout)
        return;
    d->expiryTimeout = expiryTimeout;
}

int lzh::ThreadPool::maxThreadCount() const
{
    LZH_D(const ThreadPool);
    return d->requestedMaxThreadCount;
}

void lzh::ThreadPool::setMaxThreadCount(int maxThreadCount)
{
    LZH_D(ThreadPool);
    MutexLocker locker(&d->mutex);

    if (maxThreadCount == d->requestedMaxThreadCount)
        return;

    d->requestedMaxThreadCount = maxThreadCount;
    d->tryToStartMoreThreads();
}

int lzh::ThreadPool::activeThreadCount() const
{
    LZH_D(const ThreadPool);
    MutexLocker locker(&d->mutex);
    return d->activeThreadCount();
}

void lzh::ThreadPool::setStackSize(lzh::uint32 stackSize)
{
    LZH_D(ThreadPool);
    d->stackSize = stackSize;
}

lzh::uint32 lzh::ThreadPool::stackSize() const
{
    LZH_D(const ThreadPool);
    return d->stackSize;
}

void ThreadPool::setThreadPriority(Thread::Priority priority)
{
    LZH_D(ThreadPool);
    d->threadPriority = priority;
}

Thread::Priority ThreadPool::threadPriority() const
{
    LZH_D(const ThreadPool);
    return d->threadPriority;
}

void lzh::ThreadPool::reserveThread()
{
    LZH_D(ThreadPool);
    MutexLocker locker(&d->mutex);
    ++d->reservedThreads;
}

void lzh::ThreadPool::releaseThread()
{
    LZH_D(ThreadPool);
    MutexLocker locker(&d->mutex);
    --d->reservedThreads;
    d->tryToStartMoreThreads();
}

bool lzh::ThreadPool::waitForDone(int msecs)
{
    LZH_D(ThreadPool);
    return d->waitForDone(msecs);
}

void lzh::ThreadPool::clear()
{
    LZH_D(ThreadPool);
    d->clear();
}

bool ThreadPool::contains(const Thread *thread) const
{
    LZH_D(const ThreadPool);
    const ThreadPoolThread *poolThread = dynamic_cast<const ThreadPoolThread *>(thread);
    if (!poolThread)
        return false;
    MutexLocker locker(&d->mutex);
    return d->allThreads.contains(const_cast<ThreadPoolThread *>(poolThread));
}

//void lzh::ThreadPool::cancel(lzh::Runnable *runnable)
//{

//}

bool lzh::ThreadPool::tryTake(lzh::Runnable *runnable)
{
    LZH_D(ThreadPool);

    if (runnable == nullptr)
        return false;

    MutexLocker locker(&d->mutex);
    for (QueuePage *page : d->queue) {
        if (page->tryTake(runnable)) {
            if (page->isFinished()) {
                d->queue.removeOne(page);
                delete page;
            }
            return true;
        }
    }

    return false;
}
}
