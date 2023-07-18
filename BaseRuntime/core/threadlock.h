#ifndef THREADLOCK_H
#define THREADLOCK_H

#include "core.h"
#include <mutex>

namespace lzh{
class LZHAPI MutexLocker
{
public:
    explicit MutexLocker(std::mutex *mutex);
    ~MutexLocker();
    void unlock();
    void relock();
    std::mutex* mutex()noexcept;
private:
    LZH_DISABLE_COPY(MutexLocker)
    ScopedPointer<std::unique_lock<std::mutex>> locker;
    bool isLocked;
};
}

#endif // THREADLOCK_H
