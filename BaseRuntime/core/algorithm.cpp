#include <algorithm>
#define LZH_EXPORTS
#include "algorithm.h"
#include "operator.h"
#include "tools.h"
using namespace lzh;

void* Sort::pre = nullptr;


/****************************************************************************
激活函数/损失函数
*****************************************************************************/
OMat lzh::softmax(IMat x)
{
	Mat out;
	if (x.dims() == 3) {
		out = x.clone();
		for (int32 i = 0; i < x.rows(); ++i)
		{
			for (int32 j = 0; j < x.cols(); ++j)
			{
				Mat y_ = out.cut(i, j, CHANNEL);
				y_ -= Max(y_);
				Mat y_exp;
				Exp(y_, y_exp);
				mat_t y_sum = y_exp.sum();
				(y_exp / y_sum).copyTo(y_);
			}
		}
	}
	else
	{
		Mat y_ = x.clone();
		y_ -= Max(y_);
		Mat y_exp;
		Exp(y_, y_exp);
		mat_t y_sum = y_exp.sum();
		out = y_exp / y_sum;
	}
	return out;
}
OMat lzh::L1(IMat y, IMat y0)
{
	return (y - y0).abs();
}
OMat lzh::L2(IMat y, IMat y0)
{
	return (y.pow(2) - y0.pow(2)).sqrt();
}
OMat lzh::Quadratic(IMat y, IMat y0)
{
	return _T(0.5) * (y - y0).pow(2);
}
OMat lzh::Relative(IMat y, IMat y0)
{
	return (y - y0).abs().safediv(y, NAN);
}
OMat lzh::CrossEntropy(IMat y, IMat y0)
{
	return -(y.mul(y0.log()));
}
OMat lzh::SoftmaxCrossEntropy(IMat y, IMat y0)
{
	return CrossEntropy(y, softmax(y0));
}
OMat lzh::sigmoid(IMat x)
{
	return (1.0 / (1.0 + (-x).exp()));
}
OMat lzh::tanh(IMat x)
{
	return (x.exp() - (-x).exp()) / ((x).exp() + (-x).exp());
}
OMat lzh::relu(IMat x)
{
	Mat dst;
	Max(0, x, dst);
	return dst;
}
OMat lzh::elu(IMat x)
{
	Mat x1(x.size3());
	mat_t* p = x1.data();
	const mat_t* mat = x.data();
	for (int32 i = 0; i < x.len(); ++i) {
		if (*mat <= 0)
			*p = ELU_alpha_ * (exp(*mat) - 1);
		else
			*p = *mat;
		p++;
		mat++;
	}
	return x1;
}
OMat lzh::selu(IMat x)
{
	return SELU_scale_ * elu(x);
}
OMat lzh::leaky_relu(IMat x)
{
	Mat x1(x.size3());
	mat_t* p = x1.data();
	const mat_t* mat = x.data();
	for (int32 i = 0; i < x.len(); ++i) {
		if (*mat <= 0)
			* p = *mat * LReLU_alpha_;
		else
			*p = *mat;
		p++;
		mat++;
	}
	return x1;
}
OMat lzh::softplus(IMat x)
{
	return (1 + x.exp()).log();
}

OMat lzh::d_softmax(IMat y)
{
	return y.mul(1 - y);
}
OMat lzh::D_L1(IMat y, IMat y0)
{
	return y - y0;
}
OMat lzh::D_L2(IMat y, IMat y0)
{
	return 2*(y0 - y);
}
OMat lzh::D_Quadratic(IMat y, IMat y0)
{
	return (y0 - y);
}
OMat lzh::D_Relative(IMat y, IMat y0)
{
	Mat dst;
	sign(y0 - y, dst);
	dst.safediv(y.toArray(), NAN);
	return dst;
}
OMat lzh::D_CrossEntropy(IMat y, IMat y0)
{
	return (y0 - y);
}
OMat lzh::D_SoftmaxCrossEntropy(IMat y, IMat y0)
{
	return softmax(y0) - y;
}
OMat lzh::d_sigmoid(IMat x)
{
	Mat s = sigmoid(x);
	return s.mul(1 - s);
}
OMat lzh::d_tanh(IMat x)
{
	return 1 - tanh(x).pow(2);
}
OMat lzh::d_relu(IMat x)
{
	Mat x1(x.size3());
	mat_t* p = x1.data();
	const mat_t* mat = x.data();
	for (int32 i = 0; i < x.len(); ++i) {
		if (*mat > 0)
			* p = 1;
		else
			*p = 0;
		p++;
		mat++;
	}
	return x1;
}
OMat lzh::d_elu(IMat x)
{
	Mat x1(x.size3());
	mat_t* p = x1.data();
	const mat_t* mat = x.data();
	for (int32 i = 0; i < x.len(); ++i) {
		if (*mat > 0)
			* p = 1;
		else if (*mat < 0)
			* p = ELU_alpha_ * exp(*mat);
		else
			*p = 0;
		p++;
		mat++;
	}
	return x1;
}
OMat lzh::d_selu(IMat x)
{
	return SELU_scale_ * d_elu(x);
}
OMat lzh::d_leaky_relu(IMat x)
{
	Mat x1(x.size3());
	mat_t* p = x1.data();
	const mat_t* mat = x.data();
	for (int32 i = 0; i < x.len(); ++i) {
		if (*mat > 0)
			* p = 1;
		else if (*mat < 0)
			* p = LReLU_alpha_;
		else
			*p = 0;
		p++;
		mat++;
	}
	return x1;
}
OMat lzh::d_softplus(IMat x)
{
	Mat y = x.exp();
	return y / (1 + y);
}
/****************************************************************************
排序
*****************************************************************************/
#define SORT_FUN(fun, dst, order)\
do{\
	fun(dst.begin(), dst.end(), order == MIN_TO_MAX ? comp_min<mat_t> : comp_max<mat_t>);	break;\
}while(0)

bool Sort::compare_angle(InputArray a_, InputArray b_)
{
	Array a(a_);
	Array b(b_);
#ifdef LZH_DEBUG
	CHECK_PTR(Sort::pre);
#endif //LZH_DEBUG
	Array& p = *(Array*)Sort::pre;
	mat_t xx = p(1);
	mat_t yy = p(0);
	mat_t v1 = std::atan2(a(0) - yy, a(1) - xx);
	mat_t v2 = std::atan2(b(0) - yy, b(1) - xx);
	if (v1 != v2)
		return v1 < v2;
	return a(1) < b(1);
}
bool Sort::compare_min(InputArray a, InputArray b)
{
	return a < b;
}
bool Sort::compare_max(InputArray a, InputArray b)
{
	return a > b;
}
void Sort::bubble(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(bubble, dst, order);
	dst_.set(dst);
}
void Sort::insert(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(insert, dst, order);
	dst_.set(dst);
}
void Sort::select(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(select, dst, order);
	dst_.set(dst);
}
void Sort::comb(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(comb, dst, order);
	dst_.set(dst);
}
void Sort::gnome(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(gnome, dst, order);
	dst_.set(dst);
}
void Sort::heap(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(heap, dst, order);
	dst_.set(dst);
}
void Sort::shell(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(shell, dst, order);
	dst_.set(dst);
}
void Sort::quick(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(quick, dst, order);
	dst_.set(dst);
}
void Sort::merge(InputArray src, OutputArray dst_, ORDER order)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Mat dst = m.clone();
	SORT_FUN(merge, dst, order);
	dst_.set(dst);
}

void Sort::Register(const void* p)
{
	Sort::pre = (void*)p;
}

OMat lzh::fft1D(InputArray src)
{
	Array in(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(in);
	LZH_ACCESS(if (in.channels() == 2 || in.channels() == 1), THROW_INFO(ERR_INFO_CHANNEL););
#endif
	Mat y;
	if (in.channels() == 2) {
		y = in.clone();
	}
	else {
		y = Mat::zeros(in.cols, in.rows, 2);
		in.setTo(y.channel(0).toArray());
	}	
	if (y.rows() != 1)
		y.reshape(-1, 1, 0);
	int32 l = y.cols();
	Complex u, t;
	brc(y); // 调用反转置换
	int32 stride = y.step()[1];
	mat_t* p = y.data();
	for (int32 h = 2; h <= l; h <<= 1) // 控制层数
	{
		// 初始化单位复根
		Complex wn(cos(-D_PI_ / h), sin(-D_PI_ / h));
		for (int32 j = 0; j < l; j += h) // 控制起始下标
		{
			Complex w(1, 0); // 初始化螺旋因子
			for (int32 k = j; k < j + h / 2; k++) // 配对
			{
				mat_t* val_k = p + k * stride;
				mat_t* val_h = p + (k + h / 2) * stride;
				u = Complex(val_k[0], val_k[1]);
				t = w * Complex(val_h[0], val_h[1]);
				Complex x = u + t;
				val_k[0] = x.r;
				val_k[1] = x.i;
				x = u - t;
				val_h[0] = x.r;
				val_h[1] = x.i;
				w = w * wn;
			} // 据说上面的操作叫蝴蝶操作…
		}
	}
	return y.toArray();
}
OMat lzh::ifft1D(InputArray src)
{
	Mat in = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(in);
	LZH_ACCESS(if (in.channels() == 2), THROW_INFO(ERR_INFO_CHANNEL););
#endif
	//Mat y = y.rows() != 1 ? out.t() : out.clone();
	Mat y = in.clone();
	if (y.rows() != 1)
		y.reshape(-1, 1, 0);
	int32 l = y.cols();
	Complex u, t;
	brc(y); // 调用反转置换
	int32 stride = y.step()[1];
	mat_t* p = y.data();
	for (int32 h = 2; h <= l; h <<= 1) // 控制层数
	{
		// 初始化单位复根
		Complex wn(cos(D_PI_ / h), sin(D_PI_ / h));
		for (int32 j = 0; j < l; j += h) // 控制起始下标
		{
			Complex w(1, 0); // 初始化螺旋因子
			for (int32 k = j; k < j + h / 2; k++) // 配对
			{
				mat_t* val_k = p + k * stride;
				mat_t* val_h = p + (k + h / 2) * stride;
				u = Complex(val_k[0], val_k[1]);
				t = w * Complex(val_h[0], val_h[1]);
				Complex x = u + t;
				val_k[0] = x.r;
				val_k[1] = x.i;
				x = u - t;
				val_h[0] = x.r;
				val_h[1] = x.i;
				w = w * wn;
			} // 据说上面的操作叫蝴蝶操作…
		}
	}
	//y /= l;
	for (int32 i = 0; i < l * 2; i++)
		p[i] /= l;
	return y.toArray();
}
OMat lzh::dft1D(InputArray src_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (src.channels() == 2 || src.channels() == 1), THROW_INFO(ERR_INFO_CHANNEL););
#endif
	src = src.convert(MAT_T);
	int32 width = src.rows != 1 ? src.rows : src.cols;
	Array mat = Array::zeros(width, 1, 2, src.depth());
	int32 stride = src.rows != 1 ? src.step.size() : src.step[1];
	mat_t* y = mat.data;
	const mat_t* s = src.data;
	Complex cplTemp;
	mat_t fixed_factor = -D_PI_ / width;                   // evaluate -i2π/N of -i2πux/N, and store the value for computing efficiency
	if (src.channels() == 2)
	{
		for (int32 v = 0; v < width; v++) {
			const mat_t* c = s;
			for (int32 w = 0; w < width; w++) {
				mat_t power = v * w * fixed_factor;         // evaluate -i2πux/N
				Complex val(cos(power), sin(power));
				cplTemp = Complex(c[0], c[1]) * val;
				y[0] += cplTemp.r;
				y[1] += cplTemp.i;
				c += stride;
			}
			y += 2;
		}
	}
	else
	{
		for (int32 v = 0; v < width; v++) {
			const mat_t* c = s;
			for (int32 w = 0; w < width; w++) {
				mat_t power = v * w * fixed_factor;         // evaluate -i2πux/N
				Complex val(cos(power), sin(power));
				cplTemp = Complex(c[0] * val.r, c[0] * val.i);
				y[0] += cplTemp.r;
				y[1] += cplTemp.i;
				c += stride;
			}
			y += 2;
		}
	}
	// now we have the transformed vector, keep the info
	return mat;
}
OMat lzh::idft1D(InputArray src_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (src.channels() == 2), THROW_INFO(ERR_INFO_CHANNEL););
#endif	
	int32 width = src.rows != 1 ? src.rows : src.cols;
	Array mat(width, 1, 2, src.depth());
	int32 stride = src.rows != 1 ? src.step.size() : src.step[1];
	mat_t* y = mat.data;
	const mat_t* s = src.data;
	Complex cplTemp;
	mat_t fixed_factor = D_PI_ / width;
	for (int32 w = 0; w < width; w++) {
		Complex cplResult;
		const mat_t* c = s;
		for (int32 v = 0; v < width; v++) {
			mat_t power = v * w * fixed_factor;         // evaluate -i2πux/N
			cplResult = cplResult + Complex(c[0], c[1]) * Complex(cos(power), sin(power));
			c += stride;
		}
		y[0] = cplResult.r / width;
		y[1] = cplResult.i / width;
		y += 2;
	}
	// now we have the inverse transformed matrix, keep the info
	return mat;
}
void lzh::fftMult(InputArray a, InputArray b, OutputArray dst)
{
	Mat v1 = a.getMat();
	Mat v2 = b.getMat();
	Mat a_, a1, a2;
	a1 = fft1D(v1);
	a2 = fft1D(v2);
	complexDot(a1, a2, a_);
	a_ = ifft1D(a_);
	a_ = a_.channel(0).clone();
	int32 top = 0;
	FORINT(i, a_.len() - 1, 0, -1) {
		if (!a_.isZero(i, _T(0.1))) {
			top = i;
			break;
		}
	}
	Mat result(top + 1);
	result(top) = 0;
	a_.colRange(0, top + 1).setTo(result);
	FOR_RANGE(i, 0, top) {
		int32 v = Round(result(i));
		result(i + 1) += _T(v / 10);
		result(i) = _T(v % 10);
	}
	dst.set(result);
}
const LargeNumber lzh::fftMult(const LargeNumber& a, const LargeNumber& b)
{
	vec_m vec_a(a.size());
	vec_m vec_b(b.size());
	FOR_RANGE(i, 0, a.size())
		vec_a[i] = _T(a[a.size() - 1 - i]);
	FOR_RANGE(i, 0, b.size())
		vec_b[i] = _T(b[b.size() - 1 - i]);
	int32 l1 = (int32)vec_a.size(),
		l2 = (int32)vec_b.size(), l;
	l = 1;
	while (l < l1 * 2 || l < l2 * 2)   l <<= 1;
	FOR_RANGE_(l1, l) {
		vec_a.push_back(0);
	}
	FOR_RANGE_(l2, l) {
		vec_b.push_back(0);
	}
	Mat result;
	fftMult(vec_a, vec_b, result);
	int32 top = result.len() - 1;
	LargeNumber val;
	int32 v = Round(result(top));
	while (v) {
		val.push_back(v % 10);
		v /= 10;
	}
	reverse(val.begin(), val.end());
	FOR_RANGE(i, 1, top + 1) {
		val.push_back((int32)result(top - i));
	}
	return val;
}
void lzh::complexDot(InputArray m1, InputArray m2, OutputArray dst)
{
	Mat a = m1.getMat();
	Mat b = m2.getMat();
	Mat m(a.size3());
	mat_t* p = m.data();
	if (a.rows() == 1)
		for (int32 i = 0; i < a.len() / 2; i++) {
			p[0] = a(0, i, 0) * b(0, i, 0) - a(0, i, 1) * b(0, i, 1);
			p[1] = a(0, i, 0) * b(0, i, 1) + a(0, i, 1) * b(0, i, 0);
			p += 2;
		}
	else
		for (int32 i = 0; i < a.len() / 2; i++) {
			p[0] = a(i, 0, 0) * b(i, 0, 0) - a(i, 0, 1) * b(i, 0, 1);
			p[1] = a(i, 0, 0) * b(i, 0, 1) + a(i, 0, 1) * b(i, 0, 0);
			p += 2;
		}
	dst.set(m);
}


/****************************************************************************
数值运算
*****************************************************************************/
void lzh::LeastSquare(InputArray x_, InputArray y_, OutputArray out, Mat* r)
{
	Mat x = x_.getMat();
	Mat y = y_.getMat();
	Mat a = ((x.t() * x).inv() * x.t() * y).t();
	if (r)* r = y - a * x;
	out.set(a);
}
void lzh::regress(InputArray x_, InputArray y_, OutputArray out, Mat* r)
{
	Mat x = x_.getMat();
	Mat y = y_.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(x);
	CHECK_MATRIX_(y);
	if (x.rows() != y.rows())
		THROW_INFO(ERR_INFO_SIZE);
#endif //LZH_DEBUG
	LeastSquare(x.addones(RIGHT), y, out, r);
}
void lzh::polyfit(InputArray x_, InputArray y_, uint32 n, OutputArray out, Mat* r)
{
	Mat x = x_.getMat();
	Mat y = y_.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(x);
	CHECK_MATRIX_(y);
	if (x.rows() != y.rows())
		THROW_INFO(ERR_INFO_SIZE);
#endif //LZH_DEBUG
	Mat param(n + 1, x.rows());
	for (uint32 idx = n, i = 0; idx > 0; idx--, i++)
		x.pow((mat_t)idx).setTo(param.col(i));
	Mat::ones(1, x.rows()).setTo(param.col(n));
	LeastSquare(param, y, out, r);
}
void lzh::circlefit(InputArray p_, OutputArray circle, Mat* r)
{
	Mat p = p_.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(p);
#endif //LZH_DEBUG
	Mat y = Mat::zeros(1, p.rows());
	Mat x = Mat::zeros(p.cols() + 1, p.rows());
	for (int32 w = 0; w < p.cols(); ++w)
	{
		y += p.col(w).pow(2);
		p.col(w).setTo(x.col(w));
	}
	Mat::ones(1, p.rows()).setTo(x.col(p.cols()));
	Mat res;
	LeastSquare(x, y, res, r);
	res(0) /= 2; res(1) /= 2; res(2) = sqrt(res(2) + pow(res(0), 2) + pow(res(1), 2));
	circle.set(res);
}
void lzh::circlefit(InputArray _x_, InputArray _y_, OutputArray circle, Mat* r)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(_x_);
	CHECK_MATRIX_(_y_);
#endif //LZH_DEBUG
	Mat x_ = _x_.getMat();
	Mat y_ = _y_.getMat();
	int32 size = x_.len();
	Mat y = (x_ ^ 2) + (y_ ^ 2);
	Mat x(3, size);
	int32 idx = 0;
	x_.setTo(x.col(idx++));
	y_.setTo(x.col(idx++));
	Mat::ones(1, size).setTo(x.col(idx++));
	Mat res;
	LeastSquare(x, y, res, r);
	res(0) /= 2; res(1) /= 2; res(2) = sqrt(res(2) + pow(res(0), 2) + pow(res(1), 2));
	circle.set(res);
}
void lzh::polynomial(InputArray a_, InputArray x_, OutputArray y_)
{
	Mat a = a_.getMat();
	Mat x = x_.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(x);
#endif //LZH_DEBUG
	Mat y = Mat::zeros(x.size3());
	for (int32 idx = 0, power = a.len() - 1; idx < a.len(); ++idx, --power)
	{
		y += a(idx) * x.pow(_T(power));
	}
	y_.set(y);
}
void lzh::NonLinearLeastSqures(InputArray x_, InputArray y_, InputArray a0_, OutputArray out, F f, Mat* r, Val stride, Val error)
{
	Mat x = x_.getMat();
	Mat y = y_.getMat();
	Mat a0 = a0_.getMat();
#ifdef LZH_DEBUG
	CHECK_PTR(f);
	CHECK_MATRIX_(x);
	CHECK_MATRIX_(y);
	CHECK_MATRIX_(a0);
#endif //LZH_DEBUG
	Mat a = a0;
	Mat da = a;
	while (da.norm() > error) {
		try {
			Mat p;
			NumericGradient(a, x, f, p, _T(1e-3));
			Mat q = f(a, x) - y;
			Mat deta;
			LeastSquare(p, q, deta);
			da = stride * deta;
			a = a - da;
		}
		catch (std::exception e)
		{
			std::cout << "拟合失败!" << std::endl;
			break;
		}
	}
	if (r)* r = y - f(a, x);
	out.set(a);
}
void lzh::gradient(InputArray y_, OutputArray grad, InputArray x_)
{
	Mat y = y_.getMat();
	Mat x = x_.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(y);
#endif //LZH_DEBUG
	int32 dim = y.dims();
	Mat df(y.cols(), y.rows(), dim * y.channels());
	for (int32 c = 0; c < y.channels(); ++c) {
		if (x.empty()) {
			if (y.dims() >= 2) {
				for (int32 row = 0; row < y.rows(); ++row)
					for (int32 col = 0; col < y.cols(); ++col)
						if (col == 0) {
							df(row, col, dim * c) = y(row, col + 1, c) - y(row, col, c);
						}
						else if (col == y.cols() - 1) {
							df(row, col, dim * c) = y(row, col, c) - y(row, col - 1, c);
						}
						else {
							df(row, col, dim * c) = ((y(row, col + 1, c) - y(row, col, c)) + (y(row, col, c) - y(row, col - 1, c))) / _T(2.0);
						}
				for (int32 row = 0; row < y.rows(); ++row)
					for (int32 col = 0; col < y.cols(); ++col)
						if (row == 0) {
							df(row, col, dim * c + 1) = y(row + 1, col, c) - y(row, col, c);
						}
						else if (row == y.rows() - 1) {
							df(row, col, dim * c + 1) = y(row, col, c) - y(row - 1, col, c);
						}
						else {
							df(row, col, dim * c + 1) = ((y(row + 1, col, c) - y(row, col, c)) + (y(row, col, c) - y(row - 1, col, c))) / _T(2.0);
						}
			}
			else {
				for (int32 idx = 0; idx < y.len(); ++idx)
					if (idx == 0) {
						df(idx) = y(idx + 1) - y(idx);
					}
					else if (idx == y.len() - 1) {
						df(idx) = y(idx) - y(idx - 1);
					}
					else {
						df(idx) = ((y(idx + 1) - y(idx)) + (y(idx) - y(idx - 1))) / _T(2.0);
					}
			}
		}
		else {
			if (y.dims() >= 2) {
				for (int32 row = 0; row < y.rows(); ++row)
					for (int32 col = 0; col < y.cols(); ++col)
						if (col == 0) {
							df(row, col, dim * c) = (y(row, col + 1, c) - y(row, col, c)) / (x(row, col + 1, c) - x(row, col, c));
						}
						else if (col == y.cols() - 1) {
							df(row, col, dim * c) = (y(row, col, c) - y(row, col - 1, c)) / (x(row, col, c) - x(row, col - 1, c));
						}
						else {
							df(row, col, dim * c) = ((y(row, col + 1, c) - y(row, col, c)) + (y(row, col, c) - y(row, col - 1, c)) / ((x(row, col + 1, c) - x(row, col, c)) + (x(row, col, c) - x(row, col - 1, c))));
						}
				for (int32 row = 0; row < y.rows(); ++row)
					for (int32 col = 0; col < y.cols(); ++col)
						if (row == 0) {
							df(row, col, dim * c + 1) = (y(row + 1, col, c) - y(row, col, c)) / (x(row + 1, col, c) - x(row, col, c));
						}
						else if (row == y.rows() - 1) {
							df(row, col, dim * c + 1) = (y(row, col, c) - y(row - 1, col, c)) / (x(row, col, c) - x(row - 1, col, c));
						}
						else {
							df(row, col, dim * c + 1) = ((y(row + 1, col, c) - y(row, col, c)) + (y(row, col, c) - y(row - 1, col, c)) / ((x(row, col + 1, c) - x(row, col, c)) + (x(row, col, c) - x(row, col - 1, c))));
						}
			}
			else {
				for (int32 idx = 0; idx < y.len(); ++idx)
					if (idx == 0) {
						df(idx) = (y(idx + 1) - y(idx)) / (x(idx + 1) - x(idx));
					}
					else if (idx == y.len() - 1) {
						df(idx) = (y(idx) - y(idx - 1)) / (x(idx) - x(idx - 1));
					}
					else {
						df(idx) = ((y(idx + 1) - y(idx)) + (y(idx) - y(idx - 1))) / ((x(idx + 1) - x(idx)) + (x(idx) - x(idx - 1)));
					}
			}
		}
	}
	grad.set(df);
}
mat_t lzh::NumericGradient(NF f, Val x, Val epsilon)
{
#ifdef LZH_DEBUG
	CHECK_PTR(f);
#endif //LZH_DEBUG
	mat_t tmp1 = x;
	mat_t tmp2 = x;
	tmp1 = tmp1 + epsilon;
	tmp2 = tmp2 - epsilon;
	return (f(tmp1) - f(tmp2)) / (_T(2) * epsilon);
}
void lzh::NumericGradient(InputArray x_, Fun f, OutputArray grad, Val epsilon)
{
	Mat x = x_.getMat();
#ifdef LZH_DEBUG
	CHECK_PTR(f);
#endif //LZH_DEBUG
	int32 n = x.len();
	Mat numgrad = Mat::zeros(n, 1);
	for (int32 i = 0; i < n; ++i) {
		Mat tmp1 = x.clone();
		Mat tmp2 = x.clone();
		tmp1(i) += epsilon;
		tmp2(i) -= epsilon;
		numgrad(i) = (f(tmp1) - f(tmp2)) / (_T(2) * epsilon);
	}
	grad.set(numgrad);
}
void lzh::NumericGradient(InputArray a_, InputArray x_, F f, OutputArray grad, Val epsilon)
{
	Mat x = x_.getMat();
    Mat a = a_.getMat();
#ifdef LZH_DEBUG
	CHECK_PTR(f);
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(x);
#endif //LZH_DEBUG
	int32 n = a.len();
	Mat numgrad = Mat::zeros(n, x.rows());
	for (int32 i = 0; i < n; ++i) {
		Mat tmp1 = a.clone();
		Mat tmp2 = a.clone();
		tmp1(i) += epsilon;
		tmp2(i) -= epsilon;
		((f(tmp1, x) - f(tmp2, x)) / (_T(2) * epsilon)).setTo(numgrad.col(i));
	}
	grad.set(numgrad);
}
mat_t lzh::EulerInt(NF f, Val low, Val high, Val epsilon)
{
#ifdef LZH_DEBUG
	CHECK_PTR(f);
#endif //LZH_DEBUG
	mat_t y = 0;
	for (mat_t x = low; x <= high; x += epsilon)
	{
		y += f(x) * epsilon;
	}
	return y;
}
mat_t lzh::TrapezoidInt(NF f, Val low, Val high, Val epsilon)
{
#ifdef LZH_DEBUG
	CHECK_PTR(f);
#endif //LZH_DEBUG
	mat_t y = 0;
	for (mat_t x = low; x <= high; x += epsilon)
	{
		y += _T(0.5) * epsilon * (f(x) + f(x + epsilon));
	}
	return y;
}
mat_t lzh::RungeKuttaInt(NF f, Val low, Val high, Val epsilon)
{
#ifdef LZH_DEBUG
	CHECK_PTR(f);
#endif //LZH_DEBUG	
	mat_t y = 0;
	for (mat_t x = low; x <= high; x += epsilon)
	{
		mat_t k1 = f(x);
		mat_t k2 = f(x + _T(0.5) * epsilon);
		mat_t k3 = f(x + _T(0.5) * epsilon);
		mat_t k4 = f(x + epsilon);
		y += epsilon * (k1 + 2 * k2 + 2 * k3 + k4) / _T(6);
	}
	return y;
}
Vec2m lzh::SolveLinear(const Point2m& p1, const Point2m& p2)
{
	Vec2m m;
	m[0] = saveDiv(p1.y - p2.y, p1.x - p2.x);
	m[1] = p1.y - p1.x * m[0];
	return m;
}
Point2m lzh::LinearIntersection(const Vec2m& line1, const Vec2m& line2)
{
	Point2m p;
	p.x = saveDiv(line1[1] - line2[1], line2[0] - line1[0]);
	p.y = p.x * line1[0] + line1[1];
	return p;
}
Point2m lzh::LinearIntersection(Val a1, Val b1, Val a2, Val b2)
{
	Point2m p;
	p.x = saveDiv((b1 - b2), (a2 - a1));
	p.y = p.x * a1 + b1;
	return p;
}


void lzh::mSort(InputArray src, OutputArray dst, ORDER flag, RANK r)
{
	Array in(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(in);
#endif // LZH_DEBUG
	int32 size = in.rankSize(r);
	if (size == 1)
		dst.set(in);
	else {
		vec_array vec(size);
		FOR_RANGE(i, 0, size) {
			vec[i] = in(i, r).clone();
		}
		std::sort(vec.begin(), vec.end(), flag == MIN_TO_MAX ? Sort::compare_min : Sort::compare_max);
		Array m(in.size3(), src.depth());
		FOR_RANGE(i, 0, size) {
			vec[i].setTo(m(i, r));
		}
		dst.set(m);
	}
}
void lzh::mSort(IMat src, OutputArray dst, bool(*comp)(IMat a, IMat b), RANK r)
{
	Mat in = src;
#ifdef LZH_DEBUG
	CHECK_MATRIX_(in);
#endif // LZH_DEBUG
	int32 size = in.rankSize(r);
	if (size == 1)
		dst.set(in);
	else {
		vec_mat vec(size);
		FOR_RANGE(i, 0, size) {
			vec[i] = in.cut(i, r).clone();
		}
		std::sort(vec.begin(), vec.end(), comp);
		Mat m(in.size3());
		FOR_RANGE(i, 0, size) {
			vec[i].setTo(m.cut(i, r));
		}
		dst.set(m);
	}
}
void lzh::mSort(IMat src, OutputArray dst, bool(*comp)(InputArray a, InputArray b), RANK r)
{
	Mat in = src;
#ifdef LZH_DEBUG
	CHECK_MATRIX_(in);
#endif // LZH_DEBUG
	int32 size = in.rankSize(r);
	if (size == 1)
		dst.set(in);
	else {
		vec_mat vec(size);
		FOR_RANGE(i, 0, size) {
			vec[i] = in.cut(i, r).clone();
		}
		std::sort(vec.begin(), vec.end(), comp);
		Mat m(in.size3());
		FOR_RANGE(i, 0, size) {
			vec[i].setTo(m.cut(i, r));
		}
		dst.set(m);
	}
}
mat_t lzh::Cross(IMat a, IMat b)
{
	return a(0) * b(1) - a(1) * b(0);
}
bool lzh::Clockwise(IMat p0, IMat p1, IMat p2)
{
	Mat a = p1 - p0;
	Mat b = p2 - p0;
	return Cross(a, b) < 0;
}
void lzh::ConvexHull(InputArray in, OutputArray convexhull)
{
	Array src(in);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif // LZH_DEBUG
	int32 min_idx = src.col(0).minAt();
	Array p = src.row(min_idx).clone();
	Array data(p.len(), p.data(), MAT_T);
	Sort::Register(&data);
	Array s = src.rowRange(1, src.rows).clone();
	src.row(0).setTo(s.row(min_idx - 1));
	mSort(s, s, Sort::compare_angle);
	Sort::Register(nullptr);
	vec_array u(2, p);
	u[1] = s.row(0);
	int32 idx = 1;
	FOR_RANGE(i, 2, s.rows + 1) {
		for (int32 n = (int32)u.size(); n >= 2 && Clockwise(u[n - 2], u[n - 1], s.row(idx)) != true; n--)
			u.pop_back();
		u.push_back(s.row(idx++));
	}
	Array hull(2, (int32)u.size());
	FOR_RANGE(i, 0, u.size())
		u[i].setTo(hull.row(i));
	convexhull.set(hull);
}

bool lzh::Compare(Val v1, Val v2, CompareType compareType, mat_t epsilon)
{
	bool res = false;
	switch (compareType)
	{
	case lzh::LESS:
		res = v1.v < v2.v;
		break;
	case lzh::LESS_EQUAL:
		res = v1.v <= v2.v;
		break;
	case lzh::EQUAL:
		if (isFloat(v1.type) || isFloat(v2.type)) {
			res = isEqual(v1, v2, epsilon);
		}
		else {
			res = v1.v == v2.v;
		}
		break;
	case lzh::GREATER_EQUAL:
		res = v1.v >= v2.v;
		break;
	case lzh::GREATER:
		res = v1.v > v2.v;
		break;
	default:
		break;
	}
	return res;
}
Array lzh::isCompare(InputArray a, InputArray b)
{
	Array a_(a), b_(b);
	if (a_.data == b_.data)return a.getArray();
	else if (!b_.empty() && a_.size3() == b_.size3())return b.getArray();
	else return a_.clone();
}
mat_t lzh::generateGaussianNoise(Val mu, Val sigma)
{
	const mat_t epsilon = std::numeric_limits<mat_t>::min();
	const mat_t two_pi = _T(2.0 * PI);


	static mat_t z0, z1;
	static bool generate = false;
	generate = !generate;

	if (!generate)
		return z1 * sigma + mu;

	mat_t u1, u2;
	do
	{
		u1 = rand() * (_T(1.0) / RAND_MAX);
		u2 = rand() * (_T(1.0) / RAND_MAX);
	} while (u1 <= epsilon);

	z0 = sqrt(_T(-2.0) * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(_T(-2.0) * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}
mat_t lzh::getRandData(int32 min, int32 max, bool isdouble)
{
#ifdef LZH_DEBUG
	if (min > max)
		THROW_INFO(ERR_INFO_VALUE);
#endif //LZH_DEBUG
	if (isdouble) {
		mat_t m1 = _T(rand()) / _T(RAND_MAX);
		mat_t m2 = (mat_t)((rand() % (max - min)) + min);
		return m1 + m2;
	}
	else {
		int32 m = rand() % (max - min) + 1 + min;
		return (mat_t)m;
	}
}
mat_t lzh::getRandData(mat_t min, mat_t max)
{
	mat_t v = _T(rand()) / _T(RAND_MAX);
	return v * (max - min) + min;
}
mat_t lzh::randSample(InputArray src)
{
	Array m = src.getArray();
	int32 h = rand() % m.rows;
	int32 w = rand() % m.cols;
	int32 depth = rand() % m.channels();
	return m(h, w, depth);
}
Array lzh::mRange(Val low, Val top, Val gap)
{
	if (low > top)return Array();
	int32 len = (int32)((top - low) / gap);
	return low + linspace(0, len - 1, len) * gap;
}
Array lzh::linspace(Val low, Val top, int32 len)
{
	if (low > top)return Array();
	check(len, len);
	Array mark(len, MAT_T);
	mark.at<mat_t>(0) = (mat_t)low;
	mat_t gap = _T(top - low) / (len - 1);
	for (int32 ind = 1; ind < len; ind++)
		mark.at<mat_t>(ind) = mark(ind - 1) + gap;
	return mark;
}
OMat lzh::Xavier(Size3 size, int32 n1, int32 n2)
{
	Mat m(size);
	mat_t* p = m.data();
	for (int32 i = 0; i < m.len(); ++i)
	{
		*p = generateGaussianNoise() * _T(1.0) / std::sqrt(_T(n1 + n2));
		//*p = generateGaussianNoise(0, 1) * sqrt(6.0f / (w_ + h_));
		p++;
	}
	return m;
}
OMat lzh::Xavier(int32 w, int32 h, int32 c, int32 n1, int32 n2)
{
	Mat m(w, h, c);
	mat_t* p = m.data();
	for (int32 i = 0; i < m.len(); ++i)
	{
		*p = generateGaussianNoise() * _T(1.0) / std::sqrt(_T(n1 + n2));
		//*p = generateGaussianNoise(0, 1) * sqrt(6.0f / (w_ + h_));
		p++;
	}
	return m;
}
OMat lzh::Random(Size3 size)
{
	Mat m(size);
	mat_t* p = m.data();
	for (int32 i = 0; i < m.len(); ++i)
	{
		*p = generateGaussianNoise();
		p++;
	}
	return m;
}
OMat lzh::Random(int32 w, int32 h, int32 c)
{
	Mat m(w, h, c);
	mat_t* p = m.data();
	for (int32 i = 0; i < m.len(); ++i)
	{
		*p = generateGaussianNoise();
		p++;
	}
	return m;
}
mat_t lzh::generateGaussian(Val x, Val sigma)
{
	return gauss_factor_1D_ * exp(-(x * x) / (2 * sigma * sigma)) * (1 / sigma);
}
mat_t lzh::generateGaussian(Val x, Val y, Val sigma)
{
	mat_t sigma2 = sigma * sigma;
	return gauss_factor_2D_ / sigma2 * exp(-_T(0.5) * (x * x + y * y) / sigma2);
}
mat_t lzh::generateLoG(Val x, Val sigma)
{
	mat_t sigma2 = sigma * sigma;
	mat_t a = gauss_factor_2D_ / (sigma * sigma2);
	mat_t ex = exp(-(x * x) / (2 * sigma2));
	return a * ex + a * ex * (1 / sigma2) * x;
}
mat_t lzh::generateLoG(Val x, Val y, Val sigma)
{
	mat_t xx = x * x; mat_t yy = y * y; mat_t sigma2 = sigma * sigma;
	mat_t a = (-2 * sigma2 + xx + yy) / (sigma2 * sigma2);
	mat_t b = exp(-((xx + yy) / (2 * sigma2)));
	return a * b;
}
mat_t lzh::generateDoG(Val x, Val sigma1, Val sigma2)
{
	return generateGaussian(x, sigma2) - generateGaussian(x, sigma1);
}
mat_t lzh::generateDoG(Val x, Val y, Val sigma1, Val sigma2)
{
	return generateGaussian(x, y, sigma2) - generateGaussian(x, y, sigma1);
}

int32 lzh::mod(int32 x, int32 b)
{
	int a;
	if (x < 0) {
		a = b + x;
	}
	else {
		a = x;
	}
	return a % b;
}
Mat lzh::mod(IMat x, int32 b)
{
	Mat y = Mat::zeros(x.size());
	for (int i = 0; i < x.rows()*x.cols(); i++) {
		mat_t a;
		if (x(i) < 0) {
			a = b + x(i);
		}
		else {
			a = x(i);
		}
		y(i) = a - ((int)a / b) *b;
	}
	return y;
}

bool lzh::any(InputArray s)
{
	Array x = s.getArray();
	for (int i = 0; i < x.rows*x.cols; i++)
		if (x.isZero(i))
				return true;
	return false;
}

bool lzh::any(InputArray s, Val value, CompareType op)
{
	Array x = s.getArray();
	for (int i = 0; i < x.rows*x.cols; i++)
		if (x.compare(i, value, op))
			return true;
	return false;
}

mat_t lzh::normpdf(Val x, Val mu, Val sigma)
{
	// Return NaN for out of range parameters.
	mat_t y;
	try {
		y = saturate_cast<mat_t>(exp(-0.5 * pow((x - mu) / sigma, 2)) / (sqrt(2 * PI)*sigma));
	}
	catch (...) {
		y = NAN;
	}
	return y;
}

Mat lzh::normpdf(InputArray x, Val mu, Val sigma)
{
	// Return NaN for out of range parameters.
	Mat y;
	try {
		Mat dst = x.getMat();
		y = (-0.5 *((dst - mu) / sigma).pow(2)).exp() / (sqrt(2 * PI)*sigma);
	}
	catch (...) {
		fprintf(stderr, "stats:normpdf:InputSizeMismatch\n");
	}
	return y;
}

int32 lzh::rank(InputArray G, mat_t error)
{
	Mat x = G.getArray().clone();
	for (int32 row = 0; row < x.rows() - 1; row++) {
		for (int32 i = row + 1; i < x.rows(); i++) {
			if (x(i, row) == 0)continue;
			mat_t m = -x(i, row) / x(row, row);
			for (int32 j = 0; j < x.cols(); j++) {
				x(i, j) = x(i, j) + m * x(row, j);
				if (abs(x(i, j)) < error)
					x(i, j) = 0;
			}
		}
	}
	int32 rank_ = 0;
	for (int32 row = 0; row < x.rows(); row++) {
		for (int32 col = 0; col < x.cols(); col++) {
			if (!x.isZero(row, col)) {
				++rank_;
				break;
			}
		}
	}
	return rank_;
}

mat_t lzh::fastAtan2(mat_t y, mat_t x)
{
	const mat_t atan2_p1 = 0.9997878412794807f*(mat_t)(180 / PI);
	const mat_t atan2_p3 = -0.3258083974640975f*(mat_t)(180 / PI);
	const mat_t atan2_p5 = 0.1555786518463281f*(mat_t)(180 / PI);
	const mat_t atan2_p7 = -0.04432655554792128f*(mat_t)(180 / PI);
	mat_t ax = std::abs(x), ay = std::abs(y);//首先不分象限，求得一个锐角角度
	mat_t a, c, c2;
	if (ax >= ay)
	{
		c = ay / (ax + DBL_EPSILON);
		c2 = c * c;
		a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
	}
	else
	{
		c = ax / (ay + DBL_EPSILON);
		c2 = c * c;
		a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
	}
	if (x < 0)//锐角求出后，根据x和y的正负性确定向量的方向，即角度。
		a = 180.f - a;
	if (y < 0)
		a = 360.f - a;
	return a;
}

/****************************************************************************
k均值聚类
*****************************************************************************/
void lzh::kmeans(InputArray P_, InputOutputArray k, OutputArray classifier, int32 K, const uint32 iteration, const Val error)
{
	Array P = P_.getArray();
	uint32 iterate = 0;
	Array center(P.cols, K, MAT_T);
	Array group = P;
	Mat dist(P.rows, K);
	Array kclass(1, P.rows, MAT_32S);

	//随机生成聚点
	for (int32 i = 0; i < K; ++i) {
		Array p = P.row(rand() % P.rows);
		p.copy(center, i, i, 0, P.cols - 1);
		for (int32 j = i; j != 0; j--)
			if (center.row(i) == center.row(i - 1)) {
				i--;
				break;
			}
	}

	mat_t error_ = _T(1);
	Array point = center;
	Mat a0 = Mat::zeros(1, K);

	while (iterate++ != iteration && error_ > error) {
		//计算聚点与所有点的距离
		for (int32 i = 0; i < K; ++i) {
			for (int32 j = 0; j < P.rows; ++j) {
				dist(i, j) = (center(i, ROW) - group(j, ROW)).norm(2);
			}
		}
		//数据点按最小距离分组
		for (int32 j = 0; j < P.rows; ++j) {
			int32 min = 0;
			for (int32 i = 1; i < K; ++i) {
				if (dist(i, j) < dist(min, j)) {
					min = i;
				}
			}
			kclass.at<int32>(j) = _I(min);
		}
		//计算聚点与所属数据点的距离
		Mat a = Mat::zeros(1, K);
		for (int32 i = 0; i < K; ++i) {
			for (int32 j = 0; j < P.rows; ++j) {
				if (kclass(j) == i)
					a(i) += dist(i, j);
			}
		}
		//计算与上一次距离的二范数
		error_ = (a0 - a).norm(2);
		if (error_ < error) {
			break;
		}
		a0 = a;
		Mat sum = Mat::zeros(K);
		Mat p = Mat::zeros(P.cols, K);
		//计算聚点所属数据点坐标平均值作为新的聚点
		for (int32 i = 0; i < P.rows; ++i) {
			sum(kclass.at<int32>(i)) += 1;
			p.cut((int32)kclass(i), ROW) += group(i, ROW);
		}
		for (int32 i = 0; i < K; ++i) {
			p.cut(i, ROW) /= sum(i);
		}
		center = p.toArray();
	}
	output(error_);
	k.set(kclass);
	classifier.set(center);
}

/****************************************************************************
求解方程
*****************************************************************************/
void lzh::Trapezoidal(InputArray src_, OutputArray dst_, bool normalization)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	if (src.dims() != 2)
		THROW_INFO(ERR_INFO_NOT2D);
#endif //LZH_DEBUG 
	Mat dst = src.clone();
	int32 height = dst.rows();
	int32 width = dst.cols();
	int32* ZeroNum = new int32[2 * height];
	for (int32 count = 0; count < height; count++) {
		for (int32 i = 0; i < height; i++) {
			int32 count_ = 0;
			for (int32 j = 0; j < width; j++) {
				if (dst.isZero(i * width + j))
					count_++;
				else break;
			}
			ZeroNum[i * 2] = i;
			ZeroNum[i * 2 + 1] = count_;
		}
		for (int32 i = 0; i < height - 1; i++) {
			for (int32 j = i + 1; j < height; j++) {
				if (ZeroNum[i * 2 + 1] > ZeroNum[j * 2 + 1]) {
					lzh::Swap(ZeroNum[i * 2], ZeroNum[j * 2]);
					lzh::Swap(ZeroNum[i * 2 + 1], ZeroNum[j * 2 + 1]);
				}
			}
		}
		Mat t(dst.size3());
		for (int32 i = 0; i < height; i++) {
			for (int32 j = 0; j < width; j++) {
				t[j + i * width] = dst[j + ZeroNum[i * 2] * width];
			}
		}
		dst = t.clone();
		if ((width - ZeroNum[count * 2 + 1]) != (width - count))
			continue;
		if (normalization && !dst.isOne(count + count * width)) {
			mat_t m = 1 / dst(count + count * width);
			for (int32 i = 0; i < width; i++) {
				if (dst.isZero(i + count * width))continue;
				dst[i + count * width] *= m;
			}
		}
		for (int32 i = count + 1; i < height; i++) {
			if ((width - ZeroNum[i * 2 + 1]) != (width - count))continue;
			mat_t m = dst[count + i * width] / dst[count + count * width];
			for (int32 j = ZeroNum[i * 2 + 1]; j < width; j++) {
				if (dst.isZero(j + count * width))continue;
				dst[j + i * width] -= m * dst[j + count * width];
			}
		}
	}
	FREE_ARRAY(ZeroNum);
	dst_.set(dst);
}
void lzh::RowSimplest(InputArray src, OutputArray dst_, int32* rank)
{
	Mat dst;
	Trapezoidal(src, dst);
	int32 height = dst.rows();
	int32 width = dst.cols();
	for (int32 i = 1; i < height; i++) {
		bool flag = false;
		int32 j;
		for (j = 0; j < width; j++) {
			if (!dst.isZero(i * width + j)) {
				flag = true;
				break;
			}
		}
		if (!flag)continue;
		for (int32 k = i - 1; k >= 0; k--) {
			mat_t m = dst[j + k * width] / dst[j + i * width];
			for (int32 l = j; l < width; l++) {
				dst[l + k * width] -= m * dst[l + i * width];
			}
		}
	}
	if (rank)
		* rank = dst.rank();
	dst_.set(dst);
}
void lzh::ColSimplest(InputArray src, OutputArray dst, int32* rank)
{
	RowSimplest(src.getArray().t(), dst, rank);
}
lzh::EQUATION lzh::SolveLinearEquation(InputArray src, OutputArray dst_, Mat* simplest, Mat* mark)
{
	Mat answer, dst;
	RowSimplest(src, answer);
	int32 height = answer.rows();
	int32 width = answer.cols();
	int32 rank = height;
	int32 augmentedRank = rank;
	for (int32 i = 0; i < height; i++) {
		int32 augmented_count = 0;
		int32 rank_count = 0;
		for (int32 j = 0; j < width; j++) {
			if (j < width - 1) {
				if (answer.isZero(j + i * width)) {
					rank_count++;
					augmented_count++;
				}
			}
			else {
				if (answer.isZero(j + i * width)) {
					augmented_count++;
				}
			}
		}
		if (rank_count == width - 1)
			rank--;
		if (augmented_count == width)
			augmentedRank--;
	}
	EQUATION state;
	if (rank >= augmentedRank) {
		if (rank == height) {
			answer.col(width - 1).copyTo(dst);
			if (mark != nullptr)
				* mark = Mat::ones(height);
			state = SPECIAL_SOLUTION;
		}
		else {
			int32 freeparam = height - rank;
			int32* concer = new int32[rank];
			int32* markfree = new int32[freeparam];
			for (int32 h = 0; h < rank; h++) {
				for (int32 w = h; w < width; w++) {
					if (answer(h, w) == 1)
					{
						concer[h] = w; break;
					}
				}
			}
			int32 count = 0;
			for (int32 i = 0; i < height; i++) {
				bool flag = false;
				for (int32 j = 0; j < rank; j++)
					if (i == concer[j]) {
						flag = true;
						break;
					}
				if (flag)continue;
				markfree[count++] = i;
			}
			dst = Mat::zeros(freeparam, height);
			Mat param = Mat::eye(freeparam);
			for (int32 h = 0; h < param.rows(); h++) {
				for (int32 i = 0; i < rank; i++) {
					mat_t v = answer(i, width - 1);
					for (int32 j = 0; j < freeparam; j++) {
						v -= answer(i, markfree[j]) * param(h, j);
					}
					dst(concer[i], h) = v;
				}
				for (int32 i = 0; i < freeparam; i++) {
					dst(markfree[i], h) = param(h, i);
				}
			}
			if (mark != nullptr) {
				*mark = Mat::ones(height);
				for (int32 i = 0; i < freeparam; i++) {
					mark->operator()(markfree[i]) = _T(0);
				}
			}
			FREE_ARRAY(concer);
			FREE_ARRAY(markfree);
			state = GENERAL_SOLUTION;
		}
	}
	else {
		state = NO_SOLUTION;
	}
	if (simplest != nullptr)
		* simplest = answer;
	dst_.set(dst);
	return state;
}
