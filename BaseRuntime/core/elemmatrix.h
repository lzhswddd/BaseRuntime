#pragma once
#ifndef __ELEMMATRIX_H__
#define __ELEMMATRIX_H__

#include "element.h"

namespace lzh
{
	class LZHAPI ElemMat : public PtrArray<Element>
	{
	public:
		using base = typename PtrArray<Element>;
		using iterator = typename PtrArray<Element>::iterator;
		using const_iterator = typename PtrArray<Element>::const_iterator;

		ElemMat();
		ElemMat(size_t col);
		ElemMat(size_t row, size_t col);
		ElemMat(size_t row, size_t col, Element* ptr, size_t stride);
		ElemMat(const std::initializer_list<Element> & list);
		ElemMat(const std::initializer_list<std::initializer_list<Element>> & list);

		size_t rows()const noexcept;
		size_t cols()const noexcept;
		size_t step()const noexcept;

		void create(size_t r, size_t c);

		Element& at(size_t i0);
		Element& at(size_t i0, size_t i1);
		const Element& at(size_t i0)const;
		const Element& at(size_t i0, size_t i1)const;

		Element& operator ()(size_t i0);
		Element& operator ()(size_t i0, size_t i1);
	protected:
		size_t row_;
		size_t col_;
		size_t stride;
	};
}

#endif
