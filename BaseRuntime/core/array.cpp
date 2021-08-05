#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <strstream>
#define LZH_EXPORTS
#include "array.h"
#include "tools.h"
#include "operator.h"
#include "algorithm.h"

using namespace lzh;
using namespace std;

uint32 Array::print_width = 8;
uint32 Array::print_precision = 4;
PrintType Array::print_type = MAT_FIXED;


/****************************************************************************
任意类型矩阵类
*****************************************************************************/
Array::Array()
{
	init();
	checkSquare();
}
Array::Array(int32 w, int32 flags, ColorGamut cg)
{
	init();
	create(w, flags, cg);
}
Array::Array(int32 w, int32 h, int32 flags, ColorGamut cg)
{
	init();
	create(w, h, flags, cg);
}
Array::Array(int32 w, int32 h, int32 c, int32 flags, ColorGamut cg)
{
	init();
	create(w, h, c, flags, cg);
}
Array::Array(Size size_, int32 flags, ColorGamut cg)
{
	init();
	create(size_.w, size_.h, flags, cg);
}
Array::Array(Size size_, int32 c, int32 flags, ColorGamut cg)
{
	init();
	create(size_.w, size_.h, c, flags, cg);
}
Array::Array(Size3 size_, int32 flags, ColorGamut cg)
{
	init();
	create(size_.w, size_.h, size_.c, flags, cg);
}
Array::Array(ColorGamut colorGamut, Size size, int32 flags)
{
	init();
	createColorGamut(colorGamut, size, flags);
}
lzh::Array::Array(const Vector & src, bool copyData)
{
	init();
	if (copyData)
	{
		create((int32)src.size(), src.depth());
		memcpy(data, src.obj(), memsize());
	}
	else {
		setsize((int32)src.size());
		data = src;
	}
}
Array::Array(const Vector& src, Size3 sz, bool copyData)
{
	init();
	if (copyData)
	{
		create(sz, src.depth());
		memcpy(data, src.obj(), memsize());
	}
	else {
		setsize(sz.w, sz.h, sz.c);
		data = src;
	}
}
Array::Array(const void* p, Size3 sz, int32 flags)
{
	init();
	create(sz, flags);
	memcpy(data, p, memsize());
}
Array::Array(int32 w, const void* data, int32 flags, bool copyData)
{
	init();
	*this = Array(w, 1, 1, data, flags, copyData);
}
Array::Array(int32 w, int32 h, const void* data, int32 flags, bool copyData)
{
	init();
	*this = Array(w, h, 1, data, flags, copyData);
}
Array::Array(int32 w, int32 h, int32 c, const void* data, int32 flags, bool copyData)
{
	init();
	if (!copyData) {
		setsize(w, h, c);
		this->data.set(data, len(), flags);
	}
	else {
		create(w, h, c, flags);
		memcpy(this->data, data, memsize());
	}
}
Array::Array(Size3 size, MatStep step, const void * data, const int * refcount, int32 flags)
{
	init();
	*this = Array(size.w, size.h, size.c, step, data, refcount, flags);
}
Array::Array(int32 w, int32 h, int32 c, MatStep step, const void * data, const int * refcount, int32 flags)
{
	init();
	setsize(w, h, c);
	this->step = step;
	this->data.set(data, len(), flags, refcount);
	this->data.addref();
}
Array::Array(Size size, const void* data, int32 flags, bool copyData)
{
	init();
	*this = Array(size.w, size.h, 1, data, flags, copyData);
}
Array::Array(Size size, int32 c, const void* data, int32 flags, bool copyData)
{
	init();
	*this = Array(size.w, size.h, c, data, flags, copyData);
}
Array::Array(Size3 size, const void* data, int32 flags, bool copyData)
{
	init();
	*this = Array(size.w, size.h, size.c, data, flags, copyData);
}
Array::Array(int32 w, int32 h, int32 c, MatStep step, const void* data, int32 flags)
{
	init();
	setsize(w, h, c);
	this->step = step;
	this->data.set(data, h * total(), flags);
}
lzh::Array::Array(int32 w, int32 h, int32 step, const uint8 * data, ColorGamut colorgaumt)
{
	init();
	int c;
	switch (colorgaumt)
	{
	case lzh::ColorGamutCustom:c = step / w; break;
	case lzh::ColorGamutGray:c = 1; break;
	case lzh::ColorGamutRGB:
	case lzh::ColorGamutBGR:
	case lzh::ColorGamutHSV:
	case lzh::ColorGamutXYZ:
	case lzh::ColorGamutLab:c = 3; break;
	case lzh::ColorGamutRGBA:
	case lzh::ColorGamutBGRA:c = 4; break;
	case lzh::ColorGamutMean:
	case lzh::ColorGamutR:
	case lzh::ColorGamutB:
	case lzh::ColorGamutG:c = 1; colorgaumt = ColorGamutGray; break;
	default:
		break;
	}
	setsize(w, h, c);
	this->step[0] = step / c;
	this->step[1] = c;
	this->data.set(data, h * total(), MAT_8U);
	_type = colorgaumt;
}
Array::Array(const Array& src)
{
	init();
	*this = src;
}
lzh::Array::Array(const _InputArray& inputarray)
{
	init(); *this = inputarray.getArray();
}
Array::Array(const Array& a, const Array& b, RANK merge)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(b);
	CHECK_TYPE_(a.depth(), b.depth());
#endif //LZH_DEBUG
	init();
	if (merge == ROW) {
		if (a.cols == b.cols) {
			create(a.cols, a.rows + b.rows, a.c_, a.depth());
			memcpy(data, a.data, a.memsize());
			memcpy(data.offset(a.len()), b.data, b.memsize());
		}
	}
	else if (merge == COL) {
		if (a.rows == b.rows) {
			create(a.cols + b.cols, a.rows, a.c_, a.depth());
			for (int32 i = 0; i < rows; i++) {
				memcpy(data.offset(i * cols * c_),
					a.data.offset(i * a.cols * c_),
					a.cols * c_ * calmemsize(depth()));
				memcpy(data.offset(i * cols * c_ + a.cols * c_),
					b.data.offset(i * b.cols * c_),
					b.cols * c_ * calmemsize(depth()));
			}
		}
	}
}
//lzh::Array::Array(const Mat& src, int32 flags)
//{
//	init();
//	if (flags == MAT_T)* this = src;
//	else {
//		create(src.size3(), flags);
//		FOR_RANGE(i, 0, len())
//			set(i, src(i));
//	}
//}
Array::~Array()
{
	//release();
	setsize(0, 0, 0);
}
void Array::create(int32 w, int32 flags, ColorGamut cg)
{
	create(w, 1, 1, flags, cg);
}
void Array::create(int32 w, int32 h, int32 flags, ColorGamut cg)
{
	create(w, h, 1, flags, cg);
}
void Array::create(int32 w, int32 h, int32 c, int32 flags, ColorGamut cg)
{
	release();
	if (w <= 0 || h <= 0 || c <= 0) { setsize(0, 0, 0); return; }
	setsize(w, h, c);
	data.create(len(), flags);
	_type = cg;
#ifdef LZH_DEBUG
	CHECK_PTR(data.obj());
#endif //LZH_DEBUG
}
void Array::create(Size size, int32 flags, ColorGamut cg)
{
	create(size.w, size.h, flags, cg);
}
void Array::create(Size3 size, int32 flags, ColorGamut cg)
{
	create(size.w, size.h, size.c, flags, cg);
}
void Array::createColorGamut(ColorGamut colorGamut, Size sz, int32 flags)
{
	int32 channel_ = 1;
	switch (colorGamut)
	{
	case lzh::ColorGamutCustom: case lzh::ColorGamutGray: channel_ = 1; break;
	case lzh::ColorGamutRGB: case lzh::ColorGamutBGR:
	case lzh::ColorGamutXYZ:case lzh::ColorGamutLab:
	case lzh::ColorGamutHSV: channel_ = 3; break;
	case lzh::ColorGamutRGBA:case lzh::ColorGamutBGRA:channel_ = 4; break;
	default:
		THROW_INFO_STR("不支持的色域!, 请使用->[ColorGamutCustom, ColorGamutGray, ColorGamutRGB, ColorGamutBGR, ColorGamutXYZ, ColorGamutLab, ColorGamutHSV, ColorGamutRGBA ColorGamutBGRA]");
	}
	create(sz.h, sz.w, channel_, flags, colorGamut);
}
mat_t Array::First() const
{
	return get_(0);
}
mat_t Array::Last() const
{
	return get_((int32)data.size() - 1);
}
int32* Array::memCount() const
{
	return data.memCount();
}
Vector& Array::ptr()
{
	return data;
}
const Vector& Array::ptr() const
{
	return data;
}
void* Array::Data()
{
	return data;
}
const void* Array::Data()const
{
	return data;
}
const Mat Array::toMat() const
{
	if (depth() == MAT_T) {
		Mat mat(size3(), step, (const void*)data.obj(), (const int32*)data.memCount());
		mat.setColorGamut(_type);
		return mat;
	}
	else if (depth() != NONE) {
		Mat m(size3());
		m.setColorGamut(_type);
		FOR_RANGE(i, 0, len()) {
			m(i) = operator()(i);
		}
		return m;
	}
	return Mat();
}
bool lzh::Array::isDeepCopy() const
{
	return data.memCount() != nullptr;
}
void lzh::Array::addref()
{
	data.addref();
}
uint32 Array::memsize() const
{
	return calmemsize(depth()) * rows * cols * c_;
}
uint32 lzh::Array::memdepth() const
{
	return calmemsize(depth()) * 8 * c_;
}
void Array::DimCheck()const
{
	if (c_ != 1) {
		THROW_INFO(ERR_INFO_DIM);
	}
}
void Array::copy(Array& src, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	//CHECK_TYPE_(depth(), src.depth());
#endif // LZH_DEBUG
	int32 hei = Row_End - Row_Start;
	int32 wid = Col_End - Col_Start;
	check(hei, wid);
	if (src.data == nullptr) {
		src = zeros(wid, hei, c_, depth());
	}
	for (int32 y = Row_Start, j = 0; y < Row_End; y++, j++)
		for (int32 x = Col_Start, i = 0; x < Col_End; x++, i++)
			for (int32 z = 0; z < c_; z++)
				src.set(y, x, z, get_(j, i, z));
}
void lzh::Array::copy(Array & src, Rect iou) const
{
	copy(src, iou.y, iou.y + iou.height, iou.x, iou.x + iou.width);
}
void Array::swap(Array& src)
{
	std::swap(src, *this);
}
const Array Array::addones(Dire dire) const
{
	return addvalue(dire, 1);
}
const Array Array::addzeros(Dire dire) const
{
	return addvalue(dire, 0);
}
void Array::mChannel(const Array& src, int32 channels)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	CHECK_MATRIX(src.data);
	CHECK_TYPE_(depth(), src.depth());
	if (rows != src.rows || cols != src.cols || channels >= c_)
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	for (int32 i = 0; i < rows; ++i) {
		for (int32 j = 0; j < cols; ++j) {
			set(i, j, channels, src, src.toidx(i, j));
		}
	}
}
void Array::mChannel(const Array& src, int32 w, int32 h)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	CHECK_MATRIX(src.data);
	CHECK_TYPE_(depth(), src.depth());
	if (this->rows <= rows || this->cols <= cols || src.channels() != c_)
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	for (int32 i = 0; i < c_; ++i)
		set(rows, cols, i, src, src.toidx(i));
}
Array Array::reshape(Size3 size)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	return reshape(size.w, size.h, size.c);
}
Array Array::reshape(int32 w, int32 h, int32 c)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	if (h == 0) {
		h = rows;
	}
	if (w == 0) {
		w = cols;
	}
	if (c == 0) {
		c = channels();
	}
	if (h == -1 && w == -1 && c == -1) {
		THROW_INFO(ERR_INFO_UNLESS);
	}
	bool check_ = true;
	if (h == -1) {
		h = len() / (w * c);
		if (check_)check_ = false;
		else {
			THROW_INFO(ERR_INFO_UNLESS);
		}
	}
	else if (w == -1) {
		w = len() / (h * c);
		if (check_)check_ = false;
		else {
			THROW_INFO(ERR_INFO_UNLESS);
		}
	}
	else if (c == -1) {
		c = len() / (h * w);
		if (check_)check_ = false;
		else {
			THROW_INFO(ERR_INFO_UNLESS);
		}
	}
	if (len() != h * w * c) {
		THROW_INFO(ERR_INFO_UNLESS);
	}
	else {
		setsize(w, h, c);
	}
	return *this;
}
bool Array::setSize(int32 w, int32 h, int32 c)
{
	if (h * w * c > 0) {
		create(w, h, c, depth());
		return true;
	}
	if (len() == h * w * c) {
		setsize(w, h, c);
		return true;
	}
	return false;
}
void Array::setvalue(const Array& src)
{
	create(src.size3(), src.depth(), src.colorGamut());
	if (src.step[0] == src.cols && src.step[1] == src.channels())
		memcpy(data, src.data, src.memsize());
	else
		src.setTo(*this);
}
void Array::setAdj()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (square)
		* this = adj();
	else THROW_INFO(ERR_INFO_ADJ);
}
void Array::setTran()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	* this = t();
}
void lzh::Array::setColorGamut(ColorGamut cg)
{
	_type = cg;
}
void Array::setInv()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (enable() == 0)
		* this = inv();
	else THROW_INFO(ERR_INFO_INV);
}
void Array::setPow(lzh::mat_t num)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (enable() == 0)
		* this = pow(num);
	else THROW_INFO(ERR_INFO_POW);
}
void Array::setIden()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (enable() == 0)
		* this = eye(cols, depth());
	else THROW_INFO(ERR_INFO_IND);
}

lzh::Size3 Array::size3() const
{
	return Size3(cols, rows, c_);
}
int32 Array::total() const
{
	return step.size();
}
int32 lzh::Array::total_() const
{
	return cols * c_;
}
int32 Array::dims() const
{
	return dim;
}
MatStep Array::stride() const
{
	return step;
}
Rect lzh::Array::rect() const
{
	return Rect(0, 0, cols, rows);
}
Point lzh::Array::center() const
{
	return Point(cols / 2, rows / 2);
}
int32 Array::depth() const
{
	return data.depth();
}
int32 Array::type() const
{
	return data.getTp();
}
int32 Array::channels() const
{
	return c_;
}
int32 Array::rank() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (dim != 2)
		THROW_INFO(ERR_INFO_NOT2D);
#endif //LZH_DEBUG
	int32 rank = rows;
	for (int32 i = 0; i < rows; i++) {
		int32 count = 0;
		for (int32 j = 0; j < cols; j++) {
			if (isValue(toidx(j, i * cols), 0)) {
				count++;
			}
		}
		if (count == rows)
			rank--;
	}
	return rank;
}
ColorGamut lzh::Array::colorGamut() const
{
	return _type;
}
ColorGamut lzh::Array::format() const
{
	return colorGamut();
}
lzh::Size Array::size()const
{
	return Size(cols, rows);
}

int32 Array::toidx(int32 i) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (i >= len() || i < 0)
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	int32 i0 = i / total_();
	int32 i1 = (i - i0 * total_()) / c_;
	int32 i2 = i % c_;
	return toidx(i0, i1, i2);
	//return i * stride[1] / c_;
}
int32 Array::toidx(int32 i, int32 j) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (i * j >= len() || i >= rows || j >= cols
		|| i < 0 || j < 0)
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	return (i * step[0] + j) * step[1];
}
int32 Array::toidx(int32 i, int32 j, int32 k) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (i * j * k >= len() || i >= rows || j >= cols
		|| k >= c_ || i < 0 || j < 0 || k < 0)
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	return (i * step[0] + j) * step[1] + k;
}

int32 lzh::Array::toOneIndex(int32 i, int32 j) const
{
	return (i * cols + j)*c_;
}

int32 lzh::Array::toOneIndex(int32 i, int32 j, int32 k) const
{
	return (i * cols + j)*c_ + k;
}

void Array::set(const _InputArray & src)
{
	*this = src.getArray();
}

int32 Array::len()const
{
	return rows * cols * c_;
}
int32 lzh::Array::elementlen() const
{
	return rows * total();
}
int32 Array::enable()const
{
	if (data == nullptr)
		return -1;
	if (!square)
		return -2;
	return 0;
}
bool Array::empty()const
{
	return (data == nullptr);
}
bool Array::isSquare()const
{
	return square;
}
void Array::set(int32 i0, int32 i1, const Color & v)
{
	int32 n = channels() > 4 ? 4 : channels();
	for (int32 i : Range(0, n))
		data.setv(toidx(i0, i1, i), v[i]);
}
void Array::set(int32 i, const void* v, int32 addr_offset)
{
	setp(toidx(i), v, addr_offset);
}
void Array::set(int32 i0, int32 i1, const void* v, int32 addr_offset)
{
	setp(toidx(i0, i1), v, addr_offset);
}
void Array::set(int32 i0, int32 i1, int32 i2, const void* v, int32 addr_offset)
{
	setp(toidx(i0, i1, i2), v, addr_offset);
}
void Array::assign(int32 i, int32 j)
{
	switch (depth()) {
	case lzh::TP_8S:	set(i, *MAT_GETPTR(int8, data.index(j))); break;
	case lzh::TP_8U:	set(i, *MAT_GETPTR(uint8, data.index(j))); break;
	case lzh::TP_16S:	set(i, *MAT_GETPTR(int16, data.index(j))); break;
	case lzh::TP_16U:	set(i, *MAT_GETPTR(uint16, data.index(j))); break;
	case lzh::TP_32S:	set(i, *MAT_GETPTR(int32, data.index(j))); break;
	case lzh::TP_32U:	set(i, *MAT_GETPTR(uint32, data.index(j))); break;
	case lzh::TP_64S:	set(i, *MAT_GETPTR(int64, data.index(j))); break;
	case lzh::TP_64U:	set(i, *MAT_GETPTR(uint64, data.index(j))); break;
	case lzh::TP_32F:	set(i, *MAT_GETPTR(float32, data.index(j))); break;
	case lzh::TP_64F:	set(i, *MAT_GETPTR(float64, data.index(j))); break;
	case lzh::TP_64D:	set(i, *MAT_GETPTR(float128, data.index(j))); break;
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}
void Array::swap(int32 i, int32 j)
{
	i = toidx(i); j = toidx(j);
	if (i == j)return;
	switch (depth()) {
	case lzh::TP_8S:	lzh::Swap(*MAT_GETPTR(int8, data.index(i)), *MAT_GETPTR(int8, data.index(j))); break;
	case lzh::TP_8U:	lzh::Swap(*MAT_GETPTR(uint8, data.index(i)), *MAT_GETPTR(uint8, data.index(j))); break;
	case lzh::TP_16S:	lzh::Swap(*MAT_GETPTR(int16, data.index(i)), *MAT_GETPTR(int16, data.index(j))); break;
	case lzh::TP_16U:	lzh::Swap(*MAT_GETPTR(uint16, data.index(i)), *MAT_GETPTR(uint16, data.index(j))); break;
	case lzh::TP_32S:	lzh::Swap(*MAT_GETPTR(int32, data.index(i)), *MAT_GETPTR(int32, data.index(j))); break;
	case lzh::TP_32U:	lzh::Swap(*MAT_GETPTR(uint32, data.index(i)), *MAT_GETPTR(uint32, data.index(j))); break;
	case lzh::TP_64S:	lzh::Swap(*MAT_GETPTR(int64, data.index(i)), *MAT_GETPTR(int64, data.index(j))); break;
	case lzh::TP_64U:	lzh::Swap(*MAT_GETPTR(uint64, data.index(i)), *MAT_GETPTR(uint64, data.index(j))); break;
	case lzh::TP_32F:	lzh::Swap(*MAT_GETPTR(float32, data.index(i)), *MAT_GETPTR(float32, data.index(j))); break;
	case lzh::TP_64F:	lzh::Swap(*MAT_GETPTR(float64, data.index(i)), *MAT_GETPTR(float64, data.index(j))); break;
	case lzh::TP_64D:	lzh::Swap(*MAT_GETPTR(float128, data.index(i)), *MAT_GETPTR(float128, data.index(j))); break;
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}
Array lzh::Array::mul(const _InputArray & m) const
{
	Array dst;
	Mult(*this, m, dst);
	return dst;
}
lzh::mat_t Array::get_(int32 i) const
{
	return get<mat_t>(i);
}

mat_t Array::get_(int32 i0, int32 i1) const
{
	return get<mat_t>(i0, i1);
}

mat_t Array::get_(int32 i0, int32 i1, int32 i2) const
{
	return get<mat_t>(i0, i1, i2);
}

Color lzh::Array::pixel(int32 i0, int32 i1) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(data);
#endif // LZH_DEBUG
	Array m = range(i0, i0 + 1, i1, i1 + 1);
	Color color;
	FOR_RANGE(i, 0, MIN(4, channels())) {
		color[i] = m(i);
	}
	color.setSpec(_type);
	return color;
}

Color lzh::Array::pixel(int32 i0, int32 i1, Color::Spec spec) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(data);
#endif // LZH_DEBUG
	Array m = range(i0, i0 + 1, i1, i1 + 1);
	Color color;
	FOR_RANGE(i, 0, MIN(4, channels())) {
		color[i] = m(i);
	}
	color.setSpec(spec);
	return color;
}

void lzh::Array::setPixel(int32 i0, int32 i1, Color color)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(data);
#endif // LZH_DEBUG
	FOR_RANGE(i, 0, MIN(4, channels())) {
		set(i0, i1, i, color[i]);
	}
}

void Array::fill(Val value)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(data);
#endif // LZH_DEBUG
	if (isInt(data.depth()) && isDeepCopy())
		memset(data(), 0, memsize());
	else
		FOR_RANGE(i, 0, len()) set(i, value);
}

Array lzh::Array::safediv(const Array & a, Val def) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(data);
#endif // LZH_DEBUG
	if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		return (isEqual(a(0), 0) ? isnan(def) ? *this : Array::value(def.v, size3(), depth()) : *this / a(0));
	}
	else if (rows == 1 && cols == 1 && channels() == 1) {
		return get_(0) / a;
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == channels()) {
		Array mat(size3(), depth());
		for (int32 z = 0; z < channels(); z++) {
			for (int32 i = 0; i < rows; i++) {
				for (int32 j = 0; j < cols; j++) {
					mat.set(i, j, z, saveDiv(get_(i, j, z), a(z), isnan(def) ? get_(i, j, z) : def.v));
				}
			}
		}
		return mat;
	}
#ifdef LZH_DEBUG
	if (size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Array mark(size3(), depth());
	for (int32 i = 0; i < len(); i++)
		mark.set(i, saveDiv(get_(i), a(i), isnan(def) ? get_(i) : def.v));
	return mark;
}

void Array::save(std::string file, bool binary) const
{
	if (binary) {
		FILE* out = fopen((file + ".bin").c_str(), "wb");
		if (out) {
			save(out);
			fclose(out);
		}
		else {
			THROW_INFO(ERR_INFO_FILE);
		}
	}
	else {
		std::ofstream out(file);
		if (out.is_open()) {
			show(out, false);
			out.close();
		}
		else {
			THROW_INFO(ERR_INFO_FILE);
		}
	}
}
void Array::save(FILE* out) const
{
	if (out) {
		int32 param[5] = { cols, rows, c_, data.depth(), _type };
		fwrite(param, sizeof(int32) * 5, 1, out);
		fwrite(data, memsize(), 1, out);
	}
	else {
		THROW_INFO(ERR_INFO_FILE);
	}
}
void Array::load(std::string file)
{
	FILE* in = fopen((file + ".bin").c_str(), "rb");
	if (in) {
		load(in);
		fclose(in);
	}
	else {
		THROW_INFO(ERR_INFO_FILE);
	}
}
void Array::load(FILE* in)
{
	if (in) {
		int32 param[5] = { 0 };
		fread(param, sizeof(int32) * 5, 1, in);
		create(param[0], param[1], param[2], param[3], (ColorGamut)param[4]);
		fread(data, memsize(), 1, in);
	}
	else {
		THROW_INFO(ERR_INFO_FILE);
	}
}
void Array::copyTo(Array& mat) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	mat.create(size3(), depth());
	memcpy(mat, data, memsize());
}
void Array::setTo(Array mat) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (mat.isDeepCopy() && isDeepCopy())memcpy(mat.data, data, memsize());
	else
		FOR_RANGE(i, 0, mat.len()) {
		mat.set(i, get_(i));
	}
}
void Array::convertTo(Array& m, int32 type) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (depth() == type)
		m = *this;
	else {
		Array mat;
		mat.create(size3(), type, _type);
		if (type == depth() && isDeepCopy())
			memcpy(mat, data, memsize());
		else
			FOR_RANGE(i, 0, len())
			mat.set(i, get_(i));
		m = mat;
	}
}
const Array Array::convert(int32 type) const
{
	Array cv;
	convertTo(cv, type);
	return cv;
}
void Array::setAddOnes(Dire dire)
{
	*this = addones(dire);
}
void Array::setAddZeros(Dire dire)
{
	*this = addzeros(dire);
}
void Array::release()
{
	data.release();
	setsize(0, 0, 0);
}
int32 Array::toX(int32 index)const
{
	return (index / c_) % cols;
}
int32 Array::toY(int32 index)const
{
	return (index / c_) / cols;
}
int32 Array::toZ(int32 index) const
{
	return index % c_;
}
bool Array::isZero(int32 i) const
{
	i = toidx(i);
	switch (depth()) {
	case lzh::TP_8S:	return MAT_GETARRAY(int8, data.obj(), i) == (int8)0;
	case lzh::TP_8U:	return MAT_GETARRAY(uint8, data.obj(), i) == (uint8)0;
	case lzh::TP_16S:	return MAT_GETARRAY(int16, data.obj(), i) == (int16)0;
	case lzh::TP_16U:	return MAT_GETARRAY(uint16, data.obj(), i) == (uint16)0;
	case lzh::TP_32S:	return MAT_GETARRAY(int32, data.obj(), i) == (int32)0;
	case lzh::TP_32U:	return MAT_GETARRAY(uint32, data.obj(), i) == (uint32)0;
	case lzh::TP_64S:	return MAT_GETARRAY(int64, data.obj(), i) == (int64)0;
	case lzh::TP_64U:	return MAT_GETARRAY(uint64, data.obj(), i) == (uint64)0;
	case lzh::TP_32F:	return isEqual(MAT_GETARRAY(float32, data.obj(), i), 0);
	case lzh::TP_64F:	return isEqual(MAT_GETARRAY(float64, data.obj(), i), 0);
	case lzh::TP_64D:	return isEqual(MAT_GETARRAY(float128, data.obj(), i), 0);
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
}
bool lzh::Array::isZero(Point p) const
{
	return isZero(toidx(p.y, p.x));
}
bool Array::isOne(int32 i) const
{
	i = toidx(i);
	switch (depth()) {
	case lzh::TP_8S:	return MAT_GETARRAY(int8, data.obj(), i) == (int8)1;
	case lzh::TP_8U:	return MAT_GETARRAY(uint8, data.obj(), i) == (uint8)1;
	case lzh::TP_16S:	return MAT_GETARRAY(int16, data.obj(), i) == (int16)1;
	case lzh::TP_16U:	return MAT_GETARRAY(uint16, data.obj(), i) == (uint16)1;
	case lzh::TP_32S:	return MAT_GETARRAY(int32, data.obj(), i) == (int32)1;
	case lzh::TP_32U:	return MAT_GETARRAY(uint32, data.obj(), i) == (uint32)1;
	case lzh::TP_64S:	return MAT_GETARRAY(int64, data.obj(), i) == (int64)1;
	case lzh::TP_64U:	return MAT_GETARRAY(uint64, data.obj(), i) == (uint64)1;
	case lzh::TP_32F:	return MAT_GETARRAY(float32, data.obj(), i) == (float32)1;
	case lzh::TP_64F:	return MAT_GETARRAY(float64, data.obj(), i) == (float64)1;
	case lzh::TP_64D:	return MAT_GETARRAY(float128, data.obj(), i) == (float128)1;
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
}
bool lzh::Array::isOne(Point p) const
{
	return isOne(toidx(p.y, p.x));
}
bool Array::compare(int32 i1, int32 i2, int32 op) const
{
	i1 = toidx(i1); i2 = toidx(i2);
	bool state = false;
	switch (depth()) {
	case lzh::TP_8S: {
		const int8* p = (const int8*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_8U: {
		const uint8* p = (const uint8*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_16S: {
		const int16* p = (const int16*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_16U: {
		const uint16* p = (const uint16*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_32S: {
		const int32* p = (const int32*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_32U: {
		const uint32* p = (const uint32*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_64S: {
		const int64* p = (const int64*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_64U: {
		const uint64* p = (const uint64*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_32F: {
		const float32* p = (const float32*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_64F: {
		const float64* p = (const float64*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	case lzh::TP_64D: {
		const float128* p = (const float128*)data.obj();
		PTR_COMPARE(p, op, i1, i2, state);
		break; }
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
	return state;
}
void lzh::Array::BitNot(int32 i)
{
	i = toidx(i);
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	switch (depth())
	{
	case lzh::TP_8S:	at<int8>(i) = int8(~at<int8>(i));		break;
	case lzh::TP_8U:	at<uint8>(i) = uint8(~at<uint8>(i));	break;
	case lzh::TP_16S:	at<int16>(i) = int16(~at<int16>(i)); 	break;
	case lzh::TP_16U:	at<uint16>(i) = uint16(~at<uint16>(i)); break;
	case lzh::TP_32S:	at<int32>(i) = int32(~at<int32>(i)); 	break;
	case lzh::TP_32U:	at<uint32>(i) = uint32(~at<uint32>(i)); break;
	case lzh::TP_64S:	at<int64>(i) = int64(~at<int64>(i)); 	break;
	case lzh::TP_64U:	at<uint64>(i) = uint64(~at<uint64>(i));	break;
	default:THROW_INFO_STR("位运算不支持浮点类型!");
	}
}
void lzh::Array::BitAnd(int32 i, Val val)
{
	i = toidx(i);
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	switch (depth())
	{
	case lzh::TP_8S:	at<int8>(i) &= val.get<int8>(); 	break;
	case lzh::TP_8U:	at<uint8>(i) &= val.get<uint8>(); 	break;
	case lzh::TP_16S:	at<int16>(i) &= val.get<int16>(); 	break;
	case lzh::TP_16U:	at<uint16>(i) &= val.get<uint16>(); break;
	case lzh::TP_32S:	at<int32>(i) &= val.get<int32>(); 	break;
	case lzh::TP_32U:	at<uint32>(i) &= val.get<uint32>(); break;
	case lzh::TP_64S:	at<int64>(i) &= val.get<int64>(); 	break;
	case lzh::TP_64U:	at<uint64>(i) &= val.get<uint64>(); break;
	default:THROW_INFO_STR("位运算不支持浮点类型!");
	}
}
void lzh::Array::BitOr(int32 i, Val val)
{
	i = toidx(i);
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	switch (depth())
	{
	case lzh::TP_8S:	at<int8>(i) |= val.get<int8>(); 	break;
	case lzh::TP_8U:	at<uint8>(i) |= val.get<uint8>(); 	break;
	case lzh::TP_16S:	at<int16>(i) |= val.get<int16>(); 	break;
	case lzh::TP_16U:	at<uint16>(i) |= val.get<uint16>(); break;
	case lzh::TP_32S:	at<int32>(i) |= val.get<int32>(); 	break;
	case lzh::TP_32U:	at<uint32>(i) |= val.get<uint32>(); break;
	case lzh::TP_64S:	at<int64>(i) |= val.get<int64>();	break;
	case lzh::TP_64U:	at<uint64>(i) |= val.get<uint64>(); break;
	default:THROW_INFO_STR("位运算不支持浮点类型!");
	}
}
const Array lzh::Array::BitNot() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	Array m = clone();
	FOR_RANGE(i, 0, len())m.BitNot(i);
	return m;
}
const Array lzh::Array::BitAnd(Val val) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	Array m = clone();
	switch (val.type)
	{
	case lzh::TP_8S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<int8>()); }break;
	case lzh::TP_8U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<uint8>()); }break;
	case lzh::TP_16S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<int16>()); }break;
	case lzh::TP_16U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<uint16>()); }break;
	case lzh::TP_32S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<int32>()); }break;
	case lzh::TP_32U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<uint32>()); }break;
	case lzh::TP_64S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<int64>()); }break;
	case lzh::TP_64U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, val.get<uint64>()); }break;
	default:THROW_INFO_STR("位运算不支持浮点类型!");
	}
	return m;
}
const Array lzh::Array::BitAnd(const Array& a) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	Array m = clone();
	switch (a.depth())
	{
	case lzh::TP_8S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<int8>(i)); }	break;
	case lzh::TP_8U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<uint8>(i)); }	break;
	case lzh::TP_16S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<int16>(i)); }	break;
	case lzh::TP_16U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<uint16>(i)); }break;
	case lzh::TP_32S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<int32>(i)); }	break;
	case lzh::TP_32U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<uint32>(i)); }break;
	case lzh::TP_64S:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<int64>(i)); }	break;
	case lzh::TP_64U:	FOR_RANGE(i, 0, len()) { m.BitAnd(i, a.at<uint64>(i)); }break;
	default:THROW_INFO_STR("位运算不支持浮点类型!");
	}
	return m;
}
const Array lzh::Array::BitOr(Val val) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	Array m = clone();
	switch (val.type)
	{
	case lzh::TP_8S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<int8>()); }break;
	case lzh::TP_8U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<uint8>()); }break;
	case lzh::TP_16S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<int16>()); }break;
	case lzh::TP_16U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<uint16>()); }break;
	case lzh::TP_32S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<int32>()); }break;
	case lzh::TP_32U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<uint32>()); }break;
	case lzh::TP_64S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<int64>()); }break;
	case lzh::TP_64U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, val.get<uint64>()); }break;
	default:THROW_INFO_STR("位运算不支持浮点类型!");
	}
	return m;
}
const Array lzh::Array::BitOr(const Array& a) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	Array m = clone();
	switch (a.depth())
	{
	case lzh::TP_8S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<int8>(i)); }	break;
	case lzh::TP_8U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<uint8>(i)); }	break;
	case lzh::TP_16S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<int16>(i)); }	break;
	case lzh::TP_16U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<uint16>(i)); }break;
	case lzh::TP_32S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<int32>(i)); }	break;
	case lzh::TP_32U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<uint32>(i)); }break;
	case lzh::TP_64S:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<int64>(i)); }	break;
	case lzh::TP_64U:	FOR_RANGE(i, 0, len()) { m.BitOr(i, a.at<uint64>(i)); }break;
	default:THROW_INFO_STR("位运算不支持浮点类型!");
	}
	return m;
}
Point lzh::Array::toPoint() const
{
	return Point(Round(operator()(0)), Round(operator()(1)));
}
Point2d lzh::Array::toPoint2d() const
{
	return Point2d(get_(0), get_(1));
}
int32 lzh::Array::rankSize(RANK r) const
{
	switch (r)
	{
	case lzh::ROW:return rows;
	case lzh::COL:return cols;
	case lzh::CHANNEL:return c_;
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return rows;
}
mat_t lzh::Array::max(Array mask) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	return max(false, mask);
}
lzh::mat_t Array::max(bool is_abs, Array mask) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	lzh::mat_t maxdata;
	if (mask.empty() || (!mask.empty() && mask.size() == size())) {
		maxdata = is_abs ? std::abs(get_(0)) : get_(0);
		for (int32 ind = 1; ind < len(); ind++) {
			lzh::mat_t v = is_abs ? std::abs(get_(ind)) : get_(ind);
			if (maxdata < v)
				maxdata = v;
		}
	}
	else {
		bool judge_channel = mask.channels() == channels();
		maxdata = is_abs ? std::abs(get_(0)) : get_(0);
		for (int32 ind = 1; ind < len(); ind++) {
			if (mask.isZero(judge_channel ? ind : ind / channels()))continue;
			lzh::mat_t v = is_abs ? std::abs(get_(ind)) : get_(ind);
			if (maxdata < v)
				maxdata = v;
		}
	}
	return maxdata;
}
mat_t lzh::Array::min(Array mask) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	return min(false, mask);
}
lzh::mat_t Array::min(bool is_abs, Array mask) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	lzh::mat_t mindata;
	if (mask.empty() || (!mask.empty() && mask.size() == size())) {
		mindata = is_abs ? std::abs(get_(0)) : get_(0);
		for (int32 ind = 1; ind < len(); ind++) {
			lzh::mat_t v = is_abs ? std::abs(get_(ind)) : get_(ind);
			if (mindata > v)
				mindata = v;
		}
	}
	else {
		bool judge_channel = mask.channels() == channels();
		mindata = is_abs ? std::abs(get_(0)) : get_(0);
		for (int32 ind = 1; ind < len(); ind++) {
			if (mask.isZero(judge_channel ? ind : ind / channels()))continue;
			lzh::mat_t v = is_abs ? std::abs(get_(ind)) : get_(ind);
			if (mindata > v)
				mindata = v;
		}
	}
	return mindata;
}
int32 Array::maxAt(mat_t *v)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	int32 max_adr = 0;
	for (int32 ind = 1; ind < len(); ind++)
		if (compare(max_adr, ind, -2))
			max_adr = ind;
	if (v)*v = get_(max_adr);
	return max_adr;
}
int32 Array::minAt(mat_t *v)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	int32 min_adr = 0;
	for (int32 ind = 1; ind < len(); ind++)
		if (compare(min_adr, ind, 2))
			min_adr = ind;
	if (v)*v = get_(min_adr);
	return min_adr;
}

std::string lzh::Array::infomation() const
{
	return "[row=" + to_string(rows) + ", "
		+ "col=" + to_string(cols) + ", "
		+ "channel=" + to_string(channels()) + ", "
		+ "step=(" + to_string(step[0]) + ", " + to_string(step[1]) + "), "
		+ "type=" + lzh::Type2String(depth()) + ", "
		+ "colorGaumt=" + lzh::Enum2String(_type) + "]";
}
void lzh::Array::showInfomation() const
{
	std::cout << infomation() << std::endl;
}

void Array::show(bool showInfo)const
{
	ostrstream out;
	show(out, showInfo); out << ends;
	std::cout << out.str() << std::endl;
	delete out.str();
}
std::ostream& Array::show(std::ostream& out, bool showInfo) const
{
	if (data == nullptr)return out;
	switch (print_type)
	{
	case lzh::MAT_FIXED:
		out.unsetf(std::ios::fixed);
		break;
	case lzh::MAT_SCIENTIFIC:
		out.setf(std::ios::scientific);
		break;
	default:
		break;
	}
	out.setf(std::ios::showpos);
	out.setf(std::ios::left);
	out << "Array(";
	if (showInfo)out << infomation() << std::endl;
	//else out << std::endl;
	for (int32 i = 0; i < rows; i++) {
		if (!showInfo && i == 0) out << "[";
		else out << "      [";
		for (int32 j = 0; j < cols; j++) {
			out << "[";
			for (int32 k = 0; k < c_; k++) {
				switch (depth())
				{
				case lzh::TP_8S:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << (int16)at<int8>(i, j, k); break;
				case lzh::TP_8U:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << (int16)at<uint8>(i, j, k); break;
				case lzh::TP_16S:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<int16>(i, j, k); break;
				case lzh::TP_16U:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<uint16>(i, j, k); break;
				case lzh::TP_32S:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<int32>(i, j, k); break;
				case lzh::TP_32U:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<uint32>(i, j, k); break;
				case lzh::TP_64S:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<int64>(i, j, k); break;
				case lzh::TP_64U:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<uint64>(i, j, k); break;
				case lzh::TP_32F:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<float32>(i, j, k); break;
				case lzh::TP_64F:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<float64>(i, j, k); break;
				case lzh::TP_64D:	out << std::setw(print_width) << std::setprecision(print_precision) << std::setfill(' ') << at<float128>(i, j, k); break;
				default:
					break;
				}
				if (j == cols - 1 && k == c_ - 1)
				{
					out << "]]";
				}
				else if (k == c_ - 1)
				{
					out << "]";
				}
				else {
					out << ", ";
				}
			}
		}
		if (i != rows - 1)
			out << std::endl;
	}
	out << ", type=" << TypeString[data.depth()] << ")";
	switch (print_type)
	{
	case lzh::MAT_FIXED:
		out.unsetf(std::ios::fixed);
		break;
	case lzh::MAT_SCIENTIFIC:
		out.unsetf(std::ios::scientific);
		break;
	default:
		break;
	}
	out.unsetf(std::ios::showpos);
	out.unsetf(std::ios::left);
	out << std::defaultfloat << std::setprecision(6);
	return out;
}

Array Array::row(int32 h)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (h < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (h >= rows) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	return Array(cols, 1, c_, step, data.offset(h * cols * c_), depth());
}
const Array Array::row(int32 h) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (h < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (h >= rows) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	return Array(cols, 1, c_, step, data.offset(h * cols * c_), depth());
}
Array Array::col(int32 w)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (w < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (w >= cols) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	return Array(1, rows, c_, step, data.offset(w * c_), depth());
}
const Array Array::col(int32 w) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (w < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (w >= cols) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	return Array(1, rows, c_, step, data.offset(w * c_), depth());
}
Array Array::channel(int32 c)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (c < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (c >= c_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	return Array(cols, rows, 1, step, data.offset(c), depth());
}
const Array Array::channel(int32 c) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (c < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (c >= c_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	return Array(cols, rows, 1, step, data.offset(c), depth());
}
const Array Array::range(int32 start, int32 end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (start < 0 || end < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (start > rows || end > rows) THROW_INFO(ERR_INFO_MEMOUT);
	if (start >= end) THROW_INFO(ERR_INFO_UNLESS);
#endif // DEBUG_MAT
	return Array(end - start, 1, step, data, depth());
}
const Array lzh::Array::range(Rect roi) const
{
	return Array(roi.width, roi.height, c_, step, data.offset(toidx(roi.y, roi.x)), depth());
}
const Array lzh::Array::range(int32 h_start, int32 h_end, int32 w_start, int32 w_end) const
{
	h_start = MAX(h_start, 0); w_start = MAX(w_start, 0);
	h_end = MIN(h_end, rows); w_end = MIN(w_end, cols);
	return Array(w_end - w_start, h_end - h_start, c_, step, (const void*)(data.offset(toidx(h_start, w_start))), depth());
}
const Array Array::rowRange(int32 h_start, int32 h_end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (h_start < 0 || h_end < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (h_start > rows || h_end > rows) THROW_INFO(ERR_INFO_MEMOUT);
	//if (h_start == h_end) THROW_INFO(ERR_INFO_UNLESS);
#endif // DEBUG_MAT
	if (h_start > h_end) lzh::Swap(h_start, h_end);
	return Array(cols, h_end - h_start, c_, step, data.offset(h_start * step[0] * step[1]), depth());
}
const Array Array::colRange(int32 w_start, int32 w_end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (w_start < 0 || w_end < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (w_start > cols || w_end > cols) THROW_INFO(ERR_INFO_MEMOUT);
	//if (w_start == w_end) THROW_INFO(ERR_INFO_UNLESS);
#endif // DEBUG_MAT
	if (w_start > w_end) lzh::Swap(w_start, w_end);
	return Array(w_end - w_start, rows, c_, step, data.offset(w_start * step[1]), depth());
}
const Array Array::channelRange(int32 c_start, int32 c_end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (c_start < 0 || c_end < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (c_start > c_ || c_end > c_) THROW_INFO(ERR_INFO_MEMOUT);
	//if (c_start == c_end) THROW_INFO(ERR_INFO_UNLESS);
#endif // DEBUG_MAT
	if (c_start > c_end) lzh::Swap(c_start, c_end);
	return Array(cols, rows, c_end - c_start, step, data.offset(c_start), depth());
}
const Array Array::clone() const
{
	Array dst;
	dst.setvalue(*this);
	return dst;
}
const Array Array::opp()const
{
	Array dst;
	lzh::Opp(*this, dst);
	return dst;
}
const Array Array::adj()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	CHECK_TYPE_(depth(), MAT_T);
#endif //LZH_DEBUG
	if (square) {
		Array dst;
		lzh::adj(*this, dst);
		return dst;
	}
	else THROW_INFO(ERR_INFO_ADJ);
	return Array();
}
const Array Array::t()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	Array dst;
	lzh::tran(*this, dst);
	return dst;
}
const Array Array::inv()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (!square)
		THROW_INFO(ERR_INFO_INV);
	Array dst;
	lzh::inv(*this, dst);
	return dst;
}
const Array Array::diag(int32 k) const
{
	Array dst;
	lzh::diag(*this, dst, k);
	return dst;
}
const Array Array::abs()const
{
	Array dst;
	lzh::Abs(*this, dst);
	return dst;
}
const Array Array::mPow(int32 num) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	if (square) {
		Array dst;
		lzh::POW(*this, dst, num);
		return dst;
	}
	else THROW_INFO(ERR_INFO_POW);
	return Array();
}
const Array Array::pow(lzh::mat_t num)const
{
	Array dst;
	lzh::Pow(*this, dst, num);
	return dst;
}
const Array Array::reverse()const
{
	Array dst; lzh::reverse(*this, dst); return dst;

}
mat_t Array::sum(int32 num, bool _abs)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	mat_t sum = 0;
	if (num == 0) {
		return _T(len());
	}
	else if (num == 1) {
		for (int32 ind = 0; ind < len(); ind++)
			if (_abs)
				sum += std::abs(get_(ind));
			else
				sum += get_(ind);
	}
	else
		for (int32 ind = 0; ind < len(); ind++)
			if (_abs)
				sum += std::pow(std::abs(get_(ind)), num);
			else
				sum += std::pow(get_(ind), num);
	return sum;
}
mat_t Array::mean() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	mat_t sum = 0;
	for (int32 ind = 0; ind < len(); ind++)
		sum += get_(ind);
	return sum / _T(len());
}
lzh::mat_t Array::std() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	return std::sqrt(D());
}
lzh::mat_t Array::D() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	lzh::mat_t m = mean();
	lzh::mat_t d = 0;
	lzh::mat_t n = _T(len());
	FOR_RANGE(i, 0, len())
	{
		d += std::pow(get_(i) - m, 2.0f);
	}
	return d / n;
}
lzh::mat_t Array::Det()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	if (square)
		return lzh::det(*this);
	else
		return NAN;
}
lzh::mat_t Array::norm(int32 num)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	//if (cols != 1 && rows != 1) THROW_INFO(ERR_INFO_NORM);
	if (num < 0) THROW_INFO(ERR_INFO_VALUE);
#endif // LZH_DEBUG
	if (num == 0)
		return sum();
	else if (num == 1)
		return sum(1, true);
	else if (num == 2)
		return std::sqrt(sum(2, true));
	//else if (isinf(num) == 1)
	//	return abs(matrix[find(findmax())]);
	//else if (isinf(num) == -1)
	//	return abs(matrix[find(findmin())]);
	else
		return std::pow(sum(num, true), 1 / _T(num));
}
lzh::mat_t Array::Cof(int32 x, int32 y)
{
	return lzh::cof(*this, x, y);
}
lzh::mat_t Array::EigenvalueMax(lzh::mat_t offset)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	if (square) {
		int32 count = 0;
		lzh::mat_t err = 100 * offset;
		Array v;
		Array u0 = Array::ones(cols, 1, depth());
		while (err > offset) {
			v = *this * u0;
			Array u1 = v * (1 / v.max());
			err = (u1 - u0).abs().max();
			u0 = u1; count += 1;
			if (count >= 1e+3) THROW_INFO(ERR_INFO_EIGEN);
		}
		return v.max();
	}
	else THROW_INFO(ERR_INFO_SQUARE);
	return _T(NAN);
}
const Array Array::EigenvectorsMax(lzh::mat_t offset)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif // LZH_DEBUG
	if (square) {
		int32 count = 0;
		lzh::mat_t err = 100 * offset;
		Array v;
		Array u0 = Array::ones(rows, 1, depth());
		while (err > offset) {
			v = *this * u0;
			Array u1 = v * (1 / v.max());
			err = (u1 - u0).abs().max();
			u0 = u1; count += 1;
			if (count >= 1e+3) THROW_INFO(ERR_INFO_EIGEN);
		}
		return u0;
	}
	else THROW_INFO(ERR_INFO_SQUARE);
	return Array();
}

const Array Array::sigmoid() const
{
	return lzh::sigmoid(toMat()).toArray(depth());
}
const Array Array::tanh() const
{
	return lzh::tanh(toMat()).toArray(depth());
}
const Array Array::relu() const
{
	return lzh::relu(toMat()).toArray(depth());
}
const Array Array::elu() const
{
	return lzh::elu(toMat()).toArray(depth());
}
const Array Array::selu() const
{
	return lzh::selu(toMat()).toArray(depth());
}
const Array Array::leaky_relu() const
{
	return lzh::leaky_relu(toMat()).toArray(depth());
}
const Array Array::softmax() const
{
	return lzh::softmax(toMat()).toArray(depth());
}
const Array Array::exp()const
{
	Array m;
	lzh::Exp(*this, m);
	return m;
}
const Array Array::log()const
{
	Array m;
	lzh::Log(*this, m);
	return m;
}
const Array Array::sqrt()const
{
	Array m;
	lzh::Sqrt(*this, m);
	return m;
}

void Array::setp(int32 i, const void* v, int32 addr_offset)
{
	switch (depth()) {
	case lzh::TP_8S:	set(i, MAT_GETARRAY(const int8, v, addr_offset));		break;
	case lzh::TP_8U:	set(i, MAT_GETARRAY(const uint8, v, addr_offset));		break;
	case lzh::TP_16S:	set(i, MAT_GETARRAY(const int16, v, addr_offset));		break;
	case lzh::TP_16U:	set(i, MAT_GETARRAY(const uint16, v, addr_offset));		break;
	case lzh::TP_32S:	set(i, MAT_GETARRAY(const int32, v, addr_offset));		break;
	case lzh::TP_32U:	set(i, MAT_GETARRAY(const uint32, v, addr_offset));		break;
	case lzh::TP_64S:	set(i, MAT_GETARRAY(const int64, v, addr_offset));		break;
	case lzh::TP_64U:	set(i, MAT_GETARRAY(const uint64, v, addr_offset));		break;
	case lzh::TP_32F:	set(i, MAT_GETARRAY(const float32, v, addr_offset));	break;
	case lzh::TP_64F:	set(i, MAT_GETARRAY(const float64, v, addr_offset));	break;
	case lzh::TP_64D:	set(i, MAT_GETARRAY(const float128, v, addr_offset));	break;
	default:return;
	}
}

void Array::init()
{
	dim = 0;
	rows = 0;
	cols = 0;
	c_ = 0;
	_type = ColorGamutCustom;
}

void Array::checkSquare()
{
	if (rows == cols)
		square = true;
	else
		square = false;
}
#ifdef LZH_DEBUG
void Array::checkindex(int32 index)
{
	if (rows == 0 || cols == 0) THROW_INFO(ERR_INFO_LEN);
	if (index > len() - 1) THROW_INFO(ERR_INFO_MEMOUT);
	if (index < 0) THROW_INFO(ERR_INFO_VALUE);
	if (!data()) THROW_INFO(ERR_INFO_EMPTY);
}
void Array::checkindex(int32 index_x, int32 index_y)
{
	if (rows == 0 || cols == 0)THROW_INFO(ERR_INFO_LEN);
	if (index_x < 0 || index_y < 0) THROW_INFO(ERR_INFO_MEMOUT);
	if (index_x * cols + index_y > rows * cols - 1 || index_x >= rows || index_y >= cols) THROW_INFO(ERR_INFO_VALUE);
	if (!data()) THROW_INFO(ERR_INFO_EMPTY);
}
#endif // LZH_DEBUG
void Array::setsize(int32 w, int32 h, int32 c)
{
	cols = w;
	rows = h;
	c_ = c;
	step = MatStep(w, c);
	if (w == 0 && h == 0 && c == 0)dim = 0;
	else if (w == 1 && h == 1 && c == 1)dim = 0;
	else if (w != 1 && h == 1 && c == 1)dim = 1;
	else if (w == 1 && h != 1 && c == 1)dim = 1;
	else if (w != 1 && h != 1 && c == 1)dim = 2;
	else if (w != 1 && h == 1 && c != 1)dim = 2;
	else if (w == 1 && h != 1 && c != 1)dim = 2;
	else dim = 3;
	square = (h == w);
}

Array& Array::operator = (const Array& temp)
{
	if (this == &temp)
		return *this;
	data = temp.data;
	rows = temp.rows;
	cols = temp.cols;
	c_ = temp.c_;
	step = temp.step;
	dim = temp.dim;
	square = temp.square;
	_type = temp._type;
	return *this;
}
//Array & lzh::Array::operator=(const _InputArray & temp)
//{
//	return *this = temp.getArray();
//}
//Array& lzh::Array::operator=(const Mat& temp)
//{
//	if (data.obj() == temp.data())
//		return *this;
//	data = toVector(temp);
//	rows = temp.rows();
//	cols = temp.cols();
//	c_ = temp.channels();
//	step = temp.step();
//	dim = temp.dims();
//	_type = temp.colorGamut();
//	return *this;
//}
bool Array::operator == (const Array& a)const
{
	if (cols != a.cols) {
		return false;
	}
	if (rows != a.rows) {
		return false;
	}
	if (c_ != a.c_) {
		return false;
	}
	if (memcmp(data, a.data, memsize()) == 0)
		return true;
	return false;
}
bool Array::operator != (const Array& a)const
{
	return !(*this == a);
}
mat_t Array::operator()(int32 i) const
{
	return get_(i);
}
mat_t Array::operator()(int32 i0, int32 i1) const
{
	return get_(i0, i1);
}
mat_t Array::operator()(int32 i0, int32 i1, int32 i2) const
{
	return get_(i0, i1, i2);
}
mat_t lzh::Array::operator()(Point p) const
{
	return get_(p.y, p.x);
}
mat_t lzh::Array::operator()(Point3 p) const
{
	return get_(p.y, p.x, p.z);
}
Array lzh::Array::operator()(Rect roi) const
{
	return range(roi);
}
Array lzh::Array::operator()(Range row, Range col) const
{
	return range(row.v1, row.v2, col.v1, col.v2);
}
Array Array::operator()(int32 i, RANK rc) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (i < 0) THROW_INFO(ERR_INFO_VALUE);
#endif // LZH_DEBUG
	switch (rc) {
	case ROW:
		if (i > rows - 1) THROW_INFO(ERR_INFO_MEMOUT);
		return row(i);
	case COL:
		if (i > cols - 1) THROW_INFO(ERR_INFO_MEMOUT);
		return col(i);
	case CHANNEL:
		if (i > c_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
		return channel(i);
	default:return Array();
	}
}
Array Array::operator()(int32 v1, int32 v2, RANK rc) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
	if (v1 < 0 || v2 < 0) THROW_INFO(ERR_INFO_VALUE);
#endif // LZH_DEBUG
	switch (rc) {
	case ROW:
#ifdef LZH_DEBUG
		if (v1 > cols - 1 || v2 > c_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
#endif // LZH_DEBUG
		return Array(1, rows, 1, step, data.offset(v1 * step[1] + v2), depth());
	case COL:
#ifdef LZH_DEBUG
		if (v1 > rows - 1 || v2 > c_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
#endif // LZH_DEBUG
		return Array(cols, 1, 1, step, data.offset(v1 * step[0] * step[1] + v2), depth());
	case CHANNEL:
#ifdef LZH_DEBUG
		if (v1 > rows - 1 || v2 > cols - 1) THROW_INFO(ERR_INFO_MEMOUT);
#endif // LZH_DEBUG
		return Array(1, 1, c_, step, data.offset(toidx(v1, v2)), depth());
		break;
	default:break;
	}
	return Array();
}
const Array Array::operator[](int32 idx) const
{
	if (dim == 3) {
		return channel(idx);
	}
	else if (dim == 2) {
		return row(idx);
	}
	else if (dim == 1) {
#ifdef LZH_DEBUG
		if (idx < 0) THROW_INFO(ERR_INFO_VALUE);
		if (idx >= len()) THROW_INFO(ERR_INFO_MEMOUT);
#endif // LZH_DEBUG
		return Array(1, 1, 1, data.offset(idx), depth());
	}
	else {
		THROW_INFO(ERR_INFO_UNLESS);
	}
	return Array();
}

bool lzh::Array::operator>(const Array& b) const
{
	FOR_RANGE(i, 0, len() > b.len() ? b.len() : len()) {
		if (compare(i, b(i), 2))
			return true;
		else if (!compare(i, b(i), 0))
			break;
	}
	return false;
}
bool lzh::Array::operator<(const Array& b) const
{
	FOR_RANGE(i, 0, len() > b.len() ? b.len() : len()) {
		if (compare(i, b(i), -2))
			return true;
		else if (!compare(i, b(i), 0))
			break;
	}
	return false;
}

void Array::setPrintW(uint32 w)
{
	print_width = w;
}
void Array::setPrintSignificantDigits(uint32 n)
{
	print_precision = n;
	//print_width = 2 + n + 3;
}
void Array::setPrintType(lzh::PrintType t)
{
	print_type = t;
}

const Array Array::zeros(int32 w, int32 type, ColorGamut cg)
{
	lzh::check(w);
	Array mat(w, type, cg);
	memset(mat, 0, mat.memsize());
	return mat;
}
const Array Array::zeros(int32 w, int32 h, int32 type, ColorGamut cg)
{
	lzh::check(w, h);
	Array mat(w, h, type, cg);
	memset(mat, 0, mat.memsize());
	return mat;
}
const Array Array::zeros(int32 w, int32 h, int32 c, int32 type, ColorGamut cg)
{
	lzh::check(w, h, c);
	Array mat(w, h, c, type, cg);
	memset(mat, 0, mat.memsize());
	return mat;
}
const Array Array::zeros(Size size, int32 type, ColorGamut cg)
{
	lzh::check(size.h, size.w);
	Array mat(size.w, size.h, type, cg);
	memset(mat, 0, mat.memsize());
	return mat;
}
const Array lzh::Array::zeros(Size size, int32 c, int32 type, ColorGamut cg)
{
	lzh::check(size.h, size.w, c);
	Array mat(size.w, size.h, c, type, cg);
	memset(mat, 0, mat.memsize());
	return mat;
}
const Array Array::zeros(Size3 size, int32 type, ColorGamut cg)
{
	Array mat(size, type, cg);
	memset(mat, 0, mat.memsize());
	return mat;
}
const Array Array::value(Val v, int32 w, int32 type, ColorGamut cg)
{
	lzh::check(w);
	Array mark(w, type, cg);
	for (int32 ind = 0; ind < mark.len(); ind++)
		mark.set(ind, v);
	return mark;
}
const Array Array::value(Val v, int32 w, int32 h, int32 type, ColorGamut cg)
{
	lzh::check(w, h);
	Array mark(w, h, type, cg);
	if (isInt(type)) {
		memset(mark, saturate_cast<int>(v.v), mark.memsize());
	}
	else
		for (int32 ind = 0; ind < mark.len(); ind++)
			mark.set(ind, v);
	return mark;
}
const Array Array::value(Val v, int32 w, int32 h, int32 c, int32 type, ColorGamut cg)
{
	lzh::check(w, h, c);
	Array mark(w, h, c, type, cg);
	for (int32 ind = 0; ind < mark.len(); ind++)
		mark.set(ind, v);
	return mark;
}
const Array lzh::Array::value(Val v, Size3 size, int32 flag, ColorGamut cg)
{
	lzh::check(size.w, size.h, size.c);
	Array mark(size.w, size.h, size.c, flag, cg);
	for (int32 ind = 0; ind < mark.len(); ind++)
		mark.set(ind, v);
	return mark;
}
const Array Array::ones(int32 w, int32 type, ColorGamut cg)
{
	return value(1, w, type, cg);
}
const Array Array::ones(int32 w, int32 h, int32 type, ColorGamut cg)
{
	return value(1, w, h, type, cg);
}
const Array Array::ones(int32 w, int32 h, int32 c, int32 type, ColorGamut cg)
{
	return value(1, w, h, c, type, cg);
}
const Array Array::ones(Size size, int32 type, ColorGamut cg)
{
	return value(1, size.w, size.h, type, cg);
}
const Array lzh::Array::ones(Size size, int32 c, int32 type, ColorGamut cg)
{
	return value(1, size.w, size.h, c, type, cg);
}
const Array Array::ones(Size3 size, int32 type, ColorGamut cg)
{
	return value(1, size.w, size.h, size.c, type, cg);
}
const Array lzh::Array::eye(int32 n, int32 type)
{
	lzh::check(n, n);
	Array mark = zeros(n, n, type);
	for (int32 ind = 0; ind < n; ind++)
		mark.set(ind + ind * n, 1);
	return mark;
}

