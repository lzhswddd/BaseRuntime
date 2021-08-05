#ifndef __DELEGATE_H__
#define __DELEGATE_H__

#include "core.h"
#include "pointer.h"
#include <functional>

namespace lzh
{
    template<typename _Ptr>
    class TEMPLATE_API DelegateC
    {
    public:
        DelegateC() {}
    };
    template<typename _Ret, typename ...Args>
    class TEMPLATE_API DelegateC<_Ret(Args...)>
    {
    public:
        DelegateC() {}
        DelegateC(const std::function<_Ret(Args...)>& FunPtr) { func = FunPtr; }
        _Ret operator () (const Args &... args) const {
            LZH_ACCESS(if ((*func) != nullptr), THROW_INFO(lzh::ERR_INFO_SUPPORT));
            return (*func)(args...);
        }
        _Ret f(const Args &... args) const {
            return operator()(args...);
        }
        bool empty()const { return *func == nullptr; }
    protected:
        Pointer<std::function<_Ret(Args...)>> func;
    };
    template<typename _Ret>
    class TEMPLATE_API DelegateC<_Ret(void)>
    {
    public:
        DelegateC() {}
        DelegateC(const std::function<_Ret(void)>& FunPtr) { func = FunPtr; }
        _Ret operator () () const {
            LZH_ACCESS(if ((*func) != nullptr), THROW_INFO(lzh::ERR_INFO_SUPPORT));
            return (*func)();
        }
        _Ret f() const {
            return operator()();
        }
        bool empty()const { return *func == nullptr; }
    protected:
        Pointer<std::function<_Ret(void)>> func;
    };
    template<typename _Ptr>
    class TEMPLATE_API Delegate
    {
    public:
        Delegate() {}
    };
    template<typename _Ret, typename ...Args>
    class TEMPLATE_API Delegate<_Ret(Args...)> 
    {
    public:
        Delegate(void* pre = nullptr) { this->pre = pre; }
        Delegate(void* pre, const std::function<_Ret(void*, Args...)>& FunPtr) : func(FunPtr) { this->pre = pre; }
        _Ret operator () (const Args &... args) const {
            LZH_ACCESS(if (!empty()), THROW_INFO(lzh::ERR_INFO_SUPPORT));
            return (*func)(pre, args...);
        }
        void SetPtr(void* ptr) { pre = ptr; }
        bool empty()const { return *func == nullptr; }
    protected:
        void* pre = nullptr;
        Pointer<std::function<_Ret(void*, Args...)>> func;
    };
    template<typename _Ret>
    class TEMPLATE_API Delegate<_Ret(void)>
    {
    public:
        Delegate(void* pre = nullptr) { this->pre = pre; }
        Delegate(void* pre, const std::function<_Ret(void*)>& FunPtr) : func(FunPtr) { this->pre = pre; }
        _Ret operator () () const {
            LZH_ACCESS(if (!empty()), THROW_INFO(lzh::ERR_INFO_SUPPORT));
            return (*func)(pre);
        }
        void SetPtr(void* ptr) { pre = ptr; }
        bool empty()const { return *func == nullptr; }
    protected:
        void* pre = nullptr;
        Pointer<std::function<_Ret(void*)>> func;
    };
    template<typename T, typename Ret, typename ...Args> TEMPLATE_API Delegate<Ret(Args...)> Connect(T* val, Ret(T::* slot)(Args...))
    {
        return Delegate<Ret(Args...)>(val, [=](void* ptr, const Args &... value) { return (((T*)ptr)->*slot)(value...); });
    }
    template<typename T, typename Ret> TEMPLATE_API Delegate<Ret(void)> Connect(T* val, Ret(T::* slot)())
    {
        return Delegate<Ret(void)>(val, [=](void* ptr) { return (((T*)ptr)->*slot)(); });
    }
}

#endif //__DELEGATE_H__