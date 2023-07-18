template<typename _Tp> inline lzh::Mat::Mat(const _Tp * m, int32 w, int32 h, int32 c)
{
	create(w, h, c);
	if (DataType<_Tp>::type == MAT_T)
		memcpy(m_, m, memsize());
	else {
		FOR_RANGE(i, 0, len())
			m_[i] = _T(m[i]);
	}
}
template<typename ..._Tp> inline lzh::Mat::Mat(Size3 size, const _Tp & ...args)
{
	init();
	create(size);
	int32 i = 0; addList(m_, i, args...);
	if (i < len()) {
		memset(m_ + i, 0, M_SIZE*(len() - i));
	}
}
template<typename _Tp> inline lzh::Mat::Mat(std::initializer_list<_Tp> _Ilist)
{
	*this = Array(_Ilist).toMat();
}
template<typename _Tp> inline lzh::Mat::Mat(std::initializer_list<std::initializer_list<_Tp>> _Ilist)
{
	*this = Array(_Ilist).toMat();
}
template<typename _Tp> inline lzh::Mat::Mat(std::initializer_list<std::initializer_list<std::initializer_list<_Tp>>> _Ilist)
{
	*this = Array(_Ilist).toMat();
}
template<typename _Tp> inline lzh::Mat::Mat(const std::vector<_Tp>& vec)
{
	init();
	if (!vec.empty()) {
		*this = _InputArray(vec).getMat();
	}
}
template<typename _Tp> inline lzh::Mat::Mat(const std::vector<std::vector<_Tp>> &ps)
{
	init();
	if (!ps.empty()) {
		if (DataType<_Tp>().type == NONE)
			LZH_ACCESS(if (DataType<_Tp>().type != NONE), THROW_INFO_STR("ÀàÐÍ²»·û!"));
		int32 row = (int32)ps.size();
		int32 col = 1; 
		for (const std::vector<_Tp> &v : ps) col = std::max((int32)v.size(), col);
		create(col, row);
		mat_t *ptr = m_;
		if (MAT_TYPE(_Tp) == MAT_T) {
			for (const std::vector<_Tp> & vec : ps) {
				if (vec.empty()) {
					memset(ptr, 0, sizeof(_Tp)*col);
				}
				else {
					memcpy(ptr, &vec[0], sizeof(_Tp)*vec.size());
					if (col - (int32)vec.size() > 0) {
						memset(ptr + vec.size(), 0, sizeof(_Tp)*(col - (int32)vec.size()));
					}
				}
				ptr += col;
			}
		}
		else {
			for (int32 i : Range((int32)ps.size())) {
				for (int32 j : Range(col)) {
					if (j > (int32)ps[j].size()) at(i, j) = saturate_cast<mat_t>(ps[i][j]);
					else at(i, j) = 0;
				}
			}
		}
	}
}
template<typename _Tp, lzh::int32 N> inline lzh::Mat::Mat(const Vec<_Tp, N>& vec)
{
	init();
	create(vec.w, vec.h);
	if (MAT_TYPE(_Tp) == MAT_T)
		memcpy(m_, vec.data, N * sizeof(mat_t));
	else
		for (int32 i = 0; i < N; i++)
			m_[i] = _T(vec[i]);
}
template<typename _Tp> inline lzh::Mat::Mat(const Point2_<_Tp>& p)
{
	init();
	create(2);
	m_[0] = _T(p.x);
	m_[1] = _T(p.y);
}
template<typename _Tp> inline lzh::Mat::Mat(const Point3_<_Tp>& p)
{
	init();
	create(3);
	m_[0] = p.x;
	m_[1] = p.y;
	m_[2] = p.z;
}

template<typename _Tp> inline lzh::Mat & lzh::Mat::operator=(const MatCommaInitializer_<_Tp>& temp)
{
	*this = Mat(temp);
	return *this;
}
