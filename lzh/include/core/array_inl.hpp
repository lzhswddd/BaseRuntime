template<typename _Tp> inline lzh::Array::Array(const MatCommaInitializer_<_Tp>& m) {
	create(m.cols(), m.rows(), m.channels(), DataType<_Tp>::type);
	memcpy(data, m.data(), memsize());
}
template<typename _Tp> inline lzh::Array::Array(Size3 size, int32 flags, _Tp v, ...) {
	init();
	create(size, flags);
	va_list ap;
	va_start(ap, v);
	set(0, v);
	FOR_RANGE(i, 1, size.area())
		set(i, va_arg(ap, _Tp));
	va_end(ap);
}
template<typename _Ty> inline lzh::Array::Array(std::initializer_list<_Ty> _Ilist)
{
	*this = Array((int32)_Ilist.size(), _Ilist.begin(), DataType<_Ty>::type);
}
template<typename _Ty> inline lzh::Array::Array(std::initializer_list<std::initializer_list<_Ty>> _Ilist)
{
	int row = (int32)_Ilist.size();
	int col = 0;
	for (std::initializer_list<_Ty> iter : _Ilist) {
		int size = (int32)iter.size();
		if (col < size) {
			col = size;
		}
	}
	Array mat(col, row, DataType<_Ty>::type);
	int i, j;
	i = 0;
	for (std::initializer_list<_Ty> iter : _Ilist) {
		j = 0;
		for (_Ty item : iter) {
			mat.set(i, j++, item);
		}
		i += 1;
	}
	*this = mat;
	//*this = Array((int32)_Ilist.size(), _Ilist.begin(), DataType<_Ty>::type);
}
template<typename _Ty> inline lzh::Array::Array(std::initializer_list<std::initializer_list<std::initializer_list<_Ty>>> _Ilist)
{
	int32 row = (int32)_Ilist.size();
	int32 col = 0;
	int32 channel = 0;
	for (std::initializer_list<std::initializer_list<_Ty>> iter : _Ilist) {
		int size = (int32)iter.size();
		if (col < size) {
			col = size;
		}
		for (std::initializer_list<_Ty> item : iter) {
			int32 _size = (int32)item.size();
			if (channel < _size) {
				channel = _size;
			}
		}
	}
	Array mat(col, row, channel, DataType<_Ty>::type);
	int i, j, k;
	i = 0;
	for (std::initializer_list<std::initializer_list<_Ty>> list : _Ilist) {
		j = 0;
		for (std::initializer_list<_Ty> iter : list) {
			k = 0;
			for (_Ty item : iter) {
				mat.set(i, j, k++, item);
			}
			j += 1;
		}
		i += 1;
	}
	*this = mat;
	//*this = Array((int32)_Ilist.size(), _Ilist.begin(), DataType<_Ty>::type);
}
template<typename _Tp>  _Tp & lzh::Array::first() const
{
	return data.at<_Tp>(0);
}
template<typename _Tp> inline _Tp & lzh::Array::last() const
{
	return data.at<_Tp>(data.size() - 1);
}
template<typename _Tp> inline _Tp * lzh::Array::begin()
{
	return data.begin<_Tp>();
}
template<typename _Tp> inline const _Tp * lzh::Array::begin() const
{
	return data.begin<_Tp>();
}
template<typename _Tp> inline _Tp * lzh::Array::end()
{
	return data.end<_Tp>();
}
template<typename _Tp> inline const _Tp * lzh::Array::end() const
{
	return data.end<_Tp>();
}
template<typename _Tp> inline constexpr std::vector<_Tp> lzh::Array::toVec() const noexcept
{
	if (dim == 1) {
		std::vector<_Tp> vec(len());
		if (DataType<_Tp>::type == depth())
			memcpy(vec.data(), data, memsize());
		else
			FOR_RANGE(i, 0, len())vec[i] = get<_Tp>(i);
		return vec;
	}
	else return std::vector<_Tp>();
}
template<typename _Tp> inline void lzh::Array::set(int32 i, const _Tp & v) {
	data.setv(toidx(i), v);
}
template<typename _Tp> inline void lzh::Array::set(int32 i0, int32 i1, const _Tp & v) {
	data.setv(toidx(i0, i1), v);
}
template<typename _Tp> inline void lzh::Array::set(int32 i0, int32 i1, int32 i2, const _Tp & v) {
	data.setv(toidx(i0, i1, i2), v);
}
template<typename _Tp> inline void lzh::Array::set(Point p, const _Tp & v)
{
	data.setv(toidx(p.y, p.x), v);
}
template<typename _Tp> inline void lzh::Array::set(Point3 p, const _Tp & v)
{
	data.setv(toidx(p.y, p.x, p.z), v);
}
template<typename _Tp, lzh::int32 N> inline void lzh::Array::set(int32 i0, int32 i1, const Vec<_Tp, N>& v)
{
	int32 n = channels() > N ? N : channels();
	for (int32 i : Range(n))
		data.setv(toidx(i0, i1, i), v[i]);
}
template<typename _Tp> inline void lzh::Array::add(int32 i, _Tp v)
{
	data.add(i, v);
}
template<typename _Tp> inline void lzh::Array::add(int32 i, int32 j, _Tp v)
{
	add(toidx(i, j), v);
}
template<typename _Tp> inline void lzh::Array::add(int32 i, int32 j, int32 k, _Tp v)
{
	add(toidx(i, j, k), v);
}
template<typename _Tp> inline void lzh::Array::reduce(int32 i, _Tp v)
{
	data.sub(i, v);
}
template<typename _Tp> inline void lzh::Array::reduce(int32 i, int32 j, _Tp v)
{
	reduce(toidx(i, j), v);
}
template<typename _Tp> inline void lzh::Array::reduce(int32 i, int32 j, int32 k, _Tp v)
{
	reduce(toidx(i, j, k), v);
}
template<typename _Tp> inline void lzh::Array::mul(int32 i, _Tp v)
{
	data.mul(i, v);
}
template<typename _Tp> inline void lzh::Array::mul(int32 i, int32 j, _Tp v)
{
	mul(toidx(i, j), v);
}
template<typename _Tp> inline void lzh::Array::mul(int32 i, int32 j, int32 k, _Tp v)
{
	mul(toidx(i, j, k), v);
}
template<typename _Tp> inline void lzh::Array::div(int32 i, _Tp v)
{
	data.div(i, v);
}
template<typename _Tp> inline void lzh::Array::div(int32 i, int32 j, _Tp v)
{
	div(toidx(i, j), v);
}
template<typename _Tp> inline void lzh::Array::div(int32 i, int32 j, int32 k, _Tp v)
{
	div(toidx(i, j, k), v);
}
template<typename _Tp> inline _Tp lzh::Array::get(int32 i) const { return data.get<_Tp>(toidx(i)); }
template<typename _Tp> inline _Tp lzh::Array::get(int32 i0, int32 i1) const { return data.get<_Tp>(toidx(i0, i1)); }
template<typename _Tp> inline _Tp lzh::Array::get(int32 i0, int32 i1, int32 i2) const { return data.get<_Tp>(toidx(i0, i1, i2)); }
template<typename _Tp, int N> inline void lzh::Array::fill(const Vec<_Tp, N> &value) {
	if (N == 1 && value[0] == 0 && isInt(data.depth()) && isDeepCopy())
		memset(data(), 0, memsize());
	else
		FOR_RANGE(i, 0, len()) set(i, value);
}
template<typename _Tp> inline _Tp & lzh::Array::at(int32 w) {
	return data.at<_Tp>(toidx(w));
}
template<typename _Tp> inline _Tp & lzh::Array::at(int32 h, int32 w) {
	return data.at<_Tp>(toidx(h, w));
}
template<typename _Tp> inline _Tp & lzh::Array::at(int32 h, int32 w, int32 c) {
	return data.at<_Tp>(toidx(h, w, c));
}
template<typename _Tp> inline _Tp & lzh::Array::at(Point p) {
	return at<_Tp>(toidx(p.y, p.x));
}
template<typename _Tp> inline _Tp & lzh::Array::at(Point3i p) {
	return at<_Tp>(toidx(p.y, p.x, p.z));
}
template<typename _Tp> inline const _Tp & lzh::Array::at(int32 w) const {
	return data.at<_Tp>(toidx(w));
}
template<typename _Tp> inline const _Tp & lzh::Array::at(int32 h, int32 w) const {
	return data.at<_Tp>(toidx(h, w));
}
template<typename _Tp> inline const _Tp & lzh::Array::at(int32 h, int32 w, int32 c) const {
	return data.at<_Tp>(toidx(h, w, c));
}
template<typename _Tp> inline const _Tp & lzh::Array::at(Point p) const {
	return at<_Tp>(toidx(p.y, p.x));
}
template<typename _Tp> inline const _Tp & lzh::Array::at(Point3i p) const {
	return at<_Tp>(toidx(p.y, p.x, p.z));
}
template<typename _Tp> inline lzh::int32 lzh::Array::find(_Tp value) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	for (int32 ind = 0; ind < len(); ind++)
		if (compare(ind, value, 0))
			return ind;
	return -1;
}
template<typename _Tp> inline bool lzh::Array::contains(_Tp value) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	for (int32 ind = 0; ind < len(); ind++)
		if (compare(ind, value, 0))
			return true;
	return false;
}
template<typename _Tp> inline bool lzh::Array::isValue(int32 i, _Tp v) const
{
	switch (depth()) {
	case lzh::TP_8S:	return MAT_GETARRAY(int8, data(), i) == saturate_cast<int8>(v);
	case lzh::TP_8U:	return MAT_GETARRAY(uint8, data(), i) == saturate_cast<uint8>(v);
	case lzh::TP_16S:	return MAT_GETARRAY(int16, data(), i) == saturate_cast<int16>(v);
	case lzh::TP_16U:	return MAT_GETARRAY(uint16, data(), i) == saturate_cast<uint16>(v);
	case lzh::TP_32S:	return MAT_GETARRAY(int32, data(), i) == saturate_cast<int32>(v);
	case lzh::TP_32U:	return MAT_GETARRAY(uint32, data(), i) == saturate_cast<uint32>(v);
	case lzh::TP_64S:	return MAT_GETARRAY(int64, data(), i) == saturate_cast<int64>(v);
	case lzh::TP_64U:	return MAT_GETARRAY(uint64, data(), i) == saturate_cast<uint64>(v);
	case lzh::TP_32F:	return MAT_GETARRAY(float32, data(), i) == saturate_cast<float32>(v);
	case lzh::TP_64F:	return MAT_GETARRAY(float64, data(), i) == saturate_cast<float64>(v);
	case lzh::TP_64D:	return MAT_GETARRAY(float128, data(), i) == saturate_cast<float128>(v);
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}
template<typename _Tp> inline bool lzh::Array::isValue(Point p, _Tp v) const
{
	return isValue(toidx(p.y, p.x), v);
}
template<typename _Tp> inline bool lzh::Array::compare(int32 i, _Tp v, int32 op) const
{
	i = toidx(i);
	bool state = false;
	switch (depth()) {
	case lzh::TP_8S:	COMPARE(((const int8*)data())[i], v, op, state); break;
	case lzh::TP_8U:	COMPARE(((const uint8*)data())[i], v, op, state); break;
	case lzh::TP_16S:	COMPARE(((const int16*)data())[i], v, op, state); break;
	case lzh::TP_16U:	COMPARE(((const uint16*)data())[i], v, op, state); break;
	case lzh::TP_32S:	COMPARE(((const int32*)data())[i], v, op, state); break;
	case lzh::TP_32U:	COMPARE(((const uint32*)data())[i], v, op, state); break;
	case lzh::TP_64S:	COMPARE(((const int64*)data())[i], v, op, state); break;
	case lzh::TP_64U:	COMPARE(((const uint64*)data())[i], v, op, state); break;
	case lzh::TP_32F:	COMPARE(((const float32*)data())[i], v, op, state); break;
	case lzh::TP_64F:	COMPARE(((const float64*)data())[i], v, op, state); break;
	case lzh::TP_64D:	COMPARE(((const float128*)data())[i], v, op, state); break;
	default:return false;
	}
	return state;
}
template<typename _Tp> inline bool lzh::Array::compare(int32 i0, int32 i1, _Tp v, int32 op) const
{
	int32 i = toidx(i0, i1);
	bool state = false;
	switch (depth()) {
	case lzh::TP_8S:	COMPARE(((const int8*)data())[i], v, op, state); break;
	case lzh::TP_8U:	COMPARE(((const uint8*)data())[i], v, op, state); break;
	case lzh::TP_16S:	COMPARE(((const int16*)data())[i], v, op, state); break;
	case lzh::TP_16U:	COMPARE(((const uint16*)data())[i], v, op, state); break;
	case lzh::TP_32S:	COMPARE(((const int32*)data())[i], v, op, state); break;
	case lzh::TP_32U:	COMPARE(((const uint32*)data())[i], v, op, state); break;
	case lzh::TP_64S:	COMPARE(((const int64*)data())[i], v, op, state); break;
	case lzh::TP_64U:	COMPARE(((const uint64*)data())[i], v, op, state); break;
	case lzh::TP_32F:	COMPARE(((const float32*)data())[i], v, op, state); break;
	case lzh::TP_64F:	COMPARE(((const float64*)data())[i], v, op, state); break;
	case lzh::TP_64D:	COMPARE(((const float128*)data())[i], v, op, state); break;
	default:return false;
	}
	return state;
}
template<typename _Tp> inline bool lzh::Array::compare(int32 i0, int32 i1, int32 i2, _Tp v, int32 op) const
{
	int32 i = toidx(i0, i1, i2);
	bool state = false;
	switch (depth()) {
	case lzh::TP_8S:	COMPARE(((const int8*)data())[i], v, op, state); break;
	case lzh::TP_8U:	COMPARE(((const uint8*)data())[i], v, op, state); break;
	case lzh::TP_16S:	COMPARE(((const int16*)data())[i], v, op, state); break;
	case lzh::TP_16U:	COMPARE(((const uint16*)data())[i], v, op, state); break;
	case lzh::TP_32S:	COMPARE(((const int32*)data())[i], v, op, state); break;
	case lzh::TP_32U:	COMPARE(((const uint32*)data())[i], v, op, state); break;
	case lzh::TP_64S:	COMPARE(((const int64*)data())[i], v, op, state); break;
	case lzh::TP_64U:	COMPARE(((const uint64*)data())[i], v, op, state); break;
	case lzh::TP_32F:	COMPARE(((const float32*)data())[i], v, op, state); break;
	case lzh::TP_64F:	COMPARE(((const float64*)data())[i], v, op, state); break;
	case lzh::TP_64D:	COMPARE(((const float128*)data())[i], v, op, state); break;
	default:return false;
	}
	return state;
}
template<typename _Tp> inline _Tp & lzh::Array::findmax()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	int32 max_adr = 0;
	for (int32 ind = 1; ind < len(); ind++)
		if (compare(max_adr, ind, -2))
			max_adr = ind;
	return at<_Tp>(max_adr);
}
template<typename _Tp> inline _Tp & lzh::Array::findmin()
{
#ifdef LZH_DEBUG
	CHECK_MATRIX(data);
#endif //LZH_DEBUG
	int32 min_adr = 0;
	for (int32 ind = 1; ind < len(); ind++)
		if (compare(min_adr, ind, 2))
			min_adr = ind;
	return at<_Tp>(min_adr);
}
template<typename _Tp> inline const lzh::Array lzh::Array::addvalue(Dire dire, _Tp v) const
{
	Array temp(cols + 1, rows, c_, depth());
	for (int32 i = 0; i < rows; i++) {
		for (int32 j = 0; j < cols + 1; j++) {
			for (int32 z = 0; z < c_; z++) {
				if (dire == LEFT) {
					if (j == 0)
						temp.set(temp.toidx(i, 0, z), v);
					else
						temp.assign(temp.toidx(i, j, z), temp.toidx(i, j - 1, z));
				}
				else if (dire == RIGHT) {
					if (j == cols)
						temp.set(temp.toidx(i, cols, z), v);
					else
						temp.assign(temp.toidx(i, j, z), temp.toidx(i, j, z));
				}
			}
		}
	}
	return temp;
}
template<class _Tp> inline lzh::Array & lzh::Array::operator=(const MatCommaInitializer_<_Tp>& temp)
{
	*this = Array(temp);
	return *this;
}
template<typename _Tp> inline lzh::Array::operator _Tp *() {
	return (_Tp*)data();
}
template<typename _Tp> inline lzh::Array::operator const _Tp *() const {
	return (const _Tp*)data();
}

template<typename _Tp> template<typename _T2> inline
lzh::MatCommaInitializer_<_Tp>& lzh::MatCommaInitializer_<_Tp>::operator , (_T2 v)
{
#ifdef LZH_DEBUG
	if (this->it == this->head + h * w*c)
		THROW_INFO(ERR_INFO_MEMOUT);
#endif
	*this->it = _Tp(v);
	++this->it;
	return *this;
}
template<typename _Tp, typename _T2> inline
lzh::MatCommaInitializer_<_Tp> lzh::operator << (const Array_<_Tp> & m, _T2 val)
{
	MatCommaInitializer_<_Tp> commaInitializer(m);
	return (commaInitializer, val);
}