#ifndef THREAD_H
#define THREAD_H

#include "core.h"

namespace lzh {
    class ThreadPrivate;
    class LZHAPI Thread
    {
        LZH_DECLARE_PRIVATE(Thread)
        LZH_DISABLE_COPY(Thread)
    public:
        explicit Thread();
        virtual ~Thread();

        enum State
        {
            Stoped,     ///<Í£Ö¹×´Ì¬£¬°üÀ¨´ÓÎ´Æô¶¯¹ýºÍÆô¶¯ºó±»Í£Ö¹
            Running,    ///<ÔËÐÐ×´Ì¬
            Paused      ///<ÔÝÍ£×´Ì¬
        };

        enum Priority
        {
            IDLE = -15,
            BELOW_NORMAL = -1,
            LOWEST = -2,
            NORMAL = 0,
            HIGHEST = 2,
            ABOVE_NORMAL = 1,
            TIME_CRITICAL = 15,
        };

        State state() const;

        void start(Priority priority = Priority::NORMAL);
        void stop();
        void pause();
        void resume();
        void wait();
        void waitForDone();

        Priority priority();
        void setPriority(Priority priority);

        bool isRunning()const noexcept;
        bool isFinished()const noexcept;

        static void sleep(uint32 wait_sec);
        static void msleep(uint32 wait_msec);
        static void usleep(uint32 wait_usec);
    protected:
        virtual void process() = 0;
    };
}

#endif // THREAD_H
