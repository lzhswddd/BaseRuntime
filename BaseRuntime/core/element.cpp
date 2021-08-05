#define LZH_EXPORTS
#include "element.h"
#include "data_stream.h"
#include <iostream>

using namespace lzh;
using namespace std;

lzh::ElemIter::ElemIter(const Element & container, bool b_e)
{
	type = container.type();
	if (type == Element::STD_VECTOR)
		_iter = new std::vector<Element>::iterator(b_e ? container.toList().begin() : container.toList().end());
	else if (type == Element::STD_MAP)
		_iter = new std::map<Element, Element>::iterator(b_e ? container.toDict().begin() : container.toDict().end());
}

lzh::ElemIter::ElemIter(const ElemIter & p)
	:type(Element::InValid), _iter(nullptr)
{
	*this = p;
}

lzh::ElemIter::~ElemIter()
{
	if (type == Element::STD_VECTOR)
		FREE_PTR_NONULL((std::vector<Element>::iterator*)_iter);
	else if (type == Element::STD_MAP)
		FREE_PTR_NONULL((std::map<Element, Element>::iterator*)_iter);
}

ElemIter & lzh::ElemIter::operator=(const ElemIter & p)
{
	if (this == &p)return *this;
	if (type == Element::STD_VECTOR)
		FREE_PTR_NONULL((std::vector<Element>::iterator*)_iter);
	else if (type == Element::STD_MAP)
		FREE_PTR_NONULL((std::map<Element, Element>::iterator*)_iter);
	this->type = p.type;
	if (p._iter) {
		if (type == Element::STD_VECTOR)
			_iter = new std::vector<Element>::iterator();
		else if (type == Element::STD_MAP)
			_iter = new std::map<Element, Element>::iterator();
	}
	else _iter = nullptr;
	return *this;
}

size_t lzh::ElemIter::operator-(const ElemIter & iter) const
{
	if (type != iter.type)return -1;
	if (type == Element::STD_VECTOR)
		return *((std::vector<Element>::iterator*)_iter) - *((std::vector<Element>::iterator*)iter._iter);
	else return -1;
}

bool lzh::ElemIter::operator!=(const ElemIter & iter) const
{
	if (type != iter.type)return false;
	if (type == Element::STD_VECTOR)
		return *((std::vector<Element>::iterator*)_iter) != *((std::vector<Element>::iterator*)iter._iter);
	else if (type == Element::STD_MAP)
		return *((std::map<Element, Element>::iterator*)_iter) != *((std::map<Element, Element>::iterator*)iter._iter);
	else return false;
}

bool lzh::ElemIter::operator==(const ElemIter & iter) const
{
	if (type != iter.type)return false;
	if (type == Element::STD_VECTOR)
		return *((std::vector<Element>::iterator*)_iter) == *((std::vector<Element>::iterator*)iter._iter);
	else if (type == Element::STD_MAP)
		return *((std::map<Element, Element>::iterator*)_iter) == *((std::map<Element, Element>::iterator*)iter._iter);
	else return false;
}

bool lzh::ElemIter::operator>(const ElemIter & iter)
{
	if (type != iter.type)return false;
	if (type == Element::STD_VECTOR)
		return *((std::vector<Element>::iterator*)_iter) > *((std::vector<Element>::iterator*)iter._iter);
	else return false;
}

bool lzh::ElemIter::operator<(const ElemIter & iter)
{
	if (type != iter.type)return false;
	if (type == Element::STD_VECTOR)
		return *((std::vector<Element>::iterator*)_iter) < *((std::vector<Element>::iterator*)iter._iter);
	else return false;
}

ElemIter lzh::ElemIter::operator++()
{
	if (type == Element::STD_VECTOR)
		++*((std::vector<Element>::iterator*)_iter);
	else if (type == Element::STD_MAP)
		++*((std::map<Element, Element>::iterator*)_iter);
	return *this;
}

ElemIter lzh::ElemIter::operator++(int32)
{
	ElemIter iter = *this;
	if (type == Element::STD_VECTOR)
		(*((std::vector<Element>::iterator*)_iter))++;
	else if (type == Element::STD_MAP)
		(*((std::map<Element, Element>::iterator*)_iter))++;
	return iter;
}

ElemIter lzh::ElemIter::operator+(int32 count)
{
	ElemIter iter = *this;
	if (type == Element::STD_VECTOR)
		*((std::vector<Element>::iterator*)_iter) = *((std::vector<Element>::iterator*)_iter) + count;
	return iter;
}

ElemIter lzh::ElemIter::operator+=(int32 count)
{
	if (type == Element::STD_VECTOR)*((std::vector<Element>::iterator*)_iter) += count;
	return *this;
}

ElemIter lzh::ElemIter::operator--()
{
	if (type == Element::STD_VECTOR)
		--*((std::vector<Element>::iterator*)_iter);
	else if (type == Element::STD_MAP)
		--*((std::map<Element, Element>::iterator*)_iter);
	return *this;
}

ElemIter lzh::ElemIter::operator--(int32)
{
	ElemIter iter = *this;
	if (type == Element::STD_VECTOR)
		(*((std::vector<Element>::iterator*)_iter))--;
	else if (type == Element::STD_MAP)
		(*((std::map<Element, Element>::iterator*)_iter))--;
	return iter;
}

ElemIter lzh::ElemIter::operator-(int32 count)
{
	ElemIter iter = *this;
	if (type == Element::STD_VECTOR)
		*((std::vector<Element>::iterator*)_iter) = *((std::vector<Element>::iterator*)_iter) - count;
	return iter;
}

ElemIter lzh::ElemIter::operator-=(int32 count)
{
	if (type == Element::STD_VECTOR)*((std::vector<Element>::iterator*)_iter) -= count;
	return *this;
}

Element & lzh::ElemIter::operator*()
{
	if (type == Element::STD_VECTOR)
		return **((std::vector<Element>::iterator*)_iter);
	else THROW_INFO(ERR_INFO_SUPPORT);
}

const Element & lzh::ElemIter::key() const
{
	if (type == Element::STD_MAP)
		return (*((std::map<Element, Element>::iterator*)_iter))->first;
	else THROW_INFO(ERR_INFO_SUPPORT);
}

Element & lzh::ElemIter::value()
{
	if (type == Element::STD_MAP)
		return (*((std::map<Element, Element>::iterator*)_iter))->second;
	else THROW_INFO(ERR_INFO_SUPPORT);
}

lzh::Element::Element() : _type(InValid), _typeName("Null")
{
	data_ptr = ptr.user = nullptr;
}
lzh::Element::Element(std::nullptr_t v) : _type(InValid), _typeName(typeid(v).name())
{
	data_ptr = ptr.user = v;
}
lzh::Element::Element(void * v) : _type(INTPTR), _typeName(typeid(v).name())
{
	data_ptr = ptr.user = v;
}
lzh::Element::Element(const void * v) : _type(INTPTR), _typeName(typeid(v).name())
{
	data_ptr = ptr.user = (void*)v;
}
lzh::Element::Element(const bool & v) : _type(ElemType<bool>::type), _typeName(typeid(v).name())
{
	ptr.b = v; data_ptr = &ptr.b;
}
lzh::Element::Element(const int8 & v) : _type(ElemType<int8>::type), _typeName(typeid(v).name())
{
	ptr.i8 = v; data_ptr = &ptr.i8;
}
lzh::Element::Element(const int16 & v) : _type(ElemType<int16>::type), _typeName(typeid(v).name())
{
	ptr.i16 = v; data_ptr = &ptr.i16;
}
lzh::Element::Element(const int32 & v) : _type(ElemType<int32>::type), _typeName(typeid(v).name())
{
	ptr.i32 = v; data_ptr = &ptr.i32;
}
lzh::Element::Element(const int64 & v) : _type(ElemType<int64>::type), _typeName(typeid(v).name())
{
	ptr.i64 = v; data_ptr = &ptr.i64;
}
lzh::Element::Element(const uint8 & v) : _type(ElemType<uint8>::type), _typeName(typeid(v).name())
{
	ptr.u8 = v; data_ptr = &ptr.u8;
}
lzh::Element::Element(const uint16 & v) : _type(ElemType<uint16>::type), _typeName(typeid(v).name())
{
	ptr.u16 = v; data_ptr = &ptr.u16;
}
lzh::Element::Element(const uint32 & v) : _type(ElemType<uint32>::type), _typeName(typeid(v).name())
{
	ptr.u32 = v; data_ptr = &ptr.u32;
}
lzh::Element::Element(const uint64 & v) : _type(ElemType<uint64>::type), _typeName(typeid(v).name())
{
	ptr.u64 = v; data_ptr = &ptr.u64;
}
lzh::Element::Element(const float32 & v) : _type(ElemType<float32>::type), _typeName(typeid(v).name())
{
	ptr.f32 = v; data_ptr = &ptr.f32;
}
lzh::Element::Element(const float64 & v) : _type(ElemType<float64>::type), _typeName(typeid(v).name())
{
	ptr.f64 = v; data_ptr = &ptr.f64;
}
lzh::Element::Element(const float128 & v) : _type(ElemType<float128>::type), _typeName(typeid(v).name())
{
	ptr.f128 = v; data_ptr = &ptr.f128;
}
lzh::Element::Element(int8 * v, bool isStr) : _type(InValid), _typeName(typeid(v).name())
{
	data_ptr = ptr.i8_p = nullptr;
	if (isStr)
		*this = string(v);
	else {
		data_ptr = ptr.i8_p = v;
		_type = ElemType<int8*>::type;
	}
}
lzh::Element::Element(const int8 * v, bool isStr) : _type(InValid), _typeName(typeid(v).name())
{
	data_ptr = ptr.i8_p = nullptr;
	if (isStr)
		*this = string(v);
	else {
		data_ptr = ptr.i8_p = (int8*)v;
		_type = ElemType<int8*>::type;
	}
}
lzh::Element::Element(int16 * v) : _type(ElemType<int16*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.i16_p = v;
}
lzh::Element::Element(const int16 * v) : _type(ElemType<int16*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.i16_p = (int16*)v;
}
lzh::Element::Element(int32 * v) : _type(ElemType<int32*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.i32_p = v;
}
lzh::Element::Element(const int32 * v) : _type(ElemType<int32*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.i32_p = (int32*)v;
}
lzh::Element::Element(int64 * v) : _type(ElemType<int64*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.i64_p = v;
}
lzh::Element::Element(const int64 * v) : _type(ElemType<int64*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.i64_p = (int64*)v;
}
lzh::Element::Element(uint8 * v) : _type(ElemType<uint8*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u8_p = v;
}
lzh::Element::Element(const uint8 * v) : _type(ElemType<uint8*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u8_p = (uint8*)v;
}
lzh::Element::Element(uint16 * v) : _type(ElemType<uint16*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u16_p = v;
}
lzh::Element::Element(const uint16 * v) : _type(ElemType<uint16*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u16_p = (uint16*)v;
}
lzh::Element::Element(uint32 * v) : _type(ElemType<uint32*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u32_p = v;
}
lzh::Element::Element(const uint32 * v) : _type(ElemType<uint32*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u32_p = (uint32*)v;
}
lzh::Element::Element(uint64 * v) : _type(ElemType<uint64*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u64_p = v;
}
lzh::Element::Element(const uint64 * v) : _type(ElemType<uint64*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.u64_p = (uint64*)v;
}
lzh::Element::Element(float32 * v) : _type(ElemType<float32*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.f32_p = v;
}
lzh::Element::Element(const float32 * v) : _type(ElemType<float32*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.f32_p = (float32*)v;
}
lzh::Element::Element(float64 * v) : _type(ElemType<float64*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.f64_p = v;
}
lzh::Element::Element(const float64 * v) : _type(ElemType<float64*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.f64_p = (float64*)v;
}
lzh::Element::Element(float128 * v) : _type(ElemType<float128*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.f128_p = v;
}
lzh::Element::Element(const float128 * v) : _type(ElemType<float128*>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.f128_p = (float128*)v;
}
lzh::Element::Element(const Range & v) : _type(ElemType<Range>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.ra = new Range(v);
}
lzh::Element::Element(const Rect & v) : _type(ElemType<Rect>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.r = new Rect(v);
}
lzh::Element::Element(const RectF & v) : _type(ElemType<RectF>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.rf = new RectF(v);
}
lzh::Element::Element(const Size & v) : _type(ElemType<Size>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.s2 = new Size(v);
}
lzh::Element::Element(const Size2f & v) : _type(ElemType<Size2f>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.s2f = new Size2f(v);
}
lzh::Element::Element(const Size3 & v) : _type(ElemType<Size3>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.s3i = new Size3(v);
}
lzh::Element::Element(const Size3f & v) : _type(ElemType<Size3f>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.s3f = new Size3f(v);
}
lzh::Element::Element(const Point & v) : _type(ElemType<Point>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.p2i = new Point(v);
}
lzh::Element::Element(const Point2f & v) : _type(ElemType<Point2f>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.p2f = new Point2f(v);
}
lzh::Element::Element(const Point3 & v) : _type(ElemType<Point3>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.p3i = new Point3(v);
}
lzh::Element::Element(const Point3f & v) : _type(ElemType<Point3f>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.p3f = new Point3f(v);
}
lzh::Element::Element(const Vec2b & v) : _type(ElemType<Vec2b>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v2b = new Vec2b(v);
}
lzh::Element::Element(const Vec2i & v) : _type(ElemType<Vec2i>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v2i = new Vec2i(v);
}
lzh::Element::Element(const Vec2f & v) : _type(ElemType<Vec2f>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v2f = new Vec2f(v);
}
lzh::Element::Element(const Vec2d & v) : _type(ElemType<Vec2d>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v2d = new Vec2d(v);
}
lzh::Element::Element(const Vec3b & v) : _type(ElemType<Vec3b>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v3b = new Vec3b(v);
}
lzh::Element::Element(const Vec3i & v) : _type(ElemType<Vec3i>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v3i = new Vec3i(v);
}
lzh::Element::Element(const Vec3f & v) : _type(ElemType<Vec3f>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v3f = new Vec3f(v);
}
lzh::Element::Element(const Vec3d & v) : _type(ElemType<Vec3d>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v3d = new Vec3d(v);
}
lzh::Element::Element(const Vec4b & v) : _type(ElemType<Vec4b>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v4b = new Vec4b(v);
}
lzh::Element::Element(const Vec4i & v) : _type(ElemType<Vec4i>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v4i = new Vec4i(v);
}
lzh::Element::Element(const Vec4f & v) : _type(ElemType<Vec4f>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v4f = new Vec4f(v);
}
lzh::Element::Element(const Vec4d & v) : _type(ElemType<Vec4d>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.v4d = new Vec4d(v);
}
lzh::Element::Element(const Color & v) : _type(ElemType<Color>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.rgb = new Color(v);
}
lzh::Element::Element(const Complex & v) : _type(ElemType<Complex>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.c = new Complex(v);
}
lzh::Element::Element(const LargeNumber & v) : _type(ElemType<LargeNumber>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.ln = new LargeNumber(v);
}
lzh::Element::Element(const MatPtr<uint8>& v) : _type(ElemType<MatPtr<uint8>>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.m8i = new MatPtr<uint8>(v);
}
lzh::Element::Element(const MatPtr<int32>& v) : _type(ElemType<MatPtr<int32>>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.m32i = new MatPtr<int32>(v);
}
lzh::Element::Element(const MatPtr<float64>& v) : _type(ElemType<MatPtr<float64>>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.md = new MatPtr<float64>(v);
}
lzh::Element::Element(const Vector & v) : _type(ElemType<Vector>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.pm = new Vector(v);
}
lzh::Element::Element(const std::string & v) : _type(ElemType<std::string>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.str = new std::string(v);
}
lzh::Element::Element(const Mat & v) : _type(ElemType<Mat>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.m = new Mat(v);
}
lzh::Element::Element(const Array & v) : _type(ElemType<Array>::type), _typeName(typeid(v).name())
{
	data_ptr = ptr.a = new Array(v);
}
lzh::Element::Element(const std::vector<Element>& v) : _type(ElemType<std::vector<Element>>::type), _typeName(typeid(v).name())
{
	ptr.arr = new Pointer<std::vector<Element>>(v);
	data_ptr = ptr.arr->Ptr();
}
//lzh::Element::Element(const std::initializer_list<Element>& list) : _type(ElemType<std::vector<Element>>::type)
//{
//	ptr.arr = new Pointer<std::vector<Element>>(list);
//	data_ptr = ptr.arr->Ptr();
//	_typeName = std::string(typeid(ptr.arr->Data()).name());
//}
lzh::Element::Element(const std::map<Element, Element>& dict) : _type(ElemType<std::map<Element, Element>>::type)
{
	ptr._map = new Pointer<std::map<Element, Element>>(dict);
	data_ptr = ptr._map->Ptr();
}
lzh::Element::Element(InputArray inputArray) : _type(ARRAY)
{
	data_ptr = ptr.a = new Array(inputArray.getArray());
	_typeName = std::string(typeid(ptr.a).name());
}
lzh::Element::Element(const Element & element) : _type(InValid)
{
	data_ptr = ptr.user = nullptr;
	*this = element;
}
Element & lzh::Element::operator=(const Element & element)
{
	if (this == &element)return *this;
	release();
	create(element._type);
	_typeName = element._typeName;

	if (element.custom != nullptr)
		custom = new string(*element.custom);
	else if (element.custom == nullptr)
		custom = element.custom;

	switch (element._type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::FUNCTION:data_ptr = ptr.user = (void*)element.data(); break;
	case lzh::Element::BOOL: element.toData(ptr.b); data_ptr = &ptr.b; break;
	case lzh::Element::INT8: element.toData(ptr.i8); data_ptr = &ptr.i8; break;
	case lzh::Element::UINT8: element.toData(ptr.u8); data_ptr = &ptr.u8; break;
	case lzh::Element::INT16: element.toData(ptr.i16); data_ptr = &ptr.i16; break;
	case lzh::Element::UINT16: element.toData(ptr.u16); data_ptr = &ptr.u16; break;
	case lzh::Element::INT32: element.toData(ptr.i32);  data_ptr = &ptr.i32; break;
	case lzh::Element::UINT32: element.toData(ptr.u32); data_ptr = &ptr.u32; break;
	case lzh::Element::INT64: element.toData(ptr.i64); data_ptr = &ptr.i64; break;
	case lzh::Element::UINT64: element.toData(ptr.u64); data_ptr = &ptr.u64; break;
	case lzh::Element::FLOAT32: element.toData(ptr.f32); data_ptr = &ptr.f32; break;
	case lzh::Element::FLOAT64:  element.toData(ptr.f64);  data_ptr = &ptr.f64; break;
	case lzh::Element::FLOAT128: element.toData(ptr.f128); data_ptr = &ptr.f128; break;
	case lzh::Element::INT8_PTR: element.toPtr(&ptr.i8_p); data_ptr = ptr.i8_p; break;
	case lzh::Element::UINT8_PTR: element.toPtr(&ptr.u8_p); data_ptr = ptr.i8_p; break;
	case lzh::Element::INT16_PTR:  element.toPtr(&ptr.i16_p); data_ptr = ptr.i16_p; break;
	case lzh::Element::UINT16_PTR: element.toPtr(&ptr.u16_p); data_ptr = ptr.u16_p; break;
	case lzh::Element::INT32_PTR: element.toPtr(&ptr.i32_p); data_ptr = ptr.i32_p; break;
	case lzh::Element::UINT32_PTR: element.toPtr(&ptr.u32_p); data_ptr = ptr.u32_p; break;
	case lzh::Element::INT64_PTR: element.toPtr(&ptr.i64_p); data_ptr = ptr.i64_p; break;
	case lzh::Element::UINT64_PTR: element.toPtr(&ptr.u64_p); data_ptr = ptr.u64_p; break;
	case lzh::Element::FLOAT32_PTR: element.toPtr(&ptr.f32_p); data_ptr = ptr.f32_p; break;
	case lzh::Element::FLOAT64_PTR: element.toPtr(&ptr.f64_p); data_ptr = ptr.f64_p; break;
	case lzh::Element::FLOAT128_PTR: element.toPtr(&ptr.f128_p); data_ptr = ptr.f128_p; break;
	case lzh::Element::RANGE: element.toData(*ptr.ra); data_ptr = ptr.ra; break;
	case lzh::Element::RECT: element.toData(*ptr.r); data_ptr = ptr.r; break;
	case lzh::Element::RECTF: element.toData(*ptr.rf); data_ptr = ptr.rf; break;
	case lzh::Element::SIZE: element.toData(*ptr.s2); data_ptr = ptr.s2; break;
	case lzh::Element::SIZEF: element.toData(*ptr.s2f); data_ptr = ptr.s2f; break;
	case lzh::Element::SIZE3: element.toData(*ptr.s3i); data_ptr = ptr.s3i; break;
	case lzh::Element::SIZE3F: element.toData(*ptr.s3f); data_ptr = ptr.s3f; break;
	case lzh::Element::POINT: element.toData(*ptr.p2i); data_ptr = ptr.p2i; break;
	case lzh::Element::POINTF: element.toData(*ptr.p2f); data_ptr = ptr.p2f; break;
	case lzh::Element::POINT3: element.toData(*ptr.p3i); data_ptr = ptr.p3i; break;
	case lzh::Element::POINT3F:  element.toData(*ptr.p3f); data_ptr = ptr.p3f; break;
	case lzh::Element::VEC2B: element.toData(*ptr.v2b); data_ptr = ptr.v2b; break;
	case lzh::Element::VEC2I: element.toData(*ptr.v2i); data_ptr = ptr.v2i; break;
	case lzh::Element::VEC2F: element.toData(*ptr.v2f); data_ptr = ptr.v2f; break;
	case lzh::Element::VEC2D: element.toData(*ptr.v2d); data_ptr = ptr.v2d; break;
	case lzh::Element::VEC3B: element.toData(*ptr.v3b); data_ptr = ptr.v3b; break;
	case lzh::Element::VEC3I: element.toData(*ptr.v3i); data_ptr = ptr.v3i; break;
	case lzh::Element::VEC3F: element.toData(*ptr.v3f); data_ptr = ptr.v3f; break;
	case lzh::Element::VEC3D: element.toData(*ptr.v3d); data_ptr = ptr.v3d; break;
	case lzh::Element::VEC4B: element.toData(*ptr.v4b); data_ptr = ptr.v4b; break;
	case lzh::Element::VEC4I: element.toData(*ptr.v4i); data_ptr = ptr.v4i; break;
	case lzh::Element::VEC4F: element.toData(*ptr.v4f); data_ptr = ptr.v4f; break;
	case lzh::Element::VEC4D: element.toData(*ptr.v4d); data_ptr = ptr.v4d; break;
	case lzh::Element::COLOR: element.toData(*ptr.rgb); data_ptr = ptr.rgb; break;
	case lzh::Element::COMPLEX: element.toData(*ptr.c); data_ptr = ptr.c; break;
	case lzh::Element::LARGENUMBER: element.toData(*ptr.ln); data_ptr = ptr.ln; break;
	case lzh::Element::MATPTR_UINT8: element.toData(*ptr.m8i); data_ptr = ptr.m8i; break;
	case lzh::Element::MATPTR_INT32: element.toData(*ptr.m32i); data_ptr = ptr.m32i; break;
	case lzh::Element::MATPTR_FLOAT32: element.toData(*ptr.mf); data_ptr = ptr.mf; break;
	case lzh::Element::MATPTR_FLOAT64: element.toData(*ptr.md); data_ptr = ptr.md; break;
	case lzh::Element::VECTOR: element.toData(*ptr.pm); data_ptr = ptr.pm; break;
	case lzh::Element::STRING: element.toData(*ptr.str); data_ptr = ptr.str; break;
	case lzh::Element::MAT: element.toData(*ptr.m); data_ptr = ptr.m; break;
	case lzh::Element::ARRAY: element.toData(*ptr.a); data_ptr = ptr.a; break;
	case lzh::Element::STD_VECTOR: *ptr.arr = *element.ptr.arr; data_ptr = ptr.arr->Ptr(); break;
	case lzh::Element::STD_MAP: *ptr._map = *element.ptr._map; data_ptr = ptr._map->Ptr(); break;
	case lzh::Element::STD_TUPLE:
		data_ptr = (void*)element.data();
#ifdef _WIN64
		ptr.u64 = element.ptr.u64;
#else
		ptr.u32 = element.ptr.u32;
#endif // _WIN64
		break;
	default:break;
	}
	return *this;
}

void lzh::Element::release()
{
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::FUNCTION:ptr.user = nullptr; break;
	case lzh::Element::BOOL: ptr.b = false; break;
	case lzh::Element::INT8: ptr.i8 = 0; break;
	case lzh::Element::UINT8: ptr.u8 = 0; break;
	case lzh::Element::INT16: ptr.i16 = 0; break;
	case lzh::Element::UINT16: ptr.u16 = 0; break;
	case lzh::Element::INT32: ptr.i32 = 0; break;
	case lzh::Element::UINT32: ptr.u32 = 0; break;
	case lzh::Element::INT64: ptr.i64 = 0; break;
	case lzh::Element::UINT64: ptr.u64 = 0; break;
	case lzh::Element::FLOAT32: ptr.f32 = 0; break;
	case lzh::Element::FLOAT64:  ptr.f64 = 0; break;
	case lzh::Element::FLOAT128: ptr.f128 = 0; break;
	case lzh::Element::INT8_PTR: ptr.i8_p = nullptr; break;
	case lzh::Element::UINT8_PTR: ptr.u8_p = nullptr; break;
	case lzh::Element::INT16_PTR:  ptr.i16_p = nullptr; break;
	case lzh::Element::UINT16_PTR: ptr.u16_p = nullptr; break;
	case lzh::Element::INT32_PTR: ptr.i32_p = nullptr; break;
	case lzh::Element::UINT32_PTR: ptr.u32_p = nullptr; break;
	case lzh::Element::INT64_PTR: ptr.i64_p = nullptr; break;
	case lzh::Element::UINT64_PTR: ptr.u64_p = nullptr; break;
	case lzh::Element::FLOAT32_PTR: ptr.f32_p = nullptr; break;
	case lzh::Element::FLOAT64_PTR: ptr.f64_p = nullptr; break;
	case lzh::Element::FLOAT128_PTR: ptr.f128_p = nullptr; break;
	case lzh::Element::RANGE: FREE_PTR(ptr.ra); break;
	case lzh::Element::RECT: FREE_PTR(ptr.r); break;
	case lzh::Element::RECTF: FREE_PTR(ptr.rf); break;
	case lzh::Element::SIZE: FREE_PTR(ptr.s2); break;
	case lzh::Element::SIZEF: FREE_PTR(ptr.s2f); break;
	case lzh::Element::SIZE3: FREE_PTR(ptr.s3i); break;
	case lzh::Element::SIZE3F: FREE_PTR(ptr.s3f); break;
	case lzh::Element::POINT: FREE_PTR(ptr.p2i); break;
	case lzh::Element::POINTF: FREE_PTR(ptr.p2f); break;
	case lzh::Element::POINT3: FREE_PTR(ptr.p3i); break;
	case lzh::Element::POINT3F:  FREE_PTR(ptr.p3f); break;
	case lzh::Element::VEC2B: FREE_PTR(ptr.v2b); break;
	case lzh::Element::VEC2I: FREE_PTR(ptr.v2i); break;
	case lzh::Element::VEC2F: FREE_PTR(ptr.v2f); break;
	case lzh::Element::VEC2D: FREE_PTR(ptr.v2d); break;
	case lzh::Element::VEC3B: FREE_PTR(ptr.v3b); break;
	case lzh::Element::VEC3I: FREE_PTR(ptr.v3i); break;
	case lzh::Element::VEC3F: FREE_PTR(ptr.v3f); break;
	case lzh::Element::VEC3D: FREE_PTR(ptr.v3d); break;
	case lzh::Element::VEC4B: FREE_PTR(ptr.v4b); break;
	case lzh::Element::VEC4I: FREE_PTR(ptr.v4i); break;
	case lzh::Element::VEC4F: FREE_PTR(ptr.v4f); break;
	case lzh::Element::VEC4D: FREE_PTR(ptr.v4d); break;
	case lzh::Element::COLOR: FREE_PTR(ptr.rgb); break;
	case lzh::Element::COMPLEX: FREE_PTR(ptr.c); break;
	case lzh::Element::LARGENUMBER: FREE_PTR(ptr.ln); break;
	case lzh::Element::MATPTR_UINT8: FREE_PTR(ptr.m8i); break;
	case lzh::Element::MATPTR_INT32: FREE_PTR(ptr.m32i); break;
	case lzh::Element::MATPTR_FLOAT32: FREE_PTR(ptr.mf); break;
	case lzh::Element::MATPTR_FLOAT64: FREE_PTR(ptr.md); break;
	case lzh::Element::VECTOR: FREE_PTR(ptr.pm); break;
	case lzh::Element::STRING: FREE_PTR(ptr.str); break;
	case lzh::Element::MAT: FREE_PTR(ptr.m); break;
	case lzh::Element::ARRAY: FREE_PTR(ptr.a); break;
	case lzh::Element::STD_VECTOR: FREE_PTR(ptr.arr); break;
	case lzh::Element::STD_MAP: FREE_PTR(ptr._map); break;
	case lzh::Element::STD_TUPLE:
		data_ptr = nullptr;
#ifdef _WIN64
		ptr.u64 = 0;
#else
		ptr.u32 = 0;
#endif // _WIN64
		break;
	default:break;
	}
	FREE_PTR(custom);
}
void lzh::Element::create(int32 t)
{
	_type = t;
	switch (t)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::FUNCTION: data_ptr = ptr.user = nullptr; break;
	case lzh::Element::BOOL: ptr.b = false; data_ptr = (void*)&ptr; break;
	case lzh::Element::INT8: ptr.i8 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::UINT8: ptr.u8 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::INT16: ptr.i16 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::UINT16: ptr.u16 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::INT32: ptr.i32 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::UINT32: ptr.u32 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::INT64: ptr.i64 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::UINT64: ptr.u64 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::FLOAT32: ptr.f32 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::FLOAT64:  ptr.f64 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::FLOAT128: ptr.f128 = 0; data_ptr = (void*)&ptr; break;
	case lzh::Element::INT8_PTR: data_ptr = ptr.i8_p = 0; break;
	case lzh::Element::UINT8_PTR: data_ptr = ptr.u8_p = 0; break;
	case lzh::Element::INT16_PTR:  data_ptr = ptr.i16_p = 0; break;
	case lzh::Element::UINT16_PTR: data_ptr = ptr.u16_p = 0; break;
	case lzh::Element::INT32_PTR: data_ptr = ptr.i32_p = 0; break;
	case lzh::Element::UINT32_PTR: data_ptr = ptr.u32_p = 0; break;
	case lzh::Element::INT64_PTR: data_ptr = ptr.i64_p = 0; break;
	case lzh::Element::UINT64_PTR: data_ptr = ptr.u64_p = 0; break;
	case lzh::Element::FLOAT32_PTR: data_ptr = ptr.f32_p = 0; break;
	case lzh::Element::FLOAT64_PTR: data_ptr = ptr.f64_p = 0; break;
	case lzh::Element::FLOAT128_PTR: data_ptr = ptr.f128_p = 0; break;
	case lzh::Element::RANGE: data_ptr = ptr.ra = new Range; break;
	case lzh::Element::RECT: data_ptr = ptr.r = new Rect; break;
	case lzh::Element::RECTF: data_ptr = ptr.rf = new RectF; break;
	case lzh::Element::SIZE: data_ptr = ptr.s2 = new Size; break;
	case lzh::Element::SIZEF: data_ptr = ptr.s2f = new Size2f; break;
	case lzh::Element::SIZE3: data_ptr = ptr.s3i = new Size3; break;
	case lzh::Element::SIZE3F: data_ptr = ptr.s3f = new Size3f; break;
	case lzh::Element::POINT: data_ptr = ptr.p2i = new Point; break;
	case lzh::Element::POINTF: data_ptr = ptr.p2f = new Point2f; break;
	case lzh::Element::POINT3: data_ptr = ptr.p3i = new Point3; break;
	case lzh::Element::POINT3F:  data_ptr = ptr.p3f = new Point3f; break;
	case lzh::Element::VEC2B: data_ptr = ptr.v2b = new Vec2b; break;
	case lzh::Element::VEC2I: data_ptr = ptr.v2i = new Vec2i; break;
	case lzh::Element::VEC2F: data_ptr = ptr.v2f = new Vec2f; break;
	case lzh::Element::VEC2D: data_ptr = ptr.v2d = new Vec2d; break;
	case lzh::Element::VEC3B: data_ptr = ptr.v3b = new Vec3b; break;
	case lzh::Element::VEC3I: data_ptr = ptr.v3i = new Vec3i; break;
	case lzh::Element::VEC3F: data_ptr = ptr.v3f = new Vec3f; break;
	case lzh::Element::VEC3D: data_ptr = ptr.v3d = new Vec3d; break;
	case lzh::Element::VEC4B: data_ptr = ptr.v4b = new Vec4b; break;
	case lzh::Element::VEC4I: data_ptr = ptr.v4i = new Vec4i; break;
	case lzh::Element::VEC4F: data_ptr = ptr.v4f = new Vec4f; break;
	case lzh::Element::VEC4D: data_ptr = ptr.v4d = new Vec4d; break;
	case lzh::Element::COLOR: data_ptr = ptr.rgb = new Color; break;
	case lzh::Element::COMPLEX: data_ptr = ptr.c = new Complex; break;
	case lzh::Element::LARGENUMBER: data_ptr = ptr.ln = new LargeNumber; break;
	case lzh::Element::MATPTR_UINT8: data_ptr = ptr.m8i = new MatPtr<uint8>; break;
	case lzh::Element::MATPTR_INT32: data_ptr = ptr.m32i = new MatPtr<int32>; break;
	case lzh::Element::MATPTR_FLOAT32: data_ptr = ptr.mf = new MatPtr<float32>; break;
	case lzh::Element::MATPTR_FLOAT64: data_ptr = ptr.md = new MatPtr<float64>; break;
	case lzh::Element::VECTOR: data_ptr = ptr.pm = new Vector; break;
	case lzh::Element::STRING: data_ptr = ptr.str = new std::string; break;
	case lzh::Element::MAT: data_ptr = ptr.m = new Mat; break;
	case lzh::Element::ARRAY: data_ptr = ptr.a = new Array; break;
	case lzh::Element::STD_VECTOR: ptr.arr = new Pointer<ElemList>; data_ptr = ptr.arr->Ptr(); break;
	case lzh::Element::STD_MAP: ptr._map = new Pointer<std::map<Element, Element>>; data_ptr = ptr._map->Ptr(); break;
	case lzh::Element::STD_TUPLE: 
		data_ptr = nullptr;
#ifdef _WIN64
		ptr.u64 = 0;
#else
		ptr.u32 = 0;
#endif // _WIN64
		break;
	default:break;
	}
}
bool lzh::Element::empty() const
{
	return data_ptr == nullptr;
}
lzh::Element::~Element()
{
	release();
}
intptr_t lzh::Element::toIntPtr() const
{
	LZH_ACCESS(if (_type == INTPTR), THROW_TYPE_INFO(typeName(INTPTR), typeName()));
	return intptr_t(ptr.user);
}
bool & lzh::Element::tobool()
{
	LZH_ACCESS(if (_type == BOOL), THROW_TYPE_INFO(typeName(BOOL), typeName()));
	return ptr.b;
}

int8 & lzh::Element::toChar()
{
	return toi8();
}

int8 & lzh::Element::toi8()
{
	LZH_ACCESS(if (_type == INT8), THROW_TYPE_INFO(typeName(INT8), typeName()));
	return ptr.i8;
}
int16 & lzh::Element::toi16()
{
	LZH_ACCESS(if (_type == INT16), THROW_TYPE_INFO(typeName(INT16), typeName()));
	return ptr.i16;
}
int32 lzh::Element::toInt()const
{
	if (isi32()) return toi32();
	else return saturate_cast<int32>(toNumber());
}
int32 & lzh::Element::toi32()
{
	LZH_ACCESS(if (_type == INT32), THROW_TYPE_INFO(typeName(INT32), typeName()));
	return ptr.i32;
}
int64 & lzh::Element::toi64()
{
	LZH_ACCESS(if (_type == INT64), THROW_TYPE_INFO(typeName(INT64), typeName()));
	return ptr.i64;
}
uint8 & lzh::Element::tou8()
{
	LZH_ACCESS(if (_type == UINT8), THROW_TYPE_INFO(typeName(UINT8), typeName()));
	return ptr.u8;
}
uint16 & lzh::Element::tou16()
{
	LZH_ACCESS(if (_type == UINT16), THROW_TYPE_INFO(typeName(UINT16), typeName()));
	return ptr.u16;
}
uint32 & lzh::Element::tou32()
{
	LZH_ACCESS(if (_type == UINT32), THROW_TYPE_INFO(typeName(UINT32), typeName()));
	return ptr.u32;
}
uint64 & lzh::Element::tou64()
{
	LZH_ACCESS(if (_type == UINT64), THROW_TYPE_INFO(typeName(UINT64), typeName()));
	return ptr.u64;
}
float32 lzh::Element::toFloat()const
{
	if (isf32()) return tof32();
	else return toBaseType<float32>();
}
float32 & lzh::Element::tof32()
{
	LZH_ACCESS(if (_type == FLOAT32), THROW_TYPE_INFO(typeName(FLOAT32), typeName()));
	return ptr.f32;
}
float64 lzh::Element::toDouble()const
{
	if (isf64()) return tof64();
	else return toNumber();
}
float64 & lzh::Element::tof64()
{
	LZH_ACCESS(if (_type == FLOAT64), THROW_TYPE_INFO(typeName(FLOAT64), typeName()));
	return ptr.f64;
}
float128 & lzh::Element::tof128()
{
	LZH_ACCESS(if (_type == FLOAT128), THROW_TYPE_INFO(typeName(FLOAT128), typeName()));
	return ptr.f128;
}

const bool & lzh::Element::tobool() const
{
	LZH_ACCESS(if (_type == BOOL), THROW_TYPE_INFO(typeName(BOOL), typeName()));
	return ptr.b;
}

const int8& lzh::Element::toi8() const
{
	LZH_ACCESS(if (_type == INT8), THROW_TYPE_INFO(typeName(INT8), typeName()));
	return ptr.i8;
}
const int16& lzh::Element::toi16() const
{
	LZH_ACCESS(if (_type == INT16), THROW_TYPE_INFO(typeName(INT16), typeName()));
	return ptr.i16;
}
const int32& lzh::Element::toi32() const
{
	LZH_ACCESS(if (_type == INT32), THROW_TYPE_INFO(typeName(INT32), typeName()));
	return ptr.i32;
}
const int64& lzh::Element::toi64() const
{
	LZH_ACCESS(if (_type == INT64), THROW_TYPE_INFO(typeName(INT64), typeName()));
	return ptr.i64;
}
const uint8& lzh::Element::tou8() const
{
	LZH_ACCESS(if (_type == UINT8), THROW_TYPE_INFO(typeName(UINT8), typeName()));
	return ptr.u8;
}
const uint16& lzh::Element::tou16() const
{
	LZH_ACCESS(if (_type == UINT16), THROW_TYPE_INFO(typeName(UINT16), typeName()));
	return ptr.u16;
}
const uint32& lzh::Element::tou32() const
{
	LZH_ACCESS(if (_type == UINT32), THROW_TYPE_INFO(typeName(UINT32), typeName()));
	return ptr.u32;
}
const uint64& lzh::Element::tou64() const
{
	LZH_ACCESS(if (_type == UINT64), THROW_TYPE_INFO(typeName(UINT64), typeName()));
	return ptr.u64;
}
const float32& lzh::Element::tof32() const
{
	LZH_ACCESS(if (_type == FLOAT32), THROW_TYPE_INFO(typeName(FLOAT32), typeName()));
	return ptr.f32;
}
const float64& lzh::Element::tof64() const
{
	LZH_ACCESS(if (_type == FLOAT64), THROW_TYPE_INFO(typeName(FLOAT64), typeName()));
	return ptr.f64;
}
const float128& lzh::Element::tof128() const
{
	LZH_ACCESS(if (_type == FLOAT128), THROW_TYPE_INFO(typeName(FLOAT128), typeName()));
	return ptr.f128;
}

mat_t lzh::Element::toNumber() const
{
	LZH_ACCESS(if (isNumber()), THROW_TYPE_INFO("Number", typeName()));
	if (isi8())return saturate_cast<mat_t>(toi8());
	else if (isi16())return saturate_cast<mat_t>(toi16());
	else if (isi32())return saturate_cast<mat_t>(toi32());
	else if (isi64())return saturate_cast<mat_t>(toi64());
	else if (isu8())return saturate_cast<mat_t>(tou8());
	else if (isu16())return saturate_cast<mat_t>(tou16());
	else if (isu32())return saturate_cast<mat_t>(tou32());
	else if (isu64())return saturate_cast<mat_t>(tou64());
	else if (isf32())return saturate_cast<mat_t>(tof32());
	else if (isf64())return saturate_cast<mat_t>(tof64());
	else if (isf128())return saturate_cast<mat_t>(tof128());
	else return NAN;
}

Value lzh::Element::toValue()const
{
	LZH_ACCESS(if (isNumber()), THROW_TYPE_INFO("Number", typeName()));
	if (isi8())return Value(toi8());
	else if (isi16())return Value(toi16());
	else if (isi32())return Value(toi32());
	else if (isi64())return Value(toi64());
	else if (isu8())return Value(tou8());
	else if (isu16())return Value(tou16());
	else if (isu32())return Value(tou32());
	else if (isu64())return Value(tou64());
	else if (isf32())return Value(tof32());
	else if (isf64())return Value(tof64());
	else if (isf128())return Value(tof128());
	else return Value();
}

const int8 * lzh::Element::toCString() const
{
	LZH_ACCESS(if ((_type == INT8_PTR || _type == STRING) && data_ptr), THROW_TYPE_INFO("const char* or string", typeName()));
	if (_type == INT8_PTR)return toI8Ptr();
	if (_type == STRING)return toStdString().c_str();
	return nullptr;
}

std::string lzh::Element::toString() const
{
	LZH_ACCESS(if ((_type == INT8_PTR || _type == STRING) && data_ptr), THROW_TYPE_INFO("const char* or string", typeName()));
	if (_type == INT8_PTR)return _STR(toI8Ptr());
	if (_type == STRING)return toStdString();
	return nullptr;
}

int8 * lzh::Element::toI8Ptr() const
{
	LZH_ACCESS(if (_type == INT8_PTR && data_ptr), THROW_TYPE_INFO(typeName(INT8_PTR), typeName()));
	return ptr.i8_p;
}
int16 * lzh::Element::toI16Ptr() const
{
	LZH_ACCESS(if (_type == INT16_PTR && data_ptr), THROW_TYPE_INFO(typeName(INT16_PTR), typeName()));
	return ptr.i16_p;
}
int32 * lzh::Element::toI32Ptr() const
{
	LZH_ACCESS(if (_type == INT32_PTR && data_ptr), THROW_TYPE_INFO(typeName(INT32_PTR), typeName()));
	return ptr.i32_p;
}
int64 * lzh::Element::toI64Ptr() const
{
	LZH_ACCESS(if (_type == INT64_PTR && data_ptr), THROW_TYPE_INFO(typeName(INT64_PTR), typeName()));
	return ptr.i64_p;
}
uint8 * lzh::Element::toU8Ptr() const
{
	LZH_ACCESS(if (_type == UINT8_PTR && data_ptr), THROW_TYPE_INFO(typeName(UINT8_PTR), typeName()));
	return ptr.u8_p;
}
uint16 * lzh::Element::toU16Ptr() const
{
	LZH_ACCESS(if (_type == UINT16_PTR && data_ptr), THROW_TYPE_INFO(typeName(UINT16_PTR), typeName()));
	return ptr.u16_p;
}
uint32 * lzh::Element::toU32Ptr() const
{
	LZH_ACCESS(if (_type == UINT32_PTR && data_ptr), THROW_TYPE_INFO(typeName(UINT32_PTR), typeName()));
	return ptr.u32_p;
}
uint64 * lzh::Element::toU64Ptr() const
{
	LZH_ACCESS(if (_type == UINT64_PTR && data_ptr), THROW_TYPE_INFO(typeName(UINT64_PTR), typeName()));
	return ptr.u64_p;
}
float32 * lzh::Element::toF32Ptr() const
{
	LZH_ACCESS(if (_type == FLOAT32_PTR && data_ptr), THROW_TYPE_INFO(typeName(FLOAT32_PTR), typeName()));
	return ptr.f32_p;
}
float64 * lzh::Element::toF64Ptr() const
{
	LZH_ACCESS(if (_type == FLOAT64_PTR && data_ptr), THROW_TYPE_INFO(typeName(FLOAT64_PTR), typeName()));
	return ptr.f64_p;
}
float128 * lzh::Element::toF128Ptr() const
{
	LZH_ACCESS(if (_type == FLOAT128_PTR && data_ptr), THROW_TYPE_INFO(typeName(FLOAT128_PTR), typeName()));
	return ptr.f128_p;
}

Range& lzh::Element::toRange() const
{
	LZH_ACCESS(if (_type == RANGE && data_ptr), THROW_TYPE_INFO(typeName(RANGE), typeName()));
	return *ptr.ra;
}
Rect& lzh::Element::toRect() const
{
	LZH_ACCESS(if (_type == RECT && data_ptr), THROW_TYPE_INFO(typeName(RECT), typeName()));
	return *ptr.r;
}
RectF& lzh::Element::toRectF() const
{
	LZH_ACCESS(if (_type == RECTF && data_ptr), THROW_TYPE_INFO(typeName(RECTF), typeName()));
	return *ptr.rf;
}

Size& lzh::Element::toSize() const
{
	LZH_ACCESS(if (_type == SIZE && data_ptr), THROW_TYPE_INFO(typeName(SIZE), typeName()));
	return *ptr.s2;
}
Size2f& lzh::Element::toSizeF() const
{
	LZH_ACCESS(if (_type == SIZEF && data_ptr), THROW_TYPE_INFO(typeName(SIZEF), typeName()));
	return *ptr.s2f;
}
Size3& lzh::Element::toSize3() const
{
	LZH_ACCESS(if (_type == SIZE3 && data_ptr), THROW_TYPE_INFO(typeName(SIZE3), typeName()));
	return *ptr.s3i;
}
Size3f& lzh::Element::toSize3F() const
{
	LZH_ACCESS(if (_type == SIZE3F && data_ptr), THROW_TYPE_INFO(typeName(SIZE3F), typeName()));
	return *ptr.s3f;
}

Point& lzh::Element::toPoint() const
{
	LZH_ACCESS(if (_type == POINT && data_ptr), THROW_TYPE_INFO(typeName(POINT), typeName()));
	return *ptr.p2i;
}
Point2f& lzh::Element::toPointF() const
{
	LZH_ACCESS(if (_type == POINTF && data_ptr), THROW_TYPE_INFO(typeName(POINTF), typeName()));
	return *ptr.p2f;
}
Point3& lzh::Element::toPoint3() const
{
	LZH_ACCESS(if (_type == POINT3 && data_ptr), THROW_TYPE_INFO(typeName(POINT3), typeName()));
	return *ptr.p3i;
}
Point3f& lzh::Element::toPoint3F() const
{
	LZH_ACCESS(if (_type == POINT3F && data_ptr), THROW_TYPE_INFO(typeName(POINT3F), typeName()));
	return *ptr.p3f;
}

Vec2b& lzh::Element::toVec2b() const
{
	LZH_ACCESS(if (_type == VEC2B && data_ptr), THROW_TYPE_INFO(typeName(VEC2B), typeName()));
	return *ptr.v2b;
}
Vec2i& lzh::Element::toVec2i() const
{
	LZH_ACCESS(if (_type == VEC2I && data_ptr), THROW_TYPE_INFO(typeName(VEC2I), typeName()));
	return *ptr.v2i;
}
Vec2f& lzh::Element::toVec2f() const
{
	LZH_ACCESS(if (_type == VEC2F && data_ptr), THROW_TYPE_INFO(typeName(VEC2F), typeName()));
	return *ptr.v2f;
}
Vec2d& lzh::Element::toVec2d() const
{
	LZH_ACCESS(if (_type == VEC2D && data_ptr), THROW_TYPE_INFO(typeName(VEC2D), typeName()));
	return *ptr.v2d;
}
Vec3b& lzh::Element::toVec3b() const
{
	LZH_ACCESS(if (_type == VEC3B && data_ptr), THROW_TYPE_INFO(typeName(VEC3B), typeName()));
	return *ptr.v3b;
}
Vec3i& lzh::Element::toVec3i() const
{
	LZH_ACCESS(if (_type == VEC3I && data_ptr), THROW_TYPE_INFO(typeName(VEC3I), typeName()));
	return *ptr.v3i;
}
Vec3f& lzh::Element::toVec3f() const
{
	LZH_ACCESS(if (_type == VEC3F && data_ptr), THROW_TYPE_INFO(typeName(VEC3F), typeName()));
	return *ptr.v3f;
}
Vec3d& lzh::Element::toVec3d() const
{
	LZH_ACCESS(if (_type == VEC3D && data_ptr), THROW_TYPE_INFO(typeName(VEC3D), typeName()));
	return *ptr.v3d;
}
Vec4b& lzh::Element::toVec4b() const
{
	LZH_ACCESS(if (_type == VEC4B && data_ptr), THROW_TYPE_INFO(typeName(VEC4B), typeName()));
	return *ptr.v4b;
}
Vec4i& lzh::Element::toVec4i() const
{
	LZH_ACCESS(if (_type == VEC4I && data_ptr), THROW_TYPE_INFO(typeName(VEC4I), typeName()));
	return *ptr.v4i;
}
Vec4f& lzh::Element::toVec4f() const
{
	LZH_ACCESS(if (_type == VEC4F && data_ptr), THROW_TYPE_INFO(typeName(VEC4F), typeName()));
	return *ptr.v4f;
}
Vec4d& lzh::Element::toVec4d() const
{
	LZH_ACCESS(if (_type == VEC4D && data_ptr), THROW_TYPE_INFO(typeName(VEC4D), typeName()));
	return *ptr.v4d;
}
Color& lzh::Element::toColor() const
{
	LZH_ACCESS(if (_type == COLOR && data_ptr), THROW_TYPE_INFO(typeName(COLOR), typeName()));
	return *ptr.rgb;
}

Color lzh::Element::toRGB() const
{
	if (_type == STD_VECTOR) {
		ElemList& list = toList();
		LZH_ACCESS(if (list.size() >= 1 && list.size() <= 4), THROW_INFO(ERR_INFO_VALUE));
		if (list.size() == 1)
			return Color(list[0].toNumber());
		else if (list.size() == 2)
			return Color(list[0].toNumber(), list[1].toNumber());
		else if (list.size() == 3)
			return Color(list[0].toNumber(), list[1].toNumber(), list[2].toNumber());
		else if (list.size() == 4)
			return Color(list[0].toNumber(), list[1].toNumber(), list[2].toNumber(), list[3].toNumber());
		else
			return Color();
	}
	else {
		return toColor();
	}
}
Complex& lzh::Element::toComplex() const
{
	LZH_ACCESS(if (_type == COMPLEX && data_ptr), THROW_TYPE_INFO(typeName(COMPLEX), typeName()));
	return *ptr.c;
}
LargeNumber& lzh::Element::toLargeNumber() const
{
	LZH_ACCESS(if (_type == LARGENUMBER && data_ptr), THROW_TYPE_INFO(typeName(LARGENUMBER), typeName()));
	return *ptr.ln;
}

MatPtr<uint8>& lzh::Element::toU8Array() const
{
	LZH_ACCESS(if (_type == MATPTR_UINT8 && data_ptr), THROW_TYPE_INFO(typeName(MATPTR_UINT8), typeName()));
	return *ptr.m8i;
}
MatPtr<int32>& lzh::Element::toI32Array() const
{
	LZH_ACCESS(if (_type == MATPTR_INT32 && data_ptr), THROW_TYPE_INFO(typeName(MATPTR_INT32), typeName()));
	return *ptr.m32i;
}
MatPtr<float32>& lzh::Element::toF32Array() const
{
	LZH_ACCESS(if (_type == MATPTR_FLOAT32 && data_ptr), THROW_TYPE_INFO(typeName(MATPTR_FLOAT32), typeName()));
	return *ptr.mf;
}
MatPtr<float64>& lzh::Element::toF64Array() const
{
	LZH_ACCESS(if (_type == MATPTR_FLOAT64 && data_ptr), THROW_TYPE_INFO(typeName(MATPTR_FLOAT64), typeName()));
	return *ptr.md;
}

Vector& lzh::Element::toVector() const
{
	LZH_ACCESS(if (_type == VECTOR && data_ptr), THROW_TYPE_INFO(typeName(VECTOR), typeName()));
	return *ptr.pm;
}
Mat& lzh::Element::toMat() const
{
	LZH_ACCESS(if ((_type == MAT) && data_ptr), THROW_TYPE_INFO(typeName(MAT), typeName()));
	return *ptr.m;
}

Array& lzh::Element::toImage() const
{
	LZH_ACCESS(if ((_type == ARRAY) && data_ptr), THROW_TYPE_INFO(typeName(ARRAY), typeName()));
	return *ptr.a;
}

Array& lzh::Element::toArray() const
{
	LZH_ACCESS(if ((_type == ARRAY) && data_ptr), THROW_TYPE_INFO(typeName(ARRAY), typeName()));
	return *ptr.a;
}

_OutputArray lzh::Element::toInterFace()const
{
	LZH_ACCESS(if (__RANGE__(VECTOR, _type, ARRAY) && data_ptr), THROW_TYPE_INFO("InputArray", typeName()));
	if (isVector())return _OutputArray(toVector());
	else if (isMat())return _OutputArray(toMat());
	else if (isImage())return _OutputArray(toArray());
	return noArray();
}
std::vector<Element>& lzh::Element::toStdVector() const
{
	LZH_ACCESS(if (_type == STD_VECTOR && data_ptr), THROW_TYPE_INFO(typeName(STD_VECTOR), typeName()));
	return *ptr.arr;
}
std::vector<Element>& lzh::Element::toList() const
{
	return toStdVector();
}
std::map<Element, Element>& lzh::Element::toStdMap() const
{
	LZH_ACCESS(if (_type == STD_MAP && data_ptr), THROW_TYPE_INFO(typeName(STD_MAP), typeName()));
	return *ptr._map;
}
std::map<Element, Element>& lzh::Element::toDict() const
{
	return toStdMap();
}
std::string& lzh::Element::toStdString() const
{
	LZH_ACCESS(if (_type == STRING && data_ptr), THROW_TYPE_INFO(typeName(STRING), typeName()));
	return *ptr.str;
}

void * lzh::Element::data()
{
	return data_ptr;
}

const void * lzh::Element::data() const
{
	return constdata();
}

const void * lzh::Element::constdata() const
{
	return data_ptr;
}

bool lzh::Element::isUserData() const
{
	return _type == InValid;
}

bool lzh::Element::isIntPtr() const
{
	return _type == INTPTR;
}

bool lzh::Element::isNumber() const
{
	return
		_type == INT8 ||
		_type == UINT8 ||
		_type == INT16 ||
		_type == UINT16 ||
		_type == INT32 ||
		_type == UINT32 ||
		_type == INT64 ||
		_type == UINT64 ||
		_type == FLOAT32 ||
		_type == FLOAT64 ||
		_type == FLOAT128;
}

bool lzh::Element::isPointer() const
{
	return
		_type == INT8_PTR ||
		_type == UINT8_PTR ||
		_type == INT16_PTR ||
		_type == UINT16_PTR ||
		_type == INT32_PTR ||
		_type == UINT32_PTR ||
		_type == INT64_PTR ||
		_type == UINT64_PTR ||
		_type == FLOAT32_PTR ||
		_type == FLOAT64_PTR ||
		_type == FLOAT128_PTR;
}

bool lzh::Element::isVec() const
{
	return
		_type == VEC2B ||
		_type == VEC2I ||
		_type == VEC2F ||
		_type == VEC2D ||

		_type == VEC3B ||
		_type == VEC3I ||
		_type == VEC3F ||
		_type == VEC3D ||

		_type == VEC4B ||
		_type == VEC4I ||
		_type == VEC4F ||
		_type == VEC4D;
}

bool lzh::Element::isIndex() const
{
	return isi8() || isu8() ||
		isi16() || isu16() ||
		isi32() || isu32() ||
		isi64() || isu64();
}

bool lzh::Element::isInt() const
{
	return isi32();
}

bool lzh::Element::isFloat() const
{
	return isf32();
}

bool lzh::Element::isDouble() const
{
	return isf64();
}

bool lzh::Element::isbool() const
{
	return _type == BOOL;
}

bool lzh::Element::isi8() const
{
	return _type == INT8;
}

bool lzh::Element::isi16() const
{
	return _type == INT16;
}

bool lzh::Element::isi32() const
{
	return _type == INT32;
}

bool lzh::Element::isi64() const
{
	return _type == INT64;
}

bool lzh::Element::isu8() const
{
	return _type == UINT8;
}

bool lzh::Element::isu16() const
{
	return _type == UINT16;
}

bool lzh::Element::isu32() const
{
	return _type == UINT32;
}

bool lzh::Element::isu64() const
{
	return _type == UINT64;
}

bool lzh::Element::isf32() const
{
	return _type == FLOAT32;
}

bool lzh::Element::isf64() const
{
	return _type == FLOAT64;
}

bool lzh::Element::isf128() const
{
	return _type == FLOAT128;
}

bool lzh::Element::isPtri8() const
{
	return _type == INT8_PTR;
}

bool lzh::Element::isPtri16() const
{
	return _type == INT16_PTR;
}

bool lzh::Element::isPtri32() const
{
	return _type == INT32_PTR;
}

bool lzh::Element::isPtri64() const
{
	return _type == INT64_PTR;
}

bool lzh::Element::isPtru8() const
{
	return _type == UINT8_PTR;
}

bool lzh::Element::isPtru16() const
{
	return _type == UINT16_PTR;
}

bool lzh::Element::isPtru32() const
{
	return _type == UINT32_PTR;
}

bool lzh::Element::isPtru64() const
{
	return _type == UINT64_PTR;
}

bool lzh::Element::isPtrf32() const
{
	return _type == FLOAT32_PTR;
}

bool lzh::Element::isPtrf64() const
{
	return _type == FLOAT64_PTR;
}

bool lzh::Element::isPtrf128() const
{
	return _type == FLOAT128_PTR;
}

bool lzh::Element::isRange() const
{
	return _type == RANGE;
}

bool lzh::Element::isRect() const
{
	return _type == RECT;
}

bool lzh::Element::isRectF() const
{
	return _type == RECTF;
}

bool lzh::Element::isSize() const
{
	return _type == SIZE;
}

bool lzh::Element::isSizeF() const
{
	return _type == SIZEF;
}

bool lzh::Element::isSize3() const
{
	return _type == SIZE3;
}

bool lzh::Element::isSize3F() const
{
	return _type == SIZEF;
}

bool lzh::Element::isPoint() const
{
	return _type == POINT;
}

bool lzh::Element::isPointF() const
{
	return _type == POINTF;
}

bool lzh::Element::isPoint2_() const
{
	return isPoint() || isPointF();
}

bool lzh::Element::isPoint3() const
{
	return _type == POINT3;
}

bool lzh::Element::isPoint3F() const
{
	return _type == POINT3F;
}

bool lzh::Element::isPoint3_() const
{
	return isPoint3() || isPoint3F();
}

bool lzh::Element::isVec2b() const
{
	return _type == VEC2B;
}

bool lzh::Element::isVec2i() const
{
	return _type == VEC2I;
}

bool lzh::Element::isVec2f() const
{
	return _type == VEC2F;
}

bool lzh::Element::isVec2d() const
{
	return _type == VEC2D;
}

bool lzh::Element::isVec3b() const
{
	return _type == VEC3B;
}

bool lzh::Element::isVec3i() const
{
	return _type == VEC3I;
}

bool lzh::Element::isVec3f() const
{
	return _type == VEC3F;
}

bool lzh::Element::isVec3d() const
{
	return _type == VEC3D;
}

bool lzh::Element::isVec4b() const
{
	return _type == VEC4B;
}

bool lzh::Element::isVec4i() const
{
	return _type == VEC4I;
}

bool lzh::Element::isVec4f() const
{
	return _type == VEC4F;
}

bool lzh::Element::isVec4d() const
{
	return _type == VEC4D;
}

bool lzh::Element::isColor() const
{
	return _type == COLOR;
}

bool lzh::Element::isComplex() const
{
	return _type == COMPLEX;
}

bool lzh::Element::isLargeNumber() const
{
	return _type == LARGENUMBER;
}

bool lzh::Element::isU8Ptr() const
{
	return _type == MATPTR_UINT8;
}

bool lzh::Element::isI32Ptr() const
{
	return _type == MATPTR_INT32;
}

bool lzh::Element::isF32Ptr() const
{
	return _type == MATPTR_FLOAT32;
}

bool lzh::Element::isF64Ptr() const
{
	return _type == MATPTR_FLOAT64;
}

bool lzh::Element::isVector() const
{
	return _type == VECTOR;
}

bool lzh::Element::isMat() const
{
	return _type == MAT;
}
bool lzh::Element::isArray() const
{
	return _type == ARRAY;
}
bool lzh::Element::isImage() const
{
	return _type == ARRAY;
}

bool lzh::Element::isString() const
{
	return _type == STRING || _type == INT8_PTR;
}

bool lzh::Element::isStrString() const
{
	return _type == STRING;
}
bool lzh::Element::isList() const
{
	return isStdVector();
}
bool lzh::Element::isStdVector() const
{
	return _type == STD_VECTOR;
}

bool lzh::Element::isDict() const
{
	return isStdMap();
}

bool lzh::Element::isStdMap() const
{
	return _type == STD_MAP;
}

bool lzh::Element::isTuple() const
{
	return _type == STD_TUPLE;
}

void lzh::Element::settype(const char * str)
{
	*this = astype(str);
}

Element lzh::Element::astype(const char *str) const
{
	int32 type = nameType(str);
	if (_type == type)return *this;
	Element elem;
	elem.create(_type);
	if (RANGE__(InValid, _type, FLOAT128)) {
		LZH_ACCESS(if (RANGE__(0, _type, FLOAT128) && RANGE__(0, type, FLOAT128)),
			THROW_INFO_STR("基础类型转换失败, 支持下列基础类型转换{char,uchar,short,ushort,int,uint,long long int,ulong long int,float,double,long double}"));
		switch (type)
		{
		case lzh::Element::BOOL:return toBaseType<bool>();
		case lzh::Element::INT8:return toBaseType<int8>();
		case lzh::Element::UINT8:return toBaseType<uint8>();
		case lzh::Element::INT16:return toBaseType<int16>();
		case lzh::Element::UINT16:return toBaseType<uint16>();
		case lzh::Element::INT32:return toBaseType<int32>();
		case lzh::Element::UINT32:return toBaseType<uint32>();
		case lzh::Element::INT64:return toBaseType<int64>();
		case lzh::Element::UINT64:return toBaseType<uint64>();
		case lzh::Element::FLOAT32:return toBaseType<float32>();
		case lzh::Element::FLOAT64:return toBaseType<float64>();
		case lzh::Element::FLOAT128:return toBaseType<float128>();
		default: break;
		}
	}
	else if (__RANGE__(INT8_PTR, _type, FLOAT128_PTR)) {
		LZH_ACCESS(if (__RANGE__(INT8_PTR, _type, FLOAT128_PTR) && __RANGE__(INT8_PTR, type, FLOAT128_PTR)),
			THROW_INFO_STR("基础指针类型转换失败, 支持下列基础指针类型转换{char*,uchar*,short*,ushort*,int*,uint*,long long int*,ulong long int*,float*,double*,long double*}"));
		switch (type)
		{
		case lzh::Element::INT8:toPtr(&elem.ptr.i8_p); break;
		case lzh::Element::UINT8:toPtr(&elem.ptr.u8_p); break;
		case lzh::Element::INT16:toPtr(&elem.ptr.i16_p); break;
		case lzh::Element::UINT16:toPtr(&elem.ptr.u16_p); break;
		case lzh::Element::INT32:toPtr(&elem.ptr.i32_p); break;
		case lzh::Element::UINT32:toPtr(&elem.ptr.u32_p); break;
		case lzh::Element::INT64:toPtr(&elem.ptr.i64_p); break;
		case lzh::Element::UINT64:toPtr(&elem.ptr.u64_p); break;
		case lzh::Element::FLOAT32:toPtr(&elem.ptr.f32_p); break;
		case lzh::Element::FLOAT64:toPtr(&elem.ptr.f64_p); break;
		case lzh::Element::FLOAT128:toPtr(&elem.ptr.f128_p); break;
		default: break;
		}
	}
	else if (__RANGE__(RANGE, _type, SIZE3)) {
		LZH_ACCESS(if (__RANGE__(RANGE, _type, SIZE3) && __RANGE__(INT8_PTR, type, FLOAT128_PTR)),
			THROW_INFO_STR("范围类型转换失败, 暂不支持下列范围类型转换{Range, Rect, RectF, Size, Sizef, Size3, Size3f}"));
	}
	else if (__RANGE__(POINT, _type, POINTF)) {
		LZH_ACCESS(if (__RANGE__(POINT, _type, POINTF) && __RANGE__(POINT, type, POINTF)),
			THROW_INFO_STR("二维坐标点类型转换失败, 支持下列二维坐标点类型转换{Point, Point2f}"));
		if (type == POINT)elem.toPoint() = toPointF();
		else if (type == POINTF)elem.toPointF() = toPoint();
	}
	else if (__RANGE__(POINT3, _type, POINT3F)) {
		LZH_ACCESS(if (__RANGE__(POINT3, _type, POINT3F) && __RANGE__(POINT3, type, POINT3F)),
			THROW_INFO_STR("坐标点类型转换失败, 支持下列坐标点类型转换{Point3, Point3f}"));
		if (type == POINT3)elem.toPoint3() = toPoint3F();
		else if (type == POINT3F)elem.toPoint3F() = toPoint3();
	}
	else if (__RANGE__(VEC2B, _type, VEC2D) || _type == COMPLEX) {
		LZH_ACCESS(if ((__RANGE__(VEC2B, _type, VEC2D) || _type == COMPLEX)
			&& (__RANGE__(VEC2B, type, VEC2D) || _type == COMPLEX)),
			THROW_INFO_STR("二维向量类型转换失败, 支持下列二维向量类型转换{Vec2b, Vec2i, Vec2f, Vec2d}"));
		if (type == VEC2B) {
			if (_type == VEC2I) elem.toVec2b() = toVec2i();
			else if (_type == VEC2F) elem.toVec2b() = toVec2f();
			else if (_type == VEC2D) elem.toVec2b() = toVec2d();
			else if (_type == COMPLEX) {
				Complex complex = toComplex();
				elem.toVec2b() = Vec2b(complex.r, complex.i);
			}
		}
		else if (type == VEC2I) {
			if (_type == VEC2B) elem.toVec2i() = toVec2b();
			else if (_type == VEC2F) elem.toVec2i() = toVec2f();
			else if (_type == VEC2D) elem.toVec2i() = toVec2d();
			else if (_type == COMPLEX) {
				Complex complex = toComplex();
				elem.toVec2i() = Vec2i(complex.r, complex.i);
			}
		}
		else if (type == VEC2F) {
			if (_type == VEC2B) elem.toVec2f() = toVec2b();
			else if (_type == VEC2I) elem.toVec2f() = toVec2i();
			else if (_type == VEC2D) elem.toVec2f() = toVec2d();
			else if (_type == COMPLEX) {
				Complex complex = toComplex();
				elem.toVec2f() = Vec2f(complex.r, complex.i);
			}
		}
		else if (type == VEC2D) {
			if (_type == VEC2B) elem.toVec2d() = toVec2b();
			else if (_type == VEC2I) elem.toVec2d() = toVec2i();
			else if (_type == VEC2F) elem.toVec2d() = toVec2f();
			else if (_type == COMPLEX) {
				Complex complex = toComplex();
				elem.toVec2d() = Vec2d(complex.r, complex.i);
			}
		}
		else if (type == COMPLEX) {
			if (_type == VEC2B) {
				Vec2b vec = toVec2b();
				elem.toComplex() = Complex(vec[0], vec[1]);
			}
			else if (_type == VEC2I) {
				Vec2b vec = toVec2i();
				elem.toComplex() = Complex(vec[0], vec[1]);
			}
			else if (_type == VEC2F) {
				Vec2b vec = toVec2f();
				elem.toComplex() = Complex(vec[0], vec[1]);
			}
			else if (_type == VEC2D) {
				Vec2b vec = toVec2d();
				elem.toComplex() = Complex(vec[0], vec[1]);
			}
		}
	}
	else if (__RANGE__(VEC3B, _type, VEC3D) || _type == COLOR) {
		LZH_ACCESS(if ((__RANGE__(VEC3B, _type, VEC3D) || _type == COLOR) && (__RANGE__(VEC3B, type, VEC3D) || _type == COLOR)),
			THROW_INFO_STR("三维向量类型转换失败, 支持下列三维向量类型转换{Vec3b, Vec3i, Vec3f, Vec3d}"));
		if (type == VEC3B) {
			if (_type == VEC3I) elem.toVec3b() = toVec3i();
			else if (_type == VEC3F) elem.toVec3b() = toVec3f();
			else if (_type == VEC3D) elem.toVec3b() = toVec3d();
			else if (_type == COLOR) elem.toVec3b() = toColor();
		}
		else if (type == VEC3I) {
			if (_type == VEC3B) elem.toVec3i() = toVec3b();
			else if (_type == VEC3F) elem.toVec3i() = toVec3f();
			else if (_type == VEC3D) elem.toVec3i() = toVec3d();
			else if (_type == COLOR) elem.toVec3i() = toColor();
		}
		else if (type == VEC3F) {
			if (_type == VEC3B) elem.toVec3f() = toVec3b();
			else if (_type == VEC3I) elem.toVec3f() = toVec3i();
			else if (_type == VEC3F) elem.toVec3f() = toVec3f();
			else if (_type == COLOR) elem.toVec3f() = toColor();
		}
		else if (type == VEC3D) {
			if (_type == VEC3B) elem.toVec3d() = toVec3b();
			else if (_type == VEC3I) elem.toVec3d() = toVec3i();
			else if (_type == VEC3D) elem.toVec3d() = toVec3d();
			else if (_type == COLOR) elem.toVec3d() = toColor();
		}
		else if (type == COLOR) {
			if (_type == VEC3B) elem.toColor().setVec3(toVec3b());
			else if (_type == VEC3I) elem.toColor().setVec3(toVec3i());
			else if (_type == VEC3F) elem.toColor().setVec3(toVec3f());
			else if (_type == VEC3D) elem.toColor().setVec3(toVec3d());
		}
	}
	else if (__RANGE__(VEC4B, _type, COLOR)) {
		LZH_ACCESS(if (__RANGE__(VEC4B, _type, COLOR) && __RANGE__(VEC4B, type, COLOR)),
			THROW_INFO_STR("四维向量类型转换失败, 支持下列四维向量类型转换{Vec4b, Vec4i, Vec4f, Vec4d, Color}"));
		if (type == VEC4B) {
			if (_type == VEC4I) elem.toVec4b() = toVec4i();
			else if (_type == VEC4F) elem.toVec4b() = toVec4f();
			else if (_type == VEC4D) elem.toVec4b() = toVec4d();
			else if (_type == COLOR) elem.toVec4b() = toColor();
		}
		else if (type == VEC4I) {
			if (_type == VEC4B) elem.toVec4i() = toVec4b();
			else if (_type == VEC4F) elem.toVec4i() = toVec4f();
			else if (_type == VEC4D) elem.toVec4i() = toVec4d();
			else if (_type == COLOR) elem.toVec4i() = toColor();
		}
		else if (type == VEC4F) {
			if (_type == VEC4B) elem.toVec4f() = toVec4b();
			else if (_type == VEC4I) elem.toVec4f() = toVec4i();
			else if (_type == VEC4D) elem.toVec4f() = toVec4d();
			else if (_type == COLOR) elem.toVec4f() = toColor();
		}
		else if (type == VEC4D) {
			if (_type == VEC4B) elem.toVec4d() = toVec4b();
			else if (_type == VEC4I) elem.toVec4d() = toVec4i();
			else if (_type == VEC4F) elem.toVec4d() = toVec4f();
			else if (_type == COLOR) elem.toVec4d() = toColor();
		}
		else if (type == COLOR) {
			if (_type == VEC4B) elem.toColor().setVec4(toVec4b());
			else if (_type == VEC4I) elem.toColor().setVec4(toVec4i());
			else if (_type == VEC4F) elem.toColor().setVec4(toVec4f());
			else if (_type == VEC4D) elem.toColor().setVec4(toVec4d());
		}
	}
	else if (_type == INT8_PTR || _type == UINT8_PTR || _type == STRING || _type == LARGENUMBER) {
		LZH_ACCESS(if ((type == INT8_PTR || type == UINT8_PTR || type == STRING || type == LARGENUMBER)
			&& (_type == INT8_PTR || _type == UINT8_PTR || _type == STRING || _type == LARGENUMBER)),
			THROW_INFO_STR("字符串类型转换失败, 支持字符串类型转换{int8*,uint8*,LargeNumber,String}"));
		int8 *p1 = nullptr;
		const int8 *p2 = nullptr;
		if (_type == INT8_PTR) p2 = toI8Ptr();
		else if (_type == UINT8_PTR) p2 = (int8*)toU8Ptr();
		else if (_type == STRING)  p2 = toStdString().c_str();
		else if (_type == LARGENUMBER) p2 = toLargeNumber().data();
		if (type == INT8_PTR)p1 = elem.toI8Ptr();
		else if (type == UINT8_PTR) p1 = (int8*)elem.toU8Ptr();
		else if (type == STRING && p2) { elem.toStdString() = p2; }
		else if (type == LARGENUMBER && p2) { elem.toLargeNumber() = p2; }
		if (p1 && p2)memcpy(p1, p2, sizeof(int8)*strlen(p2));
	}
	else if (_type == VECTOR) {
		LZH_ACCESS(if (type == VECTOR && (_type == MAT || _type == ARRAY)),
			THROW_INFO_STR("向量类型转换失败, 支持向量类型转换{Vector,MAT,Array}"));
		if (type == ARRAY)elem.toArray() = Array(toVector());
		else if (type == MAT)elem.toMat() = Array(toVector());
	}
	else if (__RANGE__(MAT, _type, ARRAY)) {
		LZH_ACCESS(if (__RANGE__(MAT, _type, ARRAY) && __RANGE__(MAT, type, ARRAY)),
			THROW_INFO_STR("矩阵类型转换失败, 支持类型转换{MAT,Array}"));
		if (type == ARRAY)elem.toArray() = toMat().toArray();
		else if (type == MAT)elem.toMat() = toArray();
	}
	return elem;
}

int32 lzh::Element::type() const
{
	return _type;
}
int32 lzh::Element::len() const
{
	if (empty())return 0;
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::FUNCTION:
	case lzh::Element::INT8:
	case lzh::Element::UINT8:
	case lzh::Element::INT16:
	case lzh::Element::UINT16:
	case lzh::Element::INT32:
	case lzh::Element::UINT32:
	case lzh::Element::INT64:
	case lzh::Element::UINT64:
	case lzh::Element::FLOAT32:
	case lzh::Element::FLOAT64:
	case lzh::Element::FLOAT128:
	case lzh::Element::INT8_PTR:
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:return 1;
	case lzh::Element::RANGE: return 2;
	case lzh::Element::RECT: return 4;
	case lzh::Element::RECTF: return 4;
	case lzh::Element::SIZE: return 2;
	case lzh::Element::SIZEF: return 2;
	case lzh::Element::SIZE3: return 3;
	case lzh::Element::SIZE3F: return 3;
	case lzh::Element::POINT:
	case lzh::Element::POINTF: return 2;
	case lzh::Element::POINT3:
	case lzh::Element::POINT3F: return 3;
	case lzh::Element::VEC2B:
	case lzh::Element::VEC2I:
	case lzh::Element::VEC2F:
	case lzh::Element::VEC2D: return 2;
	case lzh::Element::VEC3B:
	case lzh::Element::VEC3I:
	case lzh::Element::VEC3F:
	case lzh::Element::VEC3D: return 3;
	case lzh::Element::VEC4B:
	case lzh::Element::VEC4I:
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D:
	case lzh::Element::COLOR: return 4;
	case lzh::Element::COMPLEX: return 2;
	case lzh::Element::LARGENUMBER: return (int32)toLargeNumber().size();
	case lzh::Element::MATPTR_UINT8: return toU8Array().size();
	case lzh::Element::MATPTR_INT32: return toI32Array().size();
	case lzh::Element::MATPTR_FLOAT32: return toF32Array().size();
	case lzh::Element::MATPTR_FLOAT64: return toF64Array().size();
	case lzh::Element::VECTOR: return (int32)toVector().size();
	case lzh::Element::STRING: return (int32)toStdString().size();
	case lzh::Element::MAT: return toMat().len();
	case lzh::Element::ARRAY: return toArray().len();
	case lzh::Element::STD_VECTOR: return (int32)toStdVector().size();
	case lzh::Element::STD_MAP: return (int32)toStdMap().size();
	case lzh::Element::STD_TUPLE: 
#ifdef _WIN64
		return (int32)ptr.u64;
#else
		return (int32)ptr.u32;
#endif // _WIN64
	default:return 0;
	}
}
int32 lzh::Element::rows() const
{
	if (_type == lzh::Element::MAT)
		return toMat().rows();
	else if (_type == lzh::Element::ARRAY)
		return toArray().rows;
	else return len();
}
int32 lzh::Element::cols() const
{
	if (_type == lzh::Element::MAT)
		return toMat().cols();
	else if (_type == lzh::Element::ARRAY)
		return toArray().cols;
	else return len();
}
Size lzh::Element::size() const
{
	if (_type == lzh::Element::MAT)
		return toMat().size();
	else if (_type == lzh::Element::ARRAY)
		return toArray().size();
	else return len();
}
Size3 lzh::Element::size3() const
{
	if (_type == lzh::Element::MAT)
		return toMat().size3();
	else if (_type == lzh::Element::ARRAY)
		return toArray().size3();
	else return Size3(len());
}
const char * lzh::Element::typeName(int32 type)
{
	switch (type)
	{
	case lzh::Element::InValid:return "Null";
	case lzh::Element::INTPTR:return typeid(ptr.user).name();
	case lzh::Element::STD_FUNCTION:return typeid(ptr.user).name();
	case lzh::Element::FUNCTION:return typeid(ptr.user).name();
	case lzh::Element::BOOL:return typeid(ptr.b).name();
	case lzh::Element::INT8:return typeid(ptr.i8).name();
	case lzh::Element::UINT8:return typeid(ptr.u8).name();
	case lzh::Element::INT16:return typeid(ptr.i16).name();
	case lzh::Element::UINT16:return typeid(ptr.u16).name();
	case lzh::Element::INT32:return typeid(ptr.i32).name();
	case lzh::Element::UINT32:return typeid(ptr.u32).name();
	case lzh::Element::INT64:return typeid(ptr.i64).name();
	case lzh::Element::UINT64:return typeid(ptr.u64).name();
	case lzh::Element::FLOAT32:return typeid(ptr.f32).name();
	case lzh::Element::FLOAT64:return typeid(ptr.f64).name();
	case lzh::Element::FLOAT128:return typeid(ptr.f128).name();
	case lzh::Element::INT8_PTR:return typeid(ptr.i8_p).name();
	case lzh::Element::UINT8_PTR:return typeid(ptr.u8_p).name();
	case lzh::Element::INT16_PTR:return typeid(ptr.i16_p).name();
	case lzh::Element::UINT16_PTR:return typeid(ptr.u16_p).name();
	case lzh::Element::INT32_PTR:return typeid(ptr.i32_p).name();
	case lzh::Element::UINT32_PTR:return typeid(ptr.u32_p).name();
	case lzh::Element::INT64_PTR:return typeid(ptr.i64_p).name();
	case lzh::Element::UINT64_PTR:return typeid(ptr.u64_p).name();
	case lzh::Element::FLOAT32_PTR:return typeid(ptr.f32_p).name();
	case lzh::Element::FLOAT64_PTR:return typeid(ptr.f64_p).name();
	case lzh::Element::FLOAT128_PTR:return typeid(ptr.f128_p).name();
	case lzh::Element::RANGE: return typeid(*ptr.ra).name();
	case lzh::Element::RECT: return typeid(*ptr.r).name();
	case lzh::Element::RECTF: return typeid(*ptr.rf).name();
	case lzh::Element::SIZE: return typeid(*ptr.s2).name();
	case lzh::Element::SIZEF: return typeid(*ptr.s2f).name();
	case lzh::Element::SIZE3: return typeid(*ptr.s3i).name();
	case lzh::Element::SIZE3F: return typeid(*ptr.s3f).name();
	case lzh::Element::POINT: return typeid(*ptr.p2i).name();
	case lzh::Element::POINTF: return typeid(*ptr.p2f).name();
	case lzh::Element::POINT3: return typeid(*ptr.p3i).name();
	case lzh::Element::POINT3F: return typeid(*ptr.p3f).name();
	case lzh::Element::VEC2B: return typeid(*ptr.v2b).name();
	case lzh::Element::VEC2I: return typeid(*ptr.v2i).name();
	case lzh::Element::VEC2F: return typeid(*ptr.v2f).name();
	case lzh::Element::VEC2D: return typeid(*ptr.v2d).name();
	case lzh::Element::VEC3B: return typeid(*ptr.v3b).name();
	case lzh::Element::VEC3I: return typeid(*ptr.v3i).name();
	case lzh::Element::VEC3F: return typeid(*ptr.v3f).name();
	case lzh::Element::VEC3D: return typeid(*ptr.v3d).name();
	case lzh::Element::VEC4B: return typeid(*ptr.v4b).name();
	case lzh::Element::VEC4I: return typeid(*ptr.v4i).name();
	case lzh::Element::VEC4F: return typeid(*ptr.v4f).name();
	case lzh::Element::VEC4D: return typeid(*ptr.v4d).name();
	case lzh::Element::COLOR: return typeid(*ptr.rgb).name();
	case lzh::Element::COMPLEX: return typeid(*ptr.c).name();
	case lzh::Element::LARGENUMBER: return typeid(*ptr.ln).name();
	case lzh::Element::MATPTR_UINT8: return typeid(*ptr.m8i).name();
	case lzh::Element::MATPTR_INT32: return typeid(*ptr.m32i).name();
	case lzh::Element::MATPTR_FLOAT32: return typeid(*ptr.mf).name();
	case lzh::Element::MATPTR_FLOAT64: return typeid(*ptr.md).name();
	case lzh::Element::VECTOR: return typeid(*ptr.pm).name();
	case lzh::Element::STRING: return typeid(*ptr.str).name(); //return "string";
	case lzh::Element::MAT: return typeid(*ptr.m).name(); //return "Mat";
	case lzh::Element::ARRAY: return typeid(*ptr.a).name(); //return "image";
	case lzh::Element::STD_VECTOR: return typeid(*ptr.arr).name(); //return "vecter";
	case lzh::Element::STD_MAP: return typeid(*ptr._map).name(); 
	case lzh::Element::STD_TUPLE: return typeid(ptr.user).name();
	default:return nullptr;
	}
}
int32 lzh::Element::nameType(const char * name)
{
	for (int32 type = 0; type <= Element::INTPTR; type++) {
		if (strcmp(name, typeName(type)) == 0)return type;
	}
	THROW_INFO(ERR_INFO_TYPE);
}
const char* lzh::Element::typeName()const
{
	return _typeName.data();
}
void lzh::Element::show() const
{
	show(std::cout);
	std::cout << std::endl;
}

std::ostream & lzh::Element::show(std::ostream & out) const
{
	switch (_type)
	{
	case lzh::Element::InValid:return out << "<User Object, type(" << _typeName << ")>";//"<user data (unknow type)>";
	case lzh::Element::INTPTR:return out << toIntPtr();
	case lzh::Element::FUNCTION:return out << "<type: " << _typeName << ", address(" << ptr.user << ")>";
	case lzh::Element::STD_FUNCTION:return out << "<type: " << _typeName << ", address(" << ptr.user << ")>";
	case lzh::Element::BOOL:return out << tobool();
	case lzh::Element::INT8:return out << toi8();
	case lzh::Element::UINT8:return out << tou8();
	case lzh::Element::INT16:return out << toi16();
	case lzh::Element::UINT16:return out << tou16();
	case lzh::Element::INT32:return out << toi32();
	case lzh::Element::UINT32:return out << tou32();
	case lzh::Element::INT64:return out << toi64();
	case lzh::Element::UINT64:return out << tou64();
	case lzh::Element::FLOAT32:return out << tof32();
	case lzh::Element::FLOAT64:return out << tof64();
	case lzh::Element::FLOAT128:return out << tof128();
	case lzh::Element::INT8_PTR:return out << ptr.i8_p;
	case lzh::Element::UINT8_PTR:return out << "<pointer, address(" << ptr.u8_p << ")>";
	case lzh::Element::INT16_PTR:return out << "<pointer, address(" << ptr.i16_p << ")>";
	case lzh::Element::UINT16_PTR:return out << "<pointer, address(" << ptr.u16_p << ")>";
	case lzh::Element::INT32_PTR:return out << "<pointer, address(" << ptr.i32_p << ")>";
	case lzh::Element::UINT32_PTR:return out << "<pointer, address(" << ptr.u32_p << ")>";
	case lzh::Element::INT64_PTR:return out << "<pointer, address(" << ptr.i64_p << ")>";
	case lzh::Element::UINT64_PTR:return out << "<pointer, address(" << ptr.u64_p << ")>";
	case lzh::Element::FLOAT32_PTR:return out << "<pointer, address(" << ptr.f32_p << ")>";
	case lzh::Element::FLOAT64_PTR:return out << "<pointer, address(" << ptr.f64_p << ")>";
	case lzh::Element::FLOAT128_PTR:return out << "<pointer, address(" << ptr.f128_p << ")>";
	case lzh::Element::RANGE: return out << *ptr.ra;
	case lzh::Element::RECT: return out << *ptr.r;
	case lzh::Element::RECTF: return out << *ptr.rf;
	case lzh::Element::SIZE: return out << *ptr.s2;
	case lzh::Element::SIZEF: return out << *ptr.s2f;
	case lzh::Element::SIZE3: return out << *ptr.s3i;
	case lzh::Element::SIZE3F: return out << *ptr.s3f;
	case lzh::Element::POINT: return out << *ptr.p2i;
	case lzh::Element::POINTF: return out << *ptr.p2f;
	case lzh::Element::POINT3: return out << *ptr.p3i;
	case lzh::Element::POINT3F:  return out << *ptr.p3f;
	case lzh::Element::VEC2B: return out << *ptr.v2b;
	case lzh::Element::VEC2I: return out << *ptr.v2i;
	case lzh::Element::VEC2F: return out << *ptr.v2f;
	case lzh::Element::VEC2D: return out << *ptr.v2d;
	case lzh::Element::VEC3B: return out << *ptr.v3b;
	case lzh::Element::VEC3I: return out << *ptr.v3i;
	case lzh::Element::VEC3F: return out << *ptr.v3f;
	case lzh::Element::VEC3D: return out << *ptr.v3d;
	case lzh::Element::VEC4B: return out << *ptr.v4b;
	case lzh::Element::VEC4I: return out << *ptr.v4i;
	case lzh::Element::VEC4F: return out << *ptr.v4f;
	case lzh::Element::VEC4D: return out << *ptr.v4d;
	case lzh::Element::COLOR: return out << *ptr.rgb;
	case lzh::Element::COMPLEX: return out << *ptr.c;
	case lzh::Element::LARGENUMBER: return out << *ptr.ln;
	case lzh::Element::MATPTR_UINT8: return out << *ptr.m8i;
	case lzh::Element::MATPTR_INT32: return out << *ptr.m32i;
	case lzh::Element::MATPTR_FLOAT32: return out << *ptr.mf;
	case lzh::Element::MATPTR_FLOAT64: return out << *ptr.md;
	case lzh::Element::VECTOR: return out << *ptr.pm;
	case lzh::Element::STRING: return out << "\"" << *ptr.str << "\"";
	case lzh::Element::MAT: return out << *ptr.m;
	case lzh::Element::ARRAY: return out << *ptr.a;
	case lzh::Element::STD_VECTOR: return out << ptr.arr->Data();
	case lzh::Element::STD_MAP: return out << ptr._map->Data();
	case lzh::Element::STD_TUPLE: return out << "<type: " << _typeName << ", address(" << ptr.user << ")>";
	default:return out;
	}
}

Element & lzh::Element::operator[](bool key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](int8 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](int16 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](int32 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](int64 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](uint8 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](uint16 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](uint32 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](uint64 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](float32 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](float64 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](float128 key)
{
	return operator[](Element(key));
}

Element & lzh::Element::operator[](const char * key)
{
	LZH_ACCESS(if (isDict()), THROW_TYPE_INFO("Dict", typeName()));
	return ptr._map->Data()[key];
}
Element & lzh::Element::operator[](const Element & key)
{
	if (isDict()) return ptr._map->Data()[key];
	else if (isList() && key.isIndex()) return ptr.arr->Data()[key];
	else THROW_TYPE_INFO("list or Dict", typeName());
}

const Element& Element::operator [](bool key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](int8 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](int16 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](int32 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](int64 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](uint8 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](uint16 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](uint32 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](uint64 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](float32 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](float64 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](float128 key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](const char * key)const {
	return operator[](Element(key));
}
const Element& Element::operator [](const Element & key)const {
	if (isList() && key.isIndex()) return ptr.arr->Data()[key];
	else THROW_TYPE_INFO("list", typeName());
}

double& lzh::Element::operator ()(int index) {
	if (isMat())return ptr.m->operator()(index);
	else THROW_TYPE_INFO("Mat", typeName());
}
double& lzh::Element::operator ()(int index_1, int index_2) {
	if (isMat())return ptr.m->operator()(index_1, index_2);
	else THROW_TYPE_INFO("Mat", typeName());
}
double& lzh::Element::operator ()(int index_1, int index_2, int index_3) {
	if (isMat())return ptr.m->operator()(index_1, index_2, index_3);
	else THROW_TYPE_INFO("Mat", typeName());
}
typename Element::iterator lzh::Element::begin() {
	LZH_ACCESS(if (supportIterator()), THROW_INFO(ERR_INFO_SUPPORT));
	return iterator(*this, true);
}
typename Element::const_iterator lzh::Element::begin()const {
	LZH_ACCESS(if (supportIterator()), THROW_INFO(ERR_INFO_SUPPORT));
	return const_iterator(*this, true);
}
typename Element::iterator lzh::Element::end() {
	LZH_ACCESS(if (supportIterator()), THROW_INFO(ERR_INFO_SUPPORT));
	return iterator(*this, false);
}
typename Element::const_iterator lzh::Element::end()const {
	LZH_ACCESS(if (supportIterator()), THROW_INFO(ERR_INFO_SUPPORT));
	return const_iterator(*this, false);
}

lzh::Element::operator bool() const
{
	return toBaseType<bool>();
}

lzh::Element::operator int8() const
{
	return toBaseType<int8>();
}

lzh::Element::operator int16() const
{
	return toBaseType<int16>();
}

lzh::Element::operator int32() const
{
	return toBaseType<int32>();
}

lzh::Element::operator int64() const
{
	return toBaseType<int64>();
}

lzh::Element::operator uint8() const
{
	return toBaseType<uint8>();
}

lzh::Element::operator uint16() const
{
	return toBaseType<uint16>();
}

lzh::Element::operator uint32() const
{
	return toBaseType<uint32>();
}

lzh::Element::operator uint64() const
{
	return toBaseType<uint64>();
}

lzh::Element::operator float32() const
{
	return toBaseType<float32>();
}

lzh::Element::operator float64() const
{
	return toBaseType<float64>();
}

lzh::Element::operator float128() const
{
	return toBaseType<float128>();
}

lzh::Element::operator const char * () const
{
	return toCString();
}

lzh::Element::operator string () const
{
	return toCString();
}

bool lzh::Element::operator==(const Element & elem) const
{
	if (elem.type() != _type) {
		if (isNumber() && elem.isNumber()) {
			return isEqual(toNumber(), elem.toNumber());
		}
		else return data_ptr == elem.data_ptr;
	}
	size_t size = 0;
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::FUNCTION:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::STD_TUPLE:return data_ptr == elem.data_ptr;
	case lzh::Element::BOOL:return tobool() == elem.tobool();
	case lzh::Element::INT8:return toi8() == elem.toi8();
	case lzh::Element::UINT8:return tou8() == elem.tou8();
	case lzh::Element::INT16:return toi16() == elem.toi16();
	case lzh::Element::UINT16:return tou16() == elem.tou16();
	case lzh::Element::INT32:return toi32() == elem.toi32();
	case lzh::Element::UINT32:return tou32() == elem.tou32();
	case lzh::Element::INT64:return toi64() == elem.toi64();
	case lzh::Element::UINT64:return tou64() == elem.tou64();
	case lzh::Element::FLOAT32:return lzh::isEqual(tof32(), elem.tof32());
	case lzh::Element::FLOAT64:return lzh::isEqual(tof64(), elem.tof64());
	case lzh::Element::FLOAT128:return lzh::isEqual(tof128(), elem.tof128());
	case lzh::Element::INT8_PTR:return strcmp((const char*)data_ptr, (const char*)elem.data_ptr) == 0;
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:return data_ptr == elem.data_ptr;
	case lzh::Element::RANGE: if (size == 0)size = sizeof(Range);
	case lzh::Element::RECT: 
	case lzh::Element::RECTF: 
	case lzh::Element::SIZE: 
	case lzh::Element::SIZEF: 
	case lzh::Element::SIZE3: 
	case lzh::Element::SIZE3F: 
	case lzh::Element::POINT: 
	case lzh::Element::POINTF: 
	case lzh::Element::POINT3: 
	case lzh::Element::POINT3F:  
	case lzh::Element::VEC2B: 
	case lzh::Element::VEC2I: 
	case lzh::Element::VEC2F: 
	case lzh::Element::VEC2D: 
	case lzh::Element::VEC3B: 
	case lzh::Element::VEC3I: 
	case lzh::Element::VEC3F: 
	case lzh::Element::VEC3D: 
	case lzh::Element::VEC4B: 
	case lzh::Element::VEC4I: 
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D: 
	case lzh::Element::COLOR: 
	case lzh::Element::COMPLEX: 
	case lzh::Element::LARGENUMBER: return memcmp(data_ptr, elem.data_ptr, size) == 0;
	case lzh::Element::MATPTR_UINT8: return *ptr.m8i == *elem.ptr.m8i;
	case lzh::Element::MATPTR_INT32: return *ptr.m32i == *elem.ptr.m32i;
	case lzh::Element::MATPTR_FLOAT32: return *ptr.mf == *elem.ptr.mf;
	case lzh::Element::MATPTR_FLOAT64: return *ptr.md == *elem.ptr.md;
	case lzh::Element::VECTOR: return *ptr.pm == *elem.ptr.pm;
	case lzh::Element::STRING: return *ptr.str == *elem.ptr.str;
	case lzh::Element::MAT: return *ptr.m == *elem.ptr.m;
	case lzh::Element::ARRAY: return *ptr.a == *elem.ptr.a;
	case lzh::Element::STD_VECTOR: return ptr.arr->Data() == elem.ptr.arr->Data();
	case lzh::Element::STD_MAP: return ptr._map->Data() == elem.ptr._map->Data(); 
	default:return false;
	}
}

bool lzh::Element::operator!=(const Element & elem) const
{
	return !(*this == elem);
}

bool lzh::Element::operator>(const Element & elem) const
{
	if (elem.type() != _type) {
		if (isNumber() && elem.isNumber()) {
			return toNumber() > elem.toNumber();
		}
		if (isString() && elem.isNumber()) {
			return toCString() > Convert::toString(elem.toNumber());
		}
		else if (elem.isString() && isNumber()) {
			return elem.toCString() < Convert::toString(toNumber());
		}
		else return data_ptr > elem.data_ptr;
	}
	size_t size = 0;
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::FUNCTION:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::STD_TUPLE:return data_ptr > elem.data_ptr;
	case lzh::Element::BOOL:return tobool() > elem.tobool();
	case lzh::Element::INT8:return toi8() > elem.toi8();
	case lzh::Element::UINT8:return tou8() > elem.tou8();
	case lzh::Element::INT16:return toi16() > elem.toi16();
	case lzh::Element::UINT16:return tou16() > elem.tou16();
	case lzh::Element::INT32:return toi32() > elem.toi32();
	case lzh::Element::UINT32:return tou32() > elem.tou32();
	case lzh::Element::INT64:return toi64() > elem.toi64();
	case lzh::Element::UINT64:return tou64() > elem.tou64();
	case lzh::Element::FLOAT32:return tof32() > elem.tof32();
	case lzh::Element::FLOAT64:return tof64() > elem.tof64();
	case lzh::Element::FLOAT128:return tof128() > elem.tof128();
	case lzh::Element::INT8_PTR:return strcmp((const char*)data_ptr, (const char*)elem.data_ptr) > 0;
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:return data_ptr > elem.data_ptr;
	case lzh::Element::RANGE: if (size == 0)size = sizeof(Range);
	case lzh::Element::RECT:
	case lzh::Element::RECTF:
	case lzh::Element::SIZE:
	case lzh::Element::SIZEF:
	case lzh::Element::SIZE3:
	case lzh::Element::SIZE3F:
	case lzh::Element::POINT:
	case lzh::Element::POINTF:
	case lzh::Element::POINT3:
	case lzh::Element::POINT3F:
	case lzh::Element::VEC2B:
	case lzh::Element::VEC2I:
	case lzh::Element::VEC2F:
	case lzh::Element::VEC2D:
	case lzh::Element::VEC3B:
	case lzh::Element::VEC3I:
	case lzh::Element::VEC3F:
	case lzh::Element::VEC3D:
	case lzh::Element::VEC4B:
	case lzh::Element::VEC4I:
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D:
	case lzh::Element::COLOR:
	case lzh::Element::COMPLEX:
	case lzh::Element::LARGENUMBER: return memcmp(data_ptr, elem.data_ptr, size) > 0;
	case lzh::Element::MATPTR_UINT8: return false;
	case lzh::Element::MATPTR_INT32: return false;
	case lzh::Element::MATPTR_FLOAT32: return false;
	case lzh::Element::MATPTR_FLOAT64: return false;
	case lzh::Element::VECTOR: return false;
	case lzh::Element::STRING: return *ptr.str > *elem.ptr.str;
	case lzh::Element::MAT: return false;;
	case lzh::Element::ARRAY: return false;;
	case lzh::Element::STD_VECTOR: return ptr.arr->Data() > elem.ptr.arr->Data();
	case lzh::Element::STD_MAP: return ptr._map->Data() > elem.ptr._map->Data();
	default:return false;
	}
}

bool lzh::Element::operator>=(const Element & elem) const
{
	return !(*this < elem);
}

bool lzh::Element::operator<=(const Element & elem) const
{
	return !(*this > elem);
}

bool lzh::Element::operator<(const Element & elem) const
{
	if (elem.type() != _type) {
		if (isNumber() && elem.isNumber()) {
			return toNumber() < elem.toNumber();
		}
		else if (isString() && elem.isNumber()) {
			return toCString() < Convert::toString(elem.toNumber());
		}
		else if (elem.isString() && isNumber()) {
			return elem.toCString() > Convert::toString(toNumber());
		}
		else return data_ptr < elem.data_ptr;
	}
	size_t size = 0;
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::FUNCTION:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::STD_TUPLE:return data_ptr < elem.data_ptr;
	case lzh::Element::BOOL:return tobool() < elem.tobool();
	case lzh::Element::INT8:return toi8() < elem.toi8();
	case lzh::Element::UINT8:return tou8() < elem.tou8();
	case lzh::Element::INT16:return toi16() < elem.toi16();
	case lzh::Element::UINT16:return tou16() < elem.tou16();
	case lzh::Element::INT32:return toi32() < elem.toi32();
	case lzh::Element::UINT32:return tou32() < elem.tou32();
	case lzh::Element::INT64:return toi64() < elem.toi64();
	case lzh::Element::UINT64:return tou64() < elem.tou64();
	case lzh::Element::FLOAT32:return tof32() < elem.tof32();
	case lzh::Element::FLOAT64:return tof64() < elem.tof64();
	case lzh::Element::FLOAT128:return tof128() < elem.tof128();
	case lzh::Element::INT8_PTR:return strcmp((const char*)data_ptr, (const char*)elem.data_ptr) < 0;
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:return data_ptr < elem.data_ptr;
	case lzh::Element::RANGE: if (size == 0)size = sizeof(Range);
	case lzh::Element::RECT:
	case lzh::Element::RECTF:
	case lzh::Element::SIZE:
	case lzh::Element::SIZEF:
	case lzh::Element::SIZE3:
	case lzh::Element::SIZE3F:
	case lzh::Element::POINT:
	case lzh::Element::POINTF:
	case lzh::Element::POINT3:
	case lzh::Element::POINT3F:
	case lzh::Element::VEC2B:
	case lzh::Element::VEC2I:
	case lzh::Element::VEC2F:
	case lzh::Element::VEC2D:
	case lzh::Element::VEC3B:
	case lzh::Element::VEC3I:
	case lzh::Element::VEC3F:
	case lzh::Element::VEC3D:
	case lzh::Element::VEC4B:
	case lzh::Element::VEC4I:
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D:
	case lzh::Element::COLOR:
	case lzh::Element::COMPLEX:
	case lzh::Element::LARGENUMBER: return memcmp(data_ptr, elem.data_ptr, size) < 0;
	case lzh::Element::MATPTR_UINT8: return false;
	case lzh::Element::MATPTR_INT32: return false;
	case lzh::Element::MATPTR_FLOAT32: return false;
	case lzh::Element::MATPTR_FLOAT64: return false;
	case lzh::Element::VECTOR: return false;
	case lzh::Element::STRING: return *ptr.str < *elem.ptr.str;
	case lzh::Element::MAT: return false;;
	case lzh::Element::ARRAY: return false;;
	case lzh::Element::STD_VECTOR: return ptr.arr->Data() < elem.ptr.arr->Data();
	case lzh::Element::STD_MAP: return ptr._map->Data() < elem.ptr._map->Data();
	default:return false;
	}
}

bool lzh::Element::operator !()const
{
	return toBaseType<bool>();
}

Element::Data & lzh::Element::pre()
{
	return ptr;
}
Element::Data * lzh::Element::pointer()
{
	return &ptr;
}
const Element::Data * lzh::Element::pointer() const
{
	return &ptr;
}

void lzh::Element::setCustomText(const std::string & text)
{
	if (custom != nullptr)*custom = text;
	else custom = new std::string(text);
}

std::string lzh::Element::getCustomText() const
{
	return (custom != nullptr) ? *custom : "";
}

void lzh::Element::save(const std::string & path)const
{
	FILE* file = fopen(path.c_str(), "wb");
	if (file == NULL)return;
	save(file); fclose(file);
}

void lzh::Element::load(const std::string & path)
{
	FILE* file = fopen(path.c_str(), "rb");
	if (file == NULL)return;
	load(file); fclose(file);
}
bool lzh::Element::supportSave()const noexcept
{
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::FUNCTION:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::STD_TUPLE:return false;
	case lzh::Element::BOOL:
	case lzh::Element::INT8:
	case lzh::Element::UINT8:
	case lzh::Element::INT16:
	case lzh::Element::UINT16:
	case lzh::Element::INT32:
	case lzh::Element::UINT32:
	case lzh::Element::INT64:
	case lzh::Element::UINT64:
	case lzh::Element::FLOAT32:
	case lzh::Element::FLOAT64:
	case lzh::Element::FLOAT128:return true;
	case lzh::Element::INT8_PTR:
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:
	case lzh::Element::RANGE:
	case lzh::Element::RECT:
	case lzh::Element::RECTF:
	case lzh::Element::SIZE:
	case lzh::Element::SIZEF:
	case lzh::Element::SIZE3:
	case lzh::Element::SIZE3F:
	case lzh::Element::POINT:
	case lzh::Element::POINTF:
	case lzh::Element::POINT3:
	case lzh::Element::POINT3F:
	case lzh::Element::VEC2B:
	case lzh::Element::VEC2I:
	case lzh::Element::VEC2F:
	case lzh::Element::VEC2D:
	case lzh::Element::VEC3B:
	case lzh::Element::VEC3I:
	case lzh::Element::VEC3F:
	case lzh::Element::VEC3D:
	case lzh::Element::VEC4B:
	case lzh::Element::VEC4I:
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D:
	case lzh::Element::COLOR:
	case lzh::Element::COMPLEX:
	case lzh::Element::LARGENUMBER:
	case lzh::Element::MATPTR_UINT8:
	case lzh::Element::MATPTR_INT32:
	case lzh::Element::MATPTR_FLOAT32:
	case lzh::Element::MATPTR_FLOAT64:return false;
	case lzh::Element::VECTOR:
	case lzh::Element::STRING:
	case lzh::Element::MAT:
	case lzh::Element::ARRAY:
	case lzh::Element::STD_VECTOR:
	case lzh::Element::STD_MAP:return true;
	default:return false;
	}
}
bool lzh::Element::supportIterator() const noexcept
{
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::FUNCTION:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::STD_TUPLE:return false;
	case lzh::Element::BOOL:
	case lzh::Element::INT8:
	case lzh::Element::UINT8:
	case lzh::Element::INT16:
	case lzh::Element::UINT16:
	case lzh::Element::INT32:
	case lzh::Element::UINT32:
	case lzh::Element::INT64:
	case lzh::Element::UINT64:
	case lzh::Element::FLOAT32:
	case lzh::Element::FLOAT64:
	case lzh::Element::FLOAT128:
	case lzh::Element::INT8_PTR:
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:
	case lzh::Element::RANGE:
	case lzh::Element::RECT:
	case lzh::Element::RECTF:
	case lzh::Element::SIZE:
	case lzh::Element::SIZEF:
	case lzh::Element::SIZE3:
	case lzh::Element::SIZE3F:
	case lzh::Element::POINT:
	case lzh::Element::POINTF:
	case lzh::Element::POINT3:
	case lzh::Element::POINT3F:
	case lzh::Element::VEC2B:
	case lzh::Element::VEC2I:
	case lzh::Element::VEC2F:
	case lzh::Element::VEC2D:
	case lzh::Element::VEC3B:
	case lzh::Element::VEC3I:
	case lzh::Element::VEC3F:
	case lzh::Element::VEC3D:
	case lzh::Element::VEC4B:
	case lzh::Element::VEC4I:
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D:
	case lzh::Element::COLOR:
	case lzh::Element::COMPLEX:
	case lzh::Element::LARGENUMBER:
	case lzh::Element::MATPTR_UINT8:
	case lzh::Element::MATPTR_INT32:
	case lzh::Element::MATPTR_FLOAT32:
	case lzh::Element::MATPTR_FLOAT64:
	case lzh::Element::VECTOR:
	case lzh::Element::STRING:
	case lzh::Element::MAT:
	case lzh::Element::ARRAY:return false;
	case lzh::Element::STD_VECTOR:
	case lzh::Element::STD_MAP:return true;
	default:return false;
	}
}
void lzh::Element::save(FILE * file) const
{
	fwrite(&_type, sizeof(int32), 1, file);
	size_t size = _typeName.size();
	fwrite(&size, sizeof(size_t), 1, file);
	fwrite(_typeName->c_str(), sizeof(char)*size, 1, file);
	if (custom) {
		size = custom->size();
		fwrite(&size, sizeof(size_t), 1, file);
		fwrite(custom->c_str(), sizeof(char)*size, 1, file);
	}
	else {
		size = 0;
		fwrite(&size, sizeof(size_t), 1, file);
	}
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::FUNCTION:
	case lzh::Element::STD_FUNCTION:
	case lzh::Element::STD_TUPLE:break;
	case lzh::Element::BOOL:fwrite(data_ptr, sizeof(bool), 1, file); break;
	case lzh::Element::INT8:fwrite(data_ptr, sizeof(int8), 1, file); break;
	case lzh::Element::UINT8:fwrite(data_ptr, sizeof(uint8), 1, file); break;
	case lzh::Element::INT16:fwrite(data_ptr, sizeof(int16), 1, file); break;
	case lzh::Element::UINT16:fwrite(data_ptr, sizeof(uint16), 1, file); break;
	case lzh::Element::INT32:fwrite(data_ptr, sizeof(int32), 1, file); break;
	case lzh::Element::UINT32:fwrite(data_ptr, sizeof(uint32), 1, file); break;
	case lzh::Element::INT64:fwrite(data_ptr, sizeof(int64), 1, file); break;
	case lzh::Element::UINT64:fwrite(data_ptr, sizeof(uint64), 1, file); break;
	case lzh::Element::FLOAT32:fwrite(data_ptr, sizeof(float32), 1, file); break;
	case lzh::Element::FLOAT64:fwrite(data_ptr, sizeof(float64), 1, file); break;
	case lzh::Element::FLOAT128:fwrite(data_ptr, sizeof(float128), 1, file); break;
	case lzh::Element::INT8_PTR:
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:
	case lzh::Element::RANGE:
	case lzh::Element::RECT:
	case lzh::Element::RECTF:
	case lzh::Element::SIZE:
	case lzh::Element::SIZEF:
	case lzh::Element::SIZE3:
	case lzh::Element::SIZE3F:
	case lzh::Element::POINT:
	case lzh::Element::POINTF:
	case lzh::Element::POINT3:
	case lzh::Element::POINT3F:
	case lzh::Element::VEC2B:
	case lzh::Element::VEC2I:
	case lzh::Element::VEC2F:
	case lzh::Element::VEC2D:
	case lzh::Element::VEC3B:
	case lzh::Element::VEC3I:
	case lzh::Element::VEC3F:
	case lzh::Element::VEC3D:
	case lzh::Element::VEC4B:
	case lzh::Element::VEC4I:
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D:
	case lzh::Element::COLOR:
	case lzh::Element::COMPLEX:
	case lzh::Element::LARGENUMBER:
	case lzh::Element::MATPTR_UINT8:
	case lzh::Element::MATPTR_INT32:
	case lzh::Element::MATPTR_FLOAT32:
	case lzh::Element::MATPTR_FLOAT64:break;
	case lzh::Element::VECTOR:toVector().save(file); break;
	case lzh::Element::STRING:saveString(file, toCString()); break;
	case lzh::Element::MAT:toMat().save(file); break;
	case lzh::Element::ARRAY:toArray().save(file); break;
	case lzh::Element::STD_VECTOR:SaveList(file, toList()); break;
	case lzh::Element::STD_MAP:SaveDict(file, toDict()); break;
	default:break;
	}
}

void lzh::Element::load(FILE * file)
{
	release();
	fread(&_type, sizeof(int32), 1, file);
	create(_type);
	std::string typename_;
	lzh::readString(file, typename_);
	_typeName = typename_;
	if (custom) { delete custom; custom = nullptr; }
	size_t size = 0;
	fread(&size, sizeof(size_t), 1, file);
	if (size != 0) {
		custom = new std::string;
		custom->resize(size);
		fread(&custom->operator[](0), sizeof(char)*size, 1, file);
	}
	switch (_type)
	{
	case lzh::Element::InValid:
	case lzh::Element::INTPTR:
	case lzh::Element::FUNCTION:
	case lzh::Element::STD_FUNCTION:break;
	case lzh::Element::BOOL:fread(data_ptr, sizeof(bool), 1, file); break;
	case lzh::Element::INT8:fread(data_ptr, sizeof(int8), 1, file); break;
	case lzh::Element::UINT8:fread(data_ptr, sizeof(uint8), 1, file); break;
	case lzh::Element::INT16:fread(data_ptr, sizeof(int16), 1, file); break;
	case lzh::Element::UINT16:fread(data_ptr, sizeof(uint16), 1, file); break;
	case lzh::Element::INT32:fread(data_ptr, sizeof(int32), 1, file); break;
	case lzh::Element::UINT32:fread(data_ptr, sizeof(uint32), 1, file); break;
	case lzh::Element::INT64:fread(data_ptr, sizeof(int64), 1, file); break;
	case lzh::Element::UINT64:fread(data_ptr, sizeof(uint64), 1, file); break;
	case lzh::Element::FLOAT32:fread(data_ptr, sizeof(float32), 1, file); break;
	case lzh::Element::FLOAT64:fread(data_ptr, sizeof(float64), 1, file); break;
	case lzh::Element::FLOAT128:fread(data_ptr, sizeof(float128), 1, file); break;
	case lzh::Element::INT8_PTR:
	case lzh::Element::UINT8_PTR:
	case lzh::Element::INT16_PTR:
	case lzh::Element::UINT16_PTR:
	case lzh::Element::INT32_PTR:
	case lzh::Element::UINT32_PTR:
	case lzh::Element::INT64_PTR:
	case lzh::Element::UINT64_PTR:
	case lzh::Element::FLOAT32_PTR:
	case lzh::Element::FLOAT64_PTR:
	case lzh::Element::FLOAT128_PTR:
	case lzh::Element::RANGE:
	case lzh::Element::RECT:
	case lzh::Element::RECTF:
	case lzh::Element::SIZE:
	case lzh::Element::SIZEF:
	case lzh::Element::SIZE3:
	case lzh::Element::SIZE3F:
	case lzh::Element::POINT:
	case lzh::Element::POINTF:
	case lzh::Element::POINT3:
	case lzh::Element::POINT3F:
	case lzh::Element::VEC2B:
	case lzh::Element::VEC2I:
	case lzh::Element::VEC2F:
	case lzh::Element::VEC2D:
	case lzh::Element::VEC3B:
	case lzh::Element::VEC3I:
	case lzh::Element::VEC3F:
	case lzh::Element::VEC3D:
	case lzh::Element::VEC4B:
	case lzh::Element::VEC4I:
	case lzh::Element::VEC4F:
	case lzh::Element::VEC4D:
	case lzh::Element::COLOR:
	case lzh::Element::COMPLEX:
	case lzh::Element::LARGENUMBER:
	case lzh::Element::MATPTR_UINT8:
	case lzh::Element::MATPTR_INT32:
	case lzh::Element::MATPTR_FLOAT32:
	case lzh::Element::MATPTR_FLOAT64:break;
	case lzh::Element::VECTOR:toVector().load(file); break;
	case lzh::Element::STRING:readString(file, toStdString()); break;
	case lzh::Element::MAT:toMat().load(file); break;
	case lzh::Element::ARRAY:toArray().load(file); break;
	case lzh::Element::STD_VECTOR:LoadDict(file, toDict()); break;
	default:break;
	}
}

void lzh::Element::set(int32 t, void * p, const char *name)
{
	_type = t;  _typeName = std::string(name);
	data_ptr = ptr.user = (void*)p;
}
Element lzh::Element::CustomElem(const Element & elem, const char *text)
{
	Element _elem = elem;
	_elem.setCustomText(text);
	return _elem;
}

void lzh::Element::SaveList(FILE * file, const std::vector<Element>& list)
{
	size_t size = list.size();
	fwrite(&size, 1, sizeof(size_t), file);
	for (const Element & elem : list)elem.save(file);
}

void lzh::Element::LoadList(FILE * file, std::vector<Element>& list)
{
	size_t size;
	fread(&size, 1, sizeof(size_t), file); list.resize(size);
	for (Element & elem : list)elem.load(file);
}

void lzh::Element::SaveDict(FILE * file, const std::map<Element, Element>& dict)
{
	size_t size = dict.size();
	fwrite(&size, 1, sizeof(size_t), file);
	for (std::map<Element, Element>::const_iterator iter = dict.begin(); iter != dict.end(); iter++) {
		iter->first.save(file); iter->second.save(file);
	}
}

void lzh::Element::LoadDict(FILE * file, std::map<Element, Element>& dict)
{
	size_t size;
	fread(&size, 1, sizeof(size_t), file);
	for (size_t i : range<size_t>(size)) {
		Element elem1, elem2;
		elem1.load(file); elem2.load(file);
		dict[elem1] = elem2;
	}
}

Element lzh::Element::Create(int type)
{
	Element elem;
	elem.create(type);
	return elem;
}

lzh::ElemOp::ElemOp() : Element()
{
}
lzh::ElemOp::ElemOp(const bool & v) : Element(v)
{
}
lzh::ElemOp::ElemOp(const int & v) : Element(v)
{
}

lzh::ElemOp::ElemOp(const double & v) : Element(v)
{
}

lzh::ElemOp::ElemOp(const Vector & v) : Element(v)
{
}

lzh::ElemOp::ElemOp(const Mat & v) : Element(v)
{
}

lzh::ElemOp::ElemOp(const Array & v) : Element(v)
{
}

lzh::ElemOp::ElemOp(const char * v) : Element(std::string(v))
{
}

lzh::ElemOp::ElemOp(const std::string & v) : Element(v)
{
}

lzh::ElemOp::ElemOp(const ElemList& v) : Element(v)
{
}

//lzh::ElemOp::ElemOp(const std::initializer_list<Element>& v) : Element(v)
//{
//}

lzh::ElemOp::ElemOp(const Element & v) : Element(v)
{
}

const char * lzh::ElemOp::toChar() const
{
	LZH_ACCESS(if (_type == STRING), THROW_TYPE_INFO(typeName(STRING), typeName()));
	return ptr.str->c_str();
}

int & lzh::ElemOp::toRefInt()
{
	LZH_ACCESS(if (_type == INT32), THROW_TYPE_INFO(typeName(INT32), typeName()));
	return ptr.i32;
}

double & lzh::ElemOp::toRefDouble()
{
	LZH_ACCESS(if (_type == FLOAT64), THROW_TYPE_INFO(typeName(FLOAT64), typeName()));
	return ptr.f64;
}

std::string & lzh::ElemOp::toRefStr()
{
	LZH_ACCESS(if (_type == STRING), THROW_TYPE_INFO(typeName(STRING), typeName()));
	return *ptr.str;
}

bool lzh::ElemOp::supportOperator(int32 code) const
{
	switch (_type)
	{
	case lzh::Element::INT32:
	case lzh::Element::FLOAT64:
	case lzh::Element::COMPLEX:
	case lzh::Element::VECTOR:
	case lzh::Element::MAT:
	case lzh::Element::ARRAY:
	case lzh::Element::STD_VECTOR:return true;
	case lzh::Element::STRING:return code == ADD;
	default:return false;
	}
}
ElemOp lzh::ElemOp::Operator(const ElemOp & element, int32 code) const
{
	switch (code)
	{
	case lzh::ElemOp::ADD:return operator+(element);
	case lzh::ElemOp::SUB:return operator-(element);
	case lzh::ElemOp::MUL:return operator*(element);
	case lzh::ElemOp::DIV:return operator/(element);
	default:THROW_INFO(ERR_INFO_SUPPORT);
	}
}
ElemOp lzh::ElemOp::pow(const ElemOp &element)const
{
	LZH_ACCESS(if (supportOperator(POW) && element.isNumber()), THROW_INFO_STR("类型不支持操作"));
	if (isInt() && element.isInt())
		return saturate_cast<int32>(std::pow(toInt(), element.toInt()));
	else if (isNumber())
		return std::pow(toNumber(), element.toNumber());
	else if (isVector()) {
		Vector vec; Pow(toVector(), vec, element.toNumber()); return vec;
	}
	else if (isMat())
		return toMat().pow(element.toNumber());
	else if (isImage())
		return toArray().pow(element.toNumber());
	return ElemOp();
}
ElemOp lzh::ElemOp::sqrt() const
{
	LZH_ACCESS(if (supportOperator(POW)), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::sqrt(toNumber());
	else if (isVector()) {
		Vector vec; Sqrt(toVector(), vec); return vec;
	}
	else if (isMat())
		return toMat().sqrt();
	else if (isImage())
		return toArray().sqrt();
	return ElemOp();
}
ElemOp lzh::ElemOp::sin() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::sin(toNumber());
	else if (isVector()) {
		Vector vec;
		Sin(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		Sin(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		Sin(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::cos() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::cos(toNumber());
	else if (isVector()) {
		Vector vec;
		Cos(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		Cos(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		Cos(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::tan() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::tan(toNumber());
	else if (isVector()) {
		Vector vec;
		Tan(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		Tan(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		Tan(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::sinh() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::sinh(toNumber());
	else if (isVector()) {
		Vector vec;
		Sinh(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		Sinh(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		Sinh(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::cosh() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::cosh(toNumber());
	else if (isVector()) {
		Vector vec;
		Cosh(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		Cosh(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		Cosh(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::tanh() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::tanh(toNumber());
	else if (isVector()) {
		Vector vec;
		Tanh(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		Tanh(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		Tanh(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::asin() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::asin(toNumber());
	else if (isVector()) {
		Vector vec;
		arcSin(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		arcSin(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		arcSin(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::acos() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::acos(toNumber());
	else if (isVector()) {
		Vector vec;
		arcCos(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		arcCos(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		arcCos(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::atan() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::atan(toNumber());
	else if (isVector()) {
		Vector vec;
		arcTan(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		arcTan(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		arcTan(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::atan2(const ElemOp & element) const
{
	LZH_ACCESS(if (supportOperator() && element.supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber() && element.isNumber())
		return std::atan2(toNumber(), element.toNumber());
	else if (isVector() && element.isVector()) {
		Vector dst;
		arcTan2(toVector(), element.toVector(), dst);
		return dst;
	}
	else if (isMat() && element.isMat()) {
		Mat dst;
		arcTan2(toMat(), element.toMat(), dst);
		return dst;
	}
	else if (isMat() && element.isImage()) {
		Mat dst;
		arcTan2(toMat(), element.toArray(), dst);
		return dst;
	}
	else if (isImage() && element.isMat()) {
		Array dst;
		arcTan2(toArray(), element.toMat(), dst);
		return dst;
	}
	else if (isImage() && element.isImage()) {
		Array dst;
		arcTan2(toArray(), element.toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::asinh() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::asinh(toNumber());
	else if (isVector()) {
		Vector vec;
		arcSinh(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		arcSinh(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		arcSinh(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::acosh() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::acosh(toNumber());
	else if (isVector()) {
		Vector vec;
		arcCosh(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		arcCosh(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		arcCosh(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::atanh() const
{
	LZH_ACCESS(if (supportOperator()), THROW_INFO_STR("类型不支持操作"));
	if (isNumber())
		return std::atanh(toNumber());
	else if (isVector()) {
		Vector vec;
		arcTanh(toVector(), vec);
		return vec;
	}
	else if (isMat()) {
		Mat dst;
		arcTanh(toMat(), dst);
		return dst;
	}
	else if (isImage()) {
		Array dst;
		arcTanh(toArray(), dst);
		return dst;
	}
	return ElemOp();
}
ElemOp lzh::ElemOp::floor() const
{
	LZH_ACCESS(if (isNumber()), THROW_INFO_STR("类型不支持操作"));
	if (isInt())
		return *this;
	else if (isNumber())
		return std::floor(toNumber());
	return ElemOp();
}
ElemOp lzh::ElemOp::round() const
{
	LZH_ACCESS(if (isNumber()), THROW_INFO_STR("类型不支持操作"));
	if (isInt())
		return *this;
	else if (isNumber())
		return std::round(toNumber());
	return ElemOp();
}
ElemOp lzh::ElemOp::mod(const ElemOp &element) const
{
	LZH_ACCESS(if (isNumber() && element.isInt()), THROW_INFO_STR("类型不支持操作"));
	if (isInt() && element.isInt())
		return toInt() % element.toInt();
	else if (isNumber())
		return lzh::mod(toNumber(), element.toInt());
	return ElemOp();
}
ElemOp lzh::ElemOp::abs() const
{
	LZH_ACCESS(if (supportOperator(POW)), THROW_INFO_STR("类型不支持操作"));
	if (isInt())
		return std::abs(toInt());
	else if (isNumber())
		return std::abs(toNumber());
	else if (isVector()) {
		Vector vec;
		Abs(toVector(), vec);
		return vec;
	}
	else if (isMat())
		return toMat().abs();
	else if (isImage())
		return toArray().abs();
	return ElemOp();
}
ElemOp lzh::ElemOp::operator+(const ElemOp & element) const
{
	LZH_ACCESS(if (supportOperator(ADD) && element.supportOperator(ADD)), THROW_INFO_STR("类型不支持操作"));
	if (isInt() && element.isInt())
		return ElemOp(toInt() + element.toInt());
	else if (isNumber() && element.isNumber())
		return ElemOp(toNumber() + element.toNumber());
	else if (isNumber() && element.isMat())
		return ElemOp(toNumber() + element.toMat());
	else if (isMat() && element.isNumber())
		return ElemOp(toMat() + element.toNumber());
	else if (isNumber() && element.isImage())
		return ElemOp(toNumber() + element.toArray());
	else if (isImage() && element.isNumber())
		return ElemOp(toArray() + element.toNumber());
	else if (isNumber() && element.isVector())
		return ElemOp(toNumber() + element.toVector());
	else if (isVector() && element.isNumber())
		return ElemOp(toVector() + element.toNumber());
	else if (isVector() && element.isVector())
		return ElemOp(toVector() + element.toVector());
	else if (isMat() && element.isMat())
		return ElemOp(toMat() + element.toMat());
	else if (isImage() && element.isMat())
		return ElemOp(toArray() + element.toMat());
	else if (isMat() && element.isImage())
		return ElemOp(toMat() + element.toArray());
	else if (isImage() && element.isImage())
		return ElemOp(toArray() + element.toArray());
	else if (isStrString() && element.isInt())
		return ElemOp(toStdString() + to_string(element.toInt()));
	else if (isInt() && element.isStrString())
		return ElemOp(to_string(toInt()) + element.toStdString());
	else if (isStrString() && element.isNumber())
		return ElemOp(toStdString() + to_string(element.toNumber()));
	else if (isNumber() && element.isStrString())
		return ElemOp(to_string(toNumber()) + element.toStdString());
	else if (isStdVector() && element.isStdVector())
		return add(toStdVector(), element.toStdVector());
	else if (isStdVector())
		return add(toStdVector(), element);
	else if (element.isStdVector())
		return add(*this, element.toStdVector());
	return ElemOp();
}

ElemOp lzh::ElemOp::operator-(const ElemOp & element) const
{
	LZH_ACCESS(if (supportOperator(SUB)), THROW_INFO_STR("类型不支持操作"));
	if (isi32() && element.isi32())
		return ElemOp(toi32() - element.toi32());
	else if (isNumber() && element.isNumber())
		return ElemOp(toNumber() - element.toNumber());
	else if (isNumber() && element.isMat())
		return ElemOp(toNumber() - element.toMat());
	else if (isMat() && element.isNumber())
		return ElemOp(toMat() - element.toNumber());
	else if (isNumber() && element.isImage())
		return ElemOp(toNumber() - element.toArray());
	else if (isImage() && element.isNumber())
		return ElemOp(toArray() - element.toNumber());
	else if (isNumber() && element.isVector())
		return ElemOp(toNumber() - element.toVector());
	else if (isVector() && element.isNumber())
		return ElemOp(toVector() - element.toNumber());
	else if (isVector() && element.isVector())
		return ElemOp(toVector() - element.toVector());
	else if (isMat() && element.isMat())
		return ElemOp(toMat() - element.toMat());
	else if (isImage() && element.isMat())
		return ElemOp(toArray() - element.toMat());
	else if (isMat() && element.isImage())
		return ElemOp(toMat() - element.toArray());
	else if (isImage() && element.isImage())
		return ElemOp(toArray() - element.toArray());
	else if (isStdVector() && element.isStdVector())
		return sub(toStdVector(), element.toStdVector());
	else if (isStdVector())
		return sub(toStdVector(), element);
	else if (element.isStdVector())
		return sub(*this, element.toStdVector());
	return ElemOp();
}

ElemOp lzh::ElemOp::operator*(const ElemOp & element) const
{
	LZH_ACCESS(if (supportOperator(MUL)), THROW_INFO_STR("类型不支持操作"));
	if (isi32() && element.isi32())
		return ElemOp(toi32() * element.toi32());
	else if (isNumber() && element.isNumber())
		return ElemOp(toNumber() * element.toNumber());
	else if (isNumber() && element.isMat())
		return ElemOp(toNumber() * element.toMat());
	else if (isMat() && element.isNumber())
		return ElemOp(toMat() * element.toNumber());
	else if (isNumber() && element.isImage())
		return ElemOp(toNumber() * element.toArray());
	else if (isImage() && element.isNumber())
		return ElemOp(toArray() * element.toNumber());
	else if (isNumber() && element.isVector())
		return ElemOp(toNumber() * element.toVector());
	else if (isVector() && element.isNumber())
		return ElemOp(toVector() * element.toNumber());
	else if (isVector() && element.isVector())
		return ElemOp(toVector() * element.toVector());
	else if (isMat() && element.isMat())
		return ElemOp(toMat().mul(element.toMat()));
	else if (isImage() && element.isMat())
		return ElemOp(toArray().mul(element.toMat()));
	else if (isMat() && element.isImage())
		return ElemOp(toMat().mul(element.toArray()));
	else if (isImage() && element.isImage())
		return ElemOp(toArray().mul(element.toArray()));
	else if (isStdVector() && element.isStdVector())
		return mul(toStdVector(), element.toStdVector());
	else if (isStdVector())
		return mul(toStdVector(), element);
	else if (element.isStdVector())
		return mul(*this, element.toStdVector());
	return ElemOp();
}

ElemOp lzh::ElemOp::operator/(const ElemOp & element) const
{
	LZH_ACCESS(if (supportOperator(DIV)), THROW_INFO_STR("类型不支持操作"));
	if (isi32() && element.isi32())
		return ElemOp(toi32() / element.toi32());
	else if (isNumber() && element.isNumber())
		return ElemOp(toNumber() / element.toNumber());
	else if (isNumber() && element.isMat())
		return ElemOp(toNumber() / element.toMat());
	else if (isMat() && element.isNumber())
		return ElemOp(toMat() / element.toNumber());
	else if (isNumber() && element.isImage())
		return ElemOp(toNumber() / element.toArray());
	else if (isImage() && element.isNumber())
		return ElemOp(toArray() / element.toNumber());
	else if (isNumber() && element.isVector())
		return ElemOp(toNumber() / element.toVector());
	else if (isVector() && element.isNumber())
		return ElemOp(toVector() / element.toNumber());
	else if (isVector() && element.isVector())
		return ElemOp(toVector() / element.toVector());
	else if (isMat() && element.isMat())
		return ElemOp(toMat() / element.toMat());
	else if (isImage() && element.isMat())
		return ElemOp(toArray() / element.toMat());
	else if (isMat() && element.isImage())
		return ElemOp(toMat() / element.toArray());
	else if (isImage() && element.isImage())
		return ElemOp(toArray() / element.toArray());
	else if (isStdVector() && element.isStdVector())
		return div(toStdVector(), element.toStdVector());
	else if (isStdVector())
		return div(toStdVector(), element);
	else if (element.isStdVector())
		return div(*this, element.toStdVector());
	return ElemOp();
}

ElemOp lzh::ElemOp::operator+=(const ElemOp & element)
{
	LZH_ACCESS(if (supportOperator(ADD)), THROW_INFO_STR("类型不支持操作"));
	return *this = *this + element;
}

ElemOp lzh::ElemOp::operator-=(const ElemOp & element)
{
	LZH_ACCESS(if (supportOperator(SUB)), THROW_INFO_STR("类型不支持操作"));
	return *this = *this - element;
}

ElemOp lzh::ElemOp::operator*=(const ElemOp & element)
{
	LZH_ACCESS(if (supportOperator(MUL)), THROW_INFO_STR("类型不支持操作"));
	return *this = *this * element;
}

ElemOp lzh::ElemOp::operator/=(const ElemOp & element)
{
	LZH_ACCESS(if (supportOperator(DIV)), THROW_INFO_STR("类型不支持操作"));
	return *this = *this / element;
}

ElemOp lzh::ElemOp::Operator(const ElemOp & a, const ElemOp & b, int32 code)
{
	return a.Operator(b, code);
}

ElemOp lzh::ElemOp::Add(const Element & a, const Element & b)
{
	return *((ElemOp*)&a) + *((ElemOp*)&b);
}
ElemOp lzh::ElemOp::add(const ElemOp & a, const ElemList & b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(b.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Add(a, ElemOp(b[i]));
	return arr;
}
ElemOp lzh::ElemOp::add(const ElemList& a, const ElemList& b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	LZH_ACCESS(if (a.size() == b.size()), THROW_INFO(ERR_INFO_SIZE));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))arr[i] = Add(a[i], b[i]);
	return arr;
}
ElemOp lzh::ElemOp::add(const ElemList& a, const ElemOp & b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Add(a[i], b);
	return arr;
}

ElemOp lzh::ElemOp::Sub(const Element & a, const Element & b)
{
	return *((ElemOp*)&a) - *((ElemOp*)&b);
}
ElemOp lzh::ElemOp::sub(const ElemOp & a, const ElemList& b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(b.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Sub(a, b[i]);
	return arr;
}
ElemOp lzh::ElemOp::sub(const ElemList& a, const ElemList& b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	LZH_ACCESS(if (a.size() == b.size()), THROW_INFO(ERR_INFO_SIZE));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))arr[i] = Sub(a[i], b[i]);
	return arr;
}
ElemOp lzh::ElemOp::sub(const ElemList& a, const ElemOp & b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Sub(a[i], b);
	return arr;
}

ElemOp lzh::ElemOp::Mul(const Element & a, const Element & b)
{
	return *((ElemOp*)&a) * *((ElemOp*)&b);
}
ElemOp lzh::ElemOp::mul(const ElemOp & a, const ElemList& b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(b.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Mul(a, b[i]);
	return arr;
}
ElemOp lzh::ElemOp::mul(const ElemList& a, const ElemList& b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	LZH_ACCESS(if (a.size() == b.size()), THROW_INFO(ERR_INFO_SIZE));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))arr[i] = Mul(a[i], b[i]);
	return arr;
}
ElemOp lzh::ElemOp::mul(const ElemList& a, const ElemOp & b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Mul(a[i], b);
	return arr;
}

ElemOp lzh::ElemOp::Div(const Element & a, const Element & b)
{
	return *((ElemOp*)&a) / *((ElemOp*)&b);
}
ElemOp lzh::ElemOp::div(const ElemOp & a, const ElemList& b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(b.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Div(a, b[i]);
	return arr;
}
ElemOp lzh::ElemOp::div(const ElemList& a, const ElemList& b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	LZH_ACCESS(if (a.size() == b.size()), THROW_INFO(ERR_INFO_SIZE));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))arr[i] = Div(a[i], b[i]);
	return arr;
}
ElemOp lzh::ElemOp::div(const ElemList& a, const ElemOp & b)
{
	LZH_ACCESS(if (!a.empty() && !b.empty()), THROW_INFO(ERR_INFO_EMPTY));
	ElemList arr(a.size());
	for (msize i : RangeM(arr.size()))
		arr[i] = Div(a[i], b);
	return arr;
}

static Element elem_;
const Element& lzh::noElem()
{
	return elem_;
}

Element lzh::MakeElem(const std::initializer_list<Element>& list)
{
	return ElemList(list);
}