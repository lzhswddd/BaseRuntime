#pragma once

#include "dllapi.h"
#include <string>
#include <map>
#include <tuple>
#include <vector>

namespace lzh
{
	class LZHAPI DllAdmin
	{
	public:
		using Elem = typename FunctionApi*;
		using Container = typename std::map<std::string, Elem>;

		DllAdmin();
		~DllAdmin();

		bool checkImport(const std::string & dll, const std::string & port = "")const;
		bool tryImport(const std::string & dll, const std::string & port = "");
		const FunctionApi* findDll(const std::string & dll)const;
		std::intptr_t findPort(const std::string & port)const;
		std::intptr_t find(const std::string & dll, const std::string & port)const;
		bool appendfun(std::string port, const void* fun);
		bool append(std::string dll, std::string port = "");
		void erase(std::string dll, std::string port = "");

		template<typename R, typename ...Args> R f(std::string dll, std::string name, const Args &... args);
		template<typename R, typename ...Args> R operator () (std::string dll, std::string name, const Args &... args);

		Elem fun(const char* dll, const char* name);
		void lock();
		void unlock();
	private:

		DllAdmin(const DllAdmin& value) { *this = value; }
		DllAdmin& operator = (const DllAdmin& value) { if (this == &value) return *this; else return *this; }
		void clear();
		STLApi<std::string> defaultName = "Default";
		STLApi<Container> function;
		bool islock;
	};
}

template<typename R, typename ...Args> inline R lzh::DllAdmin::f(std::string dll, std::string name, const Args & ...args)
{
	if (name.empty())return R();
	if (dll.empty()) dll = defaultName;
	const Elem ptr = fun(dll.c_str(), name.c_str());
	if (ptr == nullptr)throw std::exception("Dynamic Link Library is null");
	R(*f)(Args...) = ptr->Ptr<R(*)(Args...)>(name);
	if (f == nullptr)throw std::exception("function is null");
	return std::function<R(Args...)>(f)(args...);
}

template<typename R, typename ...Args> inline R lzh::DllAdmin::operator()(std::string dll, std::string name, const Args & ...args)
{
	return f<R>(dll, name, args...);
}
