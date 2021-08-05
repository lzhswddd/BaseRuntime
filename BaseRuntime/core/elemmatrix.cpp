#define LZH_EXPORTS
#include "elemmatrix.h"
#include "tools.h"
using namespace lzh;

lzh::ElemMat::ElemMat()
	: base()
{
	create(0, 0);
}

lzh::ElemMat::ElemMat(size_t col)
	: base()
{
	create(1, col);
}

lzh::ElemMat::ElemMat(size_t row, size_t col)
	: base()
{
	create(row, col);
}

lzh::ElemMat::ElemMat(size_t row, size_t col, Element * ptr, size_t stride)
	: base(), row_(row), col_(col), stride(stride)
{
	base::assign(row*stride, ptr);
}

lzh::ElemMat::ElemMat(const std::initializer_list<Element> & list)
	: base()
{
	create(1, list.size());
	const Element* ptr = list.begin();
	for (size_t i : range(list.size())) {
		at(i) = ptr++;
	}
}

lzh::ElemMat::ElemMat(const std::initializer_list<std::initializer_list<Element>> & list)
	: base()
{
	size_t w = 0, h = list.size();
	for (const std::initializer_list<Element> & item : list)
		if (w < item.size())w = item.size();
	create(h, w);
	const std::initializer_list<Element> *iter = list.begin();
	for (size_t i : range(list.size())) {
		const Element *item = iter->begin();
		for (size_t j : range(iter->size())) {
			at(i, j) = *item; item++;
		}
		iter++;
	}
}

size_t lzh::ElemMat::rows() const noexcept
{
	return row_;
}

size_t lzh::ElemMat::cols() const noexcept
{
	return col_;
}

size_t lzh::ElemMat::step() const noexcept
{
	return stride;
}

void lzh::ElemMat::create(size_t r, size_t c)
{
	row_ = r;
	col_ = c;
	stride = col_;
	base::create(row_*col_);
}

Element & lzh::ElemMat::at(size_t i0)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(i0, size());
#endif
	size_t i1 = i0 / col_;
	size_t i2 = i0 % col_;
	return at(i1, i2);
}

Element & lzh::ElemMat::at(size_t i0, size_t i1)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(i0, row_);
	CHECK_INDEX(i1, col_);
#endif
	return base::at(i0 * stride + i1);
}

const Element & lzh::ElemMat::at(size_t i0) const
{
#ifdef LZH_DEBUG
	CHECK_INDEX(i0, size());
#endif
	size_t i1 = i0 / col_;
	size_t i2 = i0 % col_;
	return at(i1, i2);
}

const Element & lzh::ElemMat::at(size_t i0, size_t i1) const
{
#ifdef LZH_DEBUG
	CHECK_INDEX(i0, row_);
	CHECK_INDEX(i1, col_);
#endif
	return base::at(i0 * stride + i1);
}

Element & lzh::ElemMat::operator()(size_t i0)
{
	return at(i0);
}

Element & lzh::ElemMat::operator()(size_t i0, size_t i1)
{
	return at(i0, i1);
}