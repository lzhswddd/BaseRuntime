#pragma once
#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "interface.h"
#include "pointer.h"
#include "parse_templates_inl.h"
#include <functional>
#include <map>

namespace lzh
{
	class Element;
	class LZHAPI ElemIter
		: public std::iterator<std::random_access_iterator_tag, Element>
	{
	public:
		ElemIter(const Element & container, bool b_e);
		ElemIter(const ElemIter & p);
		~ElemIter();
		ElemIter& operator = (const ElemIter & p);
		size_t operator - (const ElemIter &iter)const;
		bool operator != (const ElemIter &iter)const;
		bool operator == (const ElemIter &iter)const;
		bool operator > (const ElemIter &iter);
		bool operator < (const ElemIter &iter);
		ElemIter operator ++ ();
		ElemIter operator ++ (int32);
		ElemIter operator + (int32 count);
		ElemIter operator += (int32 count);
		ElemIter operator -- ();
		ElemIter operator -- (int32);
		ElemIter operator - (int32 count);
		ElemIter operator -= (int32 count);
		Element& operator * ();
		const Element& key()const;
		Element& value();

		template<typename _Ty> _Ty& as() { return *static_cast<_Ty*>(_iter); }
		template<typename _Ty> const _Ty& as()const { return *static_cast<const _Ty*>(_iter); }
	private:
		int32 type;
		void* _iter;
	};
	class LZHAPI Element
	{
	public:
		enum /*Type*/
		{
			InValid,
			BOOL,
			INT8,
			UINT8,
			INT16,
			UINT16,
			INT32,
			UINT32,
			INT64,
			UINT64,
			FLOAT32,
			FLOAT64,
			FLOAT128,

			INT8_PTR,
			UINT8_PTR,
			INT16_PTR,
			UINT16_PTR,
			INT32_PTR,
			UINT32_PTR,
			INT64_PTR,
			UINT64_PTR,
			FLOAT32_PTR,
			FLOAT64_PTR,
			FLOAT128_PTR,

			RANGE,
			RECT,
			RECTF,
			SIZE,
			SIZEF,
			SIZE3,
			SIZE3F,

			POINT,
			POINTF,
			POINT3,
			POINT3F,

			VEC2B,
			VEC2I,
			VEC2F,
			VEC2D,

			VEC3B,
			VEC3I,
			VEC3F,
			VEC3D,

			VEC4B,
			VEC4I,
			VEC4F,
			VEC4D,

			COLOR,

			COMPLEX,
			LARGENUMBER,
			MATPTR_UINT8,
			MATPTR_INT32,
			MATPTR_FLOAT32,
			MATPTR_FLOAT64,
			VECTOR,

			STRING,
			MAT,
			ARRAY,

			STD_VECTOR,
			STD_MAP,
			STD_TUPLE,
			INTPTR,
			FUNCTION,
			STD_FUNCTION
		};
		template<typename _Tp> struct ElemType { using value_type = typename _Tp; enum { type = InValid }; };
		template<> struct ElemType<bool> { using value_type = typename bool; enum { type = BOOL }; };
		template<> struct ElemType<int8> { using value_type = typename int8; enum { type = INT8 }; };
		template<> struct ElemType<uint8> { using value_type = typename uint8; enum { type = UINT8 }; };
		template<> struct ElemType<int16> { using value_type = typename int16; enum { type = INT16 }; };
		template<> struct ElemType<uint16> { using value_type = typename uint16; enum { type = UINT16 }; };
		template<> struct ElemType<int32> { using value_type = typename int32; enum { type = INT32 }; };
		template<> struct ElemType<uint32> { using value_type = typename uint32; enum { type = UINT32 }; };
		template<> struct ElemType<int64> { using value_type = typename int64; enum { type = INT64 }; };
		template<> struct ElemType<uint64> { using value_type = typename uint64; enum { type = UINT64 }; };
		template<> struct ElemType<float32> { using value_type = typename float32; enum { type = FLOAT32 }; };
		template<> struct ElemType<float64> { using value_type = typename float64; enum { type = FLOAT64 }; };
		template<> struct ElemType<float128> { using value_type = typename float128; enum { type = FLOAT128 }; };

		template<> struct ElemType<int8*> { using value_type = typename int8*; enum { type = INT8_PTR }; };
		template<> struct ElemType<uint8*> { using value_type = typename uint8*; enum { type = UINT8_PTR }; };
		template<> struct ElemType<int16*> { using value_type = typename int16*; enum { type = INT16_PTR }; };
		template<> struct ElemType<uint16*> { using value_type = typename uint16*; enum { type = UINT16_PTR }; };
		template<> struct ElemType<int32*> { using value_type = typename int32*; enum { type = INT32_PTR }; };
		template<> struct ElemType<uint32*> { using value_type = typename uint32*; enum { type = UINT32_PTR }; };
		template<> struct ElemType<int64*> { using value_type = typename int64*; enum { type = INT64_PTR }; };
		template<> struct ElemType<uint64*> { using value_type = typename uint64*; enum { type = UINT64_PTR }; };
		template<> struct ElemType<float32*> { using value_type = typename float32*; enum { type = FLOAT32_PTR }; };
		template<> struct ElemType<float64*> { using value_type = typename float64*; enum { type = FLOAT64_PTR }; };
		template<> struct ElemType<float128*> { using value_type = typename float128*; enum { type = FLOAT128_PTR }; };

		template<> struct ElemType<Range> { using value_type = typename Range; enum { type = RANGE }; };
		template<> struct ElemType<Rect> { using value_type = typename Rect; enum { type = RECT }; };
		template<> struct ElemType<RectF> { using value_type = typename RectF; enum { type = RECTF }; };
		template<> struct ElemType<Size> { using value_type = typename Size; enum { type = SIZE }; };
		template<> struct ElemType<Size2f> { using value_type = typename Size2f; enum { type = SIZEF }; };
		template<> struct ElemType<Size3> { using value_type = typename Size3; enum { type = SIZE3 }; };
		template<> struct ElemType<Size3f> { using value_type = typename Size3f; enum { type = SIZE3F }; };

		template<> struct ElemType<Point> { using value_type = typename Point; enum { type = POINT }; };
		template<> struct ElemType<Point2f> { using value_type = typename Point2f; enum { type = POINTF }; };
		template<> struct ElemType<Point3> { using value_type = typename Point3; enum { type = POINT3 }; };
		template<> struct ElemType<Point3f> { using value_type = typename Point3f; enum { type = POINT3F }; };

		template<> struct ElemType<Vec2b> { using value_type = typename Vec2b; enum { type = VEC2B }; };
		template<> struct ElemType<Vec2i> { using value_type = typename Vec2i; enum { type = VEC2I }; };
		template<> struct ElemType<Vec2f> { using value_type = typename Vec2f; enum { type = VEC2F }; };
		template<> struct ElemType<Vec2d> { using value_type = typename Vec2d; enum { type = VEC2D }; };

		template<> struct ElemType<Vec3b> { using value_type = typename Vec3b; enum { type = VEC3B }; };
		template<> struct ElemType<Vec3i> { using value_type = typename Vec3i; enum { type = VEC3I }; };
		template<> struct ElemType<Vec3f> { using value_type = typename Vec3f; enum { type = VEC3F }; };
		template<> struct ElemType<Vec3d> { using value_type = typename Vec3d; enum { type = VEC3D }; };

		template<> struct ElemType<Vec4b> { using value_type = typename Vec4b; enum { type = VEC4B }; };
		template<> struct ElemType<Vec4i> { using value_type = typename Vec4i; enum { type = VEC4I }; };
		template<> struct ElemType<Vec4f> { using value_type = typename Vec4f; enum { type = VEC4F }; };
		template<> struct ElemType<Vec4d> { using value_type = typename Vec4d; enum { type = VEC4D }; };

		template<> struct ElemType<Color> { using value_type = typename Color; enum { type = COLOR }; };
		template<> struct ElemType<Complex> { using value_type = typename Complex; enum { type = COMPLEX }; };
		template<> struct ElemType<LargeNumber> { using value_type = typename LargeNumber; enum { type = LARGENUMBER }; };
		template<> struct ElemType<MatPtr<uint8>> { using value_type = typename MatPtr<uint8>; enum { type = MATPTR_UINT8 }; };
		template<> struct ElemType<MatPtr<int32>> { using value_type = typename MatPtr<int32>; enum { type = MATPTR_INT32 }; };
		template<> struct ElemType<MatPtr<float32>> { using value_type = typename MatPtr<float32>; enum { type = MATPTR_FLOAT32 }; };
		template<> struct ElemType<MatPtr<float64>> { using value_type = typename MatPtr<float64>; enum { type = MATPTR_FLOAT64 }; };
		template<> struct ElemType<Vector> { using value_type = typename Vector; enum { type = VECTOR }; };

		template<> struct ElemType<std::string> { using value_type = typename std::string; enum { type = STRING }; };
		template<> struct ElemType<Mat> { using value_type = typename Mat; enum { type = MAT }; };
		template<> struct ElemType<Array> { using value_type = typename Array; enum { type = ARRAY }; };
		template<> struct ElemType<std::vector<Element>> { using value_type = typename std::vector<Element>; enum { type = STD_VECTOR }; };
		template<> struct ElemType<std::map<Element, Element>> { using value_type = typename std::map<Element, Element>; enum { type = STD_MAP }; };
		template<> struct ElemType<void*> { using value_type = typename intptr_t; enum { type = INTPTR }; };
		template<typename R, typename ...Params> struct ElemType<R(*)(Params...)> { using value_type = typename R(*)(Params...); enum { type = FUNCTION }; };
		template<typename R, typename ...Params> struct ElemType<std::function<R(Params...)>> { using value_type = typename R(*)(Params...); enum { type = STD_FUNCTION }; };


		template<> struct ElemType<const bool> { using value_type = typename bool; enum { type = BOOL }; };
		template<> struct ElemType<const int8> { using value_type = typename int8; enum { type = INT8 }; };
		template<> struct ElemType<const uint8> { using value_type = typename uint8; enum { type = UINT8 }; };
		template<> struct ElemType<const int16> { using value_type = typename int16; enum { type = INT16 }; };
		template<> struct ElemType<const uint16> { using value_type = typename uint16; enum { type = UINT16 }; };
		template<> struct ElemType<const int32> { using value_type = typename int32; enum { type = INT32 }; };
		template<> struct ElemType<const uint32> { using value_type = typename uint32; enum { type = UINT32 }; };
		template<> struct ElemType<const int64> { using value_type = typename int64; enum { type = INT64 }; };
		template<> struct ElemType<const uint64> { using value_type = typename uint64; enum { type = UINT64 }; };
		template<> struct ElemType<const float32> { using value_type = typename float32; enum { type = FLOAT32 }; };
		template<> struct ElemType<const float64> { using value_type = typename float64; enum { type = FLOAT64 }; };
		template<> struct ElemType<const float128> { using value_type = typename float128; enum { type = FLOAT128 }; };

		template<> struct ElemType<const int8*> { using value_type = typename int8*; enum { type = INT8_PTR }; };
		template<> struct ElemType<const uint8*> { using value_type = typename uint8*; enum { type = UINT8_PTR }; };
		template<> struct ElemType<const int16*> { using value_type = typename int16*; enum { type = INT16_PTR }; };
		template<> struct ElemType<const uint16*> { using value_type = typename uint16*; enum { type = UINT16_PTR }; };
		template<> struct ElemType<const int32*> { using value_type = typename int32*; enum { type = INT32_PTR }; };
		template<> struct ElemType<const uint32*> { using value_type = typename uint32*; enum { type = UINT32_PTR }; };
		template<> struct ElemType<const int64*> { using value_type = typename int64*; enum { type = INT64_PTR }; };
		template<> struct ElemType<const uint64*> { using value_type = typename uint64*; enum { type = UINT64_PTR }; };
		template<> struct ElemType<const float32*> { using value_type = typename float32*; enum { type = FLOAT32_PTR }; };
		template<> struct ElemType<const float64*> { using value_type = typename float64*; enum { type = FLOAT64_PTR }; };
		template<> struct ElemType<const float128*> { using value_type = typename float128*; enum { type = FLOAT128_PTR }; };

		template<> struct ElemType<const Range> { using value_type = typename Range; enum { type = RANGE }; };
		template<> struct ElemType<const Rect> { using value_type = typename Rect; enum { type = RECT }; };
		template<> struct ElemType<const RectF> { using value_type = typename RectF; enum { type = RECTF }; };
		template<> struct ElemType<const Size> { using value_type = typename Size; enum { type = SIZE }; };
		template<> struct ElemType<const Size2f> { using value_type = typename Size2f; enum { type = SIZEF }; };
		template<> struct ElemType<const Size3> { using value_type = typename Size3; enum { type = SIZE3 }; };
		template<> struct ElemType<const Size3f> { using value_type = typename Size3f; enum { type = SIZE3F }; };

		template<> struct ElemType<const Point> { using value_type = typename Point; enum { type = POINT }; };
		template<> struct ElemType<const Point2f> { using value_type = typename Point2f; enum { type = POINTF }; };
		template<> struct ElemType<const Point3> { using value_type = typename Point3; enum { type = POINT3 }; };
		template<> struct ElemType<const Point3f> { using value_type = typename Point3f; enum { type = POINT3F }; };

		template<> struct ElemType<const Vec2b> { using value_type = typename Vec2b; enum { type = VEC2B }; };
		template<> struct ElemType<const Vec2i> { using value_type = typename Vec2i; enum { type = VEC2I }; };
		template<> struct ElemType<const Vec2f> { using value_type = typename Vec2f; enum { type = VEC2F }; };
		template<> struct ElemType<const Vec2d> { using value_type = typename Vec2d; enum { type = VEC2D }; };

		template<> struct ElemType<const Vec3b> { using value_type = typename Vec3b; enum { type = VEC3B }; };
		template<> struct ElemType<const Vec3i> { using value_type = typename Vec3i; enum { type = VEC3I }; };
		template<> struct ElemType<const Vec3f> { using value_type = typename Vec3f; enum { type = VEC3F }; };
		template<> struct ElemType<const Vec3d> { using value_type = typename Vec3d; enum { type = VEC3D }; };

		template<> struct ElemType<const Vec4b> { using value_type = typename Vec4b; enum { type = VEC4B }; };
		template<> struct ElemType<const Vec4i> { using value_type = typename Vec4i; enum { type = VEC4I }; };
		template<> struct ElemType<const Vec4f> { using value_type = typename Vec4f; enum { type = VEC4F }; };
		template<> struct ElemType<const Vec4d> { using value_type = typename Vec4d; enum { type = VEC4D }; };

		template<> struct ElemType<const Color> { using value_type = typename Color; enum { type = COLOR }; };
		template<> struct ElemType<const Complex> { using value_type = typename Complex; enum { type = COMPLEX }; };
		template<> struct ElemType<const LargeNumber> { using value_type = typename LargeNumber; enum { type = LARGENUMBER }; };
		template<> struct ElemType<const MatPtr<uint8>> { using value_type = typename MatPtr<uint8>; enum { type = MATPTR_UINT8 }; };
		template<> struct ElemType<const MatPtr<int32>> { using value_type = typename MatPtr<int32>; enum { type = MATPTR_INT32 }; };
		template<> struct ElemType<const MatPtr<float32>> { using value_type = typename MatPtr<float32>; enum { type = MATPTR_FLOAT32 }; };
		template<> struct ElemType<const MatPtr<float64>> { using value_type = typename MatPtr<float64>; enum { type = MATPTR_FLOAT64 }; };
		template<> struct ElemType<const Vector> { using value_type = typename Vector; enum { type = VECTOR }; };

		template<> struct ElemType<const std::string> { using value_type = typename std::string; enum { type = STRING }; };
		template<> struct ElemType<const Mat> { using value_type = typename Mat; enum { type = MAT }; };
		template<> struct ElemType<const Array> { using value_type = typename Array; enum { type = ARRAY }; };
		template<> struct ElemType<const std::vector<Element>> { using value_type = typename std::vector<Element>; enum { type = STD_VECTOR }; };
		template<> struct ElemType<const std::map<Element, Element>> { using value_type = typename std::map<Element, Element>; enum { type = STD_MAP }; };
		template<> struct ElemType<const void*> { using value_type = typename intptr_t; enum { type = INTPTR }; };
		template<typename R, typename ...Params> struct ElemType<const R(*)(Params...)> { using value_type = typename R(*)(Params...); enum { type = FUNCTION }; };
		template<typename R, typename ...Params> struct ElemType<const std::function<R(Params...)>> { using value_type = typename R(*)(Params...); enum { type = STD_FUNCTION }; };

		using iterator = typename ElemIter;
		using const_iterator = typename const ElemIter;

		Element();
		Element(std::nullptr_t v);

		Element(const bool & v);
		Element(const int8 & v);
		Element(const int16 & v);
		Element(const int32 & v);
		Element(const int64 & v);
		Element(const uint8 & v);
		Element(const uint16 & v);
		Element(const uint32 & v);
		Element(const uint64 & v);
		Element(const float32 & v);
		Element(const float64 & v);
		Element(const float128 & v);

		Element(int8 * v, bool isStr = true);
		Element(const int8 * v, bool isStr = true);
		Element(int16 * v);
		Element(const int16 * v);
		Element(int32 * v);
		Element(const int32 * v);
		Element(int64 * v);
		Element(const int64 * v);
		Element(uint8 * v);
		Element(const uint8 * v);
		Element(uint16 * v);
		Element(const uint16 * v);
		Element(uint32 * v);
		Element(const uint32 * v);
		Element(uint64 * v);
		Element(const uint64 * v);
		Element(float32 * v);
		Element(const float32 * v);
		Element(float64 * v);
		Element(const float64 * v);
		Element(float128 * v);
		Element(const float128 * v);

		Element(const Range & v);
		Element(const Rect & v);
		Element(const RectF & v);
		Element(const Size & v);
		Element(const Size2f & v);
		Element(const Size3 & v);
		Element(const Size3f & v);

		Element(const Point & v);
		Element(const Point2f & v);
		Element(const Point3 & v);
		Element(const Point3f & v);

		Element(const Vec2b & v);
		Element(const Vec2i & v);
		Element(const Vec2f & v);
		Element(const Vec2d & v);

		Element(const Vec3b & v);
		Element(const Vec3i & v);
		Element(const Vec3f & v);
		Element(const Vec3d & v);

		Element(const Vec4b & v);
		Element(const Vec4i & v);
		Element(const Vec4f & v);
		Element(const Vec4d & v);

		Element(const Color & v);
		Element(const Complex & v);
		Element(const LargeNumber & v);
		Element(const MatPtr<uint8> & v);
		Element(const MatPtr<int32> & v);
		Element(const MatPtr<float32> & v);
		Element(const MatPtr<float64> & v);
		Element(const Vector & v);

		Element(const std::string & v);
		Element(const Mat & v);
		Element(const Array & v);
		Element(const std::vector<Element> & v);
		//Element(const std::initializer_list<Element> & list);
		Element(const std::map<Element, Element> & dict);
		Element(InputArray inputArray);

		Element(const Element &element);
		Element& operator = (const Element &element);
		template<typename _Tp>Element& operator= (const _Tp& element);

		Element(void * v);
		Element(const void * v);
		template<typename _Tp> Element(const _Tp * v);
		template<typename _Tp> explicit Element(const _Tp & v);
		template<typename _Tp> Element(const std::vector<_Tp> & v);
		template<typename R, typename ...Params> Element(const std::function<R(Params...)> & v);
		template<typename ...Params> Element(const std::tuple<Params...> & v);

		~Element();

		intptr_t toIntPtr()const;

		bool& tobool();
		int8& toChar();
		int8& toi8();
		int16& toi16();
		int32 toInt()const;
		int32& toi32();
		int64& toi64();
		uint8& tou8();
		uint16& tou16();
		uint32& tou32();
		uint64& tou64();
		float32 toFloat()const;
		float32& tof32();
		float64 toDouble()const;
		float64& tof64();
		float128& tof128();
		const bool& tobool()const;
		const int8& toi8()const;
		const int16& toi16()const;
		const int32& toi32()const;
		const int64& toi64()const;
		const uint8& tou8()const;
		const uint16& tou16()const;
		const uint32& tou32()const;
		const uint64& tou64()const;
		const float32& tof32()const;
		const float64& tof64()const;
		const float128& tof128()const;

		template<typename _Tp> void toBaseType(_Tp & v)const;
		template<typename _Tp> _Tp toBaseType()const;
		mat_t toNumber()const;

		const int8 *toCString()const;
		std::string toString()const;
		int8 *toI8Ptr()const;
		int16 *toI16Ptr()const;
		int32 *toI32Ptr()const;
		int64 *toI64Ptr()const;
		uint8 *toU8Ptr()const;
		uint16 *toU16Ptr()const;
		uint32 *toU32Ptr()const;
		uint64 *toU64Ptr()const;
		float32 *toF32Ptr()const;
		float64 *toF64Ptr()const;
		float128 *toF128Ptr()const;

		Value toValue()const;

		Range& toRange()const;
		Rect& toRect()const;
		RectF& toRectF()const;
		Size& toSize()const;
		Size2f& toSizeF()const;
		Size3& toSize3()const;
		Size3f& toSize3F()const;

		Point& toPoint()const;
		Point2f& toPointF()const;
		template<typename _Tp> Point2_<_Tp> toPoint_()const;
		Point3& toPoint3()const;
		Point3f& toPoint3F()const;
		template<typename _Tp> Point3_<_Tp> toPoint3_()const;


		Vec2b& toVec2b()const;
		Vec2i& toVec2i()const;
		Vec2f& toVec2f()const;
		Vec2d& toVec2d()const;

		Vec3b& toVec3b()const;
		Vec3i& toVec3i()const;
		Vec3f& toVec3f()const;
		Vec3d& toVec3d()const;

		Vec4b& toVec4b()const;
		Vec4i& toVec4i()const;
		Vec4f& toVec4f()const;
		Vec4d& toVec4d()const;

		Color& toColor()const;
		Color toRGB()const;

		Complex& toComplex()const;
		LargeNumber& toLargeNumber()const;
		MatPtr<uint8>& toU8Array()const;
		MatPtr<int32>& toI32Array()const;
		MatPtr<float32>& toF32Array()const;
		MatPtr<float64>& toF64Array()const;

		Vector& toVector()const;
		Mat& toMat()const;
		Array& toArray()const;
		Array& toImage()const;

		_OutputArray toInterFace()const;
		std::vector<Element>& toStdVector()const;
		std::vector<Element>& toList()const;
		std::map<Element, Element>& toStdMap()const;
		std::map<Element, Element>& toDict()const;

		std::string& toStdString()const;

		template<typename _Ptr> _Ptr* toFunc();
		template<typename _Ptr> const _Ptr* toFunc()const;
		template<typename R, typename ...Params> std::function<R(Params...)> toStdFunc()const;
		template<typename R, typename ...Params> R f(const Params &... params)const;

		template<typename _Tp> _Tp& toUserData()const;
		template<typename _Tp> void toPtr(_Tp** v)const;
		template<typename _Tp> _Tp* toPtr();
		template<typename _Tp> const _Tp* toPtr()const;
		template<typename _Tp> void toData(_Tp& v)const;
		template<typename _Tp> _Tp& toData()const;

		void* data();
		const void* data()const;
		const void* constdata()const;

		bool isUserData()const;
		bool isIntPtr()const;
		bool isNumber()const;
		bool isPointer()const;
		bool isVec()const;

		bool isIndex()const;
		bool isInt()const;
		bool isFloat()const;
		bool isDouble()const;

		bool isbool()const;
		bool isi8()const;
		bool isi16()const;
		bool isi32()const;
		bool isi64()const;
		bool isu8()const;
		bool isu16()const;
		bool isu32()const;
		bool isu64()const;
		bool isf32()const;
		bool isf64()const;
		bool isf128()const;

		bool isPtri8()const;
		bool isPtri16()const;
		bool isPtri32()const;
		bool isPtri64()const;
		bool isPtru8()const;
		bool isPtru16()const;
		bool isPtru32()const;
		bool isPtru64()const;
		bool isPtrf32()const;
		bool isPtrf64()const;
		bool isPtrf128()const;

		bool isRange()const;
		bool isRect()const;
		bool isRectF()const;
		bool isSize()const;
		bool isSizeF()const;
		bool isSize3()const;
		bool isSize3F()const;

		bool isPoint()const;
		bool isPointF()const;
		bool isPoint2_()const;
		bool isPoint3()const;
		bool isPoint3F()const;
		bool isPoint3_()const;

		bool isVec2b()const;
		bool isVec2i()const;
		bool isVec2f()const;
		bool isVec2d()const;

		bool isVec3b()const;
		bool isVec3i()const;
		bool isVec3f()const;
		bool isVec3d()const;

		bool isVec4b()const;
		bool isVec4i()const;
		bool isVec4f()const;
		bool isVec4d()const;

		bool isColor()const;

		bool isComplex()const;
		bool isLargeNumber()const;
		bool isU8Ptr()const;
		bool isI32Ptr()const;
		bool isF32Ptr()const;
		bool isF64Ptr()const;
		bool isVector()const;

		bool isMat()const;
		bool isArray()const;
		bool isImage()const;

		bool isString()const;
		bool isStrString()const;

		bool isList()const;
		bool isStdVector()const;

		bool isDict()const;
		bool isStdMap()const;

		bool isTuple()const;

		void settype(const char *str);
		Element astype(const char *str)const;

		void release();
		void create(int32 t);
		bool empty()const;

		int32 type()const;
		int32 len()const;
		int32 rows()const;
		int32 cols()const;
		Size size()const;
		Size3 size3()const;
		static const char* typeName(int32 type);
		static int32 nameType(const char* name);
		const char* typeName()const;
		void show()const;
		std::ostream& show(std::ostream & out)const;

		template<typename _Tp> static std::vector<_Tp> List_(const std::vector<Element> &vec);
		template<typename _Tp> static std::vector<Element> _List(const std::vector<_Tp> &vec);

		Element& operator [](bool key);
		Element& operator [](int8 key);
		Element& operator [](int16 key);
		Element& operator [](int32 key);
		Element& operator [](int64 key);
		Element& operator [](uint8 key);
		Element& operator [](uint16 key);
		Element& operator [](uint32 key);
		Element& operator [](uint64 key);
		Element& operator [](float32 key);
		Element& operator [](float64 key);
		Element& operator [](float128 key);
		Element& operator [](const char * key);
		Element& operator [](const Element & key);

		const Element& operator [](bool key)const;
		const Element& operator [](int8 key)const;
		const Element& operator [](int16 key)const;
		const Element& operator [](int32 key)const;
		const Element& operator [](int64 key)const;
		const Element& operator [](uint8 key)const;
		const Element& operator [](uint16 key)const;
		const Element& operator [](uint32 key)const;
		const Element& operator [](uint64 key)const;
		const Element& operator [](float32 key)const;
		const Element& operator [](float64 key)const;
		const Element& operator [](float128 key)const;
		const Element& operator [](const char * key)const;
		const Element& operator [](const Element & key)const;

		double& operator ()(int index);
		double& operator ()(int index_1, int index_2) ;
		double& operator ()(int index_1, int index_2, int index_3) ;

		iterator begin();
		const_iterator begin()const;
		iterator end();
		const_iterator end()const;

		template<typename _Tp> operator _Tp & ();
		template<typename _Tp> operator const _Tp & ()const;
		template<typename _Tp> _Tp to();
		template<typename _Tp> const _Tp to()const;

		operator bool() const;
		operator int8() const;
		operator int16() const;
		operator int32() const;
		operator int64() const;
		operator uint8() const;
		operator uint16() const;
		operator uint32() const;
		operator uint64() const;
		operator float32() const;
		operator float64() const;
		operator float128() const;

		operator const char * () const;
		operator std::string () const;

		bool operator == (const Element & elem)const;
		bool operator != (const Element & elem)const;
		bool operator > (const Element & elem)const;
		bool operator >= (const Element & elem)const;
		bool operator <= (const Element & elem)const;
		bool operator < (const Element & elem)const;
		bool operator !()const;

		void save(const std::string & path)const;
		void load(const std::string & path);
		void save(FILE * file)const;
		void load(FILE * file);
		bool supportSave()const noexcept;
		bool supportIterator()const noexcept;

		void setCustomText(const std::string & text);
		std::string getCustomText() const;

		void set(int32 t, void* p, const char *name);
		static Element CustomElem(const Element & elem, const char *text);
		static void SaveList(FILE* file, const std::vector<Element>& list);
		static void LoadList(FILE* file, std::vector<Element>& list);
		static void SaveDict(FILE* file, const std::map<Element, Element>& dict);
		static void LoadDict(FILE* file, std::map<Element, Element>& dict);
		
		static Element Create(int type);
	protected:
		union Data {
			void *user;
			bool b;

			int8 i8;
			int16 i16;
			int32 i32;
			int64 i64;
			uint8 u8;
			uint16 u16;
			uint32 u32;
			uint64 u64;
			float32 f32;
			float64 f64;
			float128 f128;

			int8 *i8_p;
			int16 *i16_p;
			int32 *i32_p;
			int64 *i64_p;
			uint8 *u8_p;
			uint16 *u16_p;
			uint32 *u32_p;
			uint64 *u64_p;
			float32 *f32_p;
			float64 *f64_p;
			float128 *f128_p;

			Range *ra;
			Rect *r;
			RectF *rf;
			Size *s2;
			Size2f *s2f;
			Size3 *s3i;
			Size3f *s3f;

			Point* p2i;
			Point2f* p2f;
			Point3* p3i;
			Point3f* p3f;

			Vec2b* v2b;
			Vec2i* v2i;
			Vec2f* v2f;
			Vec2d* v2d;

			Vec3b* v3b;
			Vec3i* v3i;
			Vec3f* v3f;
			Vec3d* v3d;

			Vec4b* v4b;
			Vec4i* v4i;
			Vec4f* v4f;
			Vec4d* v4d;

			Color* rgb;

			Complex *c;
			LargeNumber *ln;
			MatPtr<uint8>* m8i;
			MatPtr<int32>* m32i;
			MatPtr<float32>* mf;
			MatPtr<float64>* md;
			Vector* pm;

			Mat *m;
			Array *a;

			std::string *str;

			Pointer<std::vector<Element>> *arr;
			Pointer<std::map<Element, Element>> *_map;
		}ptr;

		int32 _type;
		STLApi<std::string> _typeName;
		std::string *custom = nullptr;

		void *data_ptr = nullptr;
		Data& pre();
		Data* pointer();
		const Data* pointer()const;
	};


	template<typename _Tp> struct TEMPLATE_API ElemConvertType {
		using value_type = typename _Tp;
		bool Custom() { return false; }
		_Tp Convert(const Element& val) { return val; }
	};
	template<typename _Tp> struct TEMPLATE_API ConvertElemType {
		using value_type = typename _Tp;
		bool Custom() { return false; }
		Element Convert(const _Tp& val) { return val; }
	};
	extern LZHAPI const Element& noElem();
	extern LZHAPI Element MakeElem(const std::initializer_list<Element>& list);
	template<typename R, typename ...Params> TEMPLATE_API Element Func(R(*v)(Params...)) {
		Element elem; elem.set(Element::FUNCTION, v, typeid(v).name()); return elem;
	}
	template<typename _Tp> inline Element::Element(const _Tp * v) : _type(INTPTR), _typeName(typeid(v).name()) { data_ptr = ptr.user = (void*)v; }
		template<typename _Tp> inline Element::Element(const _Tp & v) : _type(InValid), _typeName(typeid(v).name()) { 
		ConvertElemType<_Tp> ConvertMachine;
		if (ConvertMachine.Custom()) {
			*this = ConvertMachine.Convert(v);
		}
		else {
			data_ptr = ptr.user = (void*)&v;
		}
	}
	template<typename _Tp> inline Element::Element(const std::vector<_Tp> & v) : _type(InValid), data_ptr(nullptr) {
		ptr.user = nullptr;
		std::vector<Element> list; list.insert(list.begin(), v.begin(), v.end());
		*this = list;
	}

	template<typename R, typename ...Params> inline Element::Element(const std::function<R(Params...)> & v)
		: _type(STD_FUNCTION), _typeName(typeid(v).name()) { data_ptr = ptr.user = (void*)&v; }
	template<typename ...Params> inline Element::Element(const std::tuple<Params...> & elem)
		: _type(STD_TUPLE), _typeName(typeid(elem).name())
	{
		data_ptr = (void*)&elem;
#ifdef _WIN64
		ptr.u64 = lzh::parameter_traits<Params...>::nargs;
#else
		ptr.u32 = parameter_traits<Params...>::nargs;
#endif
	}
	template<typename _Tp> inline void Element::toBaseType(_Tp & v) const
	{
		if (_type == ElemType<_Tp>::type) { v = toData<_Tp>(); }
		else {
			switch (_type)
			{
			case lzh::Element::BOOL: v = saturate_cast<_Tp>(tobool()); break;
			case lzh::Element::INT8: v = saturate_cast<_Tp>(toi8()); break;
			case lzh::Element::UINT8: v = saturate_cast<_Tp>(tou8()); break;
			case lzh::Element::INT16: v = saturate_cast<_Tp>(toi16()); break;
			case lzh::Element::UINT16: v = saturate_cast<_Tp>(tou16()); break;
			case lzh::Element::INT32: v = saturate_cast<_Tp>(toi32()); break;
			case lzh::Element::UINT32: v = saturate_cast<_Tp>(tou32()); break;
			case lzh::Element::INT64: v = saturate_cast<_Tp>(toi64()); break;
			case lzh::Element::UINT64: v = saturate_cast<_Tp>(tou64()); break;
			case lzh::Element::FLOAT32: v = saturate_cast<_Tp>(tof32()); break;
			case lzh::Element::FLOAT64: v = saturate_cast<_Tp>(tof64()); break;
			case lzh::Element::FLOAT128: v = saturate_cast<_Tp>(tof128()); break;
			default:THROW_INFO_STR("基础类型转换失败, 支持的基础类型转换{char,uchar,short,ushort,int,uint,long long int,ulong long int,float,double,long double}");
			}
		}
	}
	using ElemList = typename std::vector<Element>;
	template<typename _Tp> inline Element& lzh::Element::operator=(const _Tp& val) { 
		ConvertElemType<_Tp> ConvertMachine;
		if (ConvertMachine.Custom()) {
			*this = ConvertMachine.Convert(val);
		}
		else {
			*this = Element(val);
		}
		return *this;
	}

	template<typename ...Params> TEMPLATE_API inline ElemList MakeElem(const Params & ...params) {
		ElemList list;
		return MakeElem(list, params...);
	}
	template<typename _Ty> TEMPLATE_API ElemList& MakeElem(ElemList & list, const _Ty & v) {
		list.push_back(v);
		return list;
	}
	template<typename _Ty, typename ...Params> TEMPLATE_API ElemList& MakeElem(ElemList & list, const _Ty & v, const Params & ...params) {
		list.push_back(v);
		return MakeElem(list, params...);
	}
	template<typename _Tp> inline _Tp Element::toBaseType() const
	{
		_Tp v;
		toBaseType(v);
		return v;
	}
	template<typename _Tp> inline Point2_<_Tp> Element::toPoint_() const
	{
		switch (_type) {
		case POINT:return Point2_<_Tp>(toPoint());
		case POINTF:return Point2_<_Tp>(toPointF());
		default:THROW_INFO(ERR_INFO_TYPE);
		}
	}
	template<typename _Tp> inline Point3_<_Tp> Element::toPoint3_() const
	{
		switch (_type) {
		case POINT3:return Point3_<_Tp>(toPoint3());
		case POINT3F:return Point3_<_Tp>(toPoint3F());
		default:THROW_INFO(ERR_INFO_TYPE);
		}
	}
	template<typename _Ptr> 
	inline _Ptr* Element::toFunc()
	{
		LZH_ACCESS(if (_type == FUNCTION), THROW_TYPE_INFO(typeName(FUNCTION), typeName()));
		return toPtr<_Ptr>();
	}
	template<typename _Ptr> 
	inline const _Ptr* Element::toFunc()const
	{
		LZH_ACCESS(if (_type == FUNCTION), THROW_TYPE_INFO(typeName(FUNCTION), typeName()));
		return toPtr<_Ptr>();
	}
	template<typename R, typename ...Params>
	inline std::function<R(Params...)> Element::toStdFunc() const
	{
		LZH_ACCESS(if (_type == STD_FUNCTION || _type == FUNCTION), THROW_TYPE_INFO(typeName(STD_FUNCTION), typeName()));
		return toData<std::function<R(Params...)>>();
	}
	template<typename R, typename ...Params> 
	inline R Element::f(const Params &... params)const
	{
		LZH_ACCESS(if (_type == STD_FUNCTION || _type == FUNCTION), THROW_TYPE_INFO(typeName(FUNCTION), typeName()));
		if (_type == STD_FUNCTION)
			return ((std::function<R(Params...)>*)(data()))->operator()(params...);
		else if (_type == FUNCTION)
			return std::function<R(Params...)>((R(*)(Params...))data())(params...);
	}
	template<typename _Tp> inline _Tp& Element::toUserData() const {
		LZH_ACCESS(if (_type == InValid), THROW_TYPE_INFO(typeName(InValid), typeName()));
		return *(_Tp*)ptr.user;
	}
	template<typename _Tp> inline void Element::toPtr(_Tp ** v) const
	{
		if (v) *v = (_Tp*)data();
	}	
	template<typename _Tp> inline _Tp* Element::toPtr()
	{
		return (_Tp*)data();
	}
	template<typename _Tp> inline const _Tp*  Element::toPtr() const
	{
		return (const _Tp*)data();
	}
	template<typename _Tp> inline void Element::toData(_Tp & v) const {
		v = *(_Tp*)data();
	}
	template<typename _Tp> inline _Tp & Element::toData() const {
		return *(_Tp*)data();
	}
	template<typename _Tp> inline Element::operator _Tp&() {
		LZH_ACCESS(if (_type == INTPTR || _type == ElemType<_Tp>::type), THROW_TYPE_INFO(typeid(_Tp).name(), typeName()));
		return toData<_Tp>();
	}
	template<typename _Tp> inline Element::operator const _Tp&() const {
		LZH_ACCESS(if (_type == INTPTR || _type == ElemType<_Tp>::type), THROW_TYPE_INFO(typeid(_Tp).name(), typeName()));
		return toData<_Tp>();
	}
	template<typename _Tp> inline _Tp Element::to() {
		ElemConvertType<_Tp> ConvertMachine;
		if (ConvertMachine.Custom()) {
			return ConvertMachine.Convert(*this);
		}
		else {
			LZH_ACCESS(if (_type == INTPTR || _type == ElemType<_Tp>::type), THROW_TYPE_INFO(typeid(_Tp).name(), typeName()));
			return toData<_Tp>();
		}
	}
	template<typename _Tp> inline const _Tp Element::to() const {
		ElemConvertType<_Tp> ConvertMachine;
		if (ConvertMachine.Custom()) {
			return ConvertMachine.Convert(*this);
		}
		else {
			LZH_ACCESS(if (_type == INTPTR || _type == ElemType<_Tp>::type), THROW_TYPE_INFO(typeid(_Tp).name(), typeName()));
			return toData<_Tp>();
		}
	}
	template<typename _Tp> static std::vector<_Tp> Element::List_(const std::vector<Element> &vec) {
		std::vector<_Tp> elemlist;
		elemlist.insert(elemlist.end(), vec.begin(), vec.end());
		return elemlist;
	}
	template<typename _Tp> inline static std::vector<Element> Element::_List(const std::vector<_Tp> &vec) {
		std::vector<Element> elemlist;
		elemlist.insert(elemlist.end(), vec.begin(), vec.end());
		return elemlist;
	}

	template<typename ...Params>
	class TEMPLATE_API ElemExport;
	template<typename ...Params>
	class TEMPLATE_API ElemExport<Params&...>
	{
	public:
		ElemExport(Params & ... params)
			: _ptr(new std::tuple<Params&...>(params...))
		{
			nargs = parameter_traits<Params...>::nargs;
		}
		~ElemExport() { FREE_PTR(_ptr); }
		ElemExport(const ElemExport & ex) {
			*this = ex;
		}
		ElemExport<Params&...>& operator = (const Element & elem)
		{
			if (elem.isTuple())
				*_ptr = elem.toData<std::tuple<Params...>>();
			else if (elem.isList()) {
				if (nargs > elem.size())return *this;
				tuple_assign<decltype(*_ptr), sizeof...(Params), std::vector<Element>::iterator, Params...>::assign(*_ptr, elem.toList().end());
			}
			else THROW_INFO(ERR_INFO_TYPE);
			return *this;
		}
		ElemExport<Params&...> & operator = (const std::tuple<Params...> & elem)
		{
			*_ptr = elem;
			return *this;
		}
		template<typename ...Others>
		ElemExport<Params&...>& operator = (ElemExport<Others...> & ex)
		{
			*_ptr = ex.obj();
			nargs = ex.size();
			return *this;
		}
		template<typename ...Others>
		ElemExport<Params&...>& operator = (ElemExport<Others...> && ex)
		{
			*_ptr = ex.obj();
			nargs = ex.size();
			return *this;
		}
		ElemExport<Params&...>& operator = (const ElemExport<Params&...> &ex) {
			if (this == &ex) return *this;
			*_ptr = *ex._ptr;
			nargs = ex.nargs;
			return *this;
		}
		size_t size()const { return nargs; }
		const std::tuple<Params&...>& obj()const { return *_ptr; }
	private:
		size_t nargs;
		std::tuple<Params&...>* _ptr;
	};
	template<typename ...Params> TEMPLATE_API inline ElemExport<Params&...> zip(Params & ... params) {
		return ElemExport<Params&...>(params...);
	}
	template<typename ...Params> TEMPLATE_API inline ElemExport<Params...> zip(Params && ... params) {
		return ElemExport<Params...>(params...);
	}

	class LZHAPI ElemOp : public Element
	{
	public:
		enum
		{
			ADD = 0,
			SUB,
			MUL,
			DIV,
			POW
		};
		explicit ElemOp();
		ElemOp(const bool & v);
		ElemOp(const int & v);
		ElemOp(const double & v);
		ElemOp(const Vector & v);
		ElemOp(const Mat & v);
		ElemOp(const Array & v);
		ElemOp(const char * v);
		ElemOp(const std::string & v);
		ElemOp(const ElemList & v);
		//ElemOp(const std::initializer_list<Element> & v);

		ElemOp(const Element & v);

		const char * toChar()const;
		int& toRefInt();
		double& toRefDouble();
		std::string& toRefStr();

		bool supportOperator(int32 code = ADD)const;
		ElemOp Operator(const ElemOp &element, int32 code)const;

		ElemOp pow(const ElemOp &element)const;
		ElemOp sqrt()const;
		ElemOp sin()const;
		ElemOp cos()const;
		ElemOp tan()const;
		ElemOp sinh()const;
		ElemOp cosh()const;
		ElemOp tanh()const;
		ElemOp asin()const;
		ElemOp acos()const;
		ElemOp atan()const;
		ElemOp atan2(const ElemOp &element)const;
		ElemOp asinh()const;
		ElemOp acosh()const;
		ElemOp atanh()const;
		ElemOp floor()const;
		ElemOp round()const;
		ElemOp mod(const ElemOp &element)const;
		ElemOp abs()const;

		ElemOp operator + (const ElemOp &element)const;
		ElemOp operator - (const ElemOp &element)const;
		ElemOp operator * (const ElemOp &element)const;
		ElemOp operator / (const ElemOp &element)const;

		ElemOp operator += (const ElemOp &element);
		ElemOp operator -= (const ElemOp &element);
		ElemOp operator *= (const ElemOp &element);
		ElemOp operator /= (const ElemOp &element);
		static ElemOp Operator(const ElemOp & a, const ElemOp & b, int32 code);
	private:
		static ElemOp Add(const Element & a, const Element & b);
		static ElemOp add(const ElemOp & a, const ElemList & b);
		static ElemOp add(const ElemList & a, const ElemList & b);
		static ElemOp add(const ElemList & a, const ElemOp & b);

		static ElemOp Sub(const Element & a, const Element & b);
		static ElemOp sub(const ElemOp & a, const ElemList & b);
		static ElemOp sub(const ElemList & a, const ElemList & b);
		static ElemOp sub(const ElemList & a, const ElemOp & b);

		static ElemOp Mul(const Element & a, const Element & b);
		static ElemOp mul(const ElemOp & a, const ElemList & b);
		static ElemOp mul(const ElemList & a, const ElemList & b);
		static ElemOp mul(const ElemList & a, const ElemOp & b);

		static ElemOp Div(const Element & a, const Element & b);
		static ElemOp div(const ElemOp & a, const ElemList & b);
		static ElemOp div(const ElemList & a, const ElemList & b);
		static ElemOp div(const ElemList & a, const ElemOp & b);
	};
	using ElemOpList = typename	std::vector<ElemOp>;

}

#endif // !__ELEMENT_H__