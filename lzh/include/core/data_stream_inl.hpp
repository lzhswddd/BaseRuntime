template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream & out, const Range_<_Tp> & t)
{
	out << "Range(" << t.v1 << "," << t.v2 << ", type=" << DataType<_Tp>().type_name << ")";
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream & out, const Rect_<_Tp> & t)
{
	out << "Rect(" << t.x << "," << t.y << "," << t.width << "," << t.height << ", type=" << DataType<_Tp>().type_name << ")";
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream & out, const Size_<_Tp> & t)
{
	out << "Size(" << t.h << "," << t.w << ", type=" << DataType<_Tp>().type_name << ")";
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream & out, const Size3_<_Tp> & t)
{
	out << "Size(" << t.h << "," << t.w << "," << t.c << ", type=" << DataType<_Tp>().type_name << ")";
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator <<(std::ostream & out, const Point2_<_Tp>& t)
{
	out << "Point(" << t.x << "," << t.y << ", type=" << DataType<_Tp>().type_name << ")";
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream & out, const Point3_<_Tp>& t)
{
	out << "Point(" << t.x << "," << t.y << "," << t.z << ", type=" << DataType<_Tp>().type_name << ")";
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream &out, const MatPtr<_Tp> &t)
{
	out << "Ptr([";
	FOR_RANGE(i, 0, t.size())
		if (i == t.size() - 1)
			out << t[i] << "]" << ", type=" << DataType<_Tp>().type_name << ")";
		else
			out << t[i] << ", ";
	return out;
}
template<typename _Tp, lzh::int32 N> inline std::ostream & lzh::operator << (std::ostream &out, const Vec<_Tp, N> &t)
{
	out << "Vec" << std::to_string(N) <<"([";
	FOR_RANGE(i, 0, N)
		if (i == N - 1)
			out << t[i] << "]" << ", type=" << DataType<_Tp>().type_name << ")";
		else
			out << t[i] << ", ";
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream &out, const STLApi<_Tp> &api)
{
	out << api.obj();
	return out;
}
template<typename _Tp> inline std::ostream & lzh::operator << (std::ostream &out, const SeqList<_Tp> &sl)
{
	if (sl.length() != 0) {
		_FOR(i, 0, sl.length(), 1)
			out << sl.data[i];
		out << std::endl;
	}
	return out;
}

template<typename _Tp> TEMPLATE_API std::vector<_Tp>& lzh::operator<<(std::vector<_Tp>& vecs, const _Tp & v)
{
	vecs.push_back(v);
	return vecs;
}

template<typename _Tp> TEMPLATE_API std::ostream & lzh::operator<<(std::ostream & out, const std::vector<_Tp>& vecs)
{
	for (const _Tp &v : vecs)out << v << std::endl; return out;
}
