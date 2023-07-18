#pragma once
#ifndef __TYPE_H__
#define __TYPE_H__

#include <limits.h>
//#define TYPE_8U		//unsigned char
//#define TYPE_8S		//signed char
//#define TYPE_16U		//unsigned short
//#define TYPE_16S		//short
//#define TYPE_32U		//unsigned
//#define TYPE_32S		//signed
//#define TYPE_64U		//unsigned long long
//#define TYPE_64S		//signed long long
//#define TYPE_32F		//float
#define TYPE_64F		//double
//#define TYPE_64D		//long double

enum
{
	True = 0,
	False = ~True
};
#ifndef MIN
#define MIN(a,b) ((a)<(b)?a:b)
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b)?a:b)
#endif
#ifndef ABS
#define ABS(a) ((a)>(0)?(a):-(a))
#endif

#ifdef max
#undef max
template<typename _Tp> _Tp max(_Tp a, _Tp b) { return MAX(a, b); }
#endif

#ifdef min
#undef min
template<typename _Tp> _Tp min(_Tp a, _Tp b) { return MIN(a, b); }
#endif

namespace lzh
{
	typedef void*					IntPtr;

	typedef char					int8;
	typedef unsigned char			uint8;
	typedef short					int16;
	typedef unsigned short			uint16;
	typedef int						int32;
	typedef unsigned int			uint32;
	typedef long long				int64;
	typedef unsigned long long		uint64;
	typedef float					float32;
	typedef double					float64;
	typedef long double				float128;

#ifdef _WIN64
	using msize = typename uint64;
#else
	using msize = typename uint32;
#endif // _WIN64

#ifdef TYPE_32F
	typedef float32 mat_t;
#elif defined TYPE_8U
	typedef uint8 mat_t;
#elif defined TYPE_8S
	typedef int8 mat_t;
#elif defined TYPE_16U
	typedef uint16 mat_t;
#elif defined TYPE_16S
	typedef int16 mat_t;
#elif defined TYPE_32U
	typedef uint32 mat_t;
#elif defined TYPE_32S
	typedef int32 mat_t;
#elif defined TYPE_64U
	typedef uint64 mat_t;
#elif defined TYPE_64S
	typedef int64 mat_t;
#elif defined TYPE_64F
	typedef float64 mat_t;
#elif defined TYPE_64D
	typedef float128 mat_t;
#else
	typedef float32 mat_t;
#endif

	enum
	{
		SHIFT = 8,
		KIND_SHIFT = (1 << int32(SHIFT)) - 1,

		NONE = 0,
		TP_8U = 1,
		TP_8S = 2,
		TP_16U = 3,
		TP_16S = 4,
		TP_32U = 5,
		TP_32S = 6,
		TP_64U = 7,
		TP_64S = 8,
		TP_32F = 9,
		TP_64F = 10,
		TP_64D = 11,
	};
#define TOCVTYPE(x) ((x)==lzh::TP_32S?4:((x)==lzh::TP_32F?5:((x)==lzh::TP_64F?6:(((x)>=lzh::TP_8U && (x)<=lzh::TP_16S)?(x)-1:7))))
#define CVTOTYPE(x) ((x)==4?lzh::TP_32S:((x)==5?lzh::TP_32F:((x)==6?lzh::TP_64F:(x)+1)))

#define MAT_8S			lzh::TP_8S
#define MAT_8U			lzh::TP_8U
#define MAT_16S			lzh::TP_16S
#define MAT_16U			lzh::TP_16U
#define MAT_32S			lzh::TP_32S
#define MAT_32U			lzh::TP_32U
#define MAT_64S			lzh::TP_64S
#define MAT_64U			lzh::TP_64U
#define MAT_32F			lzh::TP_32F
#define MAT_64F			lzh::TP_64F
#define MAT_64D			lzh::TP_64D
#define _T(v)			(lzh::saturate_cast<lzh::mat_t>(v))
#define _I(v)			(lzh::saturate_cast<lzh::int32>(v))

	template<typename _Tp> static inline _Tp saturate_cast(uint8 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(int8 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(uint16 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(int16 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(uint32 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(int32 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(float32 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(float64 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(float128 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(int64 v) { return _Tp(v); }
	/** @overload */
	template<typename _Tp> static inline _Tp saturate_cast(uint64 v) { return _Tp(v); }

	template<> inline uint8 saturate_cast<uint8>(int8 v) { return (uint8)MAX((int32)v, 0); }
	template<> inline uint8 saturate_cast<uint8>(uint16 v) { return (uint8)MIN((uint32)v, (uint32)UCHAR_MAX); }
	template<> inline uint8 saturate_cast<uint8>(int32 v) { return (uint8)((uint32)v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
	template<> inline uint8 saturate_cast<uint8>(int16 v) { return saturate_cast<uint8>((int32)v); }
	template<> inline uint8 saturate_cast<uint8>(uint32 v) { return (uint8)MIN(v, (uint32)UCHAR_MAX); }
	template<> inline uint8 saturate_cast<uint8>(float32 v) { int32 iv = static_cast<int32>(v + 0.5); return saturate_cast<uint8>(iv); }
	template<> inline uint8 saturate_cast<uint8>(float64 v) { int32 iv = static_cast<int32>(v + 0.5); return saturate_cast<uint8>(iv); }
	template<> inline uint8 saturate_cast<uint8>(int64 v) { return (uint8)((uint64)v <= (uint64)UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
	template<> inline uint8 saturate_cast<uint8>(uint64 v) { return (uint8)MIN(v, (uint64)UCHAR_MAX); }

	template<> inline int8 saturate_cast<int8>(uint8 v) { return (int8)MIN((int32)v, SCHAR_MAX); }
	template<> inline int8 saturate_cast<int8>(uint16 v) { return (int8)MIN((uint32)v, (uint32)SCHAR_MAX); }
	template<> inline int8 saturate_cast<int8>(int32 v) { return (int8)((uint32)(v - SCHAR_MIN) <= (uint32)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
	template<> inline int8 saturate_cast<int8>(int16 v) { return saturate_cast<int8>((int32)v); }
	template<> inline int8 saturate_cast<int8>(uint32 v) { return (int8)MIN(v, (uint32)SCHAR_MAX); }
	template<> inline int8 saturate_cast<int8>(float32 v) { int32 iv = static_cast<int>(v + 0.5); return saturate_cast<int8>(iv); }
	template<> inline int8 saturate_cast<int8>(float64 v) { int32 iv = static_cast<int>(v + 0.5); return saturate_cast<int8>(iv); }
	template<> inline int8 saturate_cast<int8>(int64 v) { return (int8)((uint64)((int64)v - SCHAR_MIN) <= (uint64)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
	template<> inline int8 saturate_cast<int8>(uint64 v) { return (int8)MIN(v, (uint64)SCHAR_MAX); }

	template<> inline uint16 saturate_cast<uint16>(int8 v) { return (uint16)MAX((int32)v, 0); }
	template<> inline uint16 saturate_cast<uint16>(int16 v) { return (uint16)MAX((int32)v, 0); }
	template<> inline uint16 saturate_cast<uint16>(int32 v) { return (uint16)((uint32)v <= (uint32)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
	template<> inline uint16 saturate_cast<uint16>(uint32 v) { return (uint16)MIN(v, (uint32)USHRT_MAX); }
	template<> inline uint16 saturate_cast<uint16>(float32 v) { int32 iv = static_cast<int>(v + 0.5); return saturate_cast<uint16>(iv); }
	template<> inline uint16 saturate_cast<uint16>(float64 v) { int32 iv = static_cast<int>(v + 0.5); return saturate_cast<uint16>(iv); }
	template<> inline uint16 saturate_cast<uint16>(int64 v) { return (uint16)((uint64)v <= (uint64)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
	template<> inline uint16 saturate_cast<uint16>(uint64 v) { return (uint16)MIN(v, (uint64)USHRT_MAX); }

	template<> inline int16 saturate_cast<int16>(uint16 v) { return (int16)MIN((int32)v, SHRT_MAX); }
	template<> inline int16 saturate_cast<int16>(int32 v) { return (int16)((uint32)(v - SHRT_MIN) <= (uint32)USHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
	template<> inline int16 saturate_cast<int16>(uint32 v) { return (int16)MIN(v, (uint32)SHRT_MAX); }
	template<> inline int16 saturate_cast<int16>(float32 v) { int32 iv = static_cast<int>(v + 0.5); return saturate_cast<int16>(iv); }
	template<> inline int16 saturate_cast<int16>(float64 v) { int32 iv = static_cast<int>(v + 0.5); return saturate_cast<int16>(iv); }
	template<> inline int16 saturate_cast<int16>(int64 v) { return (int16)((uint64)((int64)v - SHRT_MIN) <= (uint64)USHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
	template<> inline int16 saturate_cast<int16>(uint64 v) { return (int16)MIN(v, (uint64)SHRT_MAX); }

	template<> inline int32 saturate_cast<int32>(uint32 v) { return (int32)MIN(v, (uint32)INT_MAX); }
	template<> inline int32 saturate_cast<int32>(int64 v) { return (int32)((uint64)(v - INT_MIN) <= (uint64)UINT_MAX ? v : v > 0 ? INT_MAX : INT_MIN); }
	template<> inline int32 saturate_cast<int32>(uint64 v) { return (int32)MIN(v, (uint64)INT_MAX); }
	template<> inline int32 saturate_cast<int32>(float32 v) { return static_cast<int>(v + 0.5); }
	template<> inline int32 saturate_cast<int32>(float64 v) { return static_cast<int>(v + 0.5); }

	template<> inline uint32 saturate_cast<uint32>(int8 v) { return (uint32)MAX(v, (int8)0); }
	template<> inline uint32 saturate_cast<uint32>(int16 v) { return (uint32)MAX(v, (int16)0); }
	template<> inline uint32 saturate_cast<uint32>(int32 v) { return (uint32)MAX(v, (int32)0); }
	template<> inline uint32 saturate_cast<uint32>(int64 v) { return (uint32)((uint64)v <= (uint64)UINT_MAX ? v : v > 0 ? UINT_MAX : 0); }
	template<> inline uint32 saturate_cast<uint32>(uint64 v) { return (uint32)MIN(v, (uint64)UINT_MAX); }
	// we intentionally do not clip negative numbers, to make -1 become 0xffffffff etc.
	template<> inline uint32 saturate_cast<uint32>(float32 v) { return static_cast<uint32>(static_cast<int>(v + 0.5)); }
	template<> inline uint32 saturate_cast<uint32>(float64 v) { return static_cast<uint32>(static_cast<int>(v + 0.5)); }

	template<> inline uint64 saturate_cast<uint64>(int8 v) { return (uint64)MAX(v, (int8)0); }
	template<> inline uint64 saturate_cast<uint64>(int16 v) { return (uint64)MAX(v, (int16)0); }
	template<> inline uint64 saturate_cast<uint64>(int32 v) { return (uint64)MAX(v, (int32)0); }
	template<> inline uint64 saturate_cast<uint64>(int64 v) { return (uint64)MAX(v, (int64)0); }

	template<> inline int64 saturate_cast<int64>(uint64 v) { return (int64)MIN(v, (uint64)LLONG_MAX); }
}
#endif // !__TYPE_H__
