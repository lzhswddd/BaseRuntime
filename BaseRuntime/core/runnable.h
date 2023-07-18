#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <queue>
#include "eventhandler.h"

namespace lzh
{
    class LZHAPI Runnable
    {
        bool m_autoDelete = true;

        LZH_DISABLE_COPY(Runnable)
    public:
        virtual void run() = 0;

        explicit Runnable() noexcept = default;
        virtual ~Runnable();
        static Runnable* create(std::function<void()> functionToRun);

        bool autoDelete() const { return m_autoDelete; }
        void setAutoDelete(bool autoDelete) { m_autoDelete = autoDelete; }
    };
}
#endif // RUNNABLE_H
