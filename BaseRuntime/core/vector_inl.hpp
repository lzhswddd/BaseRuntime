template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N>::Vec() {
	memset(data, 0, sizeof(_Tp) * N);
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N>::Vec(const _Tp * v)
{
	memcpy(data, v, sizeof(_Tp)*N);
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N>::Vec(const MatPtr<_Tp> & m)
{
	if (m.size() >= N) {
		memcpy(data, m, N * sizeof(_Tp));
	}
}
template<class _Tp, lzh::int32 N> template<typename ...Types> inline lzh::Vec<_Tp, N>::Vec(const Types & ...args) {
	int32 i = 0;
	addList(&data[0], i, args...);
	if (i != N) {
		FOR_RANGE(idx, i, N)
			data[idx] = 0;
	}
}
template<class _Tp, lzh::int32 N> template<typename T2_, lzh::int32 N1> inline lzh::Vec<_Tp, N>::Vec(const Vec<T2_, N1>& m)
{
	for (int32 i = 0; i < MIN(N, N1); i++)
		data[i] = _Tp(m[i]);
}
template<class _Tp, lzh::int32 N> template<typename _Ty> inline lzh::Vec<_Tp, N>::Vec(std::initializer_list<_Ty> & _List)
{
	int i = 0;
	for (_Ty v : _List)
	{
		if (i == N)break;
		data[i] = (_Tp)v;
		i += 1;
	}
	if (i != N) {
		FOR_RANGE(idx, i, N)
			data[idx] = 0;
	}
}
template<class _Tp, lzh::int32 N> template<typename T> inline void lzh::Vec<_Tp, N>::addvalue(int32 i, const T & arg)
{
	data[i] = (_Tp)arg;
}
template<class _Tp, lzh::int32 N> template<typename T, typename ...Types> inline void lzh::Vec<_Tp, N>::addvalue(int32 i, const T & arg1, const Types & ...args)
{
	if (i >= N)return;
	data[i] = (_Tp)arg1;
	addvalue(i + 1, args...);
}
template<class _Tp, lzh::int32 N> inline _Tp & lzh::Vec<_Tp, N>::at(int32 i) {
#ifdef LZH_DEBUG
	CHECK_INDEX(i, N);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp, lzh::int32 N> inline const _Tp & lzh::Vec<_Tp, N>::at(int32 i) const {
#ifdef LZH_DEBUG
	CHECK_INDEX(i, N);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp, lzh::int32 N> inline _Tp & lzh::Vec<_Tp, N>::operator[](int32 i) {
#ifdef LZH_DEBUG
	CHECK_INDEX(i, N);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp, lzh::int32 N> inline const _Tp & lzh::Vec<_Tp, N>::operator[](int32 i) const {
#ifdef LZH_DEBUG
	CHECK_INDEX(i, N);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp, lzh::int32 N> inline _Tp & lzh::Vec<_Tp, N>::operator()(int32 i)
{
#ifdef LZH_DEBUG
	CHECK_INDEX(i, N);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp, lzh::int32 N> inline const _Tp & lzh::Vec<_Tp, N>::operator()(int32 i) const
{
#ifdef LZH_DEBUG
	CHECK_INDEX(i, N);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp, lzh::int32 N> inline lzh::int32 lzh::Vec<_Tp, N>::size() const { return N; }
template<class _Tp, lzh::int32 N> inline _Tp * lzh::Vec<_Tp, N>::begin() {
	return data;
}
template<class _Tp, lzh::int32 N> inline const _Tp * lzh::Vec<_Tp, N>::begin() const {
	return data;
}
template<class _Tp, lzh::int32 N> inline _Tp * lzh::Vec<_Tp, N>::end() {
	return data + N;
}
template<class _Tp, lzh::int32 N> inline const _Tp * lzh::Vec<_Tp, N>::end() const {
	return data + N;
}
template<class _Tp, lzh::int32 N>
inline lzh::Vec<_Tp, N> lzh::Vec<_Tp, N>::t() const
{
	Vec<_Tp, N> vec = *this;
	if (h == 1) {
		vec.h = N;
		vec.w = 1;
	}
	else {
		vec.w = N;
		vec.h = 1;
	}
	return vec;
}
//template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N>& lzh::Vec<_Tp, N>::operator=(int32 tab)
//{
//	Color color = Color(tab);
//	if (DataType<_Tp>::type == DataType<mat_t>::type && N >= 4)
//		memcpy(data, color.data, sizeof(_Tp) * 4);
//	else if (DataType<_Tp>::type != DataType<mat_t>::type && N >= 4)
//		FOR_RANGE(i, 0, N)data[i] = (_Tp)color[i];
//	else THROW_INFO_STR("向量尺寸不符!");
//	return *this;
//}
template<class _Tp, lzh::int32 N> template<typename _T2, lzh::int32 N1> inline lzh::Vec<_Tp, N>& lzh::Vec<_Tp, N>::operator=(const Vec<_T2, N1>& vec)
{
	if (data == (_Tp*)vec.data)return *this;
	if (DataType<_Tp>::type == DataType<_T2>::type && N <= N1)
		memcpy(data, vec.data, sizeof(_Tp)*N);
	else if (DataType<_Tp>::type != DataType<_T2>::type && N <= N1)
		FOR_RANGE(i, 0, N)data[i] = (_Tp)vec[i];
	else THROW_INFO_STR("向量尺寸不符!");
	return *this;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator+(Val_ v, const Vec<_Tp, N>& vector)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] + v;
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator-(Val_ v, const Vec<_Tp, N>& vector)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = v - vector[i];
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator*(Val_ v, const Vec<_Tp, N>& vector)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] * v;
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator/(Val_ v, const Vec<_Tp, N>& vector)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = v / vector[i];
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator+(const Vec<_Tp, N> &vector, Val_ v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] + v;
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator-(const Vec<_Tp, N> &vector, Val_ v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] - v;
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator*(const Vec<_Tp, N> &vector, Val_ v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] * v;
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator/(const Vec<_Tp, N> &vector, Val_ v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] / v;
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator+(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] + v[i];
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator-(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] - v[i];
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator*(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] * v[i];
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator/(const Vec<_Tp, N> &vector, const Vec<_Tp, N> &v)
{
	Vec<_Tp, N> vec;
	FOR_RANGE(i, 0, N)
		vec[i] = vector[i] / v[i];
	return vec;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator+=(Vec<_Tp, N> &vector, Val_ v)
{
	FOR_RANGE(i, 0, N)
		vector[i] += v;
	return vector;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator-=(Vec<_Tp, N> &vector, Val_ v)
{
	FOR_RANGE(i, 0, N)
		vector[i] -= v;
	return vector;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator*=(Vec<_Tp, N> &vector, Val_ v)
{
	FOR_RANGE(i, 0, N)
		vector[i] *= v;
	return vector;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator/=(Vec<_Tp, N> &vector, Val_ v)
{
	FOR_RANGE(i, 0, N)
		vector[i] /= v;
	return vector;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator+=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v)
{
	FOR_RANGE(i, 0, N)
		vector[i] += v[i];
	return vector;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator-=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v)
{
	FOR_RANGE(i, 0, N)
		vector[i] -= v[i];
	return vector;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator*=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v)
{
	FOR_RANGE(i, 0, N)
		vector[i] *= v[i];
	return vector;
}
template<class _Tp, lzh::int32 N> inline lzh::Vec<_Tp, N> lzh::operator/=(Vec<_Tp, N> &vector, const Vec<_Tp, N> &v) {
	FOR_RANGE(i, 0, N)
		vector[i] /= v[i];
	return vector;
}


template<typename _Ty> inline lzh::Vector::Vector(std::initializer_list<_Ty> _Ilist)
{
	init(0, 0, 0, 0);
	create(_Ilist.size(), DataType<_Ty>::type);
	memcpy(data, _Ilist.begin(), sizeof(_Ty)*_Ilist.size());
}

template<typename _Tp> inline _Tp * lzh::Vector::begin()
{
#ifdef LZH_DEBUG
	CHECK_TYPE(_Tp, type);
#endif // LZH_DEBUG
	return (_Tp*)data;
}
template<typename _Tp> inline const _Tp * lzh::Vector::begin() const
{
#ifdef LZH_DEBUG
	CHECK_TYPE(_Tp, depth());
#endif // LZH_DEBUG
	return (const _Tp*)data;
}
template<typename _Tp> inline _Tp * lzh::Vector::end()
{
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_TYPE(_Tp, type);
#endif // LZH_DEBUG
	if (refcount == nullptr)
		return ((_Tp*)(data)) + len;
	return (_Tp*)refcount;
}
template<typename _Tp> inline const _Tp * lzh::Vector::end() const
{
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_TYPE(_Tp, depth());
#endif // LZH_DEBUG
	if (refcount == nullptr)return (const _Tp*)(data)+len;
	return (const _Tp*)refcount;;
}
template<typename _Tp> inline _Tp & lzh::Vector::at(msize i)
{
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_TYPE(_Tp, depth());
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	return ((_Tp*)data)[i];
}
template<typename _Tp> inline const _Tp & lzh::Vector::at(msize i) const
{
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_TYPE(_Tp, depth());
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	return ((const _Tp*)data)[i];
}
template<typename _Tp> inline _Tp & lzh::Vector::operator[](msize i) const {
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_TYPE(_Tp, depth());
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	return ((_Tp*)data)[i];
}
template<typename _Tp> inline lzh::Vector::operator _Tp*()
{
	return (_Tp*)data;
}
template<typename _Tp> inline lzh::Vector::operator const _Tp*() const
{
	return (const _Tp*)data;
}
template<typename _Tp> inline _Tp lzh::Vector::get(msize i) const {
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	_Tp v;
	MAT_VALUE(_Tp, depth(), data, i, v);
	return v;
}
template<typename _Tp> inline _Tp lzh::Vector::ptr()
{
	return (_Tp)(data);
}
template<typename _Tp> inline const _Tp lzh::Vector::ptr() const
{
	return (const _Tp)(data);
}
template<typename _Tp> inline void lzh::Vector::setv(msize i, _Tp v) {
	CHECK_MATRIX(data);
	CHECK_INDEX(i, len);
	if (depth() == MAT_TYPE(_Tp)) {
		switch (depth()) {
		case lzh::TP_8S:	MAT_GETARRAY(int8, data, i) = (int8)v; break;
		case lzh::TP_8U:	MAT_GETARRAY(uint8, data, i) = (uint8)v; break;
		case lzh::TP_16S:	MAT_GETARRAY(int16, data, i) = (int16)v; break;
		case lzh::TP_16U:	MAT_GETARRAY(uint16, data, i) = (uint16)v; break;
		case lzh::TP_32S:	MAT_GETARRAY(int32, data, i) = (int32)v; break;
		case lzh::TP_32U:	MAT_GETARRAY(uint32, data, i) = (uint32)v; break;
		case lzh::TP_64S:	MAT_GETARRAY(int64, data, i) = (int64)v; break;
		case lzh::TP_64U:	MAT_GETARRAY(uint64, data, i) = (uint64)v; break;
		case lzh::TP_32F:	MAT_GETARRAY(float32, data, i) = (float32)v; break;
		case lzh::TP_64F:	MAT_GETARRAY(float64, data, i) = (float64)v; break;
		case lzh::TP_64D:	MAT_GETARRAY(float128, data, i) = (float128)v; break;
		default:return;
		}
	}
	else {
		switch (depth()) {
		case lzh::TP_8S:	MAT_GETARRAY(int8, data, i) = saturate_cast<int8>(v); break;
		case lzh::TP_8U:	MAT_GETARRAY(uint8, data, i) = saturate_cast<uint8>(v); break;
		case lzh::TP_16S:	MAT_GETARRAY(int16, data, i) = saturate_cast<int16>(v); break;
		case lzh::TP_16U:	MAT_GETARRAY(uint16, data, i) = saturate_cast<uint16>(v); break;
		case lzh::TP_32S:	MAT_GETARRAY(int32, data, i) = saturate_cast<int32>(v); break;
		case lzh::TP_32U:	MAT_GETARRAY(uint32, data, i) = saturate_cast<uint32>(v); break;
		case lzh::TP_64S:	MAT_GETARRAY(int64, data, i) = saturate_cast<int64>(v); break;
		case lzh::TP_64U:	MAT_GETARRAY(uint64, data, i) = saturate_cast<uint64>(v); break;
		case lzh::TP_32F:	MAT_GETARRAY(float32, data, i) = saturate_cast<float32>(v);	break;
		case lzh::TP_64F:	MAT_GETARRAY(float64, data, i) = saturate_cast<float64>(v);	break;
		case lzh::TP_64D:	MAT_GETARRAY(float128, data, i) = saturate_cast<float128>(v);	break;
		default:return;
		}
	}
}

template<typename _Tp> inline void lzh::Vector::add(msize i, _Tp v)
{
	CHECK_MATRIX(data);
	CHECK_INDEX(i, len);
	switch (depth()) {
	case lzh::TP_8S:	MAT_GETARRAY(int8, data, i) += saturate_cast<int8>(v);		break;
	case lzh::TP_8U:	MAT_GETARRAY(uint8, data, i) += saturate_cast<uint8>(v);		break;
	case lzh::TP_16S:	MAT_GETARRAY(int16, data, i) += saturate_cast<int16>(v);		break;
	case lzh::TP_16U:	MAT_GETARRAY(uint16, data, i) += saturate_cast<uint16>(v);	break;
	case lzh::TP_32S:	MAT_GETARRAY(int32, data, i) += saturate_cast<int32>(v);		break;
	case lzh::TP_32U:	MAT_GETARRAY(uint32, data, i) += saturate_cast<uint32>(v);	break;
	case lzh::TP_64S:	MAT_GETARRAY(int64, data, i) += saturate_cast<int64>(v);		break;
	case lzh::TP_64U:	MAT_GETARRAY(uint64, data, i) += saturate_cast<uint64>(v);	break;
	case lzh::TP_32F:	MAT_GETARRAY(float32, data, i) += saturate_cast<float32>(v);	break;
	case lzh::TP_64F:	MAT_GETARRAY(float64, data, i) += saturate_cast<float64>(v);	break;
	case lzh::TP_64D:	MAT_GETARRAY(float128, data, i) += saturate_cast<float128>(v); break;
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}

template<typename _Tp> inline void lzh::Vector::sub(msize i, _Tp v)
{
	CHECK_MATRIX(data);
	CHECK_INDEX(i, len);
	switch (depth()) {
	case lzh::TP_8S:	MAT_GETARRAY(int8, data, i) -= saturate_cast<int8>(v);		break;
	case lzh::TP_8U:	MAT_GETARRAY(uint8, data, i) -= saturate_cast<uint8>(v);		break;
	case lzh::TP_16S:	MAT_GETARRAY(int16, data, i) -= saturate_cast<int16>(v);		break;
	case lzh::TP_16U:	MAT_GETARRAY(uint16, data, i) -= saturate_cast<uint16>(v);	break;
	case lzh::TP_32S:	MAT_GETARRAY(int32, data, i) -= saturate_cast<int32>(v);		break;
	case lzh::TP_32U:	MAT_GETARRAY(uint32, data, i) -= saturate_cast<uint32>(v);	break;
	case lzh::TP_64S:	MAT_GETARRAY(int64, data, i) -= saturate_cast<int64>(v);		break;
	case lzh::TP_64U:	MAT_GETARRAY(uint64, data, i) -= saturate_cast<uint64>(v);	break;
	case lzh::TP_32F:	MAT_GETARRAY(float32, data, i) -= saturate_cast<float32>(v);	break;
	case lzh::TP_64F:	MAT_GETARRAY(float64, data, i) -= saturate_cast<float64>(v);	break;
	case lzh::TP_64D:	MAT_GETARRAY(float128, data, i) -= saturate_cast<float128>(v); break;
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}

template<typename _Tp> inline void lzh::Vector::mul(msize i, _Tp v)
{
	CHECK_MATRIX(data);
	CHECK_INDEX(i, len);
	switch (depth()) {
	case lzh::TP_8S:	MAT_GETARRAY(int8, data, i) *= saturate_cast<int8>(v);		break;
	case lzh::TP_8U:	MAT_GETARRAY(uint8, data, i) *= saturate_cast<uint8>(v);	break;
	case lzh::TP_16S:	MAT_GETARRAY(int16, data, i) *= saturate_cast<int16>(v);	break;
	case lzh::TP_16U:	MAT_GETARRAY(uint16, data, i) *= saturate_cast<uint16>(v);	break;
	case lzh::TP_32S:	MAT_GETARRAY(int32, data, i) *= saturate_cast<int32>(v);	break;
	case lzh::TP_32U:	MAT_GETARRAY(uint32, data, i) *= saturate_cast<uint32>(v);	break;
	case lzh::TP_64S:	MAT_GETARRAY(int64, data, i) *= saturate_cast<int64>(v);	break;
	case lzh::TP_64U:	MAT_GETARRAY(uint64, data, i) *= saturate_cast<uint64>(v);	break;
	case lzh::TP_32F:	MAT_GETARRAY(float32, data, i) *= saturate_cast<float32>(v);	break;
	case lzh::TP_64F:	MAT_GETARRAY(float64, data, i) *= saturate_cast<float64>(v);	break;
	case lzh::TP_64D:	MAT_GETARRAY(float128, data, i) *= saturate_cast<float128>(v);	break;
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}

template<typename _Tp> inline void lzh::Vector::div(msize i, _Tp v)
{
	CHECK_MATRIX(data);
	CHECK_INDEX(i, len);
	if (v == 0) THROW_INFO(ERR_INFO_DIV);
	switch (depth()) {
	case lzh::TP_8S:	MAT_GETARRAY(int8, data, i) /= saturate_cast<int8>(v);		break;
	case lzh::TP_8U:	MAT_GETARRAY(uint8, data, i) /= saturate_cast<uint8>(v);	break;
	case lzh::TP_16S:	MAT_GETARRAY(int16, data, i) /= saturate_cast<int16>(v);	break;
	case lzh::TP_16U:	MAT_GETARRAY(uint16, data, i) /= saturate_cast<uint16>(v);	break;
	case lzh::TP_32S:	MAT_GETARRAY(int32, data, i) /= saturate_cast<int32>(v);	break;
	case lzh::TP_32U:	MAT_GETARRAY(uint32, data, i) /= saturate_cast<uint32>(v);	break;
	case lzh::TP_64S:	MAT_GETARRAY(int64, data, i) /= saturate_cast<int64>(v);	break;
	case lzh::TP_64U:	MAT_GETARRAY(uint64, data, i) /= saturate_cast<uint64>(v);	break;
	case lzh::TP_32F:	MAT_GETARRAY(float32, data, i) /= saturate_cast<float32>(v);	break;
	case lzh::TP_64F:	MAT_GETARRAY(float64, data, i) /= saturate_cast<float64>(v);	break;
	case lzh::TP_64D:	MAT_GETARRAY(float128, data, i) /= saturate_cast<float128>(v);	break;
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}
