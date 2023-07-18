#define LZH_EXPORTS
#include "timer.h"
#include <sstream>
#include <ctime>
#include <windows.h>
#include <io.h>
#include <direct.h>
namespace lzh
{
    std::string TimeData::toString()const
    {
        std::stringstream out;
        out << year << "/"
            << month << "/"
            << day << " "
            << h << ":"
            << m << ": "
            << s << "."
            << ms << "s";
        return out.str();
    }

    std::string TimeData::toTime() const
    {
        std::stringstream out;
        out << 1900 + year << "/"
            << 1 + month << "/"
            << day << " "
            << h << ":"
            << m << ":"
            << s;
        return out.str();
    }
    LZHAPI TimeData NowTime()
    {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        TimeData timeData;
        timeData.year =  ltm->tm_year;
        timeData.month = ltm->tm_mon;
        timeData.day = ltm->tm_mday;
        timeData.h = ltm->tm_hour;
        timeData.m = ltm->tm_min;
        timeData.s = ltm->tm_sec;
        return timeData;
    }
    Timer::Timer()
    {
        start = new LARGE_INTEGER();
        end = new LARGE_INTEGER();
        fc = new LARGE_INTEGER();
        QueryPerformanceFrequency((LARGE_INTEGER*)fc);
    }
    Timer::~Timer()
    {
        FREE_PTR_NONULL((LARGE_INTEGER*)start);
        FREE_PTR_NONULL((LARGE_INTEGER*)end);
        FREE_PTR_NONULL((LARGE_INTEGER*)fc);
    }
    void Timer::Start()
    {
        QueryPerformanceCounter((LARGE_INTEGER*)start);
    }
    uint64 Timer::End()
    {
        QueryPerformanceCounter((LARGE_INTEGER*)end);
        return lzh::saturate_cast<uint64>(((((LARGE_INTEGER*)end)->QuadPart - ((LARGE_INTEGER*)start)->QuadPart) * 1000000.0) / ((LARGE_INTEGER*)fc)->QuadPart);
    }
    TimeData Timer::EndTime()
    {
        return TimeData(End());
    }
    static LARGE_INTEGER cpuFreq;
    static LARGE_INTEGER startTime;
    static LARGE_INTEGER endTime;

    void Frequency()
    {
        if (cpuFreq.QuadPart == 0)
            QueryPerformanceFrequency(&cpuFreq);
    }
    void StartCounter()
    {
        QueryPerformanceCounter(&startTime);
    }
    mat_t EndCounter()
    {
        QueryPerformanceCounter(&endTime);
        return _T(((endTime.QuadPart - startTime.QuadPart) * 1000.0) / cpuFreq.QuadPart);
    }
}
