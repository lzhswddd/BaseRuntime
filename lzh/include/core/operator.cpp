#define LZH_EXPORTS
#include "operator.h"
#include "algorithm.h"
#include "tools.h"
#include <algorithm>
using namespace lzh;
using namespace std;

/****************************************************************************
操作函数
*****************************************************************************/
mat_t lzh::FunOp::angle2radian(mat_t v)
{
	return v * ang2rad_;
}
mat_t lzh::FunOp::radian2angle(mat_t v)
{
	return v * rad2ang_;
}
mat_t lzh::FunOp::sign(mat_t v)
{
	return _T(v > 0 ? 1 : v < 0 ? -1 : 0);
}
mat_t lzh::FunOp::EuclideanDistance_2D(mat_t x, mat_t y)
{
	return sqrt(x * x + y * y);
}
mat_t lzh::FunOp::mul(mat_t x, mat_t y)
{
	return x * y;
}
mat_t lzh::FunOp::inv_mul(mat_t x, mat_t y)
{
	return x * -y;
}

/****************************************************************************
矩阵操作
*****************************************************************************/
void lzh::split(InputArray src_, int32 c, OutputArray dst)
{
	Array src = src_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	check(src.cols, src.rows, src.channels());
	if (c > src.channels() - 1)
		THROW_INFO(ERR_INFO_MEMOUT);
	if (c < 0)
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	Array mat(src.size(), src.depth());
	for (int32 i = 0; i < src.rows; i++)
		for (int32 j = 0; j < src.cols; j++) {
			mat.set(i, j, src(i, j, c));
		}
	dst = mat;
}
void lzh::split(InputArray src_, OutputArrayOfArrays dst)
{
	Array src = src_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	dst.create(src.channels(), src.depth());
	for (int32 c = 0; c < src.channels(); ++c) {
		dst.setMatRef(src.channel(c), c);
	}
}
void lzh::split(const Mat & src, Mat* dst)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	for (int32 c = 0; c < src.channels(); ++c)
		dst[c] = src.channel(c);
}
void lzh::split(const Array& src, Array* dst)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	for (int32 c = 0; c < src.channels(); ++c)
		dst[c] = src.channel(c);
}
void lzh::Reshape(InputArray src, Size3 size, OutputArray dst)
{
	Array out = src.getArray().clone();
	out.reshape(size);
	dst = out;
}
void lzh::merge(const Array* src, int32 channels, OutputArray dst)
{
#ifdef LZH_DEBUG
	if (channels < 0)
		THROW_INFO(ERR_INFO_MEMOUT);
	if (src == nullptr)
		THROW_INFO(ERR_INFO_PTR);
	CHECK_MATRIX_(src[channels - 1]);
#endif //LZH_DEBUG
	Array mat(src[0].cols, src[0].rows, channels, src[0].depth());
	for (int32 z = 0; z < channels; z++)
		src[z].setTo(mat.channel(z));
	dst = mat;
}
void lzh::merge(InputArrayOfArrays src, OutputArray dst)
{
#ifdef LZH_DEBUG
	if (src.size() < 0)
		THROW_INFO(ERR_INFO_MEMOUT);
	if (src.empty())
		THROW_INFO(ERR_INFO_PTR);
#endif //LZH_DEBUG
	Array t = src.getArray(0);
	Array mat(t.cols, t.rows, src.size(), t.depth());
	for (int32 z = 0; z < src.size(); z++) {
		src.getArray(z).setTo(mat.channel(z));
	}
	dst.set(mat);
}
void lzh::merge(InputArrayOfArrays src, int32 channels, OutputArray dst)
{

#ifdef LZH_DEBUG
	if (src.size() < 0)
		THROW_INFO(ERR_INFO_MEMOUT);
	if (src.empty())
		THROW_INFO(ERR_INFO_PTR);
#endif //LZH_DEBUG
	Array t = src.getArray(0);
	Array mat(t.cols, t.rows, channels, t.depth());
	for (int32 z = 0; z < channels; z++) {
		src.getArray(z).setTo(mat.channel(z));
	}
	dst.set(mat);
}
void lzh::reverse(InputArray _m, OutputArray dst)
{
	Array m = _m.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	if (!(m.cols == 1 || m.rows == 1))
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	Array temp = m.clone();
	switch (TP_32F)
	{
	case lzh::TP_8S:std::reverse(temp.begin<int8>(), temp.end<int8>()); break;
	case lzh::TP_8U:std::reverse(temp.begin<uint8>(), temp.end<uint8>()); break;
	case lzh::TP_16S:std::reverse(temp.begin<int16>(), temp.end<int16>()); break;
	case lzh::TP_16U:std::reverse(temp.begin<uint16>(), temp.end<uint16>()); break;
	case lzh::TP_32S:std::reverse(temp.begin<int32>(), temp.end<int32>()); break;
	case lzh::TP_32U:std::reverse(temp.begin<uint32>(), temp.end<uint32>()); break;
	case lzh::TP_64S:std::reverse(temp.begin<int64>(), temp.end<int64>()); break;
	case lzh::TP_64U:std::reverse(temp.begin<uint64>(), temp.end<uint64>()); break;
	case lzh::TP_32F:std::reverse(temp.begin<float32>(), temp.end<float32>()); break;
	case lzh::TP_64F:std::reverse(temp.begin<float64>(), temp.end<float64>()); break;
	case lzh::TP_64D:std::reverse(temp.begin<float128>(), temp.end<float128>()); break;
	default:temp.release(); break;
	}
	dst = temp;
}
void lzh::randSample(InputArray src_, OutputArray dst_, int32 w, int32 h, int32 c)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	check(w, h, c);
	Array dst(w, h, c, src.depth());
	for (int32 ind = 0; ind < src.len(); ind++)
		dst.set(ind, randSample(src));
	dst_ = dst;
}
void lzh::randSample(InputArray m_, OutputArray dst_, RANK rc, int32 num)
{
	Array m(m_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif //LZH_DEBUG
	Array dst = m(rand() % m.rows, rc);
	for (int32 i = 1; i < num; i++) {
		dst = Array(dst, m(rand() % m.rows, rc), rc);
	}
	dst_ = dst;
}
Array lzh::Rand(mat_t low, mat_t top, Size3 size, bool isdouble)
{
#ifdef LZH_DEBUG
	check(size.w, size.h, size.c);
	if (low >= top)
		THROW_INFO(ERR_INFO_VALUE);
#endif //LZH_DEBUG
	Array m(size, isdouble ? MAT_T : MAT_32S);
	for (int32 index = 0; index < m.len(); index++)
		m.set(index, isdouble ? getRandData(low, top) : getRandData(_I(low), _I(top), false));
	return m;
}
void lzh::Block(InputArray src, OutputArray dst, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End, int32 Chennel_Start, int32 Chennel_End)
{
	Array a = src.getArray();
	int32 h = Row_End - Row_Start + 1;
	int32 w = Col_End - Col_Start + 1;
	int32 c = Chennel_End - Chennel_Start + 1;
	check(w, h, c);
	Array mark(w, h, c);
	int32 i = 0;
	for (int32 y = Row_Start, j = 0; y <= Row_End; y++, j++)
		for (int32 x = Col_Start, i = 0; x <= Col_End; x++, i++)
			for (int32 z = Chennel_Start, k = 0; z <= Chennel_End; z++, k++)
				mark.set(j, i, k, a(y, x, z));
	dst = mark;
}


/****************************************************************************
矩阵数学工具
*****************************************************************************/
mat_t lzh::Max(InputArray src, bool isAbs)
{
	return src.getArray().max(isAbs);
}
mat_t lzh::Min(InputArray src, bool isAbs)
{
	return src.getArray().min(isAbs);
}
mat_t lzh::trace(InputArray src)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	if (mat.enable() == -2) THROW_INFO(ERR_INFO_SQUARE);
#endif //LZH_DEBUG
	mat_t sum = 0;
	for (int32 index = 0; index < mat.rows; index++) {
		sum += mat((index + index * mat.cols) * mat.channels());
	}
	return sum;
}
mat_t lzh::median(InputArray src)
{
	Array pnData = src.getArray();
	int32 knLength = pnData.len();
	int32 nLow = 0;
	int32 nHigh = 0;
	int32 nMiddle = 0;
	int32 nMedian = 0;
	int32 nLTmp = 0;
	int32 nHTmp = 0;
	nMedian = (knLength - 1) >> 1;
	nHigh = knLength - 1;
	while (1)
	{
		if (nHigh == nLow)
		{
			return pnData(nHigh);
		}
		if (nHigh == nLow + 1)
		{
			return pnData(nHigh) > pnData(nLow) ? pnData(nLow) : pnData(nHigh);
		}
		nMiddle = (nHigh + nLow) >> 1;
		if (pnData(nLow) > pnData(nHigh))
		{
			pnData.swap(nHigh, nLow);
		}
		if (pnData[nMiddle] > pnData[nHigh])
		{
			pnData.swap(nMiddle, nHigh);
		}
		if (pnData[nMiddle] > pnData[nLow])
		{
			pnData.swap(nMiddle, nLow);
		}
		pnData.swap(nMiddle, nLow + 1);
		nLTmp = nLow + 2;
		nHTmp = nHigh - 1;
		while (1)
		{
			while (pnData[nLTmp] < pnData[nLow])
			{
				nLTmp++;
			}
			while (pnData[nHTmp] > pnData[nLow])
			{
				nHTmp--;
			}
			if (nLTmp > nHTmp)
			{
				pnData.swap(nHTmp, nLow);
				if (nHTmp > nMedian)
				{
					nHigh = nHTmp - 1;
				}
				else
				{
					nLow = nLTmp - 1;
				}
				break;
			}
			else
			{
				pnData.swap(nLTmp, nHTmp);
				nLTmp++;
				nHTmp--;
			}
		}
	}
}
mat_t lzh::cof(InputArray src, int32 x, int32 y)
{
	Mat mat = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	if (x >= mat.cols() || y >= mat.rows())
		THROW_INFO(ERR_INFO_MEMOUT);
#endif //LZH_DEBUG
	mat.DimCheck();
	Mat a(mat.cols() - 1, mat.rows() - 1);
	int32 n = mat.rows();
	for (int32 i = 0, k = 0; i < n; i++)
		for (int32 j = 0; j < n; j++)
			if ((i != x) && (j != y)) {
				a(k) = mat(i * n + j);
				k++;
			}
	return det(a);
}
mat_t lzh::det(InputArray src)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	mat.DimCheck();
	if (mat.enable() == -2)
		THROW_INFO(ERR_INFO_SQUARE);
#endif //LZH_DEBUG
	int32 n = mat.rows;
	if (n == 1)
		return mat(0);
	else {
		Mat a;
		Trapezoidal(mat, a, false);
		mat_t answer = 1;
		for (int32 i = 0; i < n; i++) {
			answer *= a(i, i);
			if (isinf(answer))break;
		}
		return answer;
	}
}
mat_t lzh::Norm(InputArray src, int32 num)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	if (mat.cols != 1 && mat.rows != 1)
		THROW_INFO(ERR_INFO_NORM);
	mat.DimCheck();
	if (num < 0)
		THROW_INFO(ERR_INFO_VALUE);
#endif //LZH_DEBUG
	if (num == 1)
		return mat.sum(1, true);
	else if (num == 2)
		return sqrt(mat.sum(2, true));
	//else if (isinf(num) == 1)
	//	return abs(matrix[find(findmax())]);
	//else if (isinf(num) == -1)
	//	return abs(matrix[find(findmin())]);
	else
		return pow(mat.sum(num, true), 1 / _T(num));
}
mat_t lzh::Dist(InputArray a, InputArray b, int32 num)
{
	return (a - b).norm(num);
}
void lzh::Operator(InputArray src_, OutputArray dst_, mat_t(*op)(mat_t))
{
	Array src = src_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif // LZH_DEBUG
	Array dst(src.size3(), src.depth());
	FOR_RANGE(i, 0, src.rows) {
		FOR_RANGE(j, 0, src.cols) {
			FOR_RANGE(k, 0, src.channels()) {
				dst.set(i, j, k, op(src(i, j, k)));
			}
		}
	}
	dst_ = dst;
}
void lzh::Operator(InputArray m1_, InputArray m2_, OutputArray dst_, mat_t(*op)(mat_t, mat_t))
{
	Array m1 = m1_.getArray();
	Array m2 = m2_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	if (m1.size3() != m2.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Array dst(m1.size3(), m1.depth());
	FOR_RANGE(i, 0, m1.rows) {
		FOR_RANGE(j, 0, m1.cols) {
			FOR_RANGE(k, 0, m1.channels()) {
				dst.set(i, j, k, op(m1(i, j, k), m2(i, j, k)));
			}
		}
	}
	dst_ = dst;
}
void lzh::Bitwise_not(InputOutputArray m)
{
	Array mat = m.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif // LZH_DEBUG
	FOR_RANGE(i, 0, mat.len())mat.BitNot(i);
}
void lzh::Bitwise_not(InputArray mat, OutputArray dst)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif // LZH_DEBUG
	dst = mat.getArray().BitNot();
}
void lzh::Bitwise_and(InputOutputArray m_, InputArray m2_)
{
	Array m1 = m_.getArray();
	Array m2 = m2_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	if (m1.size3() != m2.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	m_.set(m1.BitAnd(m2));
}
void lzh::Bitwise_and(InputOutputArray m, Val b)
{
	Array m1 = m.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
#endif // LZH_DEBUG
	FOR_RANGE(i, 0, m1.len())m1.BitAnd(i, b);
}
void lzh::Bitwise_and(InputArray m1_, InputArray m2_, OutputArray dst)
{
	Array m1 = m1_.getArray();
	Array m2 = m2_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	if (m1.size3() != m2.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	dst = m1.BitAnd(m2);
}
void lzh::Bitwise_and(InputArray m, Val b, OutputArray dst)
{
	Array a = m.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	dst = a.BitAnd(b);
}
void lzh::Bitwise_or(InputOutputArray m, Val b)
{
	Array a = m.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	FOR_RANGE(i, 0, a.len())a.BitOr(i, b);
}
void lzh::Bitwise_or(InputOutputArray m, InputArray m2_)
{
	Array m1 = m.getArray();
	Array m2 = m2_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	if (m1.size3() != m2.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	m.set(m1.BitOr(m2));
}
void lzh::Bitwise_or(InputArray m1_, InputArray m2_, OutputArray dst)
{
	Array m1 = m1_.getArray();
	Array m2 = m2_.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	if (m1.size3() != m2.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	dst = m1.BitOr(m2);
}
void lzh::Bitwise_or(InputArray a, Val b, OutputArray dst)
{
	Array m = a.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	dst = m.BitOr(b);
}
void lzh::Cos(InputArray src, OutputArray dst)
{
	Operator(src, dst, cos);
}
void lzh::Cosh(InputArray src, OutputArray dst)
{
	Operator(src, dst, cosh);
}
void lzh::Sin(InputArray src, OutputArray dst)
{
	Operator(src, dst, sin);
}
void lzh::Sinh(InputArray src, OutputArray dst)
{
	Operator(src, dst, sinh);
}
void lzh::Tan(InputArray src, OutputArray dst)
{
	Operator(src, dst, tan);
}
void lzh::Tanh(InputArray src, OutputArray dst)
{
	Operator(src, dst, std::tanh);
}
void lzh::arcCos(InputArray src, OutputArray dst)
{
	Operator(src, dst, acos);
}
void lzh::arcCosh(InputArray src, OutputArray dst)
{
	Operator(src, dst, acosh);
}
void lzh::arcSin(InputArray src, OutputArray dst)
{
	Operator(src, dst, asin);
}
void lzh::arcSinh(InputArray src, OutputArray dst)
{
	Operator(src, dst, asinh);
}
void lzh::arcTan(InputArray src, OutputArray dst)
{
	Operator(src, dst, atan);
}
void lzh::arcTanh(InputArray src, OutputArray dst)
{
	Operator(src, dst, atanh);
}
void lzh::sign(InputArray src, OutputArray dst)
{
	Operator(src, dst, FunOp::sign);
}
void lzh::arcTan2(InputArray y, InputArray x, OutputArray dst)
{
	Operator(y, x, dst, atan2);
}
void lzh::Radian2Angle(InputArray radian, OutputArray angle)
{
	Operator(radian, angle, FunOp::radian2angle);
}
void lzh::Angle2Radian(InputArray angle, OutputArray radian)
{
	Operator(angle, radian, FunOp::angle2radian);
}
void lzh::adj(InputArray src, OutputArray dst)
{
	Mat mat = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	if (mat.enable() == -2)
		THROW_INFO(ERR_INFO_SQUARE);
#endif //LZH_DEBUG
	int32 n = mat.rows();
	int32 channel = mat.channels();
	Mat a(n, n, channel);
	for (int32 z = 0; z < channel; z++) {
		for (int32 i = 0; i < n; i++)
			for (int32 j = 0; j < n; j++) {
				mat_t m = cof(mat, i, j);
				a(i, j, z) = _T(pow(-1, i + j + 2)) * m;
			}
	}
	a.setTran();
	dst = a;
}
void lzh::inv(InputArray src, OutputArray dst)
{
	Mat mat = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	if (mat.channels() != 1)
		THROW_INFO(ERR_INFO_DIM);
#endif //LZH_DEBUG
	Mat m = mat.clone();
	mat_t determinant = det(m);
	if (isnan(determinant) || determinant == 0) {
		THROW_INFO(ERR_INFO_DET);
	}
	else {
		Mat dst_(m, Mat::eye(m.rows()), COL);
		RowSimplest(dst_, dst_);
		m = dst_(Rect(m.rows(), 0, m.cols(), m.rows())).clone();
	}
	dst.set(m);
}
void lzh::QR(InputArray src, OutputArray Q, OutputArray R)
{
	Mat a = src.getMat();
	Mat q = Mat::zeros(a.size());
	Mat r = Mat::zeros(a.size());

	LZH_ACCESS(if (a.isSquare()), THROW_INFO(ERR_INFO_SQUARE));
	int32 n = a.cols();
	//施密特正交化
	for (int32 j = 0; j < n; j++)
	{
		Mat col_a = a.col(j);
		Mat col_q = a.col(j).clone();
		for (int32 k = 0; k < j; k++)//计算第j列以前
		{
			for (int32 i1 = 0; i1 < col_a.rows(); i1++)
			{//R=Q'A(Q'即Q的转置) 即Q的第k列和A的第j列做内积
				r(k, j) += col_a(i1) * q(i1, k);//Q的第k列
			}
			for (int32 i2 = 0; i2 < n; i2++)
			{
				col_q(i2) -= r(k, j) * q(i2, k);
			}
		}

		mat_t temp = col_q.norm(2);
		r(j, j) = temp;
		for (int32 i3 = 0; i3 < n; i3++)
		{
			//单位化Q
			q(i3, j) = col_q(i3) / temp;
		}
		//q.show(); r.show();
	}
	Q = q; R = r;
}
void lzh::eig(InputArray src, OutputArray eigenvalues, OutputArray eigenvectors, int iterator_num)
{
	Mat mat = src.getMat();
	int32 n = mat.cols();
	Mat q, r, t = mat.clone();
	for (int32 i : range(iterator_num)) {
		if (t.norm(2) == 0)break;
		QR(t, q, r);
		t = r * q;
	}
	Mat eigenValue, eigenVector;
	diagonal(t, eigenValue);
	std::sort(eigenValue.begin(), eigenValue.end(), [](const mat_t &a, const mat_t &b) {return a < b; });
	diag(eigenValue, eigenvalues);
	if (eigenvectors.empty())return;
	eigenVector = Mat::zeros(mat.size());
	mat_t eValue;
	for (int32 count = 0; count < n; count++)
	{
		eValue = eigenValue(count);//当前的特征值
		t = mat.clone();
		for (int32 i = 0; i < n; i++){
			t(i, i) -= eValue;
		}
		//将temp化为阶梯型矩阵(归一性)对角线值为一
		for (int32 i = 0; i < n - 1; i++)
		{
			mat_t coe = t(i, i);
			for (int32 j = i; j < n; j++)
			{
				t(i, j) /= coe;//让对角线值为一
			}
			for (int32 i1 = i + 1; i1 < n; i1++)
			{
				coe = t(i1, i);
				for (int32 j1 = i; j1 < n; j1++)
				{
					t(i1, j1) -= coe * t(i, j1);
				}
			}
		}
		//让最后一行为1
		mat_t sum1 = 1;
		eigenVector(n - 1, count) = 1;
		for (int i2 = n - 2; i2 >= 0; i2--)
		{
			mat_t sum2 = 0;
			for (int j2 = i2 + 1; j2 < n; j2++)
			{
				sum2 += t(i2, j2) * eigenVector(j2, count);
			}
			sum2 = -sum2 / t(i2, i2);
			sum1 += sum2 * sum2;
			eigenVector(i2, count) = sum2;
		}
		sum1 = sqrt(sum1);//当前列的模
		for (int i = 0; i < n; i++)
		{
			//单位化
			eigenVector(i, count) /= sum1;
		}
	}
	eigenvectors = eigenVector;
}
void lzh::diag(InputArray src, OutputArray dst, int32 k)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	if (m.channels() != 1) THROW_INFO(ERR_INFO_DIM);
	if (m.dims() != 1) THROW_INFO(ERR_INFO_NORM);
#endif //LZH_DEBUG
	int32 pos = k < 0 ? -k : k;
	Mat mat = Mat::zeros(m.len() + pos, m.len() + pos);
	const mat_t* data = m.data();
	mat_t* p = mat.data();
	for (int32 h = 0; h < mat.rows(); h++)
	{
		for (int32 w = 0; w < mat.cols(); w++)
		{
			if (h + k == w)
				* p = *data;
			p += 1;
		}
		data += 1;
	}
	dst.set(mat);
}
void lzh::diagonal(InputArray src, OutputArray dst)
{
	Mat m = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	if (m.channels() != 1) THROW_INFO(ERR_INFO_DIM);
	if (m.dims() != 2) THROW_INFO(ERR_INFO_DIM);
#endif //LZH_DEBUG
	Mat v = Mat::zeros(min(m.rows(), m.cols()));
	FOR_RANGE(i, 0, v.len())v(i) = m(i, i);
	dst = v;
}
void lzh::pinv(InputArray src, OutputArray dst, Dire direc)
{
	Mat mat = src.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif //LZH_DEBUG
	Mat result;
	switch (direc)
	{
	case LEFT:result = (mat.t() * mat).inv() * mat.t(); break;
	case RIGHT: {
		Mat m = mat.t();
		lzh::pinv(m, result, LEFT);
		result.setTran();
		break;
	}
	default:
		THROW_INFO(ERR_INFO_VALUE);
	}
	dst.set(result);
}
void lzh::tran(InputArray src, OutputArray dst)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif //LZH_DEBUG
	Array a(mat.rows, mat.cols, mat.channels(), mat.depth());
	int32 n = mat.rows,
		m = mat.cols;
	for (int32 z = 0; z < mat.channels(); z++)
		for (int32 i = 0; i < n; i++)
			for (int32 j = 0; j < m; j++)
				a.set(j, i, z, mat(i, j, z));
	dst.set(a);
}
void lzh::Abs(InputArray src, OutputArray dst)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif //LZH_DEBUG
	Operator(mat, dst, abs);
	//Mat m(mat.size3());
	//Val_ *p = m;
	//for (int32 ind = 0; ind < mat.len(); ind++)
	//	p[ind] = std::abs(mat(ind));
	//dst.set(m);
}
void lzh::POW(InputArray src, OutputArray dst, int32 num)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
	if (mat.enable() == -2)
		THROW_INFO(ERR_INFO_SQUARE);
#endif //LZH_DEBUG
	Array m;
	mat.swap(m);
	if (num > 0) {
		for (int32 i = 1; i < num; i++)
			m = m * mat;
	}
	else if (num < 0) {
		Array a;
		mat.swap(a);
		m.setInv();
		a.setInv();
		for (int32 i = -1; i > num; i--)
			a = a * m;
		m = a;
	}
	else
		m = Array::eye(mat.rows, mat.depth());
	dst.set(m);
}
void lzh::Pow(InputArray src, OutputArray dst, Val num)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif //LZH_DEBUG
	Array m(mat.size3(), mat.depth());
	for (int32 ind = 0; ind < mat.len(); ind++)
		m.set(ind, pow(mat(ind), num));
	dst.set(m);
}
void lzh::Sum(InputArray src, OutputArray dst, RANK r_c)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif //LZH_DEBUG
	Array m;
	if (r_c == COL) {
		m = Array::zeros(mat.cols, 1, 1, mat.depth());
		for (int32 i = 0; i < mat.cols; i++)
			for (int32 z = 0; z < mat.channels(); z++)
				for (int32 j = 0; j < mat.rows; j++)
					m.add(i, mat(j, i, z));
	}
	else if (r_c == ROW) {
		m = Array::zeros(1, mat.rows, 1, mat.depth());
		for (int32 i = 0; i < mat.rows; i++)
			for (int32 z = 0; z < mat.channels(); z++)
				for (int32 j = 0; j < mat.cols; j++)
					m.add(i, mat(i, j, z));
	}
	else if (r_c == CHANNEL) {
		m = Array::zeros(1, 1, mat.channels(), mat.depth());
		for (int32 z = 0; z < mat.channels(); z++)
			for (int32 i = 0; i < mat.rows; i++)
				for (int32 j = 0; j < mat.cols; j++)
					m.add(z, mat(i, j, z));
	}
	else
		THROW_INFO(ERR_INFO_VALUE);
	dst = m;
}
void lzh::Exp(InputArray src, OutputArray dst)
{
	Operator(src, dst, exp);
}
void lzh::Log(InputArray src, OutputArray dst)
{
	Operator(src, dst, log);
}
void lzh::Log10(InputArray src, OutputArray dst)
{
	Operator(src, dst, log10);
}
void lzh::Sqrt(InputArray src, OutputArray dst)
{
	Operator(src, dst, sqrt);
	//Mat m(mat.size3());
	//Val_ *p = m;
	//for (int32 ind = 0; ind < mat.len(); ind++)
	//	p[ind] = sqrt(mat(ind));
	//dst.set(m);
}
void lzh::Opp(InputArray src, OutputArray dst)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif //LZH_DEBUG
	Array m(mat.size3(), mat.depth(), mat.colorGamut());
	for (int32 ind = 0; ind < mat.len(); ind++)
		m.set(ind, -mat(ind));
	dst.set(m);
}
void lzh::Div(InputArray src, OutputArray dst, Val val, Dire dire)
{
	Array mat = src.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif //LZH_DEBUG
	Array m(mat.size3(), mat.depth(), mat.colorGamut());
	for (int32 ind = 0; ind < m.len(); ind++)
		if (dire == LEFT)
			m.set(ind, val / mat(ind));
		else if (dire == RIGHT)
			m.set(ind, mat(ind) / val);
	dst.set(m);
}
void lzh::Div(InputArray m1, InputArray m2, OutputArray dst, Dire dire)
{
	Array a = m1.getArray();
	Array b = m2.getArray();
	Array result;
	switch (dire)
	{
	case LEFT:	result = a.inv() * b; break;
	case RIGHT:	result = b.inv() * a; break;
	default:THROW_INFO(ERR_INFO_VALUE);
	}
	dst.set(result);
}
void lzh::Mult(InputArray m1, InputArray m2, OutputArray dst)
{
	Hadamard(m1, m2, dst);
}
void lzh::Hadamard(InputArray m1, InputArray m2, OutputArray dst)
{
	Array a = m1.getArray();
	Array b = m2.getArray();
	Array mat;
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(b);
#endif //LZH_DEBUG
	if (b.rows == 1 && b.cols == 1 && a.channels() == b.channels()) {
		mat.create(a.size3(), a.depth(), a.colorGamut());
		for (int32 z = 0; z < mat.channels(); z++) {
			mat_t v = b(z);
			for (int32 i = 0; i < mat.rows; i++)
				for (int32 j = 0; j < mat.cols; j++)
					mat.set(i, j, z, a(i, j, z) * v);
		}
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == b.channels())
	{
		mat.create(b.size3(), b.depth(), b.colorGamut());
		for (int32 z = 0; z < mat.channels(); z++) {
			mat_t v = a(z);
			for (int32 i = 0; i < mat.rows; i++)
				for (int32 j = 0; j < mat.cols; j++)
					mat.set(i, j, z, b(i, j, z) * v);
		}
	}
#ifdef LZH_DEBUG
	else if (a.rows != b.rows || a.cols != b.cols || a.channels() != b.channels())
		THROW_INFO(ERR_INFO_SIZE);
#endif //LZH_DEBUG
	else {
		mat.create(a.size3(), a.depth(), a.colorGamut());
		for (int32 ind = 0; ind < a.len(); ind++)
			mat.set(ind, a(ind) * b(ind));
	}
	dst.set(mat);
}
void lzh::Dot(InputArray m1, InputArray m2, OutputArray dst)
{
	dst.set(m1 * m2);
}
void lzh::Sub(InputArray a, InputArray b, OutputArray dst)
{
	dst.set(a - b);
}
void lzh::add(InputArray a, InputArray b, OutputArray dst)
{
	dst.set(a + b);
}

void lzh::add(InputArray src1, InputArray src2, OutputArray dst, InputArray mask, int32 dtype)
{
	Array m1(src1);
	Array m2(src2);
	Array mark_(mask);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (m1.size3() == m2.size3()), THROW_INFO(ERR_INFO_SIZE));
	LZH_ACCESS(if (mark_.empty() || m1.size() == mark_.size()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m1.size3(), dtype < 0 ? m1.depth() : dtype, m1.format());
	if (mark_.empty())
		FOR_RANGE(i, 0, m.len())
		m.set(i, m1(i) + m2(i));
	else
		FOR_RANGE(i, 0, m.len())
		if (!mark_.isZero(i))m.set(i, m1(i) + m2(i));
		else m.set(i, 0);
	dst = m;
}
void lzh::subtract(InputArray src1, InputArray src2, OutputArray dst, InputArray mask, int32 dtype)
{
	Array m1(src1);
	Array m2(src2);
	Array mark_(mask);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (m1.size3() == m2.size3()), THROW_INFO(ERR_INFO_SIZE));
	LZH_ACCESS(if (mark_.empty() || m1.size() == mark_.size()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m1.size3(), dtype < 0 ? m1.depth() : dtype, m1.format());
	if (mark_.empty())
		FOR_RANGE(i, 0, m.len())
		m.set(i, m1(i) - m2(i));
	else
		FOR_RANGE(i, 0, m.len())
		if (!mark_.isZero(i))m.set(i, m1(i) - m2(i));
		else m.set(i, 0);
	dst = m;
}
void lzh::multiply(InputArray src1, InputArray src2, OutputArray dst, InputArray mask, mat_t scale, int32 dtype)
{
	Array m1(src1);
	Array m2(src2);
	Array mark_(mask);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (m1.size3() == m2.size3()), THROW_INFO(ERR_INFO_SIZE));
	LZH_ACCESS(if (mark_.empty() || m1.size() == mark_.size()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m1.size3(), dtype < 0 ? m1.depth() : dtype, m1.format());
	if (mark_.empty())
		FOR_RANGE(i, 0, m.len())
		m.set(i, scale * m1(i) * m2(i));
	else
		FOR_RANGE(i, 0, m.len())
		if (!mark_.isZero(i))m.set(i, scale * m1(i) * m2(i));
		else m.set(i, 0);
	dst = m;
}
void lzh::divide(InputArray src1, InputArray src2, OutputArray dst, InputArray mask, mat_t scale, int32 dtype)
{
	Array m1(src1);
	Array m2(src2);
	Array mark_(mask);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (m1.empty() || m1.size3() == m2.size3()), THROW_INFO(ERR_INFO_SIZE));
	LZH_ACCESS(if (mark_.empty() || m2.size() == mark_.size()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m2.size3(), dtype < 0 ? m2.depth() : dtype, m2.format());
	if (mark_.empty() && !m1.empty())
		FOR_RANGE(i, 0, m.len())m.set(i, saveDiv(scale * m1(i), m2(i), 0));
	else if (mark_.empty() && m1.empty())
		FOR_RANGE(i, 0, m.len())m.set(i, saveDiv(scale, m2(i), 0));
	else if (!mark_.empty() && !m1.empty()) {
		FOR_RANGE(i, 0, m.len()) {
			if (!mark_.isZero(i))m.set(i, saveDiv(scale * m1(i), m2(i), 0));
			else m.set(i, 0);
		}
	}
	else {
		FOR_RANGE(i, 0, m.len()) {
			if (!mark_.isZero(i))m.set(i, saveDiv(scale, m2(i), 0));
			else m.set(i, 0);
		}
	}
	dst = m;
}
void lzh::divide(mat_t scale, InputArray src2, OutputArray dst, InputArray mask, int32 dtype)
{
	Array m2(src2);
	Array mark_(mask);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (mark_.empty() || m2.size() == mark_.size()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m2.size3(), dtype < 0 ? m2.depth() : dtype, m2.format());
	if (mark_.empty())
		FOR_RANGE(i, 0, m.len())m.set(i, saveDiv(scale, m2(i), 0));
	else {
		FOR_RANGE(i, 0, m.len()) {
			if (!mark_.isZero(i))m.set(i, saveDiv(scale, m2(i), 0));
			else m.set(i, 0);
		}
	}
	dst = m;
}
void lzh::scaleAdd(InputArray src1, mat_t alpha, InputArray src2, OutputArray dst)
{
	Array m1(src1);
	Array m2(src2);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (m1.size3() == m2.size3()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m1.size3(), m1.depth(), m1.format());
	FOR_RANGE(i, 0, m.len())
		m.set(i, m1(i)* alpha + m2(i));
	dst = m;
}
void lzh::addWeighted(InputArray src1, mat_t alpha, InputArray src2, mat_t beta, mat_t gamma, OutputArray dst, int32 dtype)
{
	Array m1(src1);
	Array m2(src2);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (m1.size3() == m2.size3()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m1.size3(), dtype < 0 ? m1.depth() : dtype, m1.format());
	FOR_RANGE(i, 0, m.len())
		m.set(i, alpha * m1(i) + beta * m2(i) + gamma);
	dst = m;
}
void lzh::convertScaleAbs(InputArray src, OutputArray dst, mat_t alpha, mat_t beta)
{
	Array m1(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
#endif
	Array m(m1.size3(), m1.depth(), m1.format());
	FOR_RANGE(i, 0, m.len())
		m.set(i, alpha * m1(i) + beta);
	dst = m.abs().convert(MAT_8U);
}
void lzh::LUT(InputArray src, InputArray lut, OutputArray dst)
{
	Array m1(src);
	Array tab(lut);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	LZH_ACCESS(if (m1.depth() == tab.depth() && tab.depth() == MAT_8U), THROW_INFO(ERR_INFO_TYPE));
	LZH_ACCESS(if (m1.channels() == tab.channels() || tab.channels() == 1), THROW_INFO(ERR_INFO_CHANNEL));
#endif
	Array m(m1.size3(), MAT_8U, m1.format());
	if(tab.channels() == 1)
		FOR_RANGE(i, 0, m.len())
			m.set(i, tab.at<uint8>(m1.at<uint8>(i)));
	else {
		FOR_RANGE(k, 0, m.channels()) {
			Array table = tab.channel(k);
			FOR_RANGE(i, 0, m.rows)FOR_RANGE(j, 0, m.cols)
				m.set(i, j, k, table.at<uint8>(m1.at<uint8>(i, j, k)));
		}
	}
	dst = m;
}
int lzh::countNonZero(InputArray src)
{
	Array m(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif
	int32 count = 0;
	FOR_RANGE(i, 0, m.len())
		if (!m.isZero(i))count += 1;
	return count;
}
void lzh::findNonZero(InputArray src, OutputArray idx)
{
	Array m(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	LZH_ACCESS(if (m.channels() == 1), THROW_INFO(ERR_INFO_CHANNEL));
#endif
	vec_32s index;
	FOR_RANGE(i, 0, m.len())
		if (!m.isZero(i))
			index.push_back(i);
	idx = index;
}
void lzh::absdiff(InputArray src1, InputArray src2, OutputArray dst)
{
	Array m1(src1);
	Array m2(src2);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m1);
	CHECK_MATRIX_(m2);
	LZH_ACCESS(if (m1.size3() == m2.size3()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Array m(m1.size3(), m1.depth(), m1.format());
	FOR_RANGE(i, 0, m.len())
		m.set(i, abs(m1(i) - m2(i)));
	dst = m;
}
void lzh::meanStdDev(InputArray src_, mat_t & mean, mat_t & stddev)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	mean = src.mean();
	lzh::mat_t d = 0;
	lzh::mat_t n = _T(src.len());
	FOR_RANGE(i, 0, src.len())
	{
		d += std::pow(src(i) - mean, 2.0f);
	}
	stddev = sqrt(d / n);
}
void lzh::meanStdDev(InputArray src_, OutputArray mean, OutputArray stddev, InputArray mask)
{
	Array src(src_);
	Array mark_(mask);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (mark_.empty() || src.size() == mark_.size()), THROW_INFO(ERR_INFO_SIZE));
#endif
	Mat mean_ = Mat::zeros(src.channels());
	Mat stddev_(src.channels(), 1);
	FOR_RANGE(k, 0, src.channels()){
		lzh::mat_t d = 0;
		lzh::mat_t n = 0;
		FOR_RANGE(i, 0, src.rows)FOR_RANGE(j, 0, src.cols) {
			if (mark_.empty()) {
				mean_(k) += src(i, j, k);
				n += 1;
			}
			else {
				if (!mark_.isZero(mark_.toOneIndex(i, j))) {
					mean_(k) += src(i, j, k);
					n += 1;
				}
			}
		}
		mean_(k) /= n;
		FOR_RANGE(i, 0, src.rows) {
			FOR_RANGE(j, 0, src.cols)
			{
				if (mark_.empty())
					stddev_(k) += std::pow(src(i, j, k) - mean_(k), 2.0f);
				else {
					if (!mark_.isZero(mark_.toOneIndex(i, j)))
						stddev_(k) += std::pow(src(i, j, k) - mean_(k), 2.0f);
				}
			}
		}
		stddev_(k) = sqrt(stddev_(k) / n);
	}
	mean = mean_;
	stddev = stddev_;
}
void lzh::brc(InputOutputArray src)
{
	Mat y = src.getMat();
	bool dir = y.cols() > y.rows();
	int32 l = dir ? y.cols() : y.rows();
	int32 k;
	mat_t* p = y.data();
	for (int32 i = 1, j = l / 2; i < l - 1; i++)
	{
		if (i < j) {
			mat_t* vi = p + (dir ? i * y.step()[1] : i * y.step().size());
			mat_t* vj = p + (dir ? j * y.step()[1] : j * y.step().size());
			swap(vi[0], vj[0]);
			swap(vi[1], vj[1]);
		}
		// i<j保证只交换一次
		k = l / 2;
		while (j >= k) // 由最高位检索，遇1变0，遇0变1，跳出
		{
			j -= k;
			k >>= 1;
		}
		if (j < k)	j += k;
	}
}
void lzh::phase(InputArray x, InputArray y, OutputArray angle, bool angleInDegrees)
{
	Array dst;
	arcTan2(x, y, dst);
	angleInDegrees ? Radian2Angle(dst, angle) : angle = dst;
}
void lzh::magnitude(InputArray x, InputArray y, OutputArray magnitude)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(x);
	CHECK_MATRIX_(y);
#endif //LZH_DEBUG
	Operator(x, y, magnitude, FunOp::EuclideanDistance_2D);
}
void lzh::mulSpectrums(InputArray x, InputArray y, OutputArray c, bool conjB)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(x);
	CHECK_MATRIX_(y);
#endif //LZH_DEBUG
	conjB ? Operator(x, y, c, FunOp::inv_mul) : Operator(x, y, c, FunOp::mul);
}
void lzh::Max(Val a, InputArray m, OutputArray dst)
{
	Array b = m.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(b);
#endif //LZH_DEBUG
	Array mark(b.size3(), b.depth(), b.colorGamut());
	for (int32 ind = 0; ind < b.len(); ind++)
		mark.set(ind, a > b(ind) ? a.v : b(ind));
	dst.set(mark);
}
void lzh::Max(InputArray m1, InputArray m2, OutputArray dst)
{
	Array a = m1.getArray();
	Array b = m2.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(b);
	if (a.rows != b.rows || a.cols != b.cols || a.channels() != b.channels())
		THROW_INFO(ERR_INFO_SIZE);
#endif //LZH_DEBUG
	Array mark(b.size3(), b.depth(), b.colorGamut());
	for (int32 ind = 0; ind < b.len(); ind++)
		mark.set(ind, a(ind) > b(ind) ? a(ind) : b(ind));
	dst.set(mark);
}
void lzh::Min(Val a, InputArray m, OutputArray dst)
{
	Array b = m.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(b);
#endif //LZH_DEBUG
	Array mark(b.size3(), b.depth(), b.colorGamut());
	for (int32 ind = 0; ind < b.len(); ind++)
		mark.set(ind, a < b(ind) ? a.v : b(ind));
	dst.set(mark);
}
void lzh::Min(InputArray m1, InputArray m2, OutputArray dst)
{
	Array a = m1.getArray();
	Array b = m2.getArray();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(b);
	if (a.rows != b.rows || a.cols != b.cols || a.channels() != b.channels())
		THROW_INFO(ERR_INFO_SIZE);
#endif //LZH_DEBUG
	Array mark(b.size3(), b.depth(), b.colorGamut());
	for (int32 ind = 0; ind < b.len(); ind++)
		mark.set(ind, a(ind) < b(ind) ? a(ind) : b(ind));
	dst.set(mark);
}


const Array lzh::operator+(const Array& m, Val val)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	Array mark(m.size3(), m.depth(), m.colorGamut());
	for (int32 i = 0; i < m.len(); i++)
		mark.set(i, m.get_(i) + val);
	return mark;
}
const Array lzh::operator+(const Array& m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	//CHECK_TYPE_(m.depth(), a.depth());
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		return m.get_(0) + a;
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		return m + a.get_(0);
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == m.channels()) {
		Array mat(m.size3(), m.depth());
		for (int32 i = 0; i < m.rows; i++)
			for (int32 j = 0; j < m.cols; j++)
				for (int32 z = 0; z < m.channels(); z++)
					mat.set(i, j, z, m.get_(i, j, z) + a.get_(z));
		return mat;
	}
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Array mark(m.size3(), m.depth(), m.colorGamut());
	for (int32 i = 0; i < m.len(); i++)
		mark.set(i, m.get_(i) + a.get_(i));
	return mark;
}
const Array lzh::operator + (Val val, const Array& mat)
{
	return mat + val;
}

const Array lzh::operator-(const Array& m)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	Array mark(m.size3(), m.depth(), m.colorGamut());
	for (int32 i = 0; i < m.len(); i++)
		mark.set(i, -m.get_(i));
	return mark;
}
const Array lzh::operator-(const Array& m, Val val)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	Array mark(m.size3(), m.depth(), m.colorGamut());
	for (int32 i = 0; i < m.len(); i++)
		mark.set(i, m.get_(i) - val);
	return mark;
}
const Array lzh::operator-(const Array& m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	//CHECK_TYPE_(m.depth(), a.depth());
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		return m.get_(0) - a;
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		return m - a.get_(0);
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == m.channels()) {
		Array mat(m.size3(), m.depth(), m.colorGamut());
		for (int32 i = 0; i < m.rows; i++)
			for (int32 j = 0; j < m.cols; j++)
				for (int32 z = 0; z < m.channels(); z++)
					mat.set(i, j, z, m.get_(i, j, z) - a.get_(z));
		return mat;
	}
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Array mark(m.size3(), m.depth(), m.colorGamut());
	for (int32 i = 0; i < m.len(); i++)
		mark.set(i, m.get_(i) - a.get_(i));
	return mark;
}
const Array lzh::operator-(Val val, const Array& mat)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(mat);
#endif // LZH_DEBUG
	Array mark(mat.size3(), mat.depth(), mat.colorGamut());
	for (int32 i = 0; i < mat.len(); i++)
		mark.set(i, val - mat.get_(i));
	return mark;
}

const Array lzh::operator*(const Array& m, Val val)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	Array mark(m.size3(), m.depth(), m.colorGamut());
	for (int32 i = 0; i < m.len(); i++)
		mark.set(i, m(i) * val);
	return mark;
}
const Array lzh::operator*(const Array& m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
	//CHECK_TYPE_(m.depth(), a.depth());
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		return m(0) * a;
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		return m * a(0);
	}
#ifdef LZH_DEBUG
	if (m.cols != a.rows) THROW_INFO(ERR_INFO_MULT);
	if (m.channels() != a.channels()) THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Array mark(a.cols, m.rows, m.channels(), m.depth());
	for (int32 z = 0; z < m.channels(); z++)
		for (int32 i = 0; i < m.rows; i++)
			for (int32 j = 0; j < a.cols; j++) {
				mat_t temp = 0;
				for (int32 d = 0; d < m.cols; d++)
					temp = temp + m(i, d, z) * a(d, j, z);
				mark.set(i, j, z, temp);
			}
	return mark;
}
const Array lzh::operator*(Val val, const Array& mat)
{
	return mat * val;
}

const Array lzh::operator/(const Array& m, Val val)
{
	return m * (1.0 / val);
}
const Array lzh::operator/(const Array& m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	//CHECK_TYPE_(m.depth(), a.depth());
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		return m.get_(0) / a;
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		return m / a.get_(0);
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == m.channels()) {
		Array mat(m.size3(), m.depth());
		for (int32 i = 0; i < m.rows; i++)
			for (int32 j = 0; j < m.cols; j++)
				for (int32 z = 0; z < m.channels(); z++)
					mat.set(i, j, z, m.get_(i, j, z) / a.get_(z));
		return mat;
	}
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Array mark(m.size3(), m.depth(), m.colorGamut());
	for (int32 i = 0; i < m.len(); i++)
		mark.set(i, m.get_(i) / a.get_(i));
	return mark;
}
const Array lzh::operator/(Val val, const Array& mat)
{
	Mat m;
	lzh::Div(mat, m, val, LEFT);
	return m.toArray(mat.depth());
}

const Array lzh::operator^(const Array& m, Val power)
{
	return m.pow(power);
}

const Array lzh::operator~(const Array& m)
{
	return m.BitNot();
}

const Array lzh::operator!(const Array& m)
{
	return m.BitNot();
}
const Array lzh::operator&(const Array& m, Val val)
{
	return m.BitAnd(val);
}
const Array lzh::operator&(const Array& m, const Array& a)
{
	return m.BitAnd(a);
}
const Array lzh::operator&(Val val, const Array& m)
{
	return m & val;
}

const Array lzh::operator|(const Array& m, Val val)
{
	return m.BitOr(val);
}
const Array lzh::operator|(const Array& m, const Array& a)
{
	return m.BitOr(a);
}
const Array lzh::operator|(Val val, const Array& m)
{
	return m | val;
}

void lzh::operator+=(Array m, Val val)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	for (int32 i = 0; i < m.len(); i++)
		m.add(m.toidx(i), val);
}
void lzh::operator+=(Array m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		m.add(0, a.First());
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		m += a.First();
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows; i++) {
				for (int32 j = 0; j < m.cols; j++) {
					m.add(m.toidx(i, j, z), a(z));
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m.add(m.toidx(i), a.get_(i));
	}
}

void lzh::operator-=(Array m, Val val)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	for (int32 i = 0; i < m.len(); i++)
		m.reduce(m.toidx(i), val);
}
void lzh::operator-=(Array m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		m.reduce(0, a.First());
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		m -= a.First();
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows; i++) {
				for (int32 j = 0; j < m.cols; j++) {
					m.reduce(m.toidx(i, j, z), a(z));
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m.reduce(m.toidx(i), a.get_(i));
	}
}

void lzh::operator*=(Array m, Val val)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	for (int32 i = 0; i < m.len(); i++)
		m.mul(m.toidx(i), val);
}
void lzh::operator*=(Array m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		m.mul(0, a.First());
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		m *= a.First();
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows; i++) {
				for (int32 j = 0; j < m.cols; j++) {
					m.mul(m.toidx(i, j, z), a(z));
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m.mul(m.toidx(i), a.get_(i));
	}
}

void lzh::operator/=(Array m, Val val)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	for (int32 i = 0; i < m.len(); i++)
		m.div(m.toidx(i), val);
}
void lzh::operator/=(Array m, const Array& a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows == 1 && m.cols == 1 && m.channels() == 1) {
		m.div(0, a.First());
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == 1) {
		m /= a.First();
	}
	else if (a.rows == 1 && a.cols == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows; i++) {
				for (int32 j = 0; j < m.cols; j++) {
					m.div(m.toidx(i, j, z), a(z));
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m.div(m.toidx(i), a.get_(i));
	}
}

void lzh::operator&=(Array m, Val val)
{
	FOR_RANGE(i, 0, m.len())m.BitAnd(i, val);
}
void lzh::operator&=(Array m, const Array& a)
{
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	FOR_RANGE(i, 0, m.len())m.BitAnd(i, a(i));
}

void lzh::operator|=(Array m, Val val)
{
	FOR_RANGE(i, 0, m.len())m.BitOr(i, val);
}
void lzh::operator|=(Array m, const Array& a)
{
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	FOR_RANGE(i, 0, m.len())m.BitOr(i, a(i));
}


OMat lzh::operator+(IMat m, Val val)
{
	mat_t v = val;
	if (val == 0)return m;
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	Mat mark(m.size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < m.len(); i++)
		p[i] = m(i) + v;
	return mark;
}
OMat lzh::operator+(IMat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		return m.first() + a;
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		return m + a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == m.channels()) {
		Mat mat(m.size3());
		mat_t* p = mat.data();
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows(); i++) {
				for (int32 j = 0; j < m.cols(); j++) {
					*p = m(i, j, z) + a(z);
					++p;
				}
			}
		}
		return mat;
	}
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Mat mark(m.size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < m.len(); i++)
		p[i] = m(i) + a(i);
	return mark;
}
OMat lzh::operator+(Val val, IMat m)
{
	return m + val;
}

OMat lzh::operator-(IMat m)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	Mat mark(m.size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < m.len(); i++)
		p[i] = -m(i);
	return mark;
}
OMat lzh::operator-(IMat m, Val val)
{
	mat_t v = val;
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	if (v == 0)return m;
	Mat mark(m.size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < m.len(); i++)
		p[i] = m(i) - v;
	return mark;
}
OMat lzh::operator-(IMat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		return m.first() - a;
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		return m - a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == m.channels()) {
		Mat mat(m.size3());
		mat_t* p = mat.data();
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows(); i++) {
				for (int32 j = 0; j < m.cols(); j++) {
					*p = m(i, j, z) - a(z);
					++p;
				}
			}
		}
		return mat;
	}
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Mat mark(m.size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < m.len(); i++)
		p[i] = m(i) - a(i);
	return mark;
}
OMat lzh::operator-(Val val, IMat m)
{
	return val + (-m);
}

OMat lzh::operator*(IMat m, Val val)
{
	mat_t v = val;
	if (v == 0)return Mat::zeros(m.size3());
	if (v == 1)return m;
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif // LZH_DEBUG
	Mat mark(m.size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < m.len(); i++)
		p[i] = m(i) * v;
	return mark;
}
OMat lzh::operator*(IMat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		return m.first() * a;
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		return m * a.first();
	}
#ifdef LZH_DEBUG
	if (m.cols() != a.rows()) THROW_INFO(ERR_INFO_MULT);
	if (m.channels() != a.channels()) THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Mat mark(a.cols(), m.rows(), m.channels());
	mat_t* ptr = mark.data();
	for (int32 z = 0; z < m.channels(); z++) {
		mat_t* p = ptr + z;
		for (int32 i = 0; i < m.rows(); i++)
			for (int32 j = 0; j < a.cols(); j++) {
				*p = 0;
				for (int32 d = 0; d < m.cols(); d++)
					* p += m(i, d, z) * a(d, j, z);
				p += m.channels();
			}
	}
	return mark;
}
OMat lzh::operator*(Val val, IMat m)
{
	return m * val;
}

OMat lzh::operator/(IMat m, Val val)
{
	mat_t v = val;
	if (v == 0)return Mat::zeros(m.size3());
	if (v == 1)return m;
	return m * (1.0f / v);
}
OMat lzh::operator/(IMat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		return m.first() / a;
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		return m / a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == m.channels()) {
		Mat mat(m.size3());
		mat_t* p = mat.data();
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows(); i++) {
				for (int32 j = 0; j < m.cols(); j++) {
					*p = m(i, j, z) / a(z);
					++p;
				}
			}
		}
		return mat;
	}
#ifdef LZH_DEBUG
	if (m.size3() != a.size3())
		THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
	Mat mark(m.size3());
	mat_t* p = mark.data();
	for (int32 i = 0; i < m.len(); i++)
		p[i] = m(i) / a(i);
	return mark;
}
OMat lzh::operator/(Val val, IMat m)
{
	Mat dst;
	Div(m, dst, val, LEFT);
	return dst;
}

OMat lzh::operator^(IMat m, Val power)
{
	return m.pow(power);
}

void lzh::operator+=(Mat m, Val val)
{
	if (val != 0) {
#ifdef LZH_DEBUG
		CHECK_MATRIX_(m);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) += val;
	}
}
void lzh::operator+=(Mat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		m.first() += a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		m += a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows(); i++) {
				for (int32 j = 0; j < m.cols(); j++) {
					m(i, j, z) += a(z);
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) += a(i);
	}
}

void lzh::operator-=(Mat m, Val val)
{
	if (val != 0) {
#ifdef LZH_DEBUG
		CHECK_MATRIX_(m);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) -= val;
	}
}
void lzh::operator-=(Mat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		m.first() -= a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		m -= a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows(); i++) {
				for (int32 j = 0; j < m.cols(); j++) {
					m(i, j, z) -= a(z);
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) -= a(i);
	}
}

void lzh::operator*=(Mat m, Val val)
{
	if (val != 0) {
#ifdef LZH_DEBUG
		CHECK_MATRIX_(m);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) *= val;
	}
}
void lzh::operator*=(Mat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		m.first() *= a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		m *= a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows(); i++) {
				for (int32 j = 0; j < m.cols(); j++) {
					m(i, j, z) *= a(z);
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) *= a(i);
	}
}

void lzh::operator/=(Mat m, Val val)
{
	if (val != 0) {
#ifdef LZH_DEBUG
		CHECK_MATRIX_(m);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) /= val;
	}
}
void lzh::operator/=(Mat m, IMat a)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
	CHECK_MATRIX_(a);
#endif // LZH_DEBUG
	if (m.rows() == 1 && m.cols() == 1 && m.channels() == 1) {
		m.first() /= a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == 1) {
		m /= a.first();
	}
	else if (a.rows() == 1 && a.cols() == 1 && a.channels() == m.channels()) {
		for (int32 z = 0; z < m.channels(); z++) {
			for (int32 i = 0; i < m.rows(); i++) {
				for (int32 j = 0; j < m.cols(); j++) {
					m(i, j, z) /= a(z);
				}
			}
		}
	}
	else {
#ifdef LZH_DEBUG
		if (m.size3() != a.size3())
			THROW_INFO(ERR_INFO_SIZE);
#endif // LZH_DEBUG
		for (int32 i = 0; i < m.len(); i++)
			m(i) /= a(i);
	}
}
