#pragma once

#include "core.h"
#include <functional>
namespace lzh {
	//template<typename T>
	//struct TEMPLATE_API parameter_traits;
	// R为返回类型
	// ...Args 为输入参数类型，个数不限
	template<typename ...Args>
	struct TEMPLATE_API parameter_traits
	{
		static const size_t nargs = sizeof...(Args);
		// 输入参数类型,i为从0开始的参数类型索引
		template <size_t i> struct arg {
			using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
		};
	};


	template<typename T>
	struct TEMPLATE_API function_traits;
	// R为返回类型
	// ...Args 为输入参数类型，个数不限
	template<typename R, typename ...Args>
	struct TEMPLATE_API function_traits<std::function<R(Args...)>>
	{
		static const size_t nargs = sizeof...(Args);
		// 返回类型
		using result_type = typename R;

		// 输入参数类型,i为从0开始的参数类型索引
		template <size_t i> struct arg {
			using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
		};
	};
	template<typename R, typename ...Args>
	struct TEMPLATE_API function_traits<std::function<R(*)(Args...)>>
	{
		static const size_t nargs = sizeof...(Args);
		// 返回类型
		using result_type = typename R;

		// 输入参数类型,i为从0开始的参数类型索引
		template <size_t i> struct arg {
			using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
		};
	};

	template<typename Tuple, size_t N>
	struct tuple_print
	{
		static void print(const Tuple& t, std::ostream& os)
		{
			tuple_print<Tuple, N - 1>::print(t, os);
			os << ", " << std::get<N - 1>(t);
		}
	};
	// 类模板的特化版本
	template<typename Tuple>
	struct tuple_print<Tuple, 1>
	{
		static void print(const Tuple& t, std::ostream& os)
		{
			os << "(" << std::get<0>(t);
		}
	};

	template<typename Tuple, size_t N, typename _Ty, typename ...Params>
	struct tuple_assign
	{
		static void assign(Tuple& t, _Ty v)
		{
			tuple_assign<Tuple, N - 1, _Ty, Params...>::assign(t, --v);
			std::get<N - 1>(t) = (parameter_traits<Params...>::arg<N - 1>::type)(*v);
		}
	};
	// 类模板的特化版本
	template<typename Tuple, typename _Ty, typename ...Params>
	struct tuple_assign<Tuple, 1, _Ty, Params...>
	{
		static void assign(Tuple& t, _Ty v)
		{
			std::get<0>(t) = (parameter_traits<Params...>::arg<0>::type)(*(--v));
		}
	};

}