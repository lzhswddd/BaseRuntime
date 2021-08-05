#ifndef __POINT_H__
#define __POINT_H__
#include "core.h"

namespace lzh
{
	template<class _Tp>
	class TEMPLATE_API Point2_
	{
	public:
		using value_type = typename _Tp;

		explicit Point2_();
		Point2_(const _Tp x, const _Tp y);
		Point2_(const _Tp *m);
		Point2_(MatPtr<_Tp> &&m);
		Point2_(const MatPtr<_Tp> & m);
		template<typename T2_>
		Point2_(const Point2_<T2_> &p);

		Point2_(const Vec<_Tp, 2>& vec);
		Point2_<_Tp>& operator = (const Vec<_Tp, 2>& vec);
		bool operator == (const Point2_<_Tp> &P)const;
		bool operator != (const Point2_<_Tp> &P)const;
		_Tp abs()const;
		template<typename T2_>
		static T2_ cross(const Point2_<T2_> &p1, const Point2_<T2_> &p2);
		template<typename T2_>
		static bool Clockwise(const Point2_<T2_> &p0, const Point2_<T2_> &p1, const Point2_<T2_> &p2);
		static bool xsort(const Point2_<_Tp> &p0, const Point2_<_Tp> &p1);
		static bool ysort(const Point2_<_Tp> &p0, const Point2_<_Tp> &p1);
		_Tp x;
		_Tp y;
	};
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator += (Point2_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator -= (Point2_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator *= (Point2_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator /= (Point2_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator += (Point2_<_Tp> &P, const Point2_<_T2> &v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator -= (Point2_<_Tp> &P, const Point2_<_T2> &v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator *= (Point2_<_Tp> &P, const Point2_<_T2> &v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point2_<_Tp> operator /= (Point2_<_Tp> &P, const Point2_<_T2> &v);

	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator + (const Point2_<_Tp> &P, const _Tp& v);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator + (const _Tp& v, const Point2_<_Tp> &P);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator + (const Point2_<_Tp> &P1, const Point2_<_Tp>& P2);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator - (const Point2_<_Tp> &P, const _Tp& v);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator - (const _Tp& v, const Point2_<_Tp> &P);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator - (const Point2_<_Tp> &P1, const Point2_<_Tp>& P2);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator / (const Point2_<_Tp> &P, const _Tp& v);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator / (const _Tp& v, const Point2_<_Tp> &P);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator / (const Point2_<_Tp> &P1, const Point2_<_Tp>& P2);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator * (const Point2_<_Tp> &P, const _Tp& v);
	template<typename _Tp> TEMPLATE_API Point2_<_Tp> operator * (const _Tp& v, const Point2_<_Tp> &P);
	template<typename _Tp> TEMPLATE_API _Tp operator * (const Point2_<_Tp> &P1, const Point2_<_Tp>& P2);

	template<class _Tp>
	class TEMPLATE_API Point3_
	{
	public:
		using value_type = typename _Tp;

		explicit Point3_();
		Point3_(const _Tp x, const _Tp y, const _Tp z);
		Point3_(const _Tp *m);
		Point3_(MatPtr<_Tp> &&m);
		Point3_(const MatPtr<_Tp> & m);
		template<typename T2_>
		Point3_(const Point3_<T2_> &p);
		Point3_(const Vec<_Tp, 3>& vec);
		Point3_<_Tp>& operator = (const Vec<_Tp, 3>& vec);
		bool operator == (Point3_<_Tp> &P)const;
		bool operator != (Point3_<_Tp> &P)const;
		_Tp x;
		_Tp y;
		_Tp z;
	};
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator += (Point3_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator -= (Point3_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator *= (Point3_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator /= (Point3_<_Tp> &P, const _T2& v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator += (Point3_<_Tp> &P, const Point3_<_T2> &v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator -= (Point3_<_Tp> &P, const Point3_<_T2> &v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator *= (Point3_<_Tp> &P, const Point3_<_T2> &v);
	template<typename _Tp, typename _T2> TEMPLATE_API Point3_<_Tp> operator /= (Point3_<_Tp> &P, const Point3_<_T2> &v);

	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator + (const _Tp& v, const Point3_<_Tp> &P);
	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator + (const Point3_<_Tp> &P, const _Tp& v);
	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator + (const Point3_<_Tp> &P1, const Point3_<_Tp>& P2);
	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator - (const _Tp& v, const Point3_<_Tp> &P);
	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator - (const Point3_<_Tp> &P, const _Tp& v);
	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator - (const Point3_<_Tp> &P1, const Point3_<_Tp>& P2);
	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator * (const _Tp& v, const Point3_<_Tp> &P);
	template<typename _Tp> TEMPLATE_API Point3_<_Tp> operator * (const Point3_<_Tp> &P, const _Tp& v);
	template<typename _Tp> TEMPLATE_API _Tp operator * (const Point3_<_Tp> &P1, const Point3_<_Tp>& P2);


	template<typename _Tp> class DataType<Point2_<_Tp>>
	{
	public:
		typedef Point2_<_Tp> value_type;
		enum { type = MAT_CREATETYPE(2) + DataType<_Tp>::type };
		const int8 * type_name = ("Point2<" + std::string(DataType<_Tp>().type_name) + ">").c_str();
	};
	template<typename _Tp> class DataType<Point3_<_Tp>>
	{
	public:
		typedef Point3_<_Tp> value_type;
		enum { type = MAT_CREATETYPE(3) + DataType<_Tp>::type };
		const int8 * type_name = ("Point3<" + std::string(DataType<_Tp>().type_name) + ">").c_str();
	};

	typedef Point2_<int8>		Point2c;
	typedef Point2_<uint8>		Point2b;
	typedef Point2_<int32>		Point2i;
	typedef Point2_<float32>	Point2f;
	typedef Point2_<float64>	Point2d;
	typedef Point2_<mat_t>		Point2m;
	typedef Point2_<float128>	Point2lf;
	typedef Point2i				Point;
	typedef Point2f				PointF;
	typedef Point2d				PointD;
	typedef Point3_<int8>		Point3c;
	typedef Point3_<uint8>		Point3b;
	typedef Point3_<int32>		Point3i;
	typedef Point3_<float32>	Point3f;
	typedef Point3_<float64>	Point3d;
	typedef Point3_<mat_t>		Point3m;
	typedef Point3_<float128>	Point3lf;
	typedef Point3i				Point3;
	typedef Point3f				Point3F;
	typedef Point3d				Point3D;
}
#include "point_inl.hpp"
#endif // __POINT_H__
