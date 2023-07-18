#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <strstream>
#define LZH_EXPORTS
#include "mat.h"
#include "tools.h"
#include "operator.h"
#include "algorithm.h"

using namespace lzh;
using namespace std;

lzh::uint32 Mat::print_width = 8;
lzh::uint32 Mat::print_precision = 4;
lzh::PrintType Mat::print_type = lzh::MAT_FIXED;
lzh::mat_t Mat::minimum = _T(1e-7);
bool Mat::setZero = true;

/****************************************************************************
mat_t类型矩阵类
*****************************************************************************/
Mat::Mat()
{
	init();
}
//Mat::Mat(int32 w)
//{
//	init();
//	create(w);
//}
Mat::Mat(int32 w, int32 h, ColorGamut cg)
{
	init();
	create(w, h, cg);
}
Mat::Mat(int32 w, int32 h, int32 c, ColorGamut cg)
{
	init();
	create(w, h, c, cg);
}
Mat::Mat(Size size_, ColorGamut cg)
{
	init();
	create(size_.w, size_.h, cg);
}
Mat::Mat(Size size_, int32 c, ColorGamut cg)
{
	init();
	create(size_.w, size_.h, c, cg);
}
Mat::Mat(Size3 size_, ColorGamut cg)
{
	init();
	create(size_.w, size_.h, size_.c, cg);
}
Mat::Mat(ColorGamut colorGamut, Size sz)
{
}
Mat::Mat(const MatPtr<mat_t>& src, Size3 sz, bool copyData)
{
	init();
	if (copyData)
	{
		create(sz);
		memcpy(m_, src.obj(), memsize());
	}
	else {
		setsize(sz.w, sz.h, sz.c);
		m_ = src.obj();
	}
}
Mat::Mat(const MatCommaInitializer_<mat_t>& m, bool copyData)
{
	init();
	if (copyData) {
		create(m.cols(), m.rows(), m.channels());
		memcpy(m_, m.data(), memsize());
	}
	else {
		setsize(m.cols(), m.rows(), m.channels());
		m_ = m.data();
	}
	//else
	//	for (int32 i = 0; i < N; i++)
	//		m_[i] = _T(vec[i]));
}
Mat::Mat(int32 w, const void* data, bool copyData)
{
	init();
	*this = Mat(w, 1, 1, data, copyData);
}
Mat::Mat(int32 w, int32 h, const void* data, bool copyData)
{
	init();
	*this = Mat(w, h, 1, data, copyData);
}
Mat::Mat(int32 w, int32 h, int32 c, const void* data, bool copyData)
{
	init();
	if (!copyData) {
		setsize(w, h, c);
		m_ = (mat_t*)data;
	}
	else {
		create(w, h, c);
		memcpy(m_, data, memsize());
	}
}
Mat::Mat(Size size, const void* data, bool copyData)
{
	init();
	*this = Mat(size.w, size.h, 1, data, copyData);
}
Mat::Mat(Size size, int32 c, const void* data, bool copyData)
{
	init();
	*this = Mat(size.w, size.h, c, data, copyData);
}
Mat::Mat(Size3 size, const void* data, bool copyData)
{
	init();
	*this = Mat(size.w, size.h, size.c, data, copyData);
}
Mat::Mat(Size3 size, MatStep stride, const void * data, const int * refcount)
{
	init();
	*this = Mat(size.w, size.h, size.c, stride, data, refcount);
}
Mat::Mat(int32 w, int32 h, int32 c, MatStep stride, const void * data, const int * refcount)
{
	init();
	setsize(w, h, c); 
	this->stride = stride;
	m_ = (mat_t*)data;
	this->refcount = (int *)refcount;
	addref();
}
Mat::Mat(int32 w, int32 h, int32 c, MatStep stride, const mat_t* data)
{
	init();
	setsize(w, h, c);
	this->stride = stride;
	m_ = (mat_t*)data;
}
Mat::Mat(const Mat& src)
{
	init();
	*this = src;
}
Mat::Mat(const Array& src)
{
	init();
	*this = src.toMat();
	//if (src.depth() == MAT_T) {
	//	*this = Mat(src.size3(), (const void*)src.data.obj(), false);
	//}
	//else if (src.depth() != NONE) {
	//	create(src.size3());
	//	FOR_RANGE(i, 0, len()) {
	//		m_[i] = src(i);
	//	}
	//}
}
Mat::Mat(const Mat& a, const Mat& b, RANK merge)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(b);
#endif //LZH_DEBUG
	init();
	if (merge == ROW) {
		if (a.w_ == b.w_) {
			create(a.w_, a.h_ + b.h_, a.c_);
			memcpy(m_, a.m_, a.memsize());
			memcpy(m_ + a.len(), b.m_, b.memsize());
		}
	}
	else if (merge == COL) {
		if (a.h_ == b.h_) {
			create(a.w_ + b.w_, a.h_, a.c_);
			for (int32 i = 0; i < h_; i++) {
				memcpy(m_ + i * w_ * c_,
					a.m_ + i * a.w_ * c_,
					a.w_ * c_ * M_SIZE);
				memcpy(m_ + i * w_ * c_ + a.w_ * c_,
					b.m_ + i * b.w_ * c_,
					b.w_ * c_ * M_SIZE);
			}
		}
	}
}
//Mat::Mat(const _InputArray & input)
//{
//	init();
//  *this = input.getMat();
//}
Mat::~Mat()
{
	release();
	setsize(0, 0, 0);
}

void Mat::addref()
{
	if (refcount)
		MAT_XADD(refcount, 1);
}
void Mat::create(int32 w, ColorGamut cg)
{
	create(w, 1, 1, cg);
}
void Mat::create(int32 w, int32 h, ColorGamut cg)
{
	create(w, h, 1, cg);
}
void Mat::create(int32 w, int32 h, int32 c, ColorGamut cg)
{
	release();
	if (w <= 0 || h <= 0 || c <= 0) { setsize(0, 0, 0); return; }
	setsize(w, h, c);
	uint32 totalsize = len() * sizeof(mat_t);
	m_ = (mat_t*)fastMalloc(totalsize + sizeof(int32));
	refcount = (int32*)(((uint8*)m_) + totalsize);
	*refcount = 1;
	_type = cg;
#ifdef LZH_DEBUG
	CHECK_PTR(m_);
#endif //LZH_DEBUG
}
void Mat::create(Size size, ColorGamut cg)
{
	create(size.w, size.h, cg);
}
void Mat::create(Size size, int32 c, ColorGamut cg)
{
	create(size.w, size.h, c, cg);
}
void Mat::create(Size3 size, ColorGamut cg)
{
	create(size.w, size.h, size.c, cg);
}

void Mat::createColorGamut(ColorGamut colorGamut, Size sz, int32 flags)
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
	create(sz.h, sz.w, channel_, colorGamut);
}

ColorGamut Mat::colorGamut() const
{
	return _type;
}
const int32* Mat::memCount() const
{
	return refcount;
}
mat_t* Mat::data()
{
	return m_;
}
const mat_t* Mat::data() const
{
	return m_;
}
mat_t* Mat::begin()
{
	return m_;
}
const mat_t* Mat::begin() const
{
	return (const mat_t*)m_;
}
mat_t* Mat::end()
{
	if (m_ == nullptr)return nullptr;
	if (refcount == nullptr)return (mat_t*)(m_ + len());
	return (mat_t*)refcount;
}
const mat_t* Mat::end() const
{
	if (m_ == nullptr)return nullptr;
	if (refcount == nullptr)return (const mat_t*)(m_ + len());
	return (const mat_t*)refcount;
}
Mat::iterator Mat::begin(RANK r)
{
	return iterator(this, r, 0);
}
Mat::const_iterator Mat::begin(RANK r) const
{
	return iterator(this, r, 0);
}
Mat::iterator Mat::end(RANK r)
{
	return iterator(this, r, rankSize(r) - 1);
}
Mat::const_iterator Mat::end(RANK r) const
{
	return iterator(this, r, rankSize(r) - 1);
}
Point Mat::toPoint() const
{
	return Point(Round(at(0)), Round(at(1)));
}
lzh::Point2m Mat::toPoint2D() const
{
	return Point2m(m_);
}
lzh::Point3m Mat::toPoint3D() const
{
	return Point3m(m_);
}
MatPtr<mat_t>  Mat::ptr()const
{
	return MatPtr<mat_t>(m_, len());
}
mat_t& Mat::first() const
{
	return m_[0];
}
mat_t& Mat::last() const
{
	return m_[len() - 1];
}
Rect Mat::rect() const
{
	return Rect(0, 0, w_, h_);
}
void Mat::set(const Mat& m)
{
	*this = m;
}
uint32 Mat::memsize() const
{
	return M_SIZE * h_ * w_ * c_;
}
void Mat::DimCheck()const
{
	if (c_ != 1) {
		THROW_INFO(ERR_INFO_DIM);
	}
}
void Mat::copy(Mat& src, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif // LZH_DEBUG
	int32 hei = Row_End - Row_Start + 1;
	int32 wid = Col_End - Col_Start + 1;
	check(hei, wid);
	if (src.m_ == nullptr) {
		src = zeros(wid, hei, c_);
	}
	for (int32 y = Row_Start, j = 0; y <= Row_End; y++, j++)
		for (int32 x = Col_Start, i = 0; x <= Col_End; x++, i++)
			for (int32 z = 0; z < c_; z++)
				src(y, x, z) = m_[toidx(j, i, z)];
}
void Mat::swap(Mat& src)
{
	std::swap(src, *this);
}

Mat Mat::addones(Dire dire) const
{
	return addvalue(dire, 1);
}
Mat Mat::addzeros(Dire dire) const
{
	return addvalue(dire, 0);
}
void Mat::mChannel(const Mat& src, int32 channels)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	CHECK_MATRIX(src.m_);
	if (h_ != src.h_ || w_ != src.w_ || channels >= c_)
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	for (int32 i = 0; i < h_; ++i) {
		for (int32 j = 0; j < w_; ++j) {
			m_[toidx(i, j, channels)] = src(i, j);
		}
	}
}
void Mat::mChannel(const Mat& src, int32 w, int32 h)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	CHECK_MATRIX(src.m_);
	if (this->h_ <= h_ || this->w_ <= w_ || src.channels() != c_)
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	for (int32 i = 0; i < c_; ++i)
		m_[toidx(h_, w_, i)] = src(i);
}
Mat Mat::reshape(Size3 size)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (len() != size.area())
		THROW_INFO(ERR_INFO_UNLESS);
#endif // LZH_DEBUG
	return reshape(size.w, size.h, size.c);
}
Mat Mat::reshape(int32 w, int32 h, int32 c)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif // LZH_DEBUG
	if (h == 0) {
		h = h_;
	}
	if (w == 0) {
		w = w_;
	}
	if (c == 0) {
		c = c_;
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
bool Mat::setSize(int32 w, int32 h, int32 c)
{
	if (h * w * c > 0) {
		create(w, h, c);
		return true;
	}
	if (len() == h * w * c) {
		setsize(w, h, c);
		return true;
	}
	return false;
}
void Mat::setvalue(const Mat& src)
{
	create(src.size3(), src.colorGamut());
	src.setTo(*this);
}
void Mat::setOpp()
{
	lzh::Opp(*this, *this);
}
void Mat::setAdj()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (square)
		* this = adj();
	else THROW_INFO(ERR_INFO_ADJ);
}
void Mat::setTran()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	* this = t();
}
std::string Mat::infomation() const
{
	return "[row=" + to_string(h_) + ", "
		+ "col=" + to_string(w_) + ", "
		+ "channel=" + to_string(channels()) + ", "
		+ "step=(" + to_string(stride[0]) + ", " + to_string(stride[1]) + "), "
		+ "type=" + lzh::Type2String(depth()) + ", "
		+ "colorGaumt=" + lzh::Enum2String(_type) + "]";
}
void Mat::showInfomation() const
{
	std::cout << infomation() << std::endl;
}
void Mat::setColorGamut(ColorGamut cg)
{
	_type = cg;
}
void Mat::setInv()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (enable() == 0)
		* this = inv();
	else THROW_INFO(ERR_INFO_INV);
}
void Mat::setPow(mat_t num)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (enable() == 0)
		* this = pow(num);
	else THROW_INFO(ERR_INFO_POW);
}
void Mat::setIden()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (enable() == 0)
		* this = eye(w_);
	else THROW_INFO(ERR_INFO_IND);
}

Size3 Mat::size3() const
{
	return Size3(w_, h_, c_);
}
int32 Mat::total() const
{
	return stride[0] * stride[1];
}
int32 Mat::element() const
{
	return h_ * stride[0] * stride[1];
}
int32 Mat::total_() const
{
	return w_ * c_;
}
MatStep Mat::step() const
{
	return stride;
}
Point2_<int32> Mat::center() const
{
	return Point(w_ / 2, h_ / 2);
}
int32 Mat::dims() const
{
	return dim;
}
int32 Mat::depth() const
{
	return MAT_T;
}
int32 Mat::type() const
{
	return MAT_T;
}
int32 Mat::rows()const
{
	return h_;
}
int32 Mat::cols()const
{
	return w_;
}
int32 Mat::channels() const
{
	return c_;
}
int32 Mat::s() const
{
	return stride[0];
}
int32 Mat::o() const
{
	return stride[1];
}
int32 Mat::h() const
{
	return h_;
}
int32 Mat::w() const
{
	return w_;
}
int32 Mat::c() const
{
	return c_;
}
int32 Mat::rank() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (dim != 2)
		THROW_INFO(ERR_INFO_NOT2D);
#endif //LZH_DEBUG
	int32 rank = h_;
	for (int32 i = 0; i < h_; i++) {
		int32 count = 0;
		for (int32 j = 0; j < w_; j++) {
			if (m_[toidx(j, i * w_)] == 0) {
				count++;
			}
		}
		if (count == h_)
			rank--;
	}
	return rank;
}
Size Mat::size()const
{
	return Size(w_, h_);
}
int32 Mat::rankSize(RANK r) const
{
	switch (r)
	{
	case lzh::ROW:return h_;
	case lzh::COL:return w_;
	case lzh::CHANNEL:return c_;
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return h_;
}
int32 Mat::toidx(int32 i) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (i >= len())
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	if (i < 0) {
		i = len() + i;
	}
	int32 i0 = i / total_();
	int32 i1 = (i - i0 * total_()) / c_;
	int32 i2 = i % c_;
	return toidx(i0, i1, i2);
	//return i * stride[1] / c_;
}
int32 Mat::toidx(int32 i, int32 j) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (i * j * c_ >= len() || i > h_ || j > w_)
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	if (i < 0) {
		i = h_ + i;
	}
	if (j < 0) {
		j = w_ + j;
	}
	return (i * stride[0] + j) * stride[1];
}
int32 Mat::toidx(int32 i, int32 j, int32 k) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (i * j >= len() || i > h_ || j > w_ || k > c_)
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	if (i < 0) {
		i = h_ + i;
	}
	if (j < 0) {
		j = w_ + j;
	}
	if (k < 0) {
		k = c_ + k;
	}
	return (i * stride[0] + j) * stride[1] + k;
}
void lzh::Mat::setStep(MatStep st)
{
	stride = st;
}
bool lzh::Mat::checkIndex(Rect rect)const
{
	return checkIndex(rect.x, rect.x + rect.width, rect.y, rect.y + rect.height);
}
bool lzh::Mat::checkIndex(int32 x0, int32 x1, int32 y0, int32 y1)const
{
	if (x0 < 0 || x1 < 0 || y0 < 0 || y1 < 0)return false;
	if (x0 >= cols() || x1 > cols() || y0 >= rows() || y1 > rows())return false;
	if (!(0 < x0 && x0 < x1))return false;
	if (!(0 < y0 && y0 < y1))return false;
	return true;
}
int32 Mat::len()const
{
	return h_ * w_ * c_;
}
int32 Mat::enable()const
{
	if (m_ == nullptr)
		return -1;
	if (!square)
		return -2;
	return 0;
}
bool Mat::empty()const
{
	return (m_ == nullptr);
}
bool Mat::isDeepCopy() const
{
	return refcount != nullptr;
}
bool Mat::isSquare()const
{
	return square;
}
void Mat::save(std::string file, bool binary) const
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
void Mat::save(FILE* out) const
{
	if (out) {
		int32 param[5] = { w_, h_, c_, MAT_T, _type };
		fwrite(param, sizeof(int32) * 5, 1, out);
		fwrite(m_, memsize(), 1, out);
	}
	else {
		THROW_INFO(ERR_INFO_FILE);
	}
}
void Mat::load(std::string file)
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
void Mat::load(FILE* in)
{
	if (in) {
		int32 param[5] = { 0 };
		fread(param, sizeof(int32) * 5, 1, in);
		create(param[0], param[1], param[2], (ColorGamut)param[4]);
		fread(m_, memsize(), 1, in);
	}
	else {
		THROW_INFO(ERR_INFO_FILE);
	}
}
void Mat::copyTo(Mat& mat) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	mat.create(size3());
	memcpy(mat.data(), m_, memsize());
}
void Mat::setTo(Mat& mat) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (mat.empty())mat.create(size3());
	if (isDeepCopy() && mat.isDeepCopy())
		memcpy(mat.data(), m_, memsize());
	else
		FOR_RANGE(i, 0, mat.len())
		mat(i) = operator()(i);
}
void Mat::setTo(Mat&& mat) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (isDeepCopy() && mat.isDeepCopy())
		memcpy(mat.data(), m_, memsize());
	else
		FOR_RANGE(i, 0, len())
		mat(i) = operator()(i);
}
void Mat::convertTo(Mat& m, int32 type) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	m.create(size3());
	FOR_RANGE(i, 0, len())
		m(i) = operator()(i);
}
void Mat::setAddOnes(Dire dire)
{
	*this = addones(dire);
}
void Mat::setAddZeros(Dire dire)
{
	*this = addzeros(dire);
}
void Mat::release()
{
	if (refcount && MAT_XADD(refcount, -1) == 1) {
		fastFree(m_);
		//fastFree(m_);
		m_ = 0;
		refcount = 0;
	}
}
mat_t & Mat::at(int32 w)
{
	return operator()(w);
}
mat_t & Mat::at(int32 h, int32 w)
{
	return operator()(h, w);
}
mat_t & Mat::at(int32 h, int32 w, int32 c)
{
	return operator()(h, w, c);
}
mat_t & Mat::at(Point p)
{
	return operator()(p.y, p.x);
}
mat_t & Mat::at(Point3i p)
{
	return operator()(p.y, p.x, p.z);
}
const mat_t & Mat::at(int32 w) const
{
	return operator()(w);
}
const mat_t & Mat::at(int32 h, int32 w) const
{
	return operator()(h, w);
}
const mat_t & Mat::at(int32 h, int32 w, int32 c) const
{
	return operator()(h, w, c);
}
const mat_t & Mat::at(Point p) const
{
	return operator()(p.y, p.x);
}
const mat_t & Mat::at(Point3i p) const
{
	return operator()(p.y, p.x, p.z);
}
int32 Mat::toX(int32 index)const
{
	return (index / c_) % w_;
}
int32 Mat::toY(int32 index)const
{
	return (index / c_) / w_;
}
int32 Mat::toZ(int32 index) const
{
	return index % c_;
}
bool Mat::isZero(int32 i) const
{
	return m_[i] == _T(0);
}
bool Mat::isZero(int32 i, Val_ offset) const
{
	if (offset == _T(0))return isZero(i);
	return _RANGE_(_T(-offset), m_[i], _T(offset));
}
bool Mat::isOne(int32 i) const
{
	return m_[i] == _T(1);
}
bool Mat::compare(int32 i1, int32 i2, int32 op) const
{
	i1 = toidx(i1);
	i2 = toidx(i2);
	bool state = false;
	PTR_COMPARE(m_, op, i1, i2, state);
	return state;
}
bool Mat::compare(int32 i, Val_ v, int32 op) const
{
	bool status;
	COMPARE(m_[toidx(i)], v, op, status);
	return status;
}
bool Mat::compare(int32 i, int32 j, Val_ v, int32 op)
{
	bool status;
	COMPARE(m_[toidx(i, j)], v, op, status);
	return status;
}
bool Mat::compare(int32 i, int32 j, int32 k, Val_ v, int32 op)
{
	bool status;
	COMPARE(m_[toidx(i, j, k)], v, op, status);
	return status;
}
void Mat::fill(mat_t v)
{
	if (v == 0 && isDeepCopy())
		memset(m_, 0, memsize());
	else
		FOR_RANGE(i, 0, len()) m_[toidx(i)] = v;
}
mat_t Mat::max(bool is_abs) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	mat_t maxdata = is_abs ? std::abs(operator()(0)) : operator()(0);
	for (int32 ind = 1; ind < len(); ind++) {
		mat_t v = is_abs ? std::abs(operator()(ind)) : operator()(ind);
		if (maxdata < v)
			maxdata = v;
	}
	return maxdata;
}
mat_t Mat::min(bool is_abs) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	mat_t mindata = is_abs ? std::abs(operator()(0)) : operator()(0);
	for (int32 ind = 1; ind < len(); ind++) {
		mat_t v = is_abs ? std::abs(operator()(ind)) : operator()(ind);
		if (mindata > v)
			mindata = v;
	}
	return mindata;
}
mat_t& Mat::findmax()
{
	return operator()(maxAt());
}
mat_t& Mat::findmin()
{
	return operator()(minAt());
}
int32 Mat::find(mat_t value) const
{
	return 0;
}
int32 Mat::maxAt()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	int32 max_adr = 0;
	for (int32 ind = 1; ind < len(); ind++)
		if (compare(max_adr, ind, LESS))
			max_adr = ind;
	return max_adr;
}
int32 Mat::minAt()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	int32 min_adr = 0;
	for (int32 ind = 1; ind < len(); ind++)
		if (compare(min_adr, ind, GREATER))
			min_adr = ind;
	return min_adr;
}

void Mat::show(bool showInfo)const
{
	ostrstream out;
	show(out, showInfo); out << ends;
	std::cout << out.str() << std::endl;
	delete out.str();
	//show(std::cout, showInfo);
	//std::cout << std::endl;
}
std::ostream& Mat::show(std::ostream& out, bool showInfo) const
{
	if (m_ == nullptr)return out;
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
	out << "Mat(";
	if(showInfo) out << infomation() << std::endl;
	for (int32 i = 0; i < h_; i++) {
		if (!showInfo && i != 0) out << "    [";
		else out << "[";
		for (int32 j = 0; j < w_; j++) {
			out << "[";
			for (int32 k = 0; k < c_; k++) {
				mat_t v = operator()(i, j, k);
				out << std::setw(print_width)
					<< std::setprecision(print_precision)
					<< std::setfill(' ')
					<< (setZero ? isEqual(v, 0, minimum) ? 0 : v : v);
				if (j == w_ - 1 && k == c_ - 1)
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
		if (i != h_ - 1)
			out << std::endl;
	}
	out << ", type=" << DataType<mat_t>().type_name << ")";
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

Mat Mat::row(int32 h)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (h >= h_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (h < 0) h = h_ - 1;
	return Mat(w_, 1, c_, stride, m_ + h * stride[0] * stride[1]);
}
const Mat Mat::row(int32 h) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (h >= h_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (h < 0) h = h_ - 1;
	return Mat(w_, 1, c_, stride, m_ + h * stride[0] * stride[1]);
}
Mat Mat::col(int32 w)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (w >= w_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (w < 0) w = w_ - 1;
	return Mat(1, h_, c_, stride, m_ + w * stride[1]);
}
const Mat Mat::col(int32 w) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (w >= w_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (w < 0) w = w_ - 1;
	return Mat(1, h_, c_, stride, m_ + w * stride[1]);
}
Mat Mat::channel(int32 c)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (c >= c_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (c < 0) c = c_ - 1;
	return Mat(w_, h_, 1, stride, m_ + c);
}
const Mat Mat::channel(int32 c) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (c >= c_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (c < 0) c = c_ - 1;
	return Mat(w_, h_, 1, stride, m_ + c);
}
Mat Mat::addvalue(Dire dire, Val_ v) const
{
	Mat temp(w_ + 1, h_, c_);
	for (int32 i = 0; i < h_; i++) {
		for (int32 j = 0; j < w_ + 1; j++) {
			for (int32 z = 0; z < c_; z++) {
				if (dire == LEFT) {
					if (j == 0)
						temp(i, 0, z) = v;
					else
						temp(i, j, z) = operator()(i, j - 1, z);
				}
				else if (dire == RIGHT) {
					if (j == w_)
						temp(i, w_, z) = v;
					else
						temp(i, j, z) = operator()(i, j, z);
				}
			}
		}
	}
	return temp;
}
Mat Mat::range(int32 start, int32 end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (start < 0 || end == 0) THROW_INFO(ERR_INFO_UNLESS);
	if (start >= len() || end > len()) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (end < 0)
		return Mat(len() + end - start, 1, 1, m_ + start);
	else
		return Mat(end - start, 1, 1, m_ + start);
}
Mat Mat::range(Rect roi_) const
{
	return Mat(roi_.width, roi_.height, c_, stride, m_ + toidx(roi_.y, roi_.x));
}
Mat Mat::range(int32 h_start, int32 h_end, int32 w_start, int32 w_end) const
{
	return Mat(w_end - w_start, h_end - h_start, c_, stride, m_ + toidx(h_start, w_start));
}
Mat Mat::rowRange(int32 h_start, int32 h_end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (h_start < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (h_start > h_ || h_end > h_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (h_end < 0)
		return Mat(w_, h_ + h_end - h_start, c_, stride, m_ + h_start * stride[0] * stride[1]);
	else
		return Mat(w_, h_end - h_start, c_, stride, m_ + h_start * stride[0] * stride[1]);
}
Mat Mat::colRange(int32 w_start, int32 w_end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (w_start < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (w_start > w_ || w_end > w_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (w_end < 0)
		return Mat(w_ + w_end - w_start, h_, c_, stride, m_ + w_start * stride[1]);
	else
		return Mat(w_end - w_start, h_, c_, stride, m_ + w_start * stride[1]);
}
Mat Mat::channelRange(int32 c_start, int32 c_end)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (c_start < 0) THROW_INFO(ERR_INFO_UNLESS);
	if (c_start > c_ || c_end > c_) THROW_INFO(ERR_INFO_MEMOUT);
#endif // DEBUG_MAT
	if (c_end < 0)
		return Mat(w_, h_, c_ + c_end - c_start, stride, m_ + c_start);
	else
		return Mat(w_, h_, c_end - c_start, stride, m_ + c_start);
	//Mat t;
	//Block(*this, t, 0, h_ - 1, 0, w_ - 1, c_start, c_end);
	//return t;
}
Mat Mat::clone() const
{
	Mat dst;
	dst.setvalue(*this);
	return dst;
}
Mat Mat::opp()const
{
	Mat dst;
	lzh::Opp(*this, dst);
	return dst;
}
Mat Mat::adj()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (square) {
		Mat dst;
		lzh::adj(*this, dst);
		return dst;
	}
	else THROW_INFO(ERR_INFO_ADJ);
	return Mat();
}
Mat Mat::t()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	Mat dst;
	lzh::tran(*this, dst);
	return dst;
}
Mat Mat::inv()const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (!square)
		THROW_INFO(ERR_INFO_INV);
	Mat dst;
	lzh::inv(*this, dst);
	return dst;
}
Mat Mat::diag(int32 k) const
{
	Mat dst;
	lzh::diag(*this, dst, k);
	return dst;
}
Mat Mat::abs()const
{
	Mat dst;
	lzh::Abs(*this, dst);
	return dst;
}
Mat Mat::mPow(int32 num) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	if (square) {
		Mat dst;
		lzh::POW(*this, dst, num);
		return dst;
	}
	else THROW_INFO(ERR_INFO_POW);
	return Mat();
}
Mat Mat::pow(Val_ num)const
{
	Mat dst;
	lzh::Pow(*this, dst, num);
	return dst;
}
Mat Mat::reverse()const
{
	Mat dst; lzh::reverse(*this, dst); return dst;
}
mat_t Mat::sum(int32 num, bool _abs)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	mat_t sum = 0;
	if (num == 0) {
		return (mat_t)len();
	}
	else if (num == 1) {
		for (int32 ind = 0; ind < len(); ind++) {
			if (_abs)sum += std::abs(operator()(ind));
			else sum += operator()(ind);
		}
	}
	else
		for (int32 ind = 0; ind < len(); ind++) {
			if (_abs) sum += std::pow(std::abs(operator()(ind)), num);
			else sum += std::pow(operator()(ind), num);
		}
	return sum;
}
mat_t Mat::mean() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	mat_t sum = 0;
	for (int32 ind = 0; ind < len(); ind++)
		sum += operator()(ind);
	return sum / (mat_t)len();
}
mat_t Mat::std() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	return std::sqrt(D());
}
mat_t Mat::D() const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif //LZH_DEBUG
	mat_t m = mean();
	mat_t d = 0;
	mat_t n = (mat_t)len();
	FOR_RANGE(i, 0, len())
	{
		d += std::pow(operator()(i) - m, 2.0f);
	}
	return d / n;
}
Mat Mat::add(const Mat& m) const
{
	return *this + m;
}
Mat Mat::reduce(const Mat& m) const
{
	return *this - m;
}
Mat Mat::div(const Mat& m) const
{
	return *this / m;
}
Mat lzh::Mat::safediv(const Mat & a, Val def) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX(m_);
#endif // LZH_DEBUG
	if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		return (isEqual(a(0), 0) ? isnan(def) ? *this : Mat::value(def.v, size3()) : *this / a(0));
	}
	else if (rows() == 1 && cols() == 1 && channels() == 1) {
		return first() / a;
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == channels()) {
		Mat mat(size3());
		mat_t* p = mat.data();
		for (int32 z = 0; z < channels(); z++) {
			for (int32 i = 0; i < rows(); i++) {
				for (int32 j = 0; j < cols(); j++) {
					*p = saveDiv(operator()(i, j, z), a(z), isnan(def) ? operator()(i, j, z) : def.v);
					++p;
				}
			}
		}
		return mat;
	}
#ifdef LZH_DEBUG
	if (size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Mat mark(size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < len(); i++)
		p[i] = saveDiv(operator()(i), a(i), isnan(def) ? operator()(i) : def.v);
	return mark;
}
Mat Mat::mul(const _InputArray& m) const
{
	Mat t;
	Mult(*this, m, t);
	return t;
}
Mat Mat::add(mat_t v) const
{
	return *this + v;
}
Mat Mat::reduce(mat_t v, Dire dire) const
{
	return dire == LEFT ? v - *this : *this - v;
}
Mat Mat::div(mat_t v, Dire dire) const
{
	return dire == LEFT ? v / *this : *this / v;
}
Mat Mat::mul(mat_t v) const
{
	return *this * v;
}
const Array lzh::Mat::toArray(int type) const
{
	if (type == MAT_T)
	{
		Array m(size3(), stride, (const void*)m_, refcount, MAT_T);
		m.setColorGamut(_type);
		return m;
	}
	else {
		Array m(size3(), type, _type);
		FOR_RANGE(i, 0, len())
			m.set(i, operator()(i));
		return m;
	}
}
mat_t Mat::tr() const
{
	return trace(*this);
}
mat_t Mat::det()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif // LZH_DEBUG
	if (square)
		return lzh::det(*this);
	else
		return NAN;
}
mat_t Mat::norm(int32 num)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	//if (w_ != 1 && h_ != 1) THROW_INFO(ERR_INFO_NORM);
	if (num < 0) THROW_INFO(ERR_INFO_VALUE);
#endif // LZH_DEBUG
	if (num == 0)
		return sum();
	else if (num == 1)
		return sum(1, true);
	else if (num == 2)
		return std::sqrt(sum(2));
	//else if (isinf(num) == 1)
	//	return abs(matrix[find(findmax())]);
	//else if (isinf(num) == -1)
	//	return abs(matrix[find(findmin())]);
	else
		return std::pow(sum(num, true), 1 / _T(num));
}
mat_t Mat::cof(int32 x, int32 y)
{
	return lzh::cof(*this, x, y);
}
mat_t Mat::EigenvalueMax(mat_t offset)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif // LZH_DEBUG
	if (square) {
		int32 count = 0;
		mat_t err = 100 * offset;
		Mat v;
		Mat u0 = ones(w_, 1);
		while (err > offset) {
			v = *this * u0;
			Mat u1 = v * (1 / v.max());
			err = (u1 - u0).abs().max();
			u0 = u1; count += 1;
			if (count >= 1e+3) THROW_INFO(ERR_INFO_EIGEN);
		}
		return v.max();
	}
	else THROW_INFO(ERR_INFO_SQUARE);
	return _T(NAN);
}
mat_t Mat::RandSample()
{
	return lzh::randSample(*this);
}
Mat Mat::EigenvectorsMax(mat_t offset)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
#endif // LZH_DEBUG
	if (square) {
		int32 count = 0;
		mat_t err = 100 * offset;
		Mat v;
		Mat u0 = ones(h_, 1);
		while (err > offset) {
			v = *this * u0;
			Mat u1 = v * (1 / v.max());
			err = (u1 - u0).abs().max();
			u0 = u1; count += 1;
			if (count >= 1e+3) THROW_INFO(ERR_INFO_EIGEN);
		}
		return u0;
	}
	else THROW_INFO(ERR_INFO_SQUARE);
	return Mat();
}

Mat Mat::sigmoid() const
{
	return lzh::sigmoid(*this);
}
Mat Mat::tanh() const
{
	return lzh::tanh(*this);
}
Mat Mat::relu() const
{
	return lzh::relu(*this);
}
Mat Mat::elu() const
{
	return lzh::elu(*this);
}
Mat Mat::selu() const
{
	return lzh::selu(*this);
}
Mat Mat::leaky_relu() const
{
	return lzh::leaky_relu(*this);
}
Mat Mat::softmax() const
{
	return lzh::softmax(*this);
}
Mat lzh::Mat::sin() const
{
	Mat dst; Sin(*this, dst); return dst;
}
Mat lzh::Mat::cos() const
{
	Mat dst; Cos(*this, dst); return dst;
}
Mat lzh::Mat::tan() const
{
	Mat dst; Tan(*this, dst); return dst;
}
Mat lzh::Mat::asin() const
{
	Mat dst; arcSin(*this, dst); return dst;
}
Mat lzh::Mat::acos() const
{
	Mat dst; arcCos(*this, dst); return dst;
}
Mat lzh::Mat::atan() const
{
	Mat dst; arcTan(*this, dst); return dst;
}
Mat Mat::exp()const
{
	Mat dst;
	Exp(*this, dst);
	return dst;
}
Mat Mat::log()const
{
	Mat dst;
	Log(*this, dst);
	return dst;
}
Mat Mat::sqrt()const
{
	Mat dst;
	Sqrt(*this, dst);
	return dst;
}

void Mat::init()
{
	dim = 0;
	h_ = 0;
	w_ = 0;
	c_ = 0;
	stride[0] = 0;
	stride[1] = 0;
	refcount = 0;
	m_ = 0;
	_type = ColorGamutCustom;
}
#ifdef LZH_DEBUG
void Mat::checkindex(int32 index)
{
	if (h_ == 0 || w_ == 0) THROW_INFO(ERR_INFO_LEN);
	if (index > len() - 1) THROW_INFO(ERR_INFO_MEMOUT);
	if (index < 0) THROW_INFO(ERR_INFO_VALUE);
	if (!m_) THROW_INFO(ERR_INFO_EMPTY);
}
void Mat::checkindex(int32 index_x, int32 index_y)
{
	if (h_ == 0 || w_ == 0)THROW_INFO(ERR_INFO_LEN);
	if (index_x < 0 || index_y < 0) THROW_INFO(ERR_INFO_MEMOUT);
	if (index_x * w_ + index_y > h_ * w_ - 1 || index_x >= h_ || index_y >= w_) THROW_INFO(ERR_INFO_VALUE);
	if (!m_) THROW_INFO(ERR_INFO_EMPTY);
}
#endif // LZH_DEBUG
void Mat::setsize(int32 w, int32 h, int32 c)
{
	w_ = w;
	h_ = h;
	c_ = c;
	stride = MatStep(w, c);
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

/****************************************************************************
生成矩阵
*****************************************************************************/
OMat Mat::zeros(int32 w, ColorGamut cg)
{
	if (w <= 0)return Mat();
	Mat mat(w, 1, cg);
	memset(mat.data(), 0, mat.memsize());
	return mat;
}
OMat Mat::zeros(int32 w, int32 h, ColorGamut cg)
{
	if (w <= 0 || h <= 0)return Mat();
	Mat mat(w, h, cg);
	memset(mat.data(), 0, mat.memsize());
	return mat;
}
OMat Mat::zeros(int32 w, int32 h, int32 c, ColorGamut cg)
{
	if (w <= 0 || h <= 0 || c <= 0)return Mat();
	Mat mat(w, h, c, cg);
	memset(mat.data(), 0, mat.memsize());
	return mat;
}
OMat Mat::zeros(Size size, ColorGamut cg)
{
	if (size.w <= 0 || size.h <= 0)return Mat();
	Mat mat(size, cg);
	memset(mat.data(), 0, mat.memsize());
	return mat;
}
OMat Mat::zeros(Size size, int32 c, ColorGamut cg)
{
	if (size.w <= 0 || size.h <= 0 || c <= 0)return Mat();
	Mat mat(size, c, cg);
	memset(mat.data(), 0, mat.memsize());
	return mat;
}
OMat Mat::zeros(Size3 size, ColorGamut cg)
{
	Mat mat(size, cg);
	memset(mat.data(), 0, mat.memsize());
	return mat;
}
OMat Mat::value(Val v, int32 w, ColorGamut cg)
{
	lzh::check(w);
	Mat mark(w, 1, cg);
	mat_t* p = mark.data();
	for (int32 ind = 0; ind < mark.len(); ind++)
		p[ind] = v;
	return mark;
}
OMat Mat::value(Val v, int32 w, int32 h, ColorGamut cg)
{
	lzh::check(w, h);
	Mat mark(w, h, cg);
	mat_t* p = mark.data();
	for (int32 ind = 0; ind < mark.len(); ind++)
		p[ind] = v;
	return mark;
}
OMat Mat::value(Val v, int32 w, int32 h, int32 c, ColorGamut cg)
{
	lzh::check(w, h, c);
	Mat mark(w, h, c, cg);
	mat_t* p = mark.data();
	for (int32 ind = 0; ind < mark.len(); ind++)
		p[ind] = v;
	return mark;
}
Mat lzh::Mat::value(Val v, Size3 size, ColorGamut cg)
{
	lzh::check(size.w, size.h, size.c);
	Mat mark(size.w, size.h, size.c, cg);
	mat_t* p = mark.data();
	for (int32 ind = 0; ind < mark.len(); ind++)
		p[ind] = v;
	return mark;
}
OMat Mat::ones(int32 w, ColorGamut cg)
{
	return value(1, w, cg);
}
OMat Mat::ones(int32 w, int32 h, ColorGamut cg)
{
	return value(1, w, h, cg);
}
OMat Mat::ones(int32 w, int32 h, int32 c, ColorGamut cg)
{
	return value(1, w, h, c, cg);
}
OMat Mat::ones(Size size, int32 c, ColorGamut cg)
{
	lzh::check(size.w, size.h);
	return value(1, size.w, size.h, c, cg);
}
OMat Mat::ones(Size size, ColorGamut cg)
{
	lzh::check(size.w, size.h);
	return value(1, size.w, size.h, cg);
}
OMat Mat::ones(Size3 size, ColorGamut cg)
{
	return value(1, size.w, size.h, size.c, cg);
}
OMat Mat::eye(int32 n)
{
	lzh::check(n, n);
	Mat mark = zeros(n, n);
	mat_t* p = mark.data();
	for (int32 ind = 0; ind < n; ind++)
		p[ind + ind * n] = 1;
	return mark;
}
Mat& Mat::operator = (const Mat& temp)
{
	if (this == &temp)
		return *this;
	if (temp.refcount)
		MAT_XADD(temp.refcount, 1);

	release();

	m_ = temp.m_;
	h_ = temp.h_;
	w_ = temp.w_;
	c_ = temp.c_;
	stride = temp.stride;
	dim = temp.dim;
	square = temp.square;
	_type = temp._type;
	refcount = temp.refcount;
	return *this;
}
Mat& lzh::Mat::operator=(const Array& temp)
{
	if (temp.ptr().memCount())
		MAT_XADD(temp.ptr().memCount(), 1);

	release();

	if (temp.depth() == MAT_T) {
		m_ = (mat_t*)temp.data.obj();
		h_ = temp.rows;
		w_ = temp.cols;
		c_ = temp.channels();
		stride = temp.step;
		dim = temp.dims();
		square = temp.isSquare();
		refcount = temp.ptr().memCount();
		_type = temp.colorGamut();
	}
	else {
		create(temp.size3());
		FOR_RANGE(i, 0, len())m_[i] = temp.get<mat_t>(i);
	}
	return *this;
}
bool Mat::operator == (const Mat& a)const
{
	return m_ == a.m_;
	//if (w_ != a.w_) {
	//	return false;
	//}
	//if (h_ != a.h_) {
	//	return false;
	//}
	//if (c_ != a.c_) {
	//	return false;
	//}
	//if (memcmp(m_, a.m_, memsize()) == 0)
	//	return true;
	//return false;
}
bool Mat::operator != (const Mat& a)const
{
	return !(*this == a);
}
mat_t& Mat::operator()(int32 i)
{
	return m_[toidx(i)];
}
mat_t& Mat::operator()(int32 i0, int32 i1)
{
	return m_[toidx(i0, i1)];
}
mat_t& Mat::operator()(int32 i0, int32 i1, int32 i2)
{
	return m_[toidx(i0, i1, i2)];
}
mat_t& Mat::operator()(Point p)
{
	return operator()(p.y, p.x);
}
mat_t& Mat::operator()(Point3i p)
{
	return operator()(p.y, p.x, p.z);
}
const mat_t& Mat::operator()(int32 i) const
{
	return m_[toidx(i)];
}
const mat_t& Mat::operator()(int32 i0, int32 i1) const
{
	return m_[toidx(i0, i1)];
}
const mat_t& Mat::operator()(int32 i0, int32 i1, int32 i2) const
{
	return m_[toidx(i0, i1, i2)];
}
const mat_t& Mat::operator()(Point p) const
{
	return operator()(p.y, p.x);
}
const mat_t& Mat::operator()(Point3i p) const
{
	return operator()(p.y, p.x, p.z);
}
Mat Mat::operator()(Rect roi) const
{
	return range(roi);
}
Mat Mat::operator()(Range row, Range col) const
{
	return range(row.v1, row.v2, col.v1, col.v2);
}
Mat Mat::cut(int32 i, RANK rc) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (i < 0) THROW_INFO(ERR_INFO_VALUE);
#endif // LZH_DEBUG
	switch (rc) {
	case ROW:
		if (i > h_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
		return row(i);
	case COL:
		if (i > w_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
		return col(i);
	case CHANNEL:
		if (i > c_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
		return channel(i);
	default:return Mat();
	}
}
Mat Mat::cut(int32 v1, int32 v2, RANK rc) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(m_);
	if (v1 < 0 || v2 < 0) THROW_INFO(ERR_INFO_VALUE);
#endif // LZH_DEBUG
	switch (rc) {
	case ROW:
#ifdef LZH_DEBUG
		if (v1 > w_ - 1 || v2 > c_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
#endif // LZH_DEBUG
		return Mat(1, h_, 1, stride, m_ + v1 * stride[1] + v2);
	case COL:
#ifdef LZH_DEBUG
		if (v1 > h_ - 1 || v2 > c_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
#endif // LZH_DEBUG
		return Mat(w_, 1, 1, stride, m_ + v1 * stride[0] * stride[1] + v2);
	case CHANNEL:
#ifdef LZH_DEBUG
		if (v1 > h_ - 1 || v2 > w_ - 1) THROW_INFO(ERR_INFO_MEMOUT);
#endif // LZH_DEBUG
		return Mat(1, 1, c_, stride, m_ + toidx(v1, v2));
		break;
	default:break;
	}
	return Mat();
}
mat_t& Mat::operator[](int32 idx) const
{
	//	if (dim == 3) {
	//		return channel(idx);
	//	}
	//	else if (dim == 2) {
	//		return row(idx);
	//	}
	//	else if (dim == 1) {
	//#ifdef LZH_DEBUG
	//		if (idx < 0) THROW_INFO(ERR_INFO_VALUE);
	//		if (idx >= len()) THROW_INFO(ERR_INFO_MEMOUT);
	//#endif // LZH_DEBUG
	//		return Mat(1, 1, 1, m_.offset(idx), depth());
	//	}
	//	else {
	//		THROW_INFO(ERR_INFO_UNLESS);
	//	}
	return m_[idx];
}
bool Mat::operator>(const Mat& b) const
{
	//FOR_RANGE(i, 0, len() > b.len() ? b.len() : len()) {
	//	if (operator()(i) > b(i))
	//		return true;
	//	else if (operator()(i) != b(i))
	//		break;
	//}
	return sum() > b.sum();
}
bool Mat::operator<(const Mat& b) const
{
	//FOR_RANGE(i, 0, len() > b.len() ? b.len() : len()) {
	//	if (operator()(i) < b(i))
	//		return true;
	//	else if (operator()(i) != b(i))
	//		break;
	//}
	return sum() < b.sum();
}

//Mat::operator Val_* ()
//{
//	return (Val_*)m_;
//}
//Mat::operator const Val_* () const
//{
//	return (const Val_*)m_;
//}

//Mat::operator mat_t* ()
//{
//	return m_;
//}
//Mat::operator const mat_t* () const
//{
//	return m_;
//}

void Mat::setPrintW(lzh::uint32 w)
{
	print_width = w;
}
void Mat::setPrintSignificantDigits(lzh::uint32 n)
{
	print_precision = n;
	//print_width = 2 + n + 3;
}
void Mat::setPrintType(lzh::PrintType t)
{
	print_type = t;
}
void Mat::setPrintMinimum(bool flag, lzh::mat_t v)
{
	minimum = v;
	setZero = flag;
}


lzh::MatIter::MatIter(const Mat* p, RANK dire, int32 i)
{
	flag = dire;
	_ptr = (Mat*)p;
	idx = i;
	update();
}
const MatIter lzh::MatIter::operator=(const MatIter& iter)
{
	if (this == &iter)
		return *this;
	//if (*_ptr == *iter._ptr)
	//{
	m = iter.m;
	//}
	//else {

	//	m = _ptr->operator()(iter.idx, iter.flag);
	//}
	flag = iter.flag;
	_ptr = iter._ptr;
	return *this;
}
int32 lzh::MatIter::operator-(const MatIter& iter) const
{
	return idx - iter.idx;
}
int32 lzh::MatIter::operator+(const MatIter& iter) const
{
	return idx + iter.idx;
}
bool lzh::MatIter::operator!=(const MatIter& iter) const
{
	return m != iter.m;
}
const MatIter lzh::MatIter::operator+=(int32 i)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx + i, _ptr->rankSize(flag));
#endif
	idx += i;
	update();
	return *this;
}
const MatIter lzh::MatIter::operator--()
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx - 1, _ptr->rankSize(flag));
#endif
	idx -= 1;
	update();
	return *this;
}
const MatIter lzh::MatIter::operator--(int32)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx - 1, _ptr->rankSize(flag));
#endif
	MatIter t = *this;
	idx += 1;
	update();
	return t;
}
const MatIter lzh::MatIter::operator-(int32 i)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx - i, _ptr->rankSize(flag));
#endif
	return MatIter(_ptr, flag, idx - i);
}
const MatIter lzh::MatIter::operator-=(int32 i)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx - i, _ptr->rankSize(flag));
#endif
	idx -= i;
	update();
	return *this;
}
Mat& lzh::MatIter::operator*() const
{
	return *p;
}
Mat* lzh::MatIter::operator()() const
{
	return p;
}
void lzh::MatIter::update()
{
	if (_ptr && idx >= 0 && idx < _ptr->rankSize(flag)) {
		m = _ptr->cut(idx, flag);
	}
	else {
		m = Mat();
	}
	p = &m;
}
bool lzh::MatIter::operator==(const MatIter& iter) const
{
	return m == iter.m;
}
bool lzh::MatIter::operator>(const MatIter& iter)
{
	switch (flag)
	{
	case lzh::ROW:
		FOR_RANGE(d, 0, m.channels()) {
			FOR_RANGE(i, 0, m.cols()) {
				if (m(0, i, d) > iter.m(0, i, d))
					return true;
			}
		}
		break;
	case lzh::COL:
		FOR_RANGE(d, 0, m.channels()) {
			FOR_RANGE(i, 0, m.rows()) {
				if (m(i, 0, d) > iter.m(i, 0, d))
					return true;
			}
		}
		break;
	case lzh::CHANNEL:
		FOR_RANGE(i, 0, m.rows()) {
			FOR_RANGE(j, 0, m.cols()) {
				if (m(i, j, 0) > iter.m(i, j, 0))
					return true;
			}
		}
		break;
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return false;
}
bool lzh::MatIter::operator<(const MatIter& iter)
{
	switch (flag)
	{
	case lzh::ROW:
		FOR_RANGE(d, 0, m.channels()) {
			FOR_RANGE(i, 0, m.cols()) {
				if (m(0, i, d) < iter.m(0, i, d))
					return true;
			}
		}
		break;
	case lzh::COL:
		FOR_RANGE(d, 0, m.channels()) {
			FOR_RANGE(i, 0, m.rows()) {
				if (m(i, 0, d) < iter.m(i, 0, d))
					return true;
			}
		}
		break;
	case lzh::CHANNEL:
		FOR_RANGE(i, 0, m.rows()) {
			FOR_RANGE(j, 0, m.cols()) {
				if (m(i, j, 0) < iter.m(i, j, 0))
					return true;
			}
		}
		break;
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return false;
}
const MatIter lzh::MatIter::operator++()
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx + 1, _ptr->rankSize(flag));
#endif
	idx += 1;
	return *this;
}
const MatIter lzh::MatIter::operator++(int32)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx + 1, _ptr->rankSize(flag));
#endif
	MatIter t = *this;
	idx += 1;
	return *this;
}
const MatIter lzh::MatIter::operator+(int32 i)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(idx + i, _ptr->rankSize(flag));
#endif
	return MatIter(_ptr, flag, idx + i);
}