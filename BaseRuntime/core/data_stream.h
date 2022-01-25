#pragma once
#ifndef _DATA_STREAM_H__
#define _DATA_STREAM_H__
#include <iostream>
#include "interface.h"
#include "structure.h"
#include "convert.h"
#include "element.h"
#include "tools.h"

namespace lzh
{
	extern LZHAPI std::ostream & operator << (std::ostream &out, const Mat &ma);
	extern LZHAPI std::ostream & operator << (std::ostream &out, const Array &ma);
	extern LZHAPI std::ostream & operator << (std::ostream & out, const Val_ & t);
	extern LZHAPI std::ostream & operator << (std::ostream & out, const Val & t);
	extern LZHAPI std::ostream & operator << (std::ostream & out, const Complex & o); 
	extern LZHAPI std::ostream & operator << (std::ostream &out, const LargeNumber &LN);
	extern LZHAPI std::ostream & operator << (std::ostream &out, const Moments &mu);
	extern LZHAPI std::ostream & operator << (std::ostream &out, const Vector &vec);
	extern LZHAPI std::ostream & operator << (std::ostream &out, const Element &element);
	extern LZHAPI std::ostream & operator << (std::ostream &out, const TimeData & timeData);
	extern LZHAPI std::vector<std::string> & operator << (std::vector<std::string> &vecs, const char* v);

	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream & out, const Range_<_Tp> & t);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream & out, const Rect_<_Tp> & t);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream & out, const Size_<_Tp> & t);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream & out, const Size3_<_Tp> & t);
	template<typename _Tp> TEMPLATE_API std::ostream & operator <<(std::ostream & out, const Point2_<_Tp>& t);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream & out, const Point3_<_Tp>& t);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream &out, const MatPtr<_Tp> &t);
	template<typename _Tp, int32 N> TEMPLATE_API std::ostream & operator << (std::ostream &out, const Vec<_Tp, N> &t);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream &out, const STLApi<_Tp> &api);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream &out, const SeqList<_Tp> &sl);
	template<typename _Tp> TEMPLATE_API std::vector<_Tp> & operator << (std::vector<_Tp> &vecs, const _Tp & v);
	template<typename _Tp> TEMPLATE_API std::ostream & operator << (std::ostream &out, const std::vector<_Tp> &vecs);
}
#include "data_stream_inl.hpp"
#endif // !_DATA_STREAM_H__

