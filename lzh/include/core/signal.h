#pragma once

#include "action.h"

namespace lzh {
	template<typename ...Args>
	class TEMPLATE_API Signal
		: public Action<void*, Args...>
	{
	public:
		Signal(void* pre = nullptr) : Action<void*, Args...>() { this->pre = pre; }
		void operator () (const Args &... args) {
			for (std::function<void(void*, Args...)>& f : this->member) {
				if (f != nullptr)f(pre, args...);
			}
		}
		void SetPtr(void* ptr) { pre = ptr; }
	protected:
		void* pre = nullptr;
	};

	template<>
	class TEMPLATE_API Signal<void>
		:public Action<void*>
	{
	public:
		Signal(void* pre) : Action<void*>() { this->pre = pre; }
		void operator () () {
			for (std::function<void(void*)>& f : this->member) {
				if (f != nullptr)f(pre);
			}
		}
		void SetPtr(void* ptr) { pre = ptr; }
	protected:
		void* pre = nullptr;
	};

	template<typename T, typename ...Args> TEMPLATE_API Signal<Args...> Connect(T* val, void(T::* slot)(Args...))
	{
		Signal<Args...> signal(val);
		signal += [=](void* ptr, const Args &... value) { (((T*)ptr)->*slot)(value...); };
		return signal;
	}

	template<typename T> TEMPLATE_API Signal<void> Connect(T* val, void(T::* slot)())
	{
		Signal<void> signal(val);
		signal += [=](void* ptr) { (((T*)ptr)->*slot)(); };
		return signal;
	}
}
