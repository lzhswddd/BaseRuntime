#pragma once

#include "core.h"
#include <functional>
namespace lzh {
	//template<typename T>
	//struct TEMPLATE_API parameter_traits;
	// RΪ��������
	// ...Args Ϊ����������ͣ���������
	template<typename ...Args>
	struct TEMPLATE_API parameter_traits
	{
		static const size_t nargs = sizeof...(Args);
		// �����������,iΪ��0��ʼ�Ĳ�����������
		template <size_t i> struct arg {
			using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
		};
	};


	template<typename T>
	struct TEMPLATE_API function_traits;
	// RΪ��������
	// ...Args Ϊ����������ͣ���������
	template<typename R, typename ...Args>
	struct TEMPLATE_API function_traits<std::function<R(Args...)>>
	{
		static const size_t nargs = sizeof...(Args);
		// ��������
		using result_type = typename R;

		// �����������,iΪ��0��ʼ�Ĳ�����������
		template <size_t i> struct arg {
			using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
		};
	};
	template<typename R, typename ...Args>
	struct TEMPLATE_API function_traits<std::function<R(*)(Args...)>>
	{
		static const size_t nargs = sizeof...(Args);
		// ��������
		using result_type = typename R;

		// �����������,iΪ��0��ʼ�Ĳ�����������
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
	// ��ģ����ػ��汾
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
	// ��ģ����ػ��汾
	template<typename Tuple, typename _Ty, typename ...Params>
	struct tuple_assign<Tuple, 1, _Ty, Params...>
	{
		static void assign(Tuple& t, _Ty v)
		{
			std::get<0>(t) = (parameter_traits<Params...>::arg<0>::type)(*(--v));
		}
	};

}