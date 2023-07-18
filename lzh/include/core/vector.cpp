#define LZH_EXPORTS
#include "vector.h"
#include <iostream>
#include <strstream>
using namespace lzh;

/****************************************************************************
½Ó¿ÚÀà
*****************************************************************************/
Vector::Vector()
{
	init(0, 0, 0, 0);
}
Vector::Vector(msize size, int32 type)
{
	init(0, 0, 0, 0);
	create(size, type);
}
Vector::Vector(const Vector& m)
{
	init(m.data, m.type, m.len, m.refcount);
	if (refcount)
		MAT_XADD(refcount, 1);
}
Vector::Vector(msize n, const void* p, int32 type, bool deepcopy)
{
	if (!deepcopy)init(p, type, n, 0);
	else {
		create(n, type);
		memcpy(data, p, calmemsize(type)*n);
	}
}
Vector::Vector(msize n, const void * p, int32 type, int32 * refcount)
{
	init(p, type, n, refcount);
}
Vector::~Vector() { release(); }
void Vector::create(msize len, int32 type) {
	release();
	this->len = len;
	this->type = type;
	msize totalsize = len * calmemsize(type);
	data = (void*)fastMalloc(totalsize + sizeof(int32));
	refcount = (int32*)(((uint8*)data) + totalsize);
	*refcount = 1;
}
void Vector::addref() {
	if (refcount)
		MAT_XADD(refcount, 1);
}
void Vector::release() {
	if (refcount && MAT_XADD(refcount, -1) == 1) {
		if (uint64(refcount) - uint64(data) > calmemsize(type)*len) {
			FREE_ARRAY(data);
			FREE_PTR(refcount);
		}
		else
			fastFree(data);
	}
	init(0, 0, 0, 0);
}
Vector lzh::Vector::clone() const
{
	return Vector(len, data, type, true);
}
Vector lzh::Vector::copy() const
{
	return clone();
}
void lzh::Vector::save(FILE * file)const
{
	if (file) {
		fwrite(&type, sizeof(int32), 1, file);
		fwrite(&len, sizeof(msize), 1, file);
		fwrite(data, calmemsize(type) * len, 1, file);
	}
	else {
		THROW_INFO(ERR_INFO_FILE);
	}
}
void lzh::Vector::load(FILE * file)
{
	if (file) {
		fread(&type, sizeof(int32), 1, file);
		fread(&len, sizeof(msize), 1, file);
		create(len, type);
		fread(data, calmemsize(type) * len, 1, file);
	}
	else {
		THROW_INFO(ERR_INFO_FILE);
	}
}
bool Vector::operator==(void* m) const {
	return (this->data == m);
}
bool Vector::operator!=(void* m) const {
	return (this->data != m);
}
Vector& Vector::operator=(const Vector& m) {
	if (this == &m)
		return *this;

	if (m.refcount)
		MAT_XADD(m.refcount, 1);

	release();
	data = m.data;
	len = m.len;
	type = m.type;
	refcount = m.refcount;
	return *this;
}
void* Vector::operator()()
{
	return data;
}
const void* lzh::Vector::operator()() const
{
	return data;
}
void lzh::Vector::show() const
{
	std::ostrstream out;
	show(out); out << std::ends;
	std::cout << out.str() << std::endl;
	delete out.str();
}
std::ostream & lzh::Vector::show(std::ostream & out) const
{
	out << "Vector([";
	for (msize i = 0; i < len; i++) {
		switch (depth())
		{
		case lzh::TP_8S:	out << (int16)at<int8>(i); break;
		case lzh::TP_8U:	out << (int16)at<uint8>(i); break;
		case lzh::TP_16S:	out << at<int16>(i); break;
		case lzh::TP_16U:	out << at<uint16>(i); break;
		case lzh::TP_32S:	out << at<int32>(i); break;
		case lzh::TP_32U:	out << at<uint32>(i); break;
		case lzh::TP_64S:	out << at<int64>(i); break;
		case lzh::TP_64U:	out << at<uint64>(i); break;
		case lzh::TP_32F:	out << at<float32>(i); break;
		case lzh::TP_64F:	out << at<float64>(i); break;
		case lzh::TP_64D:	out << at<float128>(i); break;
		default:
			break;
		}
		if (i != len - 1)out << ", ";
	}
	out << "], type=" << TypeString[depth()] << ")";
	return out;
}
//void* Vector::operator+(int32 i) const
//{
//	return offset(i);
//}
//void* Vector::operator-(int32 i) const
//{
//	return reduce(i);
//}
lzh::Value lzh::Vector::val(msize i) const
{
	switch (depth())
	{
	case lzh::TP_8S:	return Value(at<int8>(i));
	case lzh::TP_8U:	return Value(at<uint8>(i));
	case lzh::TP_16S:	return Value(at<int16>(i));
	case lzh::TP_16U:	return Value(at<uint16>(i));
	case lzh::TP_32S:	return Value(at<int32>(i));
	case lzh::TP_32U:	return Value(at<uint32>(i));
	case lzh::TP_64S:	return Value(at<int64>(i));
	case lzh::TP_64U:	return Value(at<uint64>(i));
	case lzh::TP_32F:	return Value(at<float32>(i));
	case lzh::TP_64F:	return Value(at<float64>(i));
	case lzh::TP_64D:	return Value(at<float128>(i));
	default:THROW_INFO(ERR_INFO_SUPPORT);
	}
}
mat_t lzh::Vector::operator()(msize i) const
{
	return get<mat_t>(i);
}
bool Vector::empty() const
{
	return data == nullptr;
}
int32 Vector::depth() const {
	return MAT_GETVALUE(type);
}
int32 Vector::getTp() const
{
	return type;
}
msize Vector::size() const {
	return len;
}
int32& Vector::T() {
	return type;
}
msize& Vector::L() {
	return len;
}
void*& Vector::P() {
	return data;
}
void* Vector::obj()const {
	return (void*)data;
}
void* Vector::index(msize i) const
{
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	switch (depth()) {
	case lzh::TP_8S:	return MAT_INDEX(int8, data, i);
	case lzh::TP_8U:	return MAT_INDEX(uint8, data, i);
	case lzh::TP_16S:	return MAT_INDEX(int16, data, i);
	case lzh::TP_16U:	return MAT_INDEX(uint16, data, i);
	case lzh::TP_32S:	return MAT_INDEX(int32, data, i);
	case lzh::TP_32U:	return MAT_INDEX(uint32, data, i);
	case lzh::TP_64S:	return MAT_INDEX(int64, data, i);
	case lzh::TP_64U:	return MAT_INDEX(uint64, data, i);
	case lzh::TP_32F:	return MAT_INDEX(float32, data, i);
	case lzh::TP_64F:	return MAT_INDEX(float64, data, i);
	case lzh::TP_64D:	return MAT_INDEX(float128, data, i);
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
}
void* Vector::add(msize i) const
{
	return index(i);
}
void* Vector::reduce(msize i) const
{
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	switch (depth()) {
	case lzh::TP_8S:	return MAT_GETPTR(int8, data) - i;
	case lzh::TP_8U:	return MAT_GETPTR(uint8, data) - i;
	case lzh::TP_16S:	return MAT_GETPTR(int16, data) - i;
	case lzh::TP_16U:	return MAT_GETPTR(uint16, data) - i;
	case lzh::TP_32S:	return MAT_GETPTR(int32, data) - i;
	case lzh::TP_32U:	return MAT_GETPTR(uint32, data) - i;
	case lzh::TP_64S:	return MAT_GETPTR(int64, data) - i;
	case lzh::TP_64U:	return MAT_GETPTR(uint64, data) - i;
	case lzh::TP_32F:	return MAT_GETPTR(float32, data) - i;
	case lzh::TP_64F:	return MAT_GETPTR(float64, data) - i;
	case lzh::TP_64D:	return MAT_GETPTR(float128, data) - i;
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
}
void* Vector::offset(msize i) const {
	return add(i);
}
int32* lzh::Vector::memCount() const
{
	return refcount;
}
void Vector::set(const void* p, msize length, int32 data_type) {
	data = (void*)p; type = data_type; len = length;
}
void lzh::Vector::set(const void* p, msize length, int32 data_type, const int32* count) {
	data = (void*)p; type = data_type; len = length; refcount = (int32*)count;
}
void Vector::init(const void* p, int32 type, msize len, const int32* count) {
	data = (void*)p; this->type = type; this->len = len; refcount = (int32*)count;
}
void Vector::set(const Vector& ptr) {
	data = ptr.data; type = ptr.type; len = ptr.len;
}

Vector lzh::operator+(Val v, const Vector & vec)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, vec(i) + v);
	return vec_;
}

Vector lzh::operator+(const Vector & vec, Val v)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, vec(i) + v);
	return vec_;
}

Vector lzh::operator+(const Vector & v1, const Vector & v2)
{
	if (v1.empty() || v2.empty())return Vector();
	if (v1.size() != v2.size())return Vector();
	Vector vec(v1.size(), v1.depth());
	FOR_RANGE(i, 0, v1.size())vec.setv(i, v1(i) + v2(i));
	return vec;
}

Vector lzh::operator-(Val v, const Vector & vec)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, v - vec(i));
	return vec_;
}

Vector lzh::operator-(const Vector & vec, Val v)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, vec(i) - v);
	return vec_;
}

Vector lzh::operator-(const Vector & v1, const Vector & v2)
{
	if (v1.empty() || v2.empty())return Vector();
	if (v1.size() != v2.size())return Vector();
	Vector vec(v1.size(), v1.depth());
	FOR_RANGE(i, 0, v1.size())vec.setv(i, v1(i) - v2(i));
	return vec;
}

Vector lzh::operator*(Val v, const Vector & vec)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, v * vec(i));
	return vec_;
}

Vector lzh::operator*(const Vector & vec, Val v)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, vec(i) * v);
	return vec_;
}

Vector lzh::operator*(const Vector & v1, const Vector & v2)
{
	if (v1.empty() || v2.empty())return Vector();
	if (v1.size() != v2.size())return Vector();
	Vector vec(v1.size(), v1.depth());
	FOR_RANGE(i, 0, v1.size())vec.setv(i, v1(i) * v2(i));
	return vec;
}

Vector lzh::operator/(Val v, const Vector & vec)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, v / vec(i));
	return vec_;
}

Vector lzh::operator/(const Vector & vec, Val v)
{
	if (vec.empty())return Vector();
	Vector vec_(vec.size(), vec.depth());
	FOR_RANGE(i, 0, vec.size())vec_.setv(i, vec(i) / v);
	return vec_;
}

Vector lzh::operator/(const Vector & v1, const Vector & v2)
{
	if (v1.empty() || v2.empty())return Vector();
	if (v1.size() != v2.size())return Vector();
	Vector vec(v1.size(), v1.depth());
	FOR_RANGE(i, 0, v1.size())vec.setv(i, v1(i) / v2(i));
	return vec;
}

Vector & lzh::operator+=(Vector & v1, Val v)
{
	if (!v1.empty()) FOR_RANGE(i, 0, v1.size())v1.add(i, v);
	return v1;
}

Vector & lzh::operator+=(Vector & v1, const Vector & v2)
{
	if (!v1.empty() && !v2.empty() && v1.size() == v2.size())
		FOR_RANGE(i, 0, v1.size())v1.add(i, v2(i));
	return v1;
}

Vector & lzh::operator-=(Vector & v1, Val v)
{
	if (!v1.empty()) FOR_RANGE(i, 0, v1.size())v1.sub(i, v);
	return v1;
}

Vector & lzh::operator-=(Vector & v1, const Vector & v2)
{
	if (!v1.empty() && !v2.empty() && v1.size() == v2.size())
		FOR_RANGE(i, 0, v1.size())v1.sub(i, v2(i));
	return v1;
}

Vector & lzh::operator*=(Vector & v1, Val v)
{
	if (!v1.empty()) FOR_RANGE(i, 0, v1.size())v1.mul(i, v);
	return v1;
}

Vector & lzh::operator*=(Vector & v1, const Vector & v2)
{
	if (!v1.empty() && !v2.empty() && v1.size() == v2.size())
		FOR_RANGE(i, 0, v1.size())v1.mul(i, v2(i));
	return v1;
}

Vector & lzh::operator/=(Vector & v1, Val v)
{
	if (!v1.empty()) FOR_RANGE(i, 0, v1.size())v1.div(i, v);
	return v1;
}

Vector & lzh::operator/=(Vector & v1, const Vector & v2)
{
	if (!v1.empty() && !v2.empty() && v1.size() == v2.size())
		FOR_RANGE(i, 0, v1.size())v1.div(i, v2(i));
	return v1;
}
