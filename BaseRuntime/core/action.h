#ifndef __ACTION_H__
#define __ACTION_H__

#include "core.h"
#include "interface.h"
#include <functional>

namespace lzh
{
	template<typename ...Args> 
	class TEMPLATE_API Action
	{
	public:
		Action(){}
		Action(const std::function<void(Args...)> & FunPtr) { member.push_back(FunPtr); }
		Action& operator = (std::function<void(Args...)> FunPtr) {
			member.clear();
			member.push_back(FunPtr);
			return *this;
		}
		Action& operator -= (std::function<void(Args...)> FunPtr) {
			typename std::vector<std::function<void(Args...)>>::iterator f;
			for (f = member.begin(); f != member.end();) {
				if (*f != nullptr) {
					if (strcmp((*f).target_type().name(), FunPtr.target_type().name()) == 0)
						f = member.erase(f);
					else
						f++;
				}
			}
			return *this;
		}
		Action& operator += (std::function<void(Args...)> FunPtr) {
			typename std::vector<std::function<void(Args...)>>::iterator f;
			for (f = member.begin(); f != member.end(); f++) {
				if (*f != nullptr) {
					if (strcmp((*f).target_type().name(), FunPtr.target_type().name()) == 0) {
						break;
					}
				}
			}
			if (f == member.end()) {
				member.push_back(FunPtr);
			}
			return *this;
		}

		void operator () (const Args &... args) const  {
			for (const std::function<void(Args...)>& f : member) {
				if (f != nullptr)f(args...);
			}
		}
	protected:
		STLApi<std::vector<std::function<void(Args...)>>> member;
	};

	template<> class TEMPLATE_API Action<void>
	{
	public:
		Action() {}
		Action(const std::function<void(void)> & FunPtr) { member.push_back(FunPtr); }
		Action& operator = (std::function<void(void)> FunPtr) {
			member.clear();
			member.push_back(FunPtr);
			return *this;
		}
		Action& operator -= (std::function<void(void)> FunPtr) {
			typename std::vector<std::function<void(void)>>::iterator f;
			for (f = member.begin(); f != member.end();) {
				if (*f != nullptr) {
					if (strcmp((*f).target_type().name(), FunPtr.target_type().name()) == 0)
						f = member.erase(f);
					else
						f++;
				}
			}
			return *this;
		}
		Action& operator += (std::function<void(void)> FunPtr) {
			typename std::vector<std::function<void(void)>>::iterator f;
			for (f = member.begin(); f != member.end(); f++) {
				if (*f != nullptr) {
					if (strcmp((*f).target_type().name(), FunPtr.target_type().name()) == 0) {
						break;
					}
				}
			}
			if (f == member.end()) {
				member.push_back(FunPtr);
			}
			return *this;
		}

		void operator () () const {
			for (const std::function<void()>& f : member) {
				if (f != nullptr)f();
			}
		}
	protected:
		STLApi<std::vector<std::function<void(void)>>> member;
	};
}

#endif //__ACTION_H__