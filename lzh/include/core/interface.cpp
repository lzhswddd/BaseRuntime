#define LZH_EXPORTS
#include "interface.h"

using namespace lzh;

bool lzh::_InputArray::operator>(const _InputArray & a)const
{
	return getArray() > a.getArray();
}

bool lzh::_InputArray::operator<(const _InputArray & a)const
{
	return getArray() < a.getArray();
}

bool lzh::_InputArray::operator==(const _InputArray & a) const
{
	return obj == a.obj;
}

bool lzh::_InputArray::operator!=(const _InputArray & a) const
{
	return obj != a.obj;
}

/****************************************************************************
接口类
*****************************************************************************/
void _InputArray::init(const void* obj, int32 flags, int32 type)
{
	this->flags = flags; this->obj = (void*)obj; this->type = type;
}
void _InputArray::init(const void* obj, int32 flags, int32 type, Size sz)
{
	this->flags = flags; this->obj = (void*)obj; this->type = type; this->sz = sz;
}
_InputArray::_InputArray(IMat m) {
	init(&m, MAT, m.type());
}
_InputArray::_InputArray(const Array& v) {
	init(&v, CVMAT, v.depth());
}
lzh::_InputArray::_InputArray(const Vector & v){
	init(&v, VECTOR, v.depth(), Size((int32)v.size()));
}
_InputArray::_InputArray(const vec_mat& v) {
	init(&v, STD_VECTOR_MAT, NONE, Size(1, (int32)v.size()));
}
_InputArray::_InputArray(const vec_array& v) {
	init(&v, STD_VECTOR_CVMAT, NONE, Size(1, (int32)v.size()));
}
Array lzh::_InputArray::getArray(int32 i) const
{
	if (obj == nullptr)return Array();
	if (flags == MAT && i < 0)
		return (*(Mat*)obj).toArray(MAT_T);
	if (flags == MAT_ITER && i < 0) {
		MatCommaInitializer_<uint8> iter = *(MatCommaInitializer_<uint8>*)obj;
		return Array(iter.rows() * iter.cols() * iter.channels(), iter.data(), type);
	}
	if (flags == CVMAT && i < 0)
		return *(Array*)obj;
	int32 t = depth();
	int32 esz = value_type();
	switch (flags)
	{
	case lzh::_InputArray::VALUE:
		if (i >= 0)THROW_INFO(ERR_INFO_UNLESS);
		switch (t)
		{
		case lzh::TP_8S:
			return Array(esz, sz.h, (const int8*)obj, t);
		case lzh::TP_8U:
			return Array(esz, sz.h, (const uint8*)obj, t);
		case lzh::TP_16S:
			return Array(esz, sz.h, (const int16*)obj, t);
		case lzh::TP_16U:
			return Array(esz, sz.h, (const uint16*)obj, t);
		case lzh::TP_32S:
			return Array(esz, sz.h, (const int32*)obj, t);
		case lzh::TP_32U:
			return Array(esz, sz.h, (const uint32*)obj, t);
		case lzh::TP_64S:
			return Array(esz, sz.h, (const int64*)obj, t);
		case lzh::TP_64U:
			return Array(esz, sz.h, (const uint64*)obj, t);
		case lzh::TP_32F:
			return Array(esz, sz.h, (const float32*)obj, t);
		case lzh::TP_64F:
			return Array(esz, sz.h, (const float64*)obj, t);
		case lzh::TP_64D:
			return Array(esz, sz.h, (const float128*)obj, t);
		default:THROW_INFO(ERR_INFO_TYPE); break;
		}
	case lzh::_InputArray::VECTOR:
		if (i >= 0)THROW_INFO(ERR_INFO_UNLESS);
		return Array(*(const Vector*)obj);
	case lzh::_InputArray::STD_VECTOR:
		if (i >= 0)THROW_INFO(ERR_INFO_UNLESS);
		switch (t)
		{
		case lzh::TP_8S:
			if (((const vec_8s*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_8s*)obj)[0], t);
		case lzh::TP_8U:
			if (((const vec_8b*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_8b*)obj)[0], t);
		case lzh::TP_16S:
			if (((const vec_16s*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_16s*)obj)[0], t);
		case lzh::TP_16U:
			if (((const vec_16u*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_16u*)obj)[0], t);
		case lzh::TP_32S:
			if (((const vec_32s*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_32s*)obj)[0], t);
		case lzh::TP_32U:
			if (((const vec_32u*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_32u*)obj)[0], t);
		case lzh::TP_64S:
			if (((const vec_64s*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_64s*)obj)[0], t);
		case lzh::TP_64U:
			if (((const vec_64u*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_64u*)obj)[0], t);
		case lzh::TP_32F:
			if (((const vec_32f*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_32f*)obj)[0], t);
		case lzh::TP_64F:
			if (((const vec_64f*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_64f*)obj)[0], t);
		case lzh::TP_64D:
			if (((const vec_64d*)obj)->empty())return Array();
			return Array(esz, sz.h, &(*(const vec_64d*)obj)[0], t);
		default:THROW_INFO(ERR_INFO_TYPE); break;
		}
	case lzh::_InputArray::STD_VECTOR_MAT:
		if (i < 0)THROW_INFO(ERR_INFO_UNLESS);
		return ((vec_mat*)obj)->at(i).toArray();
	case lzh::_InputArray::STD_VECTOR_CVMAT:
		if (i < 0)THROW_INFO(ERR_INFO_UNLESS);
		return ((std::vector<Array>*)obj)->at(i);
	case lzh::_InputArray::STD_VECTOR_VECTOR:
		if (i < 0)THROW_INFO(ERR_INFO_UNLESS);
		switch (t)
		{
		case lzh::TP_8S:
			return Array(esz, int((*(const vecs_8s*)obj)[i].size()) / esz, &(*(const vecs_8s*)obj)[i][0], t);
		case lzh::TP_8U:
			return Array(esz, int((*(const vecs_8b*)obj)[i].size()) / esz, &(*(const vecs_8b*)obj)[i][0], t);
		case lzh::TP_16S:
			return Array(esz, int((*(const vecs_16s*)obj)[i].size()) / esz, &(*(const vecs_16s*)obj)[i][0], t);
		case lzh::TP_16U:
			return Array(esz, int((*(const vecs_16u*)obj)[i].size()) / esz, &(*(const vecs_16u*)obj)[i][0], t);
		case lzh::TP_32S:
			return Array(esz, int((*(const vecs_32s*)obj)[i].size()) / esz, &(*(const vecs_32s*)obj)[i][0], t);
		case lzh::TP_32U:
			return Array(esz, int((*(const vecs_32u*)obj)[i].size()) / esz, &(*(const vecs_32u*)obj)[i][0], t);
		case lzh::TP_64S:
			return Array(esz, int((*(const vecs_64s*)obj)[i].size()) / esz, &(*(const vecs_64s*)obj)[i][0], t);
		case lzh::TP_64U:
			return Array(esz, int((*(const vecs_64u*)obj)[i].size()) / esz, &(*(const vecs_64u*)obj)[i][0], t);
		case lzh::TP_32F:
			return Array(esz, int((*(const vecs_32f*)obj)[i].size()) / esz, &(*(const vecs_32f*)obj)[i][0], t);
		case lzh::TP_64F:
			return Array(esz, int((*(const vecs_64f*)obj)[i].size()) / esz, &(*(const vecs_64f*)obj)[i][0], t);
		case lzh::TP_64D:
			return Array(esz, int((*(const vecs_64d*)obj)[i].size()) / esz, &(*(const vecs_64d*)obj)[i][0], t);
		default:THROW_INFO(ERR_INFO_TYPE); break;
		}
	case lzh::_InputArray::VEC:
		if (i >= 0)THROW_INFO(ERR_INFO_UNLESS);
		switch (t)
		{
		case lzh::TP_8S:
			return Array(esz, sz.h, (const int8*)obj, t);
		case lzh::TP_8U:
			return Array(esz, sz.h, (const uint8*)obj, t);
		case lzh::TP_16S:
			return Array(esz, sz.h, (const int16*)obj, t);
		case lzh::TP_16U:
			return Array(esz, sz.h, (const uint16*)obj, t);
		case lzh::TP_32S:
			return Array(esz, sz.h, (const int32*)obj, t);
		case lzh::TP_32U:
			return Array(esz, sz.h, (const uint32*)obj, t);
		case lzh::TP_64S:
			return Array(esz, sz.h, (const int64*)obj, t);
		case lzh::TP_64U:
			return Array(esz, sz.h, (const uint64*)obj, t);
		case lzh::TP_32F:
			return Array(esz, sz.h, (const float32*)obj, t);
		case lzh::TP_64F:
			return Array(esz, sz.h, (const float64*)obj, t);
		case lzh::TP_64D:
			return Array(esz, sz.h, (const float128*)obj, t);
		default:THROW_INFO(ERR_INFO_TYPE); break;
		}
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
	return Array();
}
Mat lzh::_InputArray::getMat(int32 i) const
{
	if (obj == nullptr)return Mat();
	if (flags == MAT && i < 0)
		return *(Mat*)obj;
	if (flags == CVMAT && i < 0)
		return ((Array*)obj)->toMat();
	return getMat_(i);
}
Mat lzh::_InputArray::getMat_(int32 i) const
{
	if (obj == nullptr)return Mat();
	int32 t = depth();
	int32 esz = value_type();
	switch (flags)
	{
	case lzh::_InputArray::VALUE:
	case lzh::_InputArray::VECTOR:
	case lzh::_InputArray::VEC:
	case lzh::_InputArray::MAT_ITER:
	case lzh::_InputArray::STD_VECTOR:
	case lzh::_InputArray::STD_VECTOR_VECTOR:
		return getArray(i).toMat();
	case lzh::_InputArray::STD_VECTOR_MAT:
		if (i < 0)THROW_INFO(ERR_INFO_UNLESS);
		return (*(const std::vector<Mat>*)obj)[i];
	case lzh::_InputArray::STD_VECTOR_CVMAT:
		if (i < 0)THROW_INFO(ERR_INFO_UNLESS);
		return ((std::vector<Array>*)obj)->at(i).toMat();
	default:THROW_INFO(ERR_INFO_TYPE); break;
	}
	return Mat();
}
void lzh::_InputArray::release()
{
}
void lzh::_InputArray::swap(Mat& dst)
{
	Mat t = dst;
	dst = getMat();
	*this = _InputArray(t);
}
bool lzh::_InputArray::empty()const {
	return obj == nullptr;
}
int32 lzh::_InputArray::kind()const
{
	return flags;
}
int32 lzh::_InputArray::depth()const
{
	return MAT_GETVALUE(type);
}
int32 lzh::_InputArray::getTp() const
{
	return type;
}
int32 lzh::_InputArray::size()const
{
	return sz.h;
}
int32 lzh::_InputArray::rows() const
{
	return getArray().rows;
}
int32 lzh::_InputArray::cols() const
{
	return getArray().cols;
}
int32 lzh::_InputArray::channels() const
{
	return getArray().channels();
}
int32 lzh::_InputArray::len() const
{
	return getArray().len();
}
Size lzh::_InputArray::getSz() const
{
	return sz;
}
Size lzh::_InputArray::getSz_() const
{
	if (flags == MAT)
		return ((Mat*)obj)->size();
	if (flags == MAT_ITER)
		return ((MatCommaInitializer_<uint8>*)obj)->size();
	int32 esz = value_type();
	return Size(esz, sz.h);
}
bool lzh::_InputArray::isStdVector()const {
	return flags == STD_VECTOR;
}
bool lzh::_InputArray::isImage() const
{
	return flags == MAT || flags == CVMAT;
}
bool lzh::_InputArray::isMat() const
{
	return flags == MAT;
}
int32 lzh::_InputArray::value_type() const
{
	return MAT_GETDEPTH(type);
}

void lzh::_OutputArray::create(Size sz, int32 mtype) const
{
	create(sz.h, sz.w, 1, mtype);
}
void lzh::_OutputArray::create(Size3 sz, int32 mtype) const
{
	create(sz.h, sz.w, sz.c, mtype);
}
void lzh::_OutputArray::create(int32 rows, int32 mtype) const
{
	create(rows, 1, 1, mtype);
}
void lzh::_OutputArray::create(int32 rows, int32 cols, int32 mtype) const
{
	create(rows, cols, 1, mtype);
}
void lzh::_OutputArray::create(int32 rows, int32 cols, int32 channels, int32 mtype) const
{
	switch (flags)
	{
	case lzh::_InputArray::MAT:(*(Mat*)obj).create(cols, rows, channels); break;
	case lzh::_InputArray::CVMAT:(*(Array*)obj).create(cols, rows, channels, mtype); break;
	case lzh::_InputArray::VECTOR:(*(Vector*)obj).create(cols, mtype); break;
	case lzh::_InputArray::STD_VECTOR:
	{
		int32 len = cols * rows * channels * MAT_GETDEPTH(type);
		*(Size*)& sz = Size(cols, rows);
		switch (MAT_GETVALUE(type))
		{
		case lzh::TP_8S:	((vec_8s*)obj)->resize(len);	break;
		case lzh::TP_8U:	((vec_8b*)obj)->resize(len);	break;
		case lzh::TP_32S:	((vec_32s*)obj)->resize(len);	break;
		case lzh::TP_32F:	((vec_32f*)obj)->resize(len);	break;
		case lzh::TP_64F:	((vec_64f*)obj)->resize(len);	break;
		case lzh::TP_64D:	((vec_64d*)obj)->resize(len);	break;
		default:THROW_INFO(ERR_INFO_TYPE);
		}
		break;
	}
	case lzh::_InputArray::STD_VECTOR_MAT: {
		int32 len = cols * rows * channels;
		*(Size*)& sz = Size(cols, rows);
		((vec_mat*)obj)->resize(len);
		break;
	}
	case lzh::_InputArray::STD_VECTOR_CVMAT: {
		int32 len = cols * rows * channels;
		*(Size*)& sz = Size(cols, rows);
		((vec_array*)obj)->resize(len);
		break;
	}
	default:
		THROW_INFO(ERR_INFO_TYPE);
	}
}
void lzh::_OutputArray::createSameSize(InputArray arr, int32 mtype) const
{
	if (arr.object() == object())return;
	if (arr.kind() == MAT)
		create(arr.getMat().size3(), MAT_T);
	else if (arr.kind() == CVMAT)
		create(arr.getArray().size3(), mtype);
	else if (arr.kind() == STD_VECTOR_VECTOR)
	{
		int32 d = MAT_GETDEPTH(type);
		*(Size*)& sz = arr.getSz();
		int32 len = sz.h;
		switch (MAT_GETVALUE(type))
		{
		case lzh::TP_8S:	((vecs_8s*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_8s*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_8U:	((vecs_8b*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_8b*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_16S:	((vecs_16s*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_16s*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_16U:	((vecs_16u*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_16u*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_32S:	((vecs_32s*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_32s*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_32U:	((vecs_32u*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_32u*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_64S:	((vecs_64s*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_64s*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_64U:	((vecs_64u*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_64u*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_32F:	((vecs_32f*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_32f*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_64F:	((vecs_64f*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_64f*)obj)->at(i).resize(arr.getArray(i).len()); break;
		case lzh::TP_64D:	((vecs_64d*)obj)->resize(len);	FOR_RANGE(i, 0, len)((vecs_64d*)obj)->at(i).resize(arr.getArray(i).len()); break;
		default:THROW_INFO(ERR_INFO_TYPE);
		}
	}
	else
		create(arr.getSz(), mtype);
}
void lzh::_OutputArray::release() const
{
}
void lzh::_OutputArray::clear() const
{
}
Mat& lzh::_OutputArray::getMatRef(int32 i) const
{
	if (flags == MAT && i < 0)
		return *(Mat*)obj;
	else if (flags == STD_VECTOR_MAT && i >= 0) {
#ifdef LZH_DEBUG
		CHECK_INDEX(i, sz.h);
#endif //LZH_DEBUG
		return (*(vec_mat*)obj)[i];
	}
	else {
		THROW_INFO(ERR_INFO_TYPE);
	}
	return *(Mat*)(0);//为了不警告
}
Array& lzh::_OutputArray::getArrayRef(int32 i) const
{
	if (flags == CVMAT && i < 0)
		return *(Array*)obj;
	else if (flags == STD_VECTOR_CVMAT && i >= 0) {
#ifdef LZH_DEBUG
		CHECK_INDEX(i, sz.h);
#endif //LZH_DEBUG
		return (*(std::vector<Array>*)obj)[i];
	}
	else {
		THROW_INFO(ERR_INFO_TYPE);
	}
	return *(Array*)(0);//为了不警告
}
void lzh::_OutputArray::setMatRef(InputArray m, int32 i) const
{
	if (flags == MAT && i < 0)
		* (Mat*)obj = m.getMat().clone();
	else if (flags == STD_VECTOR_MAT && i >= 0) {
#ifdef LZH_DEBUG
		CHECK_INDEX(i, sz.h);
#endif //LZH_DEBUG
		(*(vec_mat*)obj)[i] = m.getMat().clone();
	}
	else if (flags == CVMAT && i < 0)
		* (Array*)obj = m.getArray().clone();
	else if (flags == STD_VECTOR_CVMAT && i >= 0) {
#ifdef LZH_DEBUG
		CHECK_INDEX(i, sz.h);
#endif //LZH_DEBUG
		(*(vec_array*)obj)[i] = m.getArray().clone();
	}
	else {
		THROW_INFO(ERR_INFO_TYPE);
	}
}
void lzh::_OutputArray::set(InputArray input) const
{
	if (input.object() == object())return;
	if (len() != input.len() && flags != CVMAT && flags != MAT)
		createSameSize(input, input.depth());
	else if (getArray().empty())
		createSameSize(input, input.depth());
	setTo(input);
}
void lzh::_OutputArray::setTo(InputArray input) const
{
	if (input.empty())return;
	if (input.object() == object())return;
	if (flags == MAT) {
		if (!getMat().empty() && !getMat().isDeepCopy()) {
			input.getArray().setTo(getArray());
		}
		else
			getMatRef() = input.getArray();
	}
	else if (flags == CVMAT) {
		if (!getArray().empty() && !getArray().isDeepCopy())
			input.getArray().setTo(getArray());
		else
			getArrayRef() = input.getArray();
	}
	else if (flags == VEC) {
		input.getArray().setTo(getArray());
	}
	else if (flags == VECTOR) {
		input.getArray().setTo(getArray());
	}
	else if (flags == STD_VECTOR || flags == VALUE)
	{
		if (input.getArray().empty())return;
		input.getArray().setTo(getArray());
	}
	else if (flags == STD_VECTOR_VECTOR)
	{
		FOR_RANGE(i, 0, input.getSz().h) {
			if (input.getArray(i).empty())return;
			input.getArray(i).setTo(getArray(i));
		}
	}
	else if (flags == STD_VECTOR_CVMAT)
	{
		FOR_RANGE(i, 0, input.getSz().h) {
			if (!getArray(i).empty() && !getArray(i).isDeepCopy())
				input.getArray(i).setTo(getArray(i));
			else
				getArrayRef(i) = input.getArray(i);
		}
	}
	else if (flags == STD_VECTOR_MAT)
	{
		FOR_RANGE(i, 0, input.getSz().h) {
			if (!getMat(i).empty() && !getMat(i).isDeepCopy())
				input.getMat(i).setTo(getMat(i));
			else
				getMatRef(i) = input.getArray(i);
		}
	}
	else {
		THROW_INFO(ERR_INFO_TYPE);
	}
}
void lzh::_OutputArray::copyTo(InputArray input) const
{
	if (input.empty())return;
	if (flags == MAT || flags == STD_VECTOR) {
		setMatRef(input);
	}
	else {
		THROW_INFO(ERR_INFO_TYPE);
	}
}
void lzh::_OutputArray::operator=(InputArray input)const
{
	set(input);
}

static _InputOutputArray _none;
InputOutputArray lzh::noArray() { return _none; }
