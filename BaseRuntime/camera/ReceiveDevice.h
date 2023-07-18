#ifndef CAMERARECEIVE_H
#define CAMERARECEIVE_H

#include <queue>
#include <mutex>
#include "../core/image.h"
#include "../core/eventhandler.h"
#include "../core/mult_thread.h"

namespace lzh
{
    class LZHAPI CameraReceive
    {
    public:
        CameraReceive(): Receive() {}
        virtual void Grab(const lzh::Image & image){
            Receive(image);
        }
        EventHandler<lzh::Image> Receive;
    };
    template<typename _Tp>
    class TEMPLATE_API ProcessReceive
    {
    public:
        ProcessReceive(int threadNum = 1): Receive(), thread(threadNum) {
            funHandle = [&](const lzh::Image &image){
                if(allow){
                    _Tp task = Pending.first();
                    while(task->Empty()){
                        Pending.dequeue();
                        if(!Pending.empty()){
                            task = Pending.first();
                        }
                        else return;
                        //qDebug()<<"camera shoot count: "<<count;count=0;
                    }
                    Receive(task, image);
                }
            };
            thread.Process = funHandle;
        }
        ~ProcessReceive(){if(thread.IsRunning())thread.WaitExit();}
        bool isRunning()const{ return thread.IsRunning();}
        void Start(){if(isRunning())return;allow=true;thread.Start();}
        void Stop(){thread.Stop();}
        void WaitExit(){thread.WaitExit();}
        void Abort(){allow=false;thread.Abort();Pending.clear();}
        void Grab(const lzh::Image & image) {
//            qDebug()<<"camera shoot count: "<<count;
            if(block)return;
            if(!image.empty())count+=1;
            if(!image.empty() && !Pending.empty())
            {
                if(thread.IsRunning()){
                    thread.Append(image);
                }else{
                    funHandle(image);
                }
            }
            else if(Pending.empty()){
//                err_count+=1;
                //qDebug()<<"shooting_count:"<<err_count;
                //lzh::imwrite("error\\"+lzh::Convert::toString(err_count)+".jpg", image);
            }
        }
        void ReleasePop(){
            if(!Pending.empty()){
                _Tp task = Pending.dequeue();
                FREE_PTR(task);
            }
            //qDebug()<<"camera shoot count: "<<count;
            count=0;
        }
        void Clear(){count=0;err_count=0;Pending.clear();}
        void Block(bool status){block=status;}
        bool block = false;
        bool allow;
        int count=0;
        int err_count=0;
        std::queue<_Tp> Pending;
        EventHandler<_Tp, const lzh::Image &> Receive;
        lzh::MultThread<lzh::Image> thread;
        std::function<void(const lzh::Image &)> funHandle;
        std::mutex lock;
    };
}
#endif // CAMERARECEIVE_H
