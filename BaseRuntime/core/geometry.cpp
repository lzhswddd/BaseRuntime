#define LZH_EXPORTS
#include "geometry.h"
#include "tools.h"
#include "operator.h"
#include "algorithm.h"

using namespace lzh;
//
//lzh::Line::Line() : Vec2m() {}
//lzh::Line::Line(IMat a) : Vec2m(toVec<mat_t, 2>(a)) {}
//lzh::Line::Line(const Vec2m& v) : Vec2m(v) {}
//lzh::Line::Line(Val_ a, Val_ b) : Vec2m(a, b) {}
//lzh::Line::Line(std::initializer_list<mat_t> list) : Vec2m() {
//	int32 size = int32(list.end() - list.begin());
//	LZH_ACCESS(if (size == 2), THROW_INFO_STR("椭圆只需要中心点坐标和半径, 3-4个参数!"));
//	memcpy(data, list.begin(), M_SIZE * size);
//}
//lzh::Line::Line(Point2m p0, Point2m p1) : Vec2m()
//{
//	LZH_ACCESS(if (!isEqual(p0.x, p1.y)), THROW_INFO_STR("该类不支持描述x=b的直线!"));
//	data[0] = (p0.y - p1.y) / (p0.x - p1.x);
//	data[1] = p0.y - data[0] * p0.x;
//}
//Val_ lzh::Line::a() const
//{
//	return data[0];
//}
//Val_ lzh::Line::b() const
//{
//	return data[1];
//}
//Vec2m lzh::Line::operator()(Val_ x) const
//{
//	return data[0] * x + data[1];
//}
//Mat lzh::Line::operator()(Mat X) const
//{
//	return data[0] * X + data[1];
//}
//Vec2m lzh::Line::fit(IMat x, IMat y, Mat* r)
//{
//	Mat a;
//	polyfit(x, y, 1, a, r);
//	return toVec<mat_t, 2>(a);
//}


Ellipse::Ellipse() : Vec4m() {}
Ellipse::Ellipse(IMat a) : Vec4m(toVec<mat_t, 4>(a)) {}
Ellipse::Ellipse(const Vec4m& v) : Vec4m(v) {}
Ellipse::Ellipse(Val_ x0, Val_ y0, Val_ a, Val_ b) : Vec4m(x0, y0, a, b) {}
Ellipse::Ellipse(std::initializer_list<mat_t> list) : Vec4m() {
	int32 size = int32(list.end() - list.begin());
	LZH_ACCESS(if (size == 3 || size == 4), THROW_INFO_STR("椭圆只需要中心点坐标和半径, 3-4个参数!"));
	if (size == 4)memcpy(data, list.begin(), M_SIZE * 4);
	else { memcpy(data, list.begin(), M_SIZE * 3); data[3] = data[2]; }
}
Val_ Ellipse::S() const {
	return data[2] * data[3] * PI_;
}
Val_ Ellipse::C() const {
	return PI * data[2] * data[3];
}
Val_ lzh::Ellipse::a() const
{
	return data[2];
}
Val_ lzh::Ellipse::b() const
{
	return data[3];
}
Val_ lzh::Ellipse::x(Val_ y) const
{
	mat_t x = sqrt((1 - pow(y - data[1], 2) / pow(data[3], 2)) * pow(data[2], 2)) + data[0];
	return x;
}
Val_ lzh::Ellipse::y(Val_ x) const
{
	mat_t y = sqrt((1 - pow(x - data[0], 2) / pow(data[2], 2)) * pow(data[3], 2)) + data[1];
	return y;
}
Point2m Ellipse::center() const {
	return Point2m(data[0], data[1]);
}
Vec2m Ellipse::operator()(Val_ theta) const {
	LZH_ACCESS(if (0 <= theta && theta <= 2 * PI), THROW_INFO_STR("椭圆参数方程的输入theta不在定义域[0, 2pi]内!"));
	return Vec2m(data[0] + data[2] * cos(theta), data[1] + data[3] * sin(theta));
}
Mat lzh::Ellipse::operator()(Mat theta) const
{
	return Mat(data[0] + data[2] * theta.cos(), data[1] + data[3] * theta.sin(), theta.rows() == 1 ? ROW : theta.cols() == 1 ? COL : CHANNEL);
}
bool lzh::Ellipse::contains(Point2m p) const
{
	mat_t r = _T(pow((p.x - data[0]), 2) + pow((p.y - data[1]), 2));
	if (isEqual(r, 0))return true;
	mat_t theta = atan2((p.y - data[1]), (p.x - data[0]));
	if (theta == theta) {
		Point2m pos = operator()(theta);
		mat_t r1 = _T(pow((pos.x - data[0]), 2) + pow((pos.y - data[1]), 2));
		return r1 > r;
	}
	else {
		return false;
	}
	//if (r <= Math.Pow(R, 2))
	//{
	//	return true;
	//}
	//return false;
}
Vec4m Ellipse::fit(IMat x, IMat y, Mat* r)
{
	return Vec4m();
}
Vec3m lzh::Circle::fit(IMat x, IMat y, Mat* r)
{
	Mat a;
	circlefit(x, y, a, r);
	return toVec<mat_t, 3>(a);
}
bool lzh::Circle::contains(Point2m p) const
{
	mat_t r = _T(pow((p.x - data[0]), 2) + pow((p.y - data[1]), 2));
	if (r <= pow(data[2], 2))
	{
		return true;
	}
	return false;
}
Val_ lzh::Circle::radius() const
{
	return data[2];
}

Ellipsoid::Ellipsoid() : Vec<mat_t, 6>() {}
Ellipsoid::Ellipsoid(IMat a) : Vec<mat_t, 6>(toVec<mat_t, 6>(a)) {}
Ellipsoid::Ellipsoid(const Vec<mat_t, 6> & v) : Vec<mat_t, 6>(v) {}
Ellipsoid::Ellipsoid(Val_ x0, Val_ y0, Val_ z0, Val_ a, Val_ b, Val_ c) : Vec<mat_t, 6>(x0, y0, z0, a, b, c) {}
Ellipsoid::Ellipsoid(std::initializer_list<mat_t> list) : Vec<mat_t, 6>() {
	int32 size = int32(list.end() - list.begin());
	LZH_ACCESS(if (size >= 4 && size <= 6), THROW_INFO_STR("椭球只需要中心点坐标与半径, 4-6个参数!"));
	if (size == 6) memcpy(data, list.begin(), M_SIZE * 6);
	else if (size == 5) { memcpy(data, list.begin(), M_SIZE * 5); data[5] = data[4]; }
	else { memcpy(data, list.begin(), M_SIZE * 4); data[5] = data[4] = data[3]; }
}
Val_ Ellipsoid::V() const {
	return Val_(4.0 / 3.0) * PI * data[3] * data[4] * data[5];
}
Val_ Ellipsoid::S() const {
	return Val_(4.0 / 3.0) * (data[3] * data[4] + data[4] * data[5] + data[3] * data[5]) * PI_;
}
Vec3m Ellipsoid::center() const {
	return Vec3m(data[0], data[1], data[2]);
}
Vec3m Ellipsoid::operator()(Val_ theta, Val_ fai)const {
	LZH_ACCESS(if (0 <= theta && theta <= 2 * PI), THROW_INFO_STR("球参数方程的输入theta不在定义域[0, 2pi]内!"));
	LZH_ACCESS(if (0 <= fai && fai <= PI), THROW_INFO_STR("球参数方程的输入t不在定义域[0, pi]内!"));
	return Vec3m(data[0] + data[3] * sin(fai) * cos(theta), data[1] + data[4] * sin(fai) * sin(theta), data[2] + data[5] * cos(fai));
}
Mat lzh::Ellipsoid::operator()(Mat theta_fai) const
{
	Mat theta, fai;
	if (theta_fai.rows() == 2) {
		theta = theta_fai.row(0);
		fai = theta_fai.row(1);
	}
	else {
		theta = theta_fai.col(0);
		fai = theta_fai.col(1);
	}
	Mat res(3, theta_fai.rows());
	(data[0] + data[3] * fai.sin() * theta.cos()).setTo(res.channel(0).toArray());
	(data[1] + data[4] * fai.sin() * theta.sin()).setTo(res.channel(1).toArray());
	(data[2] + data[5] * fai.cos()).setTo(res.channel(2).toArray());
	return res;
}
Vec<mat_t, 6> lzh::Ellipsoid::fit(IMat x, IMat y, Mat * r)
{
	return Vec<mat_t, 6>();
}

Vec<mat_t, 4> lzh::Sphere::fit(IMat x, IMat y, Mat * r)
{
	return Vec<mat_t, 4>();
}

Polynomial::Polynomial() : param() {}
Polynomial::Polynomial(IMat a) : param(a) {}
Polynomial::Polynomial(int power) : param(power, 1) {}
Polynomial::Polynomial(std::initializer_list<mat_t> list) : param(list) {}
Val_ Polynomial::operator()(Val_ x) const {
	LZH_ACCESS(if (!param.empty()), THROW_INFO_STR("多项式参数为空!"));
	mat_t y = 0;
	int32 power = (int32)param.len() - 1;
	for (const mat_t& a : param) {
		y += a * pow(x, power--);
	}
	return y;
}
Mat Polynomial::operator()(IMat x) const
{
	LZH_ACCESS(if (!param.empty()), THROW_INFO_STR("多项式参数为空!"));
	Mat y = Mat::zeros(x.size3());
	int32 power = (int32)param.len() - 1;
	for (const mat_t& a : param) {
		y += a * x.pow(power--);
	}
	return y;
}
Mat& Polynomial::get()
{
	return param;
}
Polynomial Polynomial::fit(IMat x, IMat y, int32 power, Mat* r)
{
	Polynomial poly;
	polyfit(x, y, power, poly.get(), r);
	return poly;
}

/****************************************************************************
B-样条曲线类
*****************************************************************************/
lzh::BSpline::BSpline() :
	type(UNIFORM), k(0),
	P(), nodevector()
{

}
lzh::BSpline::BSpline(BSplineType type, int32 k, IMat p) :
	type(type), k(k),
	P(p), nodevector()
{

}
void lzh::BSpline::setCtrlPoint(IMat p)
{
	P = p;
}
Mat lzh::BSpline::CtrlPoint()const
{
	return P;
}
Mat lzh::BSpline::Node()const
{
	return nodevector;
}
mat_t lzh::BSpline::BF(int32 i, int32 k, Val t)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(nodevector);
#endif //LZH_DEBUG
	if (k == 0)
		if (t >= nodevector(i) && t < nodevector(i + 1))
			return _T(1);
		else
			return _T(0);
	else {
		mat_t len1 = nodevector(i + k) - nodevector(i);
		mat_t len2 = nodevector(i + k + 1) - nodevector(i + 1);
		if (len1 == _T(0))
			len1 = _T(1);
		if (len2 == _T(0))
			len2 = _T(1);
		mat_t s1 = (t - nodevector(i)) / len1 * BF(i, k - 1, t);
		mat_t s2 = (nodevector(i + k + 1) - t) / len2 * BF(i + 1, k - 1, t);
		return s1 + s2;
	}
}
Mat lzh::BSpline::BaseFunction(Val t)const
{
	Mat mark(1, P.cols());
	for (int32 i = 0; i < P.cols(); i++)
		mark(i) = BF(i, k, t);
	return mark;
}
void lzh::BSpline::NodeVector(IMat node)
{
	if (!node.empty())
		nodevector = node;
	else {
		switch (type)
		{
		case UNIFORM:
			nodevector = linspace(0, 1, P.cols() + k + 1);
			break;
		case QUASI_UNIFORM: {
			nodevector = Mat::zeros(P.cols() + k + 1);
			int32 linepage = P.cols() - k;
			if (linepage == 1)
				for (int32 ind = P.cols() + 1; ind < P.cols() + k + 1; ind++)
					nodevector(ind) = 1;
			else {
				int32 judge = 1;
				while (judge != linepage) {
					nodevector(k + judge) = nodevector(k + judge - 1) + 1 / linepage;
					judge++;
				}
				for (int32 ind = P.cols() + 1; ind < P.cols() + k + 1; ind++)
					nodevector(ind) = 1;
			}
			break;
		}
		default:
			THROW_INFO(ERR_INFO_UNLESS);
		}
	}
}
Mat lzh::BSpline::BPiont(IMat T)const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(P);
	CHECK_MATRIX_(nodevector);
	CHECK_MATRIX_(T);
#endif //LZH_DEBUG
	Mat m(2, T.len());
	for (int32 i = 0; i < T.len(); ++i) {
		Mat p_u = P * BaseFunction(T(i));
		m(2 * i) = p_u(0);
		m(2 * i + 1) = p_u(1);
	}
	return m;
}
Mat lzh::BSpline::BPoint(int32 number)const
{
	int32 n = P.cols() - 1;
	mat_t start = _T(k) / (n + k + 1);
	mat_t end = _T(n + 1) / (n + k + 1);
	Mat t = linspace(start, end, number);
	return BPiont(t);
}
Mat lzh::BSpline::operator()(Val t) const
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(P);
	CHECK_MATRIX_(nodevector);
#endif //LZH_DEBUG
	return P * BaseFunction(t);
}
lzh::BSpline lzh::BSpline::fit(IMat P, int32 n, int32 k)
{
	int32 r = P.rows() - 1;
	n = n + 2;
	mat_t start = _T(k) / (n + k + 1),
		end = _T(n + 1) / (n + k + 1);
	Mat t = linspace(start, end, P.rows());
	Mat Node = linspace(0, 1, n + k + 1);
	//t.show();
	BSpline bs(BSpline::UNIFORM, k);
	bs.NodeVector(Node);
	Mat N(n, r + 1);
	//n = n - 1;
	for (int32 i = 0; i < n; i++) {
		for (int32 j = 0; j < P.rows(); j++) {
			N(j, i) = bs.BF(i, k, t(j));
		}
	}
	Mat m;
	LeastSquare(N, P, m);
	m = m.t();
	Mat d;
	Block(m, d, 1, m.rows() - 2, 0, 1);
	Mat p(d, d.range(0, k, 0, 1), ROW);
	bs.setCtrlPoint(p.t());
	bs.NodeVector();
	return bs;
}



const mat_t ChessBoard::scale = _T(37.79527559055118);

lzh::ChessBoard::ChessBoard() : squareSize(0)
{
}

lzh::ChessBoard::ChessBoard(const Array & chessboard, mat_t squaresize, Size concersize)
{
	setSquareSize(squaresize);
	setConcerSize(concersize);
	setImage(chessboard);
}

lzh::ChessBoard::ChessBoard(const Array & chessboard, mat_t squaresize, Size concersize, Size2f realsize)
{
	setSquareSize(squaresize);
	setConcerSize(concersize);
	setRealSize(realsize);
	setImage(chessboard);
}

void lzh::ChessBoard::setSquareSize(mat_t size)
{
	squareSize = size;
}

void lzh::ChessBoard::setConcerSize(Size size)
{
	concerSize = size;
}

void lzh::ChessBoard::setRealSize(Size2f size)
{
	realSize = size;
}

void lzh::ChessBoard::setImage(const Array & chessboard)
{
	chessBoard = chessboard;
}

mat_t lzh::ChessBoard::getSquareSize() const
{
	return squareSize;
}

Size lzh::ChessBoard::getConcerSize() const
{
	return concerSize;
}

Size2f lzh::ChessBoard::getRealSize() const
{
	return realSize;
}

Array lzh::ChessBoard::getImage() const
{
	return chessBoard;
}

ChessBoard lzh::ChessBoard::generateChessboard(mat_t square_size, Size2f real_size, bool lockConcer)
{
	mat_t chessboard_pixel = square_size * ChessBoard::scale;
	int32 width, height, width_pix, height_pix, col, row;
	if (!lockConcer) {
		width = Round(real_size.w / square_size);
		height = Round(real_size.h / square_size);
		width_pix = Round(real_size.w * ChessBoard::scale);
		height_pix = Round(real_size.h * ChessBoard::scale);
		col = Round((width + 1)*chessboard_pixel);
		row = Round((height + 1)*chessboard_pixel);
	}
	else {
		width = int(real_size.w / square_size);
		height = int(real_size.h / square_size);
		width_pix = col = int((width + 1)*chessboard_pixel);
		height_pix = row = int((height + 1)*chessboard_pixel);
	}
	int32 cell_pixel = Round(chessboard_pixel);
	mat_t size = real_size.w / real_size.h;
	Color color = Color::White;
	Color fill_color = Color::White;
	Array image = Array::value(255, col, row, MAT_8U, ColorGamutGray);
	FOR_RANGE(j, 0, height + 1) {
		int32 y = Round(j * chessboard_pixel);
		FOR_RANGE(i, 0, width + 1) {
			int32 x = Round(i * chessboard_pixel);
			rectangle(image, Rect(x, y, cell_pixel, cell_pixel), fill_color, 1, true);
			if (width % 2) {
				if (i != width)
					fill_color = (fill_color == Color::Black ? Color::White : Color::Black);
			}
			else {
				if (i != width + 1)
					fill_color = (fill_color == Color::Black ? Color::White : Color::Black);
			}
		}
	}
	if (height_pix < image.rows && width_pix < image.cols) {
		image = image(Range(height_pix), Range(width_pix)).clone();
		width = int32(width_pix / chessboard_pixel - 1);
		height = int32(height_pix / chessboard_pixel - 1);
	}
	return ChessBoard(image, square_size, Size(width, height), real_size);
}