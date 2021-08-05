#define LZH_EXPORTS
#include "core.h"
#include <time.h>
//#include <strstream>

using namespace lzh;

size_t lzh::alignSize(size_t sz, int32 n) {
	return (sz + n - 1) & -n;
}

void* lzh::fastMalloc(size_t size) {
	uint8* udata = (uint8*)malloc(size + sizeof(void*) + MALLOC_ALIGN);
	if (!udata)
		return 0;
	uint8** adata = alignPtr((uint8 * *)udata + 1, MALLOC_ALIGN);
	adata[-1] = udata;
	return adata;
}

void lzh::fastFree(void* data) {
	if (data) {
		uint8* udata = ((uint8 * *)data)[-1];
		free(udata);
	}
}

uint32 lzh::calmemsize(int32 type)
{
	switch (type) {
	case lzh::TP_8S:	return sizeof(int8);
	case lzh::TP_8U:	return sizeof(uint8);
	case lzh::TP_16S:	return sizeof(int16);
	case lzh::TP_16U:	return sizeof(uint16);
	case lzh::TP_32S:	return sizeof(int32);
	case lzh::TP_32U:	return sizeof(uint32);
	case lzh::TP_64S:	return sizeof(int64);
	case lzh::TP_64U:	return sizeof(uint64);
	case lzh::TP_32F:	return sizeof(float32);
	case lzh::TP_64F:	return sizeof(float64);
	case lzh::TP_64D:	return sizeof(float128);
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
	return 0;
}

Value::Value() : _type(NONE) { }
Value::Value(int8 v) : _type(DataType<int8>::type) { data.i8 = v; }
Value::Value(uint8 v) : _type(DataType<uint8>::type) { data.u8 = v; }
Value::Value(int16 v) : _type(DataType<int16>::type) { data.i16 = v; }
Value::Value(uint16 v) : _type(DataType<uint16>::type) { data.u16 = v; }
Value::Value(int32 v) : _type(DataType<int32>::type) { data.i32 = v; }
Value::Value(uint32 v) : _type(DataType<uint32>::type) { data.u32 = v; }
Value::Value(float32 v) : _type(DataType<float32>::type) { data.f32 = v; }
Value::Value(float64 v) : _type(DataType<float64>::type) { data.f64 = v; }
Value::Value(float128 v) : _type(DataType<float128>::type) { data.f128 = v; }
Value::Value(int64 v) : _type(DataType<int64>::type) { data.i64 = v; }
Value::Value(uint64 v) : _type(DataType<uint64>::type) { data.u64 = v; }

int lzh::Value::type() const
{
	return _type;
}

bool lzh::Value::isi8() const
{
	return DataType<int8>::type == _type;
}

bool lzh::Value::isi16() const
{
	return DataType<int16>::type == _type;
}

bool lzh::Value::isi32() const
{
	return DataType<int32>::type == _type;
}

bool lzh::Value::isi64() const
{
	return DataType<int64>::type == _type;
}

bool lzh::Value::isu8() const
{
	return DataType<uint8>::type == _type;
}

bool lzh::Value::isu16() const
{
	return DataType<uint16>::type == _type;
}

bool lzh::Value::isu32() const
{
	return DataType<uint32>::type == _type;
}

bool lzh::Value::isu64() const
{
	return DataType<uint64>::type == _type;
}

bool lzh::Value::isf32() const
{
	return DataType<float32>::type == _type;
}

bool lzh::Value::isf64() const
{
	return DataType<float64>::type == _type;
}

bool lzh::Value::isf128() const
{
	return DataType<float128>::type == _type;
}

int8 lzh::Value::Char() const
{
	return toData<int8>();
}

uint8 lzh::Value::UChar() const
{
	return toData<uint8>();
}

int16 lzh::Value::Int16() const
{
	return toData<int16>();
}

uint16 lzh::Value::UInt16() const
{
	return toData<uint16>();
}

int32 lzh::Value::Int32() const
{
	return toData<int32>();
}

uint32 lzh::Value::UInt32() const
{
	return toData<uint32>();
}

int64 lzh::Value::Int64() const
{
	return toData<int64>();
}

uint64 lzh::Value::UInt64() const
{
	return toData<uint64>();
}

float32 lzh::Value::Float() const
{
	return toData<float32>();
}

float64 lzh::Value::Double() const
{
	return toData<float64>();
}

float128 lzh::Value::LongDouble() const
{
	return toData<float128>();
}

int8 & lzh::Value::toi8()
{
	LZH_ACCESS(if (_type == DataType<int8>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.i8;
}

int16 & lzh::Value::toi16()
{
	LZH_ACCESS(if (_type == DataType<int16>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.i16;
}

int32 & lzh::Value::toi32()
{
	LZH_ACCESS(if (_type == DataType<int32>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.i32;
}

int64 & lzh::Value::toi64()
{
	LZH_ACCESS(if (_type == DataType<int64>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.i64;
}

uint8 & lzh::Value::tou8()
{
	LZH_ACCESS(if (_type == DataType<uint8>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.u8;
}

uint16 & lzh::Value::tou16()
{
	LZH_ACCESS(if (_type == DataType<uint16>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.u16;
}

uint32 & lzh::Value::tou32()
{
	LZH_ACCESS(if (_type == DataType<uint32>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.u32;
}

uint64 & lzh::Value::tou64()
{
	LZH_ACCESS(if (_type == DataType<uint64>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.u64;
}

float32 & lzh::Value::tof32()
{
	LZH_ACCESS(if (_type == DataType<float32>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.f32;
}

float64 & lzh::Value::tof64()
{
	LZH_ACCESS(if (_type == DataType<float64>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.f64;
}

float128 & lzh::Value::tof128()
{
	LZH_ACCESS(if (_type == DataType<float128>::type), THROW_INFO(ERR_INFO_TYPE));
	return data.f128;
}

std::string lzh::Value::toString() const
{
	switch (_type)
	{
	case lzh::TP_8U:return std::to_string(data.u8);
	case lzh::TP_8S:return std::to_string(data.i8);
	case lzh::TP_16U:return std::to_string(data.u16);
	case lzh::TP_16S:return std::to_string(data.i16);
	case lzh::TP_32U:return std::to_string(data.u32);
	case lzh::TP_32S:return std::to_string(data.i32);
	case lzh::TP_64U:return std::to_string(data.u64);
	case lzh::TP_64S:return std::to_string(data.i64);
	case lzh::TP_32F:return std::to_string(data.f32);
	case lzh::TP_64F:return std::to_string(data.f64);
	case lzh::TP_64D:return std::to_string(data.f128);
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}


/****************************************************************************
随机数
*****************************************************************************/
void lzh::Srandom()
{
	srand(uint32(time(NULL)));
}
bool lzh::isFloat(int32 type)
{
	return type == MAT_32F || type == MAT_64F || type == MAT_64D;
}
bool lzh::isInt(int32 type)
{
	return !isFloat(type);
}
bool lzh::isUnsigned(int32 type)
{
	switch (type)
	{
	case lzh::TP_8U:
	case lzh::TP_16U:
	case lzh::TP_32U:
	case lzh::TP_64U:return true;
	default:break;
	}
	return false;
}
bool lzh::isSigned(int32 type)
{
	return !isUnsigned(type);
}
bool lzh::isEqual(float32 a, float32 b, float32 epsilon)
{
	return abs(a - b) < epsilon;
}
bool lzh::isEqual(float64 a, float64 b, float64 epsilon)
{
	return abs(a - b) < epsilon;
}
bool lzh::isEqual(float128 a, float128 b, float128 epsilon)
{
	return abs(a - b) < epsilon;
}
int32 lzh::mod(Val x, int32 val)
{
	return Round(x) % val;
}
int32 lzh::mod(float32 x, int32 val)
{
	return Round(x) % val;
}
int32 lzh::mod(float64 x, int32 val)
{
	return Round(x) % val;
}
int32 lzh::mod(float128 x, int32 val)
{
	return Round(x) % val;
}
mat_t lzh::rem(Val x, Val val)
{
	return x - (Floor(x / val) * val);
}
template<>int32 lzh::Round(float32 x)
{
	return (int32)(x + 0.5f);
}
template<>int32 lzh::Round(float64 x)
{
	return (int32)(x + 0.5);
}
template<>int32 lzh::Round(float128 x)
{
	return (int32)(x + 0.5);
}

lzh::Handle::Handle()
{
	ptr = nullptr;
}

lzh::Handle::Handle(void* p)
{
	ptr = p;
}

lzh::Handle::~Handle()
{
	ptr = nullptr;
}

lzh::Handle::operator void* ()
{
	return ptr;
}

lzh::Handle::operator const void* () const
{
	return ptr;
}
