template<typename _Tp> void lzh::output(const _Tp &t)
{
	std::cout << t << std::endl;
}

template<typename _Tp> inline lzh::Rect_<_Tp> lzh::toRect(const Vec<_Tp, 4>& vec)
{
	return Rect_<_Tp>(vec[0], vec[1], vec[2] - vec[0], vec[3] - vec[1]);
}
template<typename _Tp> inline lzh::Rect_<_Tp> lzh::toRect(const Range_<_Tp>& r, const Range_<_Tp>& c)
{
	return Rect_<_Tp>(r.v1, c.v1, r.dv(), c.dv());
}
template<typename _Tp, int N> lzh::Vec<_Tp, N> lzh::toVec(const Mat & m)
{
	Vec<_Tp, N> vec;
	if (m.len() >= N) {
		if (DataType<_Tp>::type == DataType<mat_t>::type)
			memcpy(vec.data, m.data(), N * sizeof(_Tp));
		else
			for (int32 i = 0; i < N; i++)
				vec[i] = _Tp(m(i));
	}
	return vec;
}
template<typename _Tp, int N> lzh::Vec<_Tp, N> lzh::toVec(const Array & m)
{
	Vec<_Tp, N> vec;
	if (m.len() >= N) {
		if (DataType<_Tp>::type == m.depth())
			memcpy(vec.data, m.data, N * sizeof(_Tp));
		else
			for (int32 i = 0; i < N; i++)
				vec[i] = _Tp(m(i));
	}
	return vec;
}
template<typename _Tp> inline std::vector<_Tp> lzh::str2Data(const std::vector<std::string> &str)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(str);
#endif //LZH_DEBUG
	std::vector<_Tp> v;
	for (const std::string &s : str) {
		if (!isNumber(s))continue;
		switch (MAT_TYPE(_Tp))
		{
		case lzh::TP_8U:
			v.push_back((_Tp)std::stoi(s));
			break;
		case lzh::TP_32S:
			v.push_back((_Tp)std::stoi(s));
			break;
		case lzh::TP_32F:
			v.push_back((_Tp)std::stof(s));
			break;
		case lzh::TP_64F:
			v.push_back((_Tp)std::stod(s));
			break;
		case lzh::TP_64D:
			v.push_back((_Tp)std::stold(s));
			break;
		default:
			break;
		}
	}
	return v;
}
template<typename _Tp> inline const lzh::Mat lzh::Vec2Mat(const std::vector<_Tp>& p)
{
	if (p.empty())return Mat();
	return Array(p).toMat();
}
template<typename _Tp> inline const lzh::Mat lzh::Vec2Mat(const std::vector<std::vector<_Tp>>& ps)
{
	if (ps.empty())return Mat();
	if (DataType<_Tp>().type == NONE)
		LZH_ACCESS(if (DataType<_Tp>().type != NONE), THROW_INFO_STR("类型不符!"));
	int32 row = (int32)ps.size();
	int32 col = (int32)ps[0].size();
	Mat res(col, row);
	mat_t *ptr = res.data();
	for (const std::vector<_Tp> & vec : ps) {
		if (col != (int32)vec.size())
			LZH_ACCESS(if (row != (int32)vec.size()), THROW_INFO_STR("维度不符!"));
		for (const _Tp & p : vec) {
			*ptr++ = _T(p);
		}
	}
	return res;
}
template<typename _Tp> inline std::vector<_Tp> lzh::Mat2Vec(IMat m)
{
	if (m.empty())return std::vector<_Tp>();
	std::vector<_Tp> p(m.size());
	memcpy(&p[0], m.data(), m.memsize());
	return p;
}
template<typename _Tp> inline std::vector<std::vector<_Tp>> lzh::Mat2Vecs(IMat m)
{
	if (m.empty())return std::vector<std::vector<_Tp>>();
	const _Tp *ptr = m;
	std::vector<std::vector<_Tp>> ps(m.rows());
	for (int32 h = 0; h != m.rows(); ++h) {
		ps[h].resize(m.cols());
		memcpy(&ps[h][0], ptr, sizeof(_Tp)*m.cols());
	}
	return ps;
}
template<typename _Tp> inline typename std::vector<_Tp>::iterator lzh::find(std::vector<_Tp>& vec, const _Tp & key)
{
	return std::find(vec.begin(), vec.end(), key);
}
template<typename _Tp> inline typename std::vector<_Tp>::const_iterator lzh::find(const std::vector<_Tp>& vec, const _Tp & key)
{
	return std::find(vec.begin(), vec.end(), key);
}
template<typename _Tp> inline std::vector<lzh::Point2_<_Tp>> lzh::MatToPoint2(IMat m)
{
	std::vector<Point2_<_Tp>> vec;
	int32 l1, l2, flag = -1;
	if (m.rows() == 2) {
		l1 = m.cols();
		l2 = m.channels();
		flag = 0;
	}
	else if (m.cols() == 2) {
		l1 = m.rows();
		l2 = m.channels();
		flag = 1;
	}
	else if (m.channels() == 2) {
		l1 = m.rows();
		l2 = m.cols();
		flag = 2;
	}
	else
		return vec;
	vec.resize(l1*l2);
	for (int32 j = 0; j < l1; j++)
		for (int32 k = 0; k < l2; k++)
			switch (flag)
			{
			case 0:vec[j*l2 + k] = Point2_<_Tp>((_Tp)m(0, j, k), (_Tp)m(1, j, k)); break;
			case 1:vec[j*l2 + k] = Point2_<_Tp>((_Tp)m(j, 0, k), (_Tp)m(j, 1, k)); break;
			case 2:vec[j*l2 + k] = Point2_<_Tp>((_Tp)m(j, k, 0), (_Tp)m(j, k, 1)); break;
			default:return vec;
			}
	return vec;
}
template<typename _Tp> inline std::vector<lzh::Point3_<_Tp>> lzh::MatToPoint3(IMat m)
{
	std::vector<Point3_<_Tp>> vec;
	int32 l1, l2, flag = -1;
	if (m.rows() == 3) {
		l1 = m.cols();
		l2 = m.channels();
		flag = 0;
	}
	else if (m.cols() == 3) {
		l1 = m.rows();
		l2 = m.channels();
		flag = 1;
	}
	else if (m.channels() == 3) {
		l1 = m.rows();
		l2 = m.cols();
		flag = 2;
	}
	else
		return vec;
	vec.resize(l1*l2);
	for (int32 j = 0; j < l1; j++)
		for (int32 k = 0; k < l2; k++)
			switch (flag)
			{
			case 0:vec[j*l2 + k] = Point3_<_Tp>((_Tp)m(0, j, k), (_Tp)m(1, j, k), (_Tp)m(2, j, k)); break;
			case 1:vec[j*l2 + k] = Point3_<_Tp>((_Tp)m(j, 0, k), (_Tp)m(j, 1, k), (_Tp)m(j, 2, k)); break;
			case 2:vec[j*l2 + k] = Point3_<_Tp>((_Tp)m(j, k, 0), (_Tp)m(j, k, 1), (_Tp)m(j, k, 2)); break;
			default:return vec;
			}
	return vec;
}
template<typename _Tp> inline const lzh::Mat lzh::Point3ToMat(const std::vector<Point3_<_Tp>> & ps)
{
	if (ps.empty())return Mat();
	Mat m((int32)ps.size(), 3);
	int32 idx = 0;
	for (const Point3_<_Tp> &p : ps)
	{
		Mat(Size(3), p.x, p.y, p.z).copyTo(m.col(idx++));
	}
	return m;
}
template<typename _Tp> inline const lzh::Mat lzh::Point2ToMat(const std::vector<Point2_<_Tp>> & ps)
{
	if (ps.empty())return Mat();
	Mat m((int32)ps.size(), 2);
	int32 idx = 0;
	for (const Point2_<_Tp> &p : ps)
	{
		Mat(Size(2), _T(p.x), _T(p.y)).copyTo(m.col(idx++));
	}
	return m;
}
template<typename _Tp> std::vector<_Tp> lzh::range(_Tp top, Val gap)
{
	return range((_Tp)0, top, gap);
}
template<typename _Tp> std::vector<_Tp> lzh::range(_Tp low, _Tp top, Val gap)
{
	Mat mat = mRange(low, top, gap);
	std::vector<_Tp> vec(mat.len());
	FOR_RANGE(i, 0, mat.len())vec[i] = saturate_cast<_Tp>(mat(i));
	return vec;
}