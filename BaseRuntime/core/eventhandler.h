#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "signal.h"
#include "delegate.h"

namespace lzh
{
    template<typename _Ret>
    class TEMPLATE_API DelegateHandler
    {
    public:
    };
    template<typename _Ret, typename ...Args>
    class TEMPLATE_API DelegateHandler<_Ret(Args...)>
    {
    public:
        DelegateHandler():CHandle(nullptr),CppHandle(nullptr){}
        ~DelegateHandler(){clear();}
        void Connect(std::function<_Ret(Args...)> handle){
            if(CppHandle!=nullptr)FREE_PTR(CppHandle);
            if(CHandle==nullptr){
                CHandle = new DelegateC<_Ret(Args...)>();
            }
            *CHandle = DelegateC<_Ret(Args...)>(handle);
        }
        template<typename T> void Connect (T* val, _Ret(T::* slot)(Args...)){
            if(CHandle!=nullptr)FREE_PTR(CHandle);
            if(CppHandle==nullptr){
                CppHandle = new Delegate<_Ret(Args...)>();
            }
            *CppHandle = Connect(val, slot);device=val;
        }

        _Ret operator ()(const Args &... args){
            if(CHandle != nullptr){
                return CHandle->operator()(args...);
            }
            if(CppHandle != nullptr){
                return CppHandle->operator()(args...);
            }
            return _Ret();
        }

        bool empty() const{
            return (CppHandle==nullptr && CHandle==nullptr);
        }
        void clear() {
            FREE_PTR(CHandle);FREE_PTR(CppHandle);
        }
        void* GetDevice(){return device;}
    protected:
        void* device = nullptr;
        Delegate<_Ret(Args...)> *CppHandle;
        DelegateC<_Ret(Args...)> *CHandle;

    private:
        DelegateHandler(const DelegateHandler&){}
        DelegateHandler& operator = (const DelegateHandler&){ return *this; }
    };
    template<typename _Ret>
    class TEMPLATE_API DelegateHandler<_Ret(void)>
    {
    public:
        DelegateHandler():CHandle(nullptr),CppHandle(nullptr){}
        ~DelegateHandler(){clear();}
        void Connect(std::function<_Ret(void)> handle){
            if(CHandle==nullptr){
                CHandle = new DelegateC<_Ret(void)>();
            }
            *CHandle = DelegateC<_Ret(void)>(handle);
        }
        template<typename T> void Connect (T* val, _Ret(T::* slot)()){
            if(CppHandle==nullptr){
                CppHandle = new Delegate<_Ret()>();
            }
            *CppHandle = Connect(val, slot);
        }

        _Ret operator ()(){
            if(CHandle != nullptr){
                return CHandle->operator()();
            }
            if(CppHandle != nullptr){
                return CppHandle->operator()();
            }
            return _Ret();
        }

        bool empty() const{
            return (CppHandle==nullptr && CHandle==nullptr);
        }
        void clear() {
            FREE_PTR(CHandle);FREE_PTR(CppHandle);
        }
    protected:
        Delegate<_Ret(void)> *CppHandle;
        DelegateC<_Ret(void)> *CHandle;

    private:
        DelegateHandler(const DelegateHandler&){}
        DelegateHandler& operator = (const DelegateHandler&){ return *this; }
    };

    template<typename ...Args>
    class TEMPLATE_API EventHandler
    {
    public:
        EventHandler():CHandle(nullptr),CppHandle(nullptr){}
        ~EventHandler(){clear();}
        void Connect(std::function<void(Args...)> handle){
            if(CHandle==nullptr){
                CHandle = new Action<Args...>();
            }
            *CHandle += handle;
        }
        void DisConnect(std::function<void(Args...)> handle){
            if(CHandle==nullptr){
                CHandle = new Action<Args...>();
            }
            *CHandle -= handle;
        }
        template<typename T> void Connect (T* val, void(T::* slot)(Args...)){
            if(CppHandle==nullptr){
                CppHandle = new Signal<Args...>();
            }
            *CppHandle = Connect(val, slot);
        }

        template<typename T> void DisConnect (){
            if(CppHandle==nullptr){
                CppHandle = new Signal<Args...>();
            }
            *CppHandle = Signal<Args...>();
        }

        void operator ()(const Args &... args){
            if(CHandle != nullptr){
                CHandle->operator()(args...);
            }
            if(CppHandle != nullptr){
                CppHandle->operator()(args...);
            }
        }

        bool empty() const{
            return (CppHandle==nullptr && CHandle==nullptr);
        }
        void clear() {
            FREE_PTR(CHandle);FREE_PTR(CppHandle);
        }
    protected:
        Signal<Args...> *CppHandle;
        Action<Args...> *CHandle;

    private:
        EventHandler(const EventHandler&){}
        EventHandler& operator = (const EventHandler&){ return *this; }
    };

    template<> class TEMPLATE_API EventHandler<void>
    {
    public:
        EventHandler():CppHandle(nullptr),CHandle(nullptr){}
        ~EventHandler(){clear();}
        void Connect(std::function<void(void)> handle){
            if(CHandle==nullptr){
                CHandle = new Action<void>();
            }
            *CHandle += handle;
        }
        template<typename T> void Connect (T* val, void(T::* slot)()){
            if(CppHandle==nullptr){
                CppHandle = new Signal<void>(nullptr);
            }
            *CppHandle = Connect(val, slot);
        }

        void operator ()(){
            if(CHandle != nullptr){
                CHandle->operator()();
            }
            if(CppHandle != nullptr){
                CppHandle->operator()();
            }
        }

        bool empty() const{
            return (CppHandle==nullptr && CHandle==nullptr);
        }
        void clear() {
            FREE_PTR(CHandle);FREE_PTR(CppHandle);
        }
    protected:
        Signal<void> *CppHandle;
        Action<void> *CHandle;

    private:
        EventHandler(const EventHandler&){}
        EventHandler& operator = (const EventHandler&){ return *this; }
    };
}

#endif // EVENTHANDLER_H
