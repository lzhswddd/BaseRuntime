#ifndef MULT_THREAD
#define MULT_THREAD

#include <queue>
#include <thread>
#include <map>
#include "action.h"

namespace lzh
{
    template<typename _Tp>
    class TEMPLATE_API MultThread
    {
    public:
        MultThread(int threadNum);
        ~MultThread();

        void Start();
        void Stop();
        void Terminate();
        void Abort();
        void Join();
        void WaitExit();

        bool IsRunning()const;

        void Append(const _Tp& val);
        void Running();

        Action<_Tp> Process;

        static void ForEach(_Tp begin, _Tp end, const std::function<void(_Tp)>& ptr, int threadNum);
        static void ForEach(_Tp begin, _Tp end, const Action<_Tp> & ptr, int threadNum);
    protected:
        void release();
        void init(int threadNum);
    private:
        MultThread() {}
        MultThread(const MultThread<_Tp>& _) {}
        MultThread<_Tp>& operator = (const MultThread<_Tp>& _) { return this; }

        int count;
        int length;
        std::queue<_Tp>* taskQueue;
        std::thread* thread;
        std::map<std::thread::id, int>* threadID;
        std::map<std::thread::id, bool>* threadState;
        bool terminate;
        bool finish;
        int gapTime;
    };
}

#include "mult_thread_inl.hpp"

#endif // !MULT_THREAD
