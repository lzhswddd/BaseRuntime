#ifndef __CORE_H__
#define __CORE_H__

#include <string>
#include <vector>
#include <exception>
#include <initializer_list>
#include "type.h"

//#define LZH_LIB
#ifdef LZH_LIB
#define LZHAPI
#define TEMPLATE_API
#else
#ifdef LZH_EXPORTS
#define LZHAPI __declspec(dllexport)
#else 
#define LZHAPI __declspec(dllimport)
#endif
#define TEMPLATE_API __declspec(dllexport)
#endif

//#define LZH_DEBUG
#ifdef _DEBUG
#ifndef MAT_FAST
#define LZH_DEBUG
#endif // DEBUG
#endif

#ifndef LENGTH
#define LENGTH(x) (sizeof(x)/sizeof(lzh::mat_t))
#endif
#ifndef ILength
#define ILength(x) (sizeof(x)/sizeof(lzh::int32))
#endif

#define MALLOC_ALIGN 16

#define DISABEL_ASSIGN(ClassType) \
	ClassType(const ClassType &value){} \
	ClassType& operator = (const ClassType &value){ return *this }

#define _NOP do{}while(0)

#define FREE_PTR_NONULL(data) do{if((data)!=nullptr) {delete (data);}}while(0)
#define FREE_PTR(data) do{if((data)!=nullptr) {delete (data); (data)=nullptr;}}while(0)
#define FREE_POINT(data) do{if((*(data))!=nullptr) {delete (*(data)); (*(data))=nullptr;}}while(0)
#define FREE_ARRAY(data) do{if((data)!=nullptr) {delete[] (data); (data)=nullptr;}}while(0)

#define ARRAY_LEN(start, end) lzh::int32(((end) - (start)))

#define _RANGE_(low, v, high) ((low)<(v)&&(v)<(high))
#define __RANGE(low, v, high) ((low)<=(v)&&(v)<(high))
#define RANGE__(low, v, high) ((low)<(v)&&(v)<=(high))
#define __RANGE__(low, v, high) ((low)<=(v)&&(v)<=(high))

//#define FOR(i, range_) for(int32 i = range_.v1; i < range_.v2; i+=(int32)range_.gap_)

#define FOR_RANGE_(start, end) for(lzh::int32 virtual_itereator = (start); virtual_itereator != (end); ++virtual_itereator)
#define FOR_RANGE(i, low, high) for(lzh::int32 i = (low); (i) != (high); ++(i))

#define GET_VALUE(type, v) (*(type*)(v))

#define CHECK_INDEX_IMAGE(v, limit) (MIN((MAX((v), 0)), (limit-1)))
#define FOR_IMAGE(i, img, dire) for(lzh::int32 (i) = 0; (i) < ((dire) == 1?(img).rows():((dire) == 2 ? (img).cols() : (img).channels())); ++i)
#define FOR_IMAGE_(i, img, dire) for(lzh::int32 (i) = 0; (i) < ((dire) == 1?(img).rows:((dire) == 2 ? (img).cols : (img).channels())); ++i)


#define STR1(R)  #R
#define STR2(R)  STR1(R)

extern LZHAPI void print(int colorCode, const char *info, ...);

#define SHOW_ERRINFO
//#define EXCEPTION_CHINESE

#define LZH_CHECK(judge, reval) do{judge{_NOP;}else{return reval;}}while(0)
#define LZH_INV_CHECK(judge, reval) do{judge{return reval;}}while(0)

#define THROW_COLOR 0x0004

#ifdef SHOW_ERRINFO
#ifdef EXCEPTION_CHINESE
#define THROW_STR "(位于函数%s中)引发的异常, 第%s行处, @异常信息:"
#define THROW_STRING(str) (std::string("ERROR: (位于函数") + __FUNCTION__ + "中)引发的异常, 第" + STR2(__LINE__) + "行处, @异常信息:" + str).c_str()
#define LZH_ACCESS(judge, info) do{judge{_NOP;}else{print(THROW_COLOR, "ERROR: 在判断中%s程序不符合要求 @异常位置:",STR2(judge));info;}}while(0)
#else
#define THROW_STR "Exception thrown in function %s, in %s line, @Exception Information:"
#define THROW_STRING(str) (std::string("ERROR: Exception thrown (In function ") + __FUNCTION__ + "), in " + STR2(__LINE__) + "line, @Exception Information:" + str).c_str()
#define LZH_ACCESS(judge, info) do{judge{_NOP;}else{print(THROW_COLOR, "ERROR: %s, the program does not meet the requirements @Abnormal Position:",STR2(judge));info;}}while(0)
#endif
#define THROW_LOG print(THROW_COLOR, THROW_STR, __FUNCTION__, STR2(__LINE__))
#define THROW_LOG_END fprintf(stderr, "\n")
#define THROW_INFO(info) do{THROW_LOG; print(THROW_COLOR, lzh::errinfo[info]);THROW_LOG_END;throw std::exception(THROW_STRING(lzh::errinfo[(info)]));}while(0)
#define THROW_INFO_STR(info) do{THROW_LOG; print(THROW_COLOR, info);THROW_LOG_END;throw std::exception(THROW_STRING(info));}while(0)
#define THROW_TYPE_INFO(should_type, actually_type) THROW_INFO_STR((std::string("need type is ")+(should_type)+", but getting actually type is "+(actually_type)).c_str())
#else
#ifdef EXCEPTION_CHINESE
#define THROW_STR "(位于函数%s中)引发的异常, 第%s行处, @异常信息:"
#define THROW_STRING(str) (std::string("ERROR: (位于函数") + __FUNCTION__ + "中)引发的异常, 第" + STR2(__LINE__) + "行处, @异常信息:" + str).c_str()
#define LZH_ACCESS(judge, info) do{judge{_NOP;}else{print(THROW_COLOR, "ERROR: 在判断中%s程序不符合要求 @异常位置:",STR2(judge));info;}}while(0)
#else
#define THROW_STR "Exception thrown in function %s, in %s line, @Exception Information:"
#define THROW_STRING(str) (std::string("ERROR: Exception thrown (In function ") + __FUNCTION__ + "), in " + STR2(__LINE__) + " line, @Exception Information:" + str).c_str()
#endif
#define THROW_INFO(info) do{throw std::exception(THROW_STRING(lzh::errinfo[(info)]));}while(0)
#define THROW_INFO_STR(info) do{throw std::exception(THROW_STRING(info));}while(0)
#define THROW_TYPE_INFO(should_type, actually_type) THROW_INFO_STR((std::string("need type is ")+(should_type)+", but getting actually type is "+(actually_type)).c_str())
#define LZH_ACCESS(judge, info) do{judge{_NOP;}else{info;}}while(0)
#endif

#define CHECK_MATRIX(m_) LZH_ACCESS(if((m_) != nullptr), THROW_INFO(ERR_INFO_EMPTY))
#define CHECK_MATRIX_(mat) LZH_ACCESS(if(!(mat).empty()), THROW_INFO(ERR_INFO_EMPTY))
#define CHECK_PTR_ORDER(start, end) LZH_ACCESS(if((end) - (start) >= 0), THROW_INFO(lzh::ERR_INFO_UNLESS))
#define CHECK_PTR(s) LZH_ACCESS(if((s) != nullptr), THROW_INFO(lzh::ERR_INFO_PTR))
#define CHECK_INDEX(i, len) LZH_ACCESS(if((i) >= 0 && i < len), THROW_INFO(lzh::ERR_INFO_UNLESS))

#define CHECK_TYPE(_tp, v) LZH_ACCESS(if((lzh::DataType<_tp>::type & lzh::KIND_SHIFT)==(v)), THROW_INFO(lzh::ERR_INFO_TYPE))
#define CHECK_TYPE_(t1, t2) LZH_ACCESS(if((t1)==(t2)), THROW_INFO(lzh::ERR_INFO_TYPE))

#define MAT_GETPTR(type, data) (((type*)data))
#define MAT_INDEX(type, data ,i) ((((type*)data)) + i)
#define MAT_GETARRAY(type, data ,i) ((((type*)data))[i])
#define MAT_VALUE(_Tp, t, data ,i, out) do{ switch (t){ \
		case lzh::TP_8S:	out = (_Tp)MAT_GETARRAY(int8,	data, i); break; \
		case lzh::TP_8U:	out = (_Tp)MAT_GETARRAY(uint8,	data, i); break; \
		case lzh::TP_16S:	out = (_Tp)MAT_GETARRAY(int16,	data, i); break; \
		case lzh::TP_16U:	out = (_Tp)MAT_GETARRAY(uint16,	data, i); break; \
		case lzh::TP_32S:	out = (_Tp)MAT_GETARRAY(int32,	data, i); break; \
		case lzh::TP_32U:	out = (_Tp)MAT_GETARRAY(uint32,	data, i); break; \
		case lzh::TP_64S:	out = (_Tp)MAT_GETARRAY(int64,	data, i); break; \
		case lzh::TP_64U:	out = (_Tp)MAT_GETARRAY(uint64,	data, i); break; \
		case lzh::TP_32F:	out = (_Tp)MAT_GETARRAY(float32,	data, i); break; \
		case lzh::TP_64F:	out = (_Tp)MAT_GETARRAY(float64,	data, i); break; \
		case lzh::TP_64D:	out = (_Tp)MAT_GETARRAY(float128, data, i); break; \
		default:THROW_INFO(ERR_INFO_TYPE);}}while(0)
#define MAT_GET(_Tp, t, data, out) do{ switch (t){ \
		case lzh::TP_8S:	out = (_Tp)*GET_VALUE(int8,		data); break; \
		case lzh::TP_8U:	out = (_Tp)*GET_VALUE(uint8,	data); break; \
		case lzh::TP_16S:	out = (_Tp)*GET_VALUE(int16,	data); break; \
		case lzh::TP_16U:	out = (_Tp)*GET_VALUE(uint16,	data); break; \
		case lzh::TP_32S:	out = (_Tp)*GET_VALUE(int32,	data); break; \
		case lzh::TP_32U:	out = (_Tp)*GET_VALUE(uint32,	data); break; \
		case lzh::TP_64S:	out = (_Tp)*GET_VALUE(int64,	data); break; \
		case lzh::TP_64U:	out = (_Tp)*GET_VALUE(uint64,	data); break; \
		case lzh::TP_32F:	out = (_Tp)*GET_VALUE(float32,	data); break; \
		case lzh::TP_64F:	out = (_Tp)*GET_VALUE(float64,	data); break; \
		case lzh::TP_64D:	out = (_Tp)*GET_VALUE(float128, data); break; \
		default:THROW_INFO(ERR_INFO_TYPE);}}while(0)
#define COMPARE(v1, v2 ,op, out)do{switch (op) {\
		case -2:	out = ((v1) < (v2));break;\
		case -1:	out = ((v1) <= (v2));break;\
		case 0:		out = ((v1) == (v2));break;\
		case 1:		out = ((v1) >= (v2));break;\
		case 2:		out = ((v1) > (v2));break;\
		default:break;}}while(0)

#define MAT_CREATEDEPTH(v)		(((v)-1) << lzh::int32(lzh::SHIFT))
#define MAT_CREATETYPE(v)		MAT_CREATEDEPTH(v)
#define MAT_GETVALUE(v)			((v) & KIND_SHIFT)
#define MAT_GETDEPTH(v)			((((v) &~KIND_SHIFT) >> lzh::int32(SHIFT)) + 1)

#define _FOR(index,start,end,stride) for(lzh::msize index=(msize)(start);index<(msize)(end);index+=(stride))
#define FORINT(index,start,end,stride) for(lzh::int32 index=(int32)(start);(stride)<0?index>=(int32)(end):index<(int32)(end);index+=(stride))
#define toVarray(x) (lzh::msize(x))

#define MAT_SWITCH(angle, flag) (((angle)&(flag)) != 0)

#ifdef SHOW_ERRINFO
#define MAT_NOEXCEPT(function, op) do{try{function;}catch(std::exception ex){op;}}while (0)
#else 
#define MAT_NOEXCEPT(function, op) do{try{function;}catch(std::exception ex){printf("%s\n", ex.what());op;}}while (0)
#endif

#define GET_ANGLE(angle) (1 << (int32(angle) / 45))

// exchange-add operation for atomic operations on reference counters
#if defined __INTEL_COMPILER && !(defined WIN32 || defined _WIN32)
	// atomic increment on the linux version of the Intel(tm) compiler
#  define MAT_XADD(addr, delta) (int)_InterlockedExchangeAdd(const_cast<void*>(reinterpret_cast<volatile void*>(addr)), delta)
#elif defined __GNUC__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)
#    ifdef __ATOMIC_ACQ_REL
#      define MAT_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define MAT_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
// version for gcc >= 4.7
#      define MAT_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define MAT_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && !defined RC_INVOKED
#  include <intrin.h>
#  define MAT_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
static inline void MAT_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
#endif

namespace lzh 
{
#define MAT_TYPE(t)		(lzh::DataType<t>::type)
#define MAT_T			(lzh::DataType<lzh::mat_t>::type)
#ifndef EXP
#define EXP					2.7182818284590452353602874713527
#endif // EXP
#ifndef EXP_
#define EXP_				_T(EXP)
#endif // EXP_
#ifndef PI
#define PI					3.1415926535897932384626433832795
#endif // PI
#ifndef PI_
#define PI_					_T(PI)
#endif // PI_
#ifndef D_PI
#define D_PI				6.283185307179586476925286766559
#endif // D_PI
#ifndef D_PI_
#define D_PI_				_T(D_PI)
#endif // D_PI
//LeakyReLU的超参
#define LReLU_alpha			0.2
#define LReLU_alpha_		_T(LReLU_alpha)
//ELU的超参
#define ELU_alpha			1.6732632423543772848170429916717
#define ELU_alpha_			_T(ELU_alpha)
//SeLU的超参
#define SELU_scale			1.0507009873554804934193349852946
#define SELU_scale_			_T(SELU_scale)

#define shrink_factor		1.247330950103979
#define shrink_factor_		_T(shrink_factor)

#define ang2rad				0.01745329251994329576923690768489
#define rad2ang				57.295779513082320876798154814105
#define ang2rad_			_T(ang2rad)
#define rad2ang_			_T(rad2ang)

#define gauss_factor_1D		1.0/2.506628274631000502415765284811
#define gauss_factor_2D		1.0/6.283185307179586476925286766559
#define gauss_factor_1D_	_T(gauss_factor_1D)
#define gauss_factor_2D_	_T(gauss_factor_2D)

	static const uint32 M_SIZE = sizeof(mat_t);

	enum PrintType {
		MAT_FIXED = 0,
		MAT_SCIENTIFIC = 1
	};
	enum BorderType {
		BORDER_CONSTANT = 0,	//!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
		BORDER_REPLICATE = 1,	//!< `aaaaaa|abcdefgh|hhhhhhh`
		BORDER_REFLECT = 2,		//!< `fedcba|abcdefgh|hgfedcb`
		BORDER_WRAP = 3,		//!< `cdefgh|abcdefgh|abcdefg`
		BORDER_REFLECT_101 = 4, //!< `gfedcb|abcdefgh|gfedcba`
		//BORDER_TRANSPARENT = 5, //!< `uvwxyz|abcdefgh|ijklmno`
		//BORDER_ISOLATED = 16,	//!< do not look outside of ROI
		BORDER_DEFAULT = BORDER_REFLECT_101
	};
	enum MatErrorInfo {
		ERR_INFO_EMPTY = 0,
		ERR_INFO_SQUARE = 1,
		ERR_INFO_ADJ = 2,
		ERR_INFO_INV = 3,
		ERR_INFO_POW = 4,
		ERR_INFO_IND = 5,
		ERR_INFO_CON = 6,
		ERR_INFO_EIGEN = 7,
		ERR_INFO_LEN = 8,
		ERR_INFO_MEMOUT = 9,
		ERR_INFO_UNLESS = 10,
		ERR_INFO_SIZE = 11,
		ERR_INFO_MULT = 12,
		ERR_INFO_NORM = 13,
		ERR_INFO_VALUE = 14,
		ERR_INFO_PINV = 15,
		ERR_INFO_DET = 16,
		ERR_INFO_DIM = 17,
		ERR_INFO_PTR = 18,
		ERR_INFO_NOT2D = 19,
		ERR_INFO_FILE = 20,
		ERR_INFO_TYPE = 21,
		ERR_INFO_DIV = 22,
		ERR_INFO_FFT = 23,
		ERR_INFO_CHANNEL = 24,
		ERR_INFO_SUPPORT = 25
	}; 
#ifdef EXCEPTION_CHINESE
	static const int8 *errinfo[] = {
		"error 0: 数据为空!\0",
		"error 1: 矩阵不是方阵!\0",
		"error 2: 矩阵不是方阵，不能设置伴随矩阵!\0",
		"error 3: 矩阵不是方阵，不能设置逆矩阵!\0",
		"error 4: 矩阵不是方阵，不能进行次幂运算!\0",
		"error 5: 矩阵不是方阵，不能设置为单位矩阵!\0",
		"error 6: 矩阵不收敛!\0",
		"error 7: 矩阵没有实数特征值!\0",
		"error 8: 矩阵维度为0!\0",
		"error 9: 矩阵索引出界!\0",
		"error 10: 矩阵索引无效!\0",
		"error 11: 两个矩阵维度不一致!\0",
		"error 12: 两个矩阵维度不满足乘法条件!\0",
		"error 13: 矩阵维度不为1，不是向量!\0",
		"error 14: 参数违法!\0",
		"error 15: 计算逆矩阵失败!\0",
		"error 16: 行列式为0!\0",
		"error 17: 不支持三维操作!\0",
		"error 18: 指针为空!\0",
		"error 19: 输入矩阵维度必须为2D!\0",
		"error 20: 没有找到文件!\0",
		"error 21: 类型不符!\0",
		"error 22: 除数为0!\0",
		"error 23: 快速傅里叶变换矩阵尺寸必须是2的次幂!\0",
		"error 24: 矩阵通道数不符!\0",
		"error 25: 不支持的操作!\0"
	};
#else
	static const int8 *errinfo[] = {
		"error 0: data is empty!\0",
		"error 1: matrix is not a square array!\0",
		"error 2: matrix is not a square matrix, cannot set adjoint matrix!\0",
		"error 3: the matrix is not a square matrix, and the inverse matrix cannot be set!\0",
		"error 4: the matrix is not a square matrix, so it cannot be exponentially operated on!\0",
		"error 5: matrix is not a square matrix and cannot be set as a unit matrix!\0",
		"error 6: matrix does not converge!\0",
		"error 7: matrix does not have a real eigenvalue!\0",
		"error 8: matrix dimension is 0!\0",
		"error 9: matrix index out of bounds!\0",
		"error 10: invalid matrix index!\0",
		"error 11: the two matrix dimensions are not consistent!\0",
		"error 12: two matrix dimensions do not satisfy the multiplication condition!\0",
		"error 13: the matrix dimension is not 1, not a vector!\0",
		"error 14: illegal parameter!\0",
		"error 15: calculation of inverse matrix failed!\0",
		"error 16: the determinant is 0!\0",
		"error 17: 3D operation not supported!\0",
		"error 18: null pointer!\0",
		"error 19: input matrix dimension must be 2D!\0",
		"error 20: file not found!\0",
		"error 21: type mismatch!\0",
		"error 22: divisor is 0!\0",
		"error 23: the FFT matrix size must be a power of 2!\0",
		"error 24: number of matrix channels does not match!\0",
		"error 25: unsupported operation!\0"
	};
#endif
	/**
	SPECIAL_SOLUTION	方程有特解
	GENERAL_SOLUTION	方程有通解
	NO_SOLUTION			方程无解
	*/
	enum EQUATION
	{
		SPECIAL_SOLUTION = 0,	//方程有特解
		GENERAL_SOLUTION = 1,	//方程有通解
		NO_SOLUTION = -1		//方程无解
	};
	/**
	MIN_TO_MAX	从小到大
	MAX_TO_MIN	从大到小
	*/
	enum ORDER
	{
		MIN_TO_MAX = 0,
		MAX_TO_MIN = 1,
		DISTANCE = 3
	};
	/**
	ROW		行
	COL		列
	CHANNEL	通道
	*/
	enum RANK {
		ROW = 0,
		COL = 1,
		CHANNEL = 2
	};
	enum Dire {
		LEFT = 0,
		RIGHT = 1,
	};
	enum Axis {
		X = 0,
		Y = 1,
		Z = 2,
	};
	enum CompareType
	{
		LESS = -2,
		LESS_EQUAL = -1,
		EQUAL = 0,
		GREATER_EQUAL = 1,
		GREATER = 2
	};
	enum ColorGamut
	{
		ColorGamutShift = (1 << 5),
		ColorGamutMask = 0x1F,

		ColorGamutCustom = 0,
		ColorGamutGray,
		ColorGamutRGB,
		ColorGamutBGR,
		ColorGamutRGBA,
		ColorGamutBGRA,
		ColorGamutCMYK,
		ColorGamutHSV,
		ColorGamutHSL,
		ColorGamutXYZ,
		ColorGamutYXY,
		ColorGamutLab,
		ColorGamutLuv,
		ColorGamutMean,
		ColorGamutMaxMin,
		ColorGamutR,
		ColorGamutB,
		ColorGamutG
	};

	template<typename _Tp> TEMPLATE_API _Tp *alignPtr(_Tp *data, int32 n = (int32) sizeof(_Tp));
	// 将缓冲区大小与指定的字节数对齐
	//函数返回大于或等于sz且可被n整除的最小数
	//要对齐的sz缓冲区大小
	//n对齐大小必须是2的幂
	extern LZHAPI size_t alignSize(size_t sz, int32 n);
	extern LZHAPI void *fastMalloc(size_t size);
	extern LZHAPI void fastFree(void *data);
	template <typename _Tp, typename _T2> TEMPLATE_API void addList(_Tp *p, int32 &i, const _T2 & arg);
	template <typename _Tp, typename _T2, typename ... Types> TEMPLATE_API void addList(_Tp *p, int32 &i, const _T2 & arg1, const Types & ... args);

	extern LZHAPI uint32 calmemsize(int32 type);
	static const uint32 TypeMemSize[] = { 0,1,1,2,2,4,4,8,8,4,8,8 };
	static const int8 * TypeString[] = {
		"unknow",
		"uint8",	"int8",
		"uint16",	"int16",
		"uint32",	"int32",
		"uint64",	"int64",
		"float32",	"float64", "float64"
	};
	class LZHAPI Null {};
	template<class _Tp> class TEMPLATE_API DataType
	{
	public:
		typedef _Tp value_type;
		enum { type = NONE };
		const int8 * type_name = TypeString[type];
	};

	template<> class DataType<int8>
	{
	public:
		typedef int8 value_type;
		enum { type = TP_8S };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<uint8>
	{
	public:
		typedef uint8 value_type;
		enum { type = TP_8U };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<int16>
	{
	public:
		typedef int32 value_type;
		enum { type = TP_16S };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<uint16>
	{
	public:
		typedef int32 value_type;
		enum { type = TP_16U };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<int32>
	{
	public:
		typedef int32 value_type;
		enum { type = TP_32S };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<uint32>
	{
	public:
		typedef int32 value_type;
		enum { type = TP_32U };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<float32>
	{
	public:
		typedef float32 value_type;
		enum { type = TP_32F };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<float64>
	{
	public:
		typedef float64 value_type;
		enum { type = TP_64F };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<float128>
	{
	public:
		typedef float128 value_type;
		enum { type = TP_64D };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<int64>
	{
	public:
		typedef float64 value_type;
		enum { type = TP_64S };
		const int8 * type_name = TypeString[type];
	};
	template<> class DataType<uint64>
	{
	public:
		typedef float128 value_type;
		enum { type = TP_64U };
		const int8 * type_name = TypeString[type];
	};
//	template<> class DataType<msize>
//	{
//	public:
//		typedef msize value_type;
//#ifdef _WIN64
//		enum { type = TP_64U };
//#else
//		enum { type = TP_32U };
//#endif // _WIN64
//		const int8 * type_name = TypeString[type];
//	};


	class LZHAPI Handle
	{
	public:
		Handle();
		Handle(void* p);
		~Handle();

		operator void* ();
		operator const void* ()const;

		void* ptr;
	};

	class LZHAPI Value
	{
	public:
		Value();
		Value(int8 v);
		Value(uint8 v);
		Value(int16 v);
		Value(uint16 v);
		Value(int32 v);
		Value(uint32 v);
		Value(float32 v);
		Value(float64 v);
		Value(float128 v);
		Value(int64 v);
		Value(uint64 v);

		int type()const;
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

		int8 Char()const;
		uint8 UChar()const;
		int16 Int16()const;
		uint16 UInt16()const;
		int32 Int32()const;
		uint32 UInt32()const;
		int64 Int64()const;
		uint64 UInt64()const;
		float32 Float()const;
		float64 Double()const;
		float128 LongDouble()const;

		int8& toi8();
		int16& toi16();
		int32& toi32();
		int64& toi64();
		uint8& tou8();
		uint16& tou16();
		uint32& tou32();
		uint64& tou64();
		float32& tof32();
		float64& tof64();
		float128& tof128();

		std::string toString()const;
	protected:
		template<typename _Tp> _Tp toData() const;

		union Data
		{
			int8 i8;
			uint8 u8;
			int16 i16;
			uint16 u16;
			int32 i32;
			uint32 u32;
			int64 i64;
			uint64 u64;
			float32 f32;
			float64 f64;
			float128 f128;
		}data;
		int _type;
	};
	class LZHAPI Val_
	{
	public:
		explicit Val_() : v(0) {}
		Val_(int8 v) : v(_T(v)) {}
		Val_(uint8 v) : v(_T(v)) {}
		Val_(int16 v) : v(_T(v)) {}
		Val_(uint16 v) : v(_T(v)) {}
		Val_(int32 v) : v(_T(v)) {}
		Val_(uint32 v) : v(_T(v)) {}
		Val_(float32 v) : v(_T(v)) {}
		Val_(float64 v) : v(_T(v)) {}
		Val_(float128 v) : v(_T(v)) {}
		Val_(int64 v) : v(_T(v)) {}
		Val_(uint64 v) : v(_T(v)) {}

		operator mat_t&() { return v; }
		operator const mat_t&()const { return v; }
		mat_t& operator () () { return v; }
		const mat_t& operator ()()const { return v; }
		template<typename _Tp> _Tp get()const { return (_Tp)v; }

		mat_t v;
	};
	template<> class DataType<Val_>
	{
	public:
		typedef Val_ value_type;
		enum { type = MAT_T };
		const int8 * type_name = "Val_";
	};
	class LZHAPI Val : public Val_
	{
	public:
		explicit Val() : Val_(0) {}
		Val(Val_ v) : Val_(v), type(DataType<mat_t>::type) {}
		Val(const int8 v) : Val_(v), type(TP_8S) {}
		Val(const uint8 v) : Val_(v), type(TP_8U) {}
		Val(const int16 v) : Val_(v), type(TP_16S) {}
		Val(const uint16 v) : Val_(v), type(TP_16U) {}
		Val(const int32 v) : Val_(v), type(TP_32S) {}
		Val(const uint32 v) : Val_(v), type(TP_32U) {}
		Val(const int64 v) : Val_(v), type(TP_64S) {}
		Val(const uint64 v) : Val_(v), type(TP_64U) {}
		Val(const float32 v) : Val_(v), type(TP_32F) {}
		Val(const float64 v) : Val_(v), type(TP_64F) {}
		Val(const float128 v) : Val_(v), type(TP_64D) {}

		int32 type;
	};
	template<> class DataType<Val>
	{
	public:
		typedef Val value_type;
		enum { type = MAT_T };
		const int8 * type_name = "Val";
	}; 
	template<class _Tp> class TEMPLATE_API Range_
	{
	public:
		using value_type = typename _Tp;
		class Range_Iterator
		{
		public:
			explicit Range_Iterator(_Tp v, _Tp gap);

			Range_Iterator operator + (int32 off)const;
			Range_Iterator operator - (int32 off)const;

			Range_Iterator operator += (int32 off);
			Range_Iterator operator -= (int32 off);

			Range_Iterator operator -- ();
			Range_Iterator operator -- (int32);
			Range_Iterator operator ++ ();
			Range_Iterator operator ++ (int32);

			_Tp& operator * ();
			const _Tp& operator * ()const;

			_Tp& operator -> ();
			const _Tp& operator -> ()const;

			bool operator == (const Range_Iterator & iter)const;
			bool operator != (const Range_Iterator & iter)const;
			bool operator > (const Range_Iterator & iter)const;
			bool operator >= (const Range_Iterator & iter)const;
			bool operator <= (const Range_Iterator & iter)const;
			bool operator < (const Range_Iterator & iter)const;
		private:
			_Tp v;
			_Tp gap;
		};

		using iterator = typename Range_Iterator;
		using const_iterator = typename const Range_Iterator;

		explicit Range_() : v1(0), v2(0) {}
		Range_(const _Tp& v, bool equal = false) : v1(0), v2(v), gap_(1) { if (equal) v2 += gap_; }
		Range_(const _Tp& v1, const _Tp& v2, bool equal = false) : v1(v1), v2(v2), gap_(1) { if (equal) this->v2 += gap_; }
		Range_(const _Tp& v1, const _Tp& v2, const _Tp& gap_, bool equal = false) : v1(v1), v2(v2), gap_(gap_) { if (equal) this->v2 += gap_; }
		Range_<_Tp> gap(_Tp _gap)const noexcept { return Range_<_Tp>(v1, v2, _gap); }
		int32 len()const noexcept { return saturate_cast<int32>((v2 - v1) / _T(gap_)); }
		_Tp dv()const noexcept { return v2 - v1; }
		_Tp size()const noexcept { return v1 * v2; }
		iterator begin() noexcept { return iterator(v1, gap_); }
		const_iterator begin()const noexcept { return iterator(v1, gap_); }
		iterator end() noexcept { return iterator(v2, gap_); }
		const_iterator end()const noexcept { return iterator(v2, gap_); }
		std::vector<_Tp> toStdVec()const noexcept {
			std::vector<_Tp> vec(len()); int32 count = 0; for (_Tp i : *this)vec[count++] = i; return vec;
		}
		_Tp v1; _Tp v2; _Tp gap_;
	};
	using RangeB = typename Range_<int8>;
	using Range = typename Range_<int32>;
	using RangeM = typename Range_<msize>;
	using RangeF = typename Range_<float32>;
	using RangeD = typename Range_<float64>;
	template<class _Tp> class Point2_;
	template<class _Tp, int N> class Vec;
	template<class _Tp> class TEMPLATE_API Rect_
	{
	public:
		using value_type = typename _Tp;

		explicit Rect_() : x(0), y(0), width(0), height(0) {}
		Rect_(const _Tp x, const _Tp y, const _Tp width, const _Tp height) : x(x), y(y), width(width), height(height) {}
		Rect_(const Vec<_Tp, 4>& v) : x(v[0]), y(v[1]), width(v[2]), height(v[3]) {}
		_Tp area()const { return width * height; }
		Point2_<_Tp> topLeft() const noexcept { return Point2_<_Tp>(x, y); }
		Point2_<_Tp> bottomRight() const noexcept { return Point2_<_Tp>(x + width, y + height); }
		Point2_<_Tp> topRight() const noexcept { return Point2_<_Tp>(x + width, y); }
		Point2_<_Tp> bottomLeft() const noexcept { return Point2_<_Tp>(x, y + height); }
		Point2_<_Tp> center() const noexcept { return Point2_<_Tp>(x + width / 2, y + height / 2); }

		_Tp Top() const noexcept { return y; }
		_Tp Bottom() const noexcept { return y + height; }
		_Tp Left() const noexcept { return x; }
		_Tp Right() const noexcept { return x + width; }
		
		_Tp x; _Tp y; _Tp width; _Tp height;
	};
	typedef Rect_<int32> Rect;
	typedef Rect_<float32> RectF;
	template<class _Tp> class TEMPLATE_API Size_
	{
	public:
		using value_type = typename _Tp;

		explicit Size_() :h(0), w(0) {}
		Size_(const _Tp width, const _Tp height = 1) :h(height), w(width) {}
		_Tp area()const { return h * w; }
		constexpr bool operator == (Size_<_Tp> size)const noexcept { return (h == size.h && w == size.w); }
		constexpr bool operator != (Size_<_Tp> size)const noexcept { return !(*this == size); }
		constexpr bool operator > (Size_<_Tp> size)const noexcept { return area() > size(); }
		constexpr bool operator < (Size_<_Tp> size)const noexcept { return area() < size(); }
		constexpr bool operator >= (Size_<_Tp> size)const noexcept { return area() >= size(); }
		constexpr bool operator <= (Size_<_Tp> size)const noexcept { return area() <= size(); }
		operator _Tp () const noexcept { return area(); }
		_Tp operator ()() const noexcept { return area(); }

		Size_<_Tp> operator + (Val v) const noexcept { return Size_<_Tp>(w + v, h + v); }
		Size_<_Tp> operator - (Val v) const noexcept { return Size_<_Tp>(w - v, h - v); }
		Size_<_Tp> operator * (Val v) const noexcept { return Size_<_Tp>(w * v, h * v); }
		Size_<_Tp> operator / (Val v) const noexcept { return Size_<_Tp>(w / v, h / v); }

		Size_<_Tp> operator + (const Size_<_Tp> &size) const noexcept { return Size_<_Tp>(w + size.w, h + size.h); }
		Size_<_Tp> operator - (const Size_<_Tp> &size) const noexcept { return Size_<_Tp>(w - size.w, h - size.h); }
		Size_<_Tp> operator * (const Size_<_Tp> &size) const noexcept { return Size_<_Tp>(w * size.w, h * size.h); }
		Size_<_Tp> operator / (const Size_<_Tp> &size) const noexcept { return Size_<_Tp>(w / size.w, h / size.h); }
		
		_Tp h; _Tp w;
	};
	typedef Size_<int32> Size;
	typedef Size_<float32> Size2f;
	typedef Size_<float64> Size2d;
	template<class _Tp> class TEMPLATE_API Size3_
	{
	public:
		using value_type = typename _Tp;

		explicit Size3_() : h(0), w(0), c(0) {}
		Size3_(const _Tp w, const _Tp h, const _Tp c = 1) : h(h), w(w), c(c) {}
		Size3_(const Size_<_Tp> &size) : h(size.h), w(size.w), c(1) {}
		constexpr int32 area()const noexcept { return h * w * c; }
		constexpr bool operator == (Size3_<_Tp> size)const noexcept { return (h == size.h && w == size.w && c == size.c); }
		constexpr bool operator != (Size3_<_Tp> size)const noexcept { return !(*this == size); }
		constexpr bool operator > (Size3_<_Tp> size)const noexcept { return area() > size(); }
		constexpr bool operator < (Size3_<_Tp> size)const noexcept { return area() < size(); }
		constexpr bool operator >= (Size3_<_Tp> size)const noexcept { return area() >= size(); }
		constexpr bool operator <= (Size3_<_Tp> size)const noexcept { return area() <= size(); }
		operator _Tp () const noexcept { return area(); }
		_Tp operator ()() const noexcept { return area(); }

		_Tp h; _Tp w; _Tp c;
	};
	typedef Size3_<int32> Size3;
	typedef Size3_<float32> Size3f;
	class LZHAPI MatStep
	{
	public:
		explicit MatStep() { stride[0] = 0; stride[1] = 0; stride[2] = 0; }
		explicit MatStep(const int32 s) { stride[0] = s; stride[1] = 1; if (s % 4 == 0) stride[2] = s; else stride[2] = s + (4 - (s % 4));}
		explicit MatStep(const int32 s1, const int32 s2) { stride[0] = s1; stride[1] = s2;  if ((s1*s2) % 4 == 0) stride[2] = s1 * s2; else stride[2] = s1 * s2 + (4 - ((s1 * s2) % 4)); }
		explicit MatStep(const int32 s1, const int32 s2, const int32 s3) { stride[0] = s1; stride[1] = s2; stride[2] = s3;}
		int32& operator [](int32 i) { return stride[i]; }
		const int32& operator [](int32 i)const noexcept { return stride[i]; }
		int32* operator ()()const noexcept { return (int32*)&stride[0]; }
		operator int32()const noexcept { return size(); }
		int32 size()const noexcept { return stride[0] * stride[1]; }
		int32 stride[3];
	};
	class LZHAPI Complex
	{
	public:
		explicit Complex(const mat_t real = 0.0, const mat_t imaginary = 0.0) { r = real; i = imaginary; }
		void set(mat_t real, mat_t imaginary) noexcept { r = real; i = imaginary; }
		mat_t modulus()const noexcept { return sqrt(r*r + i * i); }
		// 以下为三种虚数运算的定义
		Complex operator + (const Complex & o)const noexcept { return Complex(r + o.r, i + o.i); }
		Complex operator - (const Complex & o)const noexcept { return Complex(r - o.r, i - o.i); }
		Complex operator * (const Complex & o)const noexcept { return Complex(r*o.r - i * o.i, r*o.i + i * o.r); }
		Complex operator + (mat_t val)const noexcept { return Complex(r + val, i + val); }
		Complex operator - (mat_t val)const noexcept { return Complex(r - val, i - val); }
		Complex operator * (mat_t val)const noexcept { return Complex(r * val, i * val); }
		Complex operator / (mat_t val)const noexcept { return Complex(r / val, i / val); }
		friend Complex operator + (mat_t val, Complex p) { return p + val; }
		friend Complex operator - (mat_t val, Complex p) { return Complex(val - p.r, val - p.i); }
		friend Complex operator * (mat_t val, Complex p) { return p * val; }
		friend Complex operator / (mat_t val, Complex p) { return Complex(val / p.r, val / p.i); }
		bool operator == (const Complex &o)const noexcept { return (r == o.r) && (i == o.i); }
		bool operator != (const Complex &o)const noexcept { return !((*this) == o); }
		mat_t r;
		mat_t i;
	};
	/**
	内存管理类
	*/
	template<class _Tp> class TEMPLATE_API MatPtr
	{
	public:
		using value_type = typename _Tp;

		explicit MatPtr();
		MatPtr(const int32 size);
		MatPtr(const MatPtr<_Tp> &m);
		MatPtr(const _Tp *p, int32 n);
		template <typename ... Types> MatPtr(int32 size, const Types & ... args);
		~MatPtr();
		void create(uint32 len);
		void addref();
		void release();
		_Tp* begin();
		const _Tp* begin()const;
		_Tp* end();
		const _Tp* end()const;
		_Tp& at(int32 i)const;
		bool operator == (void *m) const;
		bool operator != (void *m) const;
		_Tp& operator [] (int32 i) const;
		bool empty()const;
		int32 size()const;
		int32 &L();
		_Tp* &P();
		_Tp* obj()const;
		void set(const MatPtr<_Tp> &ptr);
		int32* getCount()const;
		void setCount(int32*refcount);
		MatPtr<_Tp> & operator=(const MatPtr<_Tp> & m);
		bool operator==(const MatPtr<_Tp> & m)const;
		bool operator!=(const MatPtr<_Tp> & m)const;
		operator _Tp*();
		operator const _Tp*()const;

		void set(const void* p, int32 len);
		//protected:
		int32 len;
		_Tp *data;
		void init(const _Tp* p, int32 len, const int32 *count);
	private:
		int32 *refcount = 0;
	};

	template<> class DataType<Complex>
	{
	public:
		typedef Complex value_type;
		enum { type = MAT_CREATETYPE(2) + DataType<mat_t>::type };
		const int8 * type_name = "Complex";
	};
	template<typename _Tp> class DataType<Size_<_Tp>>
	{
	public:
		typedef Size_<_Tp> value_type;
		enum { type = MAT_CREATETYPE(2) + DataType<_Tp>::type };
		const int8 * type_name = ("Size<" + std::string(DataType<_Tp>().type_name) + ">").c_str();
	};
	template<typename _Tp> class DataType<Rect_<_Tp>>
	{
	public:
		typedef Rect_<_Tp> value_type;
		enum { type = MAT_CREATETYPE(4) + DataType<_Tp>::type };
		const int8 * type_name = ("Rect<" + std::string(DataType<_Tp>().type_name) + ">").c_str();
	};

	/**
	设置随机数种子
	*/
	extern LZHAPI void Srandom();
	extern LZHAPI bool isFloat(int32 type);
	extern LZHAPI bool isInt(int32 type);
	extern LZHAPI bool isUnsigned(int32 type);
	extern LZHAPI bool isSigned(int32 type);
	extern LZHAPI bool isEqual(float32 a, float32 b, float32 epsilon = 1e-6);
	extern LZHAPI bool isEqual(float64 a, float64 b, float64 epsilon = 1e-6);
	extern LZHAPI bool isEqual(float128 a, float128 b, float128 epsilon = 1e-6);
	extern LZHAPI int32 mod(Val x, int32 val);
	extern LZHAPI int32 mod(float32 x, int32 val);
	extern LZHAPI int32 mod(float64 x, int32 val);
	extern LZHAPI int32 mod(float128 x, int32 val);
	extern LZHAPI mat_t rem(Val x, Val val);
	template<typename _Tp> TEMPLATE_API bool isType(int32 type);
	template<typename _Tp> TEMPLATE_API int32 Round(_Tp x) { return (int32)(x + (_Tp)0.5); }
	template<>extern LZHAPI int32 Round(float32 x);
	template<>extern LZHAPI int32 Round(float64 x);
	template<>extern LZHAPI int32 Round(float128 x);
	template<typename _Tp> TEMPLATE_API int32 Floor(_Tp x);	
}

#include "core_inl.hpp"
#endif // !__INTERFACE_H__
