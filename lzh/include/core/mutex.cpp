#define LZH_EXPORTS
#include "threadlock.h"

lzh::MutexLocker::MutexLocker(std::mutex *mutex)
    :locker(new std::unique_lock<std::mutex>(*mutex)) {
    isLocked = true;
}

lzh::MutexLocker::~MutexLocker()
{

}

void lzh::MutexLocker::unlock()
{
    if(isLocked){
        locker->unlock();
        isLocked = false;
    }
}

void lzh::MutexLocker::relock()
{
    if (isLocked)
        return;
    locker->lock();
    isLocked = true;
}

std::mutex *lzh::MutexLocker::mutex() noexcept
{
    return locker->mutex();
}
