#include "mult_thread.h"
template<typename _Tp> inline lzh::MultThread<_Tp>::MultThread(int threadNum)
    : taskQueue(nullptr), thread(nullptr), threadID(nullptr), threadState(nullptr)
{
    init(threadNum);
}

template<typename _Tp> inline lzh::MultThread<_Tp>::~MultThread()
{
    if (IsRunning())Abort();
    release();
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::Start()
{
    if (!finish)return;
    finish = false; terminate = false;
    (*threadID).clear();
    (*threadState).clear();
    count = 0;
    int id = 0;
    for (int i = 0; i < length; i++)
    {
        thread[i] = std::thread([&]() {
            Running();
        });
        (*threadID)[thread[i].get_id()] = id++;
        (*threadState)[thread[i].get_id()] = true;
    }
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::Stop()
{
    finish = true;
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::Terminate()
{
    terminate = true;
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::Abort()
{
    for (int i = 0; i < length; i++)
    {
        thread[i].detach();
    }
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::Join()
{
    for (int i = 0; i < length; i++)
    {
        if (thread[i].joinable())
            thread[i].join();
    }
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::WaitExit()
{
    Stop(); Join();
}

template<typename _Tp> inline bool lzh::MultThread<_Tp>::IsRunning()const
{
    for (std::pair<std::thread::id, const bool> iter : *threadState)
    {
        if (iter.second)
            return true;
    }
    return false;
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::Append(const _Tp& val)
{
    taskQueue[count].push(val);
    count = (count + 1) % length;
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::Running()
{
    lzh::Wait(30);
    std::thread::id tid = std::this_thread::get_id();
    int id = (*threadID)[tid];
    std::queue<_Tp>* queue = &taskQueue[id];
    try {
        while ((!finish || queue->size() != 0) && !terminate)
        {
            if (queue->empty())
            {
                if (gapTime > 0)
                {
                    lzh::Wait(gapTime);
                }
                continue;
            }
            Process(queue->front());
            queue->pop();
        }
    }
    catch (std::exception ex) {

    }
    std::queue<_Tp>().swap(taskQueue[id]);
    (*threadState)[tid] = false;
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::ForEach(_Tp begin, _Tp end, const std::function<void(_Tp)>& ptr, int threadNum)
{
    MultThread<int> thread(threadNum > 0 ? threadNum : end - begin);
    thread.Process = ptr;
    for (_Tp i = begin; i != end; i++)
        thread.Append(i);
    thread.Start();
    thread.WaitExit();
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::ForEach(_Tp begin, _Tp end, const Action<_Tp>& ptr, int threadNum)
{
    MultThread<int> thread(threadNum > 0 ? threadNum : end - begin);
    thread.Process = ptr;
    for (_Tp i = begin; i != end; i++)
        thread.Append(i);
    thread.Start();
    thread.WaitExit();
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::release()
{
    if (taskQueue != nullptr) { delete[] taskQueue; taskQueue = nullptr; }
    if (thread != nullptr) { delete[] thread; thread = nullptr; }
    if (threadID != nullptr) { delete threadID; threadID = nullptr; }
    if (threadState != nullptr) { delete threadState; threadState = nullptr; }
}

template<typename _Tp> inline void lzh::MultThread<_Tp>::init(int threadNum)
{
    release();
    taskQueue = new std::queue<_Tp>[threadNum];
    thread = new std::thread[threadNum];
    threadID = new std::map<std::thread::id, int>();
    threadState = new std::map<std::thread::id, bool>();

    count = 0;
    length = threadNum;
    gapTime = 0;
    finish = true;
    Process = Action<_Tp>();
}

