#pragma once

#include "interface.h"

#include <map>
#include <string>
#include <functional>

namespace lzh
{
	class LZHAPI FunctionApi
	{
	public:
		using keyType = typename std::string;
		explicit FunctionApi();
		explicit FunctionApi(const void *fun, const std::string & port);
        explicit FunctionApi(const std::vector<const void *> & fun, const std::vector<std::string> & port);
        virtual ~FunctionApi();
		template<typename _Tp> _Tp Ptr()const;

		const void* f(keyType key)const;
		void set(keyType key, const void *fun);
		void erase(keyType key);
		virtual void clear();
		virtual bool enable()const;
		virtual bool base()const;

		virtual void append(keyType port);

		template<typename _Tp> _Tp Ptr(keyType key)const;
	protected:
		STLApi<std::map<keyType, const void *>> func;
	};
	template<typename _Tp> inline _Tp FunctionApi::Ptr(keyType key) const {
		return _Tp(f(key));
	}

	class LZHAPI DynamicLibrary : public FunctionApi
	{
	public:
		using keyType = typename FunctionApi::keyType;
		explicit DynamicLibrary(std::string dll_name);
		explicit DynamicLibrary(std::string dll_name, const std::string & dll_port);
		explicit DynamicLibrary(std::string dll_name, const std::vector<std::string> & dll_port);
		~DynamicLibrary();

		void setDllName(std::string dll_name);
		std::string getDllName()const;

		bool tryPause(keyType port)const;
        void append(keyType port) override;

        void clear() override;
		bool base()const override { return false; };
		bool empty()const noexcept;
	private:
		DynamicLibrary(const DynamicLibrary& dynamicLibrary) { *this = dynamicLibrary; }
		DynamicLibrary& operator = (const DynamicLibrary& dynamicLibrary) { if (this == &dynamicLibrary) return *this; else return *this; }

		STLApi<std::string> name;
		void *hDll;
	};

	class LZHAPI DllApiData
	{
	public:
		using keyType = typename FunctionApi::keyType;
		explicit DllApiData(const char* dll_name, const char *dll_port) : api(new DynamicLibrary(dll_name, dll_port)) {}
		explicit DllApiData(const void *fun, const char *name) : api(new FunctionApi(fun, name)) {}
		~DllApiData() { if (api)delete api; api = nullptr; }
		void clear() { if (api)api->clear(); }
		template<typename _Tp> _Tp Ptr(keyType key)const { return api->Ptr<_Tp>(key); }
	protected:
		FunctionApi *api;
	};

    template<typename _Ptr> class TEMPLATE_API DllApi : public DllApiData
	{
	public:
		using keyType = typename FunctionApi::keyType;
		using func_type = typename std::function<_Ptr>;

        explicit DllApi(const char* dll_name, const char *dll_port) : DllApiData(dll_name, dll_port) {}
        explicit DllApi(const void *fun, const char *name) : DllApiData(fun, name) {}

		template<typename ...Args> auto f(keyType key, const Args &... args)const;
		template<typename ...Args> auto operator () (keyType key, const Args &... args)const;

	private:
		DllApi(const DllApi<_Ptr>& value) { *this = value; }
		DllApi& operator = (const DllApi<_Ptr>& value) { if (this == &value) return *this; else return *this; }
	};

    template<typename R, typename ...Args>
    class DllApi<R(*)(Args...)> : public DllApiData
	{
	public:
		using keyType = typename FunctionApi::keyType;
		using func_type = typename std::function<R(Args...)>;
        using result_type = R;

		explicit DllApi(const char* dll_name, const char *dll_port) : DllApiData(dll_name, dll_port) {}
        explicit DllApi(const void *fun, const char *name) : DllApiData(fun, name) {}

		//template<typename ...Args2> result_type f(const Args2 &... args)const;
		R f(keyType key, const Args &... args)const;
		R operator () (keyType key, const Args &... args)const;

	private:
		DllApi(const DllApi<R(*)(Args...)>& value) { *this = value; }
		DllApi& operator = (const DllApi<R(*)(Args...)>& value) { if (this == &value) return *this; else return *this; }
	};
    template<typename R, typename ...Args> class DllApi<R(Args...)> : public DllApiData
	{
	public:
		using keyType = typename FunctionApi::keyType;
		using func_type = typename std::function<R(Args...)>;
        using result_type = R;

		explicit DllApi(const char* dll_name, const char *dll_port) : DllApiData(dll_name, dll_port) {}
        explicit DllApi(const void *fun, const char *name) : DllApiData(fun, name) {}

		template<typename ...Args2> R f(keyType key, const Args2 &... args)const;
		//result_type f(const Args &... args)const;
		R operator () (keyType key, const Args &... args)const;

	private:
		DllApi(const DllApi<R(Args...)>& value) { *this = value; }
		DllApi& operator = (const DllApi<R(Args...)>& value) { if (this == &value) return *this; else return *this; }
	};
}
template<typename _Ptr> template<typename ...Args> inline auto lzh::DllApi<_Ptr>::operator()(keyType key, const Args &... args) const
{
    return f(key, args...);
}
template<typename R, typename ...Args> inline R lzh::DllApi<R(*)(Args...)>::operator()(keyType key, const Args & ...args) const
{
    return f(key, args...);
}
template<typename R, typename ...Args> inline R lzh::DllApi<R(Args...)>::operator()(keyType key, const Args & ...args) const
{
    return f(key, args...);
}

template<typename _Ptr> template<typename ...Args> inline auto lzh::DllApi<_Ptr>::f(keyType key, const Args &... args) const
{
	_Ptr f = Ptr<_Ptr>(key);
	if (!f)return NULL;
	return std::function<_Ptr>(f)(args...);
}
template<typename R, typename ...Args> inline R lzh::DllApi<R(*)(Args...)>::f(keyType key, const Args & ...args) const
{
	R(*f)(Args...) = Ptr<R(*)(Args...)>(key);
	if (!f)return result_type();
	return std::function<R(Args...)>(f)(args...);
}
//template<typename R, typename ...Args> inline typename DllApi<R(Args...)>::result_type DllApi<R(Args...)>::f(const Args & ...args) const
//{
//	R(*f)(Args...) = Ptr<R(*)(Args...)>();
//	if (!f)return result_type();
//	return std::function<R(Args...)>(f)(args...);
//}

template<typename R, typename ...Args> template<typename ...Args2> inline R lzh::DllApi<R(Args...)>::f(keyType key, const Args2 & ...args) const
{
	R(*f)(Args2...) = Ptr<R(*)(Args2...)>(key);
	if (!f)return result_type();
	return std::function<R(Args...)>(f)(args...);
}
