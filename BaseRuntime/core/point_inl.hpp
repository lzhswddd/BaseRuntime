template<class _Tp> template<typename T2_> inline lzh::Point2_<_Tp>::Point2_(const Point2_<T2_>& p) : x(_Tp(p.x)), y(_Tp(p.y)) {}
template<class _Tp> template<typename T2_> inline T2_ lzh::Point2_<_Tp>::cross(const Point2_<T2_>& p1, const Point2_<T2_>& p2)
{
	return p1.x *p2.y - p1.y *p2.x;
}
template<class _Tp> template<typename T2_> inline bool lzh::Point2_<_Tp>::Clockwise(const Point2_<T2_>& p0, const Point2_<T2_>& p1, const Point2_<T2_>& p2)
{
	Point2_<T2_> a = p1 - p0;
	Point2_<T2_> b = p2 - p0;
	return cross(a, b) < 0;
}
template<class _Tp> inline bool lzh::Point2_<_Tp>::ysort(const Point2_<_Tp>& p0, const Point2_<_Tp>& p1)
{
	if (p0.y != p1.y)
		return p0.y < p1.y;
	return p0.x < p1.x;
}
template<class _Tp> inline lzh::Point2_<_Tp>::Point2_() :x(), y() {}
template<class _Tp> inline lzh::Point2_<_Tp>::Point2_(_Tp x, _Tp y) : x(x), y(y) {}
template<class _Tp> inline lzh::Point2_<_Tp>::Point2_(const _Tp * m)
{
	x = m[0];
	y = m[1];
}
template<class _Tp> inline lzh::Point2_<_Tp>::Point2_(MatPtr<_Tp> && m)
{
	if (m.len >= 2) {
		if (DataType<_Tp>::type == m.depth()) {
			x = m.at<_Tp>(0);
			y = m.at<_Tp>(1);
		}
		else {
			x = m.get<_Tp>(0);
			y = m.get<_Tp>(1);
		}
	}
}
template<class _Tp> inline lzh::Point2_<_Tp>::Point2_(const MatPtr<_Tp> & m)
{
	if (m.size() >= 2) {
		x = m[0];
		y = m[1];
	}
}
template<class _Tp> inline lzh::Point2_<_Tp>::Point2_(const Vec<_Tp, 2>& vec)
{
	*this = vec;
}
template<class _Tp> inline lzh::Point2_<_Tp>& lzh::Point2_<_Tp>::operator=(const Vec<_Tp, 2>& vec)
{
	x = vec[0]; y = vec[1];
	return *this;
}
template<class _Tp> inline bool lzh::Point2_<_Tp>::operator==(const Point2_<_Tp>& P) const
{
	return (x == P.x) && (y == P.y);
}
template<class _Tp> inline bool lzh::Point2_<_Tp>::operator!=(const Point2_<_Tp>& P) const
{
	return !((*this) == P);
}
template<class _Tp> inline _Tp lzh::Point2_<_Tp>::abs() const
{
	return sqrt(x*x + y * y);
}
template<class _Tp> inline bool lzh::Point2_<_Tp>::xsort(const Point2_<_Tp>& p0, const Point2_<_Tp>& p1)
{
	if (p0.x != p1.x)
		return p0.x < p1.x;
	return p0.y < p1.y;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator+=(Point2_<_Tp> &P, const _T2 & v)
{
	P.x += v;
	P.y += v;
	return P;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator-=(Point2_<_Tp> &P, const _T2 & v)
{
	P.x -= v;
	P.y -= v;
	return P;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator*=(Point2_<_Tp> &P, const _T2 & v)
{
	P.x *= v;
	P.y *= v;
	return P;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator/=(Point2_<_Tp> &P, const _T2 & v)
{
	P.x /= v;
	P.y /= v;
	return P;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator+=(Point2_<_Tp>& P, const Point2_<_T2>& v)
{
	P.x += v.x;
	P.y += v.y;
	return P;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator-=(Point2_<_Tp>& P, const Point2_<_T2>& v)
{
	P.x -= v.x;
	P.y -= v.y;
	return P;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator*=(Point2_<_Tp>& P, const Point2_<_T2>& v)
{
	P.x *= v.x;
	P.y *= v.y;
	return P;
}
template<typename _Tp, typename _T2> lzh::Point2_<_Tp> lzh::operator/=(Point2_<_Tp>& P, const Point2_<_T2>& v)
{
	P.x /= v.x;
	P.y /= v.y;
	return P;
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator+(const Point2_<_Tp>& P, const _Tp & v)
{
	return Point2_<_Tp>(P.x + v, P.y + v);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator + (const _Tp& v, const Point2_<_Tp> &P)
{
	return Point2_<_Tp>(P.x + v, P.y + v);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator + (const Point2_<_Tp> &P1, const Point2_<_Tp>& P2)
{
	return Point2_<_Tp>(P1.x + P2.x, P1.y + P2.y);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator-(const Point2_<_Tp>& P, const _Tp & v)
{
	return Point2_<_Tp>(v - P.x, v - P.y);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator - (const _Tp& v, const Point2_<_Tp> &P)
{
	return Point2_<_Tp>(v - P.x, v - P.y);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator - (const Point2_<_Tp> &P1, const Point2_<_Tp>& P2)
{
	return Point2_<_Tp>(P1.x - P2.x, P1.y - P2.y);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator/(const Point2_<_Tp>& P, const _Tp & v)
{
	return Point2_<_Tp>(P.x / v, P.y / v);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator/(const _Tp & v, const Point2_<_Tp>& P)
{
	return Point2_<_Tp>(v / P.x, v / P.y);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator/(const Point2_<_Tp>& P1, const Point2_<_Tp>& P2)
{
	return Point2_<_Tp>(P1.x / P2.x, P1.y / P2.y);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator * (const Point2_<_Tp>& P, const _Tp & v)
{
	return Point2_<_Tp>(P.x * v, P.y * v);
}
template<typename _Tp> inline lzh::Point2_<_Tp> lzh::operator * (const _Tp& v, const Point2_<_Tp> &P)
{
	return Point2_<_Tp>(P.x * v, P.y * v);
}
template<typename _Tp> inline _Tp lzh::operator * (const Point2_<_Tp> &P1, const Point2_<_Tp>& P2)
{
	return P1.x * P2.x + P1.y * P2.y;
}

template<class _Tp> inline lzh::Point3_<_Tp>::Point3_() :x(), y(), z() {}
template<class _Tp> inline lzh::Point3_<_Tp>::Point3_(_Tp x, _Tp y, _Tp z) : x(x), y(y), z(z) {}
template<class _Tp> inline lzh::Point3_<_Tp>::Point3_(const _Tp * m)
{
	x = m[0];
	y = m[1];
	z = m[2];
}
template<class _Tp> inline lzh::Point3_<_Tp>::Point3_(MatPtr<_Tp> && m)
{
	if (m.len >= 3) {
		if (DataType<_Tp>::type == m.depth()) {
			x = m.at<_Tp>(0);
			y = m.at<_Tp>(1);
			z = m.at<_Tp>(2);
		}
		else {
			x = m.get<_Tp>(0);
			y = m.get<_Tp>(1);
			z = m.get<_Tp>(2);
		}
	}
}
template<class _Tp> inline lzh::Point3_<_Tp>::Point3_(const MatPtr<_Tp> & m)
{
	if (m.size() >= 3) {
		x = m[0];
		y = m[1];
		z = m[2];
	}
}
template<class _Tp> inline lzh::Point3_<_Tp>::Point3_(const Vec<_Tp, 3>& vec)
{
	*this = vec;
}
template<class _Tp> inline lzh::Point3_<_Tp>& lzh::Point3_<_Tp>::operator=(const Vec<_Tp, 3>& vec)
{
	x = vec[0]; y = vec[1]; z = vec[2];
	return *this;
}
template<class _Tp> inline bool lzh::Point3_<_Tp>::operator==(Point3_<_Tp>& P) const
{
	return (x == P.x) && (y == P.y) && (z == P.z);
}
template<class _Tp> inline bool lzh::Point3_<_Tp>::operator!=(Point3_<_Tp>& P) const
{
	return !((*this) == P);
}
template<class _Tp>template<typename T2_> inline lzh::Point3_<_Tp>::Point3_(const Point3_<T2_>& p) : x(_Tp(p.x)), y(_Tp(p.y)), z(_Tp(p.z)) {}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator+=(Point3_<_Tp>& P, const _T2 & v)
{
	P.x += v;
	P.y += v;
	P.z += v;
	return P;
}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator-=(Point3_<_Tp>& P, const _T2 & v)
{
	P.x -= v;
	P.y -= v;
	P.z -= v;
	return P;
}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator*=(Point3_<_Tp>& P, const _T2 & v)
{
	P.x *= v;
	P.y *= v;
	P.z *= v;
	return P;
}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator/=(Point3_<_Tp>& P, const _T2 & v)
{
	P.x /= v;
	P.y /= v;
	P.z /= v;
	return P;
}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator+=(Point3_<_Tp>& P, const Point3_<_T2>& v)
{
	P.x += v.x;
	P.y += v.y;
	P.z += v.z;
	return P;
}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator-=(Point3_<_Tp>& P, const Point3_<_T2>& v)
{
	P.x -= v.x;
	P.y -= v.y;
	P.z -= v.z;
	return P;
}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator*=(Point3_<_Tp>& P, const Point3_<_T2>& v)
{
	P.x *= v.x;
	P.y *= v.y;
	P.z *= v.z;
	return P;
}
template<typename _Tp, typename _T2> inline lzh::Point3_<_Tp> lzh::operator/=(Point3_<_Tp>& P, const Point3_<_T2>& v)
{
	P.x /= v.x;
	P.y /= v.y;
	P.z /= v.z;
	return P;
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator + (const _Tp& v, const Point3_<_Tp> &P)
{
	return Point3_<_Tp>(P.x + v, P.y + v, P.z + v);
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator + (const Point3_<_Tp> &P, const _Tp& v)
{
	return Point3_<_Tp>(P.x + v, P.y + v, P.z + v);
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator + (const Point3_<_Tp> &P1, const Point3_<_Tp>& P2)
{
	return Point3_<_Tp>(P1.x + P2.x, P1.y + P2.y, P1.z + P2.z);
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator - (const _Tp& v, const Point3_<_Tp> &P)
{
	return Point3_<_Tp>(v - P.x, v - P.y, v - P.z);
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator - (const Point3_<_Tp> &P, const _Tp& v)
{
	return Point3_<_Tp>(P.x - v, P.y - v, P.z - v);
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator - (const Point3_<_Tp> &P1, const Point3_<_Tp>& P2)
{
	return Point2_<_Tp>(P1.x - P2.x, P1.y - P2.y, P1.z - P2.z);
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator * (const _Tp& v, const Point3_<_Tp> &P)
{
	return Point3_<_Tp>(P.x * v, P.y * v, P.z * v);
}
template<typename _Tp> inline lzh::Point3_<_Tp> lzh::operator * (const Point3_<_Tp> &P, const _Tp& v)
{
	return Point3_<_Tp>(P.x * v, P.y * v, P.z * v);
}
template<typename _Tp> inline _Tp lzh::operator * (const Point3_<_Tp> &P1, const Point3_<_Tp>& P2)
{
	return P1.x * P2.x + P1.y * P2.y + P1.z * P2.z;
}