#ifndef TIMER_H
#define TIMER_H

#include "core.h"

#define MEASURE_TIME(function) do{\
        lzh::Timer _timer_;\
        _timer_.Start();\
        function;\
        print(0x0002, "running function %s -> cast time is %0.4f sec\n", STR2(function), _timer_.End() / 1000000.0);\
        }while(0)

namespace lzh
{
    class LZHAPI TimeData
    {
    public:
        TimeData(){
            memset(this, 0, sizeof(TimeData));
        }
        TimeData(uint64 t) : t(t) {
            ms = uint32((t) % 1000);
            s = uint32((t / 1000) % 60);
            m = uint32((t / 1000) / 60 % 60);
            h = uint32((t / 1000) / 3600 % 60);
            day = uint32((t / 1000) / (3600 * 24) % 31);
            month = 0;
            year = 0;
        }
        uint32 year;
        uint32 month;
        uint32 day;
        uint32 h;
        uint32 m;
        uint32 s;
        uint32 ms;
        uint64 t;

        std::string toString()const;
        std::string toTime()const;
    };
    extern LZHAPI TimeData NowTime();
    class LZHAPI Timer
    {
    public:
        Timer();
        ~Timer();
        void Start();
        uint64 End();
        TimeData EndTime();
    protected:
        void* start;
        void* end;
        void* fc;
    private:
        Timer(const Timer&){}
        Timer& operator = (const Timer&){return *this;}
    };
    extern LZHAPI void Wait(uint32 ms);
    extern LZHAPI void Frequency();
    extern LZHAPI void StartCounter();
    extern LZHAPI mat_t EndCounter();
}

#endif // TIMER_H
