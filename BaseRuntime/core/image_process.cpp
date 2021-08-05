#define LZH_EXPORTS
#include "image_process.h"
#include "operator.h"
#include "algorithm.h"
#include "tools.h"
#include "image.h"
#include <thread>
#include <algorithm>

using namespace lzh;
using namespace std;

lzh::Moments::Moments() :
	m00(0), m10(0), m01(0), m20(0), m11(0), m02(0), m30(0), m21(0), m12(0), m03(0),
	mu20(0), mu11(0), mu02(0), mu30(0), mu21(0), mu12(0), mu03(0),
	nu20(0), nu11(0), nu02(0), nu30(0), nu21(0), nu12(0), nu03(0)
{
}
lzh::Moments::Moments(float64 m00, float64 m10, float64 m01, float64 m20, float64 m11, float64 m02, float64 m30, float64 m21, float64 m12, float64 m03) : 
	m00(m00), m10(m10), m01(m01), m20(m20), m11(m11), m02(m02), m30(m30), m21(m21), m12(m12), m03(m03), 
	mu20(0), mu11(0), mu02(0), mu30(0), mu21(0), mu12(0), mu03(0),
	nu20(0), nu11(0), nu02(0), nu30(0), nu21(0), nu12(0), nu03(0)
{
}

lzh::Point2d lzh::Moments::center() const
{
	return Point2d(m10 / m00, m01 / m00);
}

lzh::float64 lzh::Moments::theta() const
{
	Point2d center = this->center();//此为重心
	   //计算方向
	float64 a = m20 / m00 - center.x*center.x;
	float64 b = m11 / m00 - center.x*center.y;
	float64 c = m02 / m00 - center.y*center.y;
	float64 theta = fastAtan2(2 * b, (a - c)) / 2;//此为形状的方向
	return theta;
}

mat_t FilterMask::operator()(int32 x, int32 y) const
{
	return f(x, y);
}
mat_t ButterworthSpectrum::f(int32 i, int32 j) const
{
	mat_t radius = pow(i, 2) + pow(j, 2);
	mat_t r = exp(-n * radius / d);
	if (radius < 0)
		return upper;
	else
		return (upper - lower) * (1 - r) + lower;
}
mat_t lzh::GaussianSpectrum::f(int32 x, int32 y) const
{
	return generateGaussian(x, y, sigma);
}
/****************************************************************************
图像运算
*****************************************************************************/
lzh::Size3 lzh::CalSize(Size3 src, Size3 kern, Point anchor, Size strides, int32& top, int32& bottom, int32& left, int32& right)
{
	int32 kern_row = kern.h;
	int32 kern_col = kern.w;
	if (anchor == Point(-1, -1)) {
		anchor.x = kern_row / 2;
		anchor.y = kern_col / 2;
	}
	top = anchor.x;
	bottom = kern_row - anchor.x - 1;
	left = anchor.y;
	right = kern_col - anchor.y - 1;
	return Size3(Round(_T(src.w - kern_col + 1) / _T(strides.w)), Round(_T(src.h - kern_row + 1) / _T(strides.h)), kern.c / src.c);
}
lzh::Size3 lzh::CalSize(Size3 src, Size3 kern, Point anchor, Size strides)
{
	int32 kern_row = kern.h;
	int32 kern_col = kern.w;
	if (anchor == Point(-1, -1)) {
		anchor.x = kern_row / 2;
		anchor.y = kern_col / 2;
	}
	return Size3(int32(_T(src.w - kern_col + 1) / _T(strides.w) + 0.5f), int32(_T(src.h - kern_row + 1) / _T(strides.h) + 0.5f), kern.c / src.c);
}
lzh::Size3 lzh::CalSize(Size3 src, Size kern, Point anchor, Size strides)
{
	int32 kern_row = kern.h;
	int32 kern_col = kern.w;
	if (anchor == Point(-1, -1)) {
		anchor.x = kern_row / 2;
		anchor.y = kern_col / 2;
	}
	return Size3(int32(_T(src.w - kern_col + 1) / _T(strides.w) + 0.5f), int32(_T(src.h - kern_row + 1) / _T(strides.h) + 0.5f), src.c);
}
OMat lzh::GaussianKernel(Size ksize, Val sigma, bool isnorm)
{
	Mat kernel(ksize);
	mat_t* k = kernel.data();
	FOR_RANGE(i, -ksize.h / 2, ksize.h / 2 + ksize.h % 2) {
		FOR_RANGE(j, -ksize.w / 2, ksize.w / 2 + ksize.w % 2) {
			*k = generateGaussian(_T(i), _T(j), sigma);
			k++;
		}
	}
	if (isnorm)normalize(kernel, kernel, 1, 0, NORM_SUM);
	return kernel;
}
OMat lzh::LoGKernel(Size ksize, Val sigma, bool isnorm)
{
	Mat kernel(ksize);
	mat_t* k = kernel.data();
	FOR_RANGE(i, -ksize.h / 2, ksize.h / 2 + ksize.h % 2) {
		FOR_RANGE(j, -ksize.w / 2, ksize.w / 2 + ksize.w % 2) {
			*k = generateLoG(_T(i), _T(j), sigma);
			k++;
		}
	}
	if (isnorm)normalize(kernel, kernel, 0, 1, NORM_ZSCORE);
	return kernel;
}
OMat lzh::DoGKernel(Size ksize, Val sigma1, Val sigma2, bool isnorm)
{
	Mat kernel(ksize);
	mat_t* k = kernel.data();
	FOR_RANGE(i, -ksize.h / 2, ksize.h / 2 + ksize.h % 2) {
		FOR_RANGE(j, -ksize.w / 2, ksize.w / 2 + ksize.w % 2) {
			*k = generateDoG(_T(i), _T(j), sigma1, sigma2);
			k++;
		}
	}
	if (isnorm) {
		if (sigma1 != sigma2)
			normalize(kernel, kernel, 0, 1, NORM_ZSCORE);
	}
	return kernel;
}
OMat lzh::SharpenKernel(SharpenScale level)
{
	switch (level)
	{
	case lzh::SHARPEN_LEVEL1:
		return (Mat_(3, 3) <<
			0, -1, 0,
			-1, 5, -1,
			0, -1, 0);
	case lzh::SHARPEN_LEVEL2:
		return (Mat_(3, 3) <<
			-1, -1, -1,
			-1, 9, -1,
			-1, -1, -1);
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
}
void lzh::ConvKernel(InputArray src, OutputArray dst, int32 depth, OMat(*CreatKernel)(int32), int32 angle)
{
	vec_mat ms;
	Mat m;
	if (MAT_SWITCH(angle, ANGLE0)) {
		Filter2D(src, m, CreatKernel(ANGLE0), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}if (MAT_SWITCH(angle, ANGLE45)) {
		Filter2D(src, m, CreatKernel(ANGLE45), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}if (MAT_SWITCH(angle, ANGLE90)) {
		Filter2D(src, m, CreatKernel(ANGLE90), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}if (MAT_SWITCH(angle, ANGLE135)) {
		Filter2D(src, m, CreatKernel(ANGLE135), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}if (MAT_SWITCH(angle, ANGLE180)) {
		Filter2D(src, m, CreatKernel(ANGLE180), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}if (MAT_SWITCH(angle, ANGLE225)) {
		Filter2D(src, m, CreatKernel(ANGLE225), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}if (MAT_SWITCH(angle, ANGLE270)) {
		Filter2D(src, m, CreatKernel(ANGLE270), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}if (MAT_SWITCH(angle, ANGLE315)) {
		Filter2D(src, m, CreatKernel(ANGLE315), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
		ms.push_back(m);
	}
	if (ms.empty())return;
	if (ms.size() == 1) { dst.set(ms[0]); return; }
	m = ms[0].pow(2);
	FOR_RANGE(i, 1, ms.size())
		m += ms[i].pow(2);
	dst.set(m.sqrt());
}
void lzh::Roberts(InputArray src, OutputArray dst, int32 depth, int32 angle)
{
	ConvKernel(src, dst, depth, RobertsKernel, angle);
}
void lzh::Prewitt(InputArray src, OutputArray dst, int32 depth, int32 angle)
{
	ConvKernel(src, dst, depth, PrewittKernel, angle);
}
void lzh::Sobel(InputArray src, OutputArray dst, int32 depth, int32 angle)
{
	ConvKernel(src, dst, depth, SobelKernel, angle);
}
void lzh::Sharpen(InputArray src, OutputArray dst, int32 depth, SharpenScale level)
{
	Filter2D(src, dst, SharpenKernel(level), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
}
void lzh::Lapacian(InputArray src, OutputArray dst, int32 depth, NeighborhoodType laplacetype)
{
	Filter2D(src, dst, LapacianKernel(laplacetype), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
}
void lzh::LoG(InputArray src, OutputArray dst, int32 depth, Size ksize, Val sigma)
{
	Filter2D(src, dst, LoGKernel(ksize, sigma), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
}
void lzh::DoG(InputArray src, OutputArray dst, int32 depth, Size ksize, Val sigma1, Val sigma2)
{
	Filter2D(src, dst, DoGKernel(ksize, sigma1, sigma2), depth, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
}
void lzh::Roberts(InputArray src_, OutputArray dst, int32 depth)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Mat kernx = RobertsKernel(ANGLE0);
	Mat kerny = RobertsKernel(ANGLE90);

	Mat x, y;
	conv2(src, y, kerny, depth, CONVOLUTION_SAME);
	conv2(src, x, kernx, depth, CONVOLUTION_SAME);
	dst.set((x.pow(2) + y.pow(2)).sqrt());
}
void lzh::Prewitt(InputArray src_, OutputArray dst, int32 depth)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Mat kernx = PrewittKernel(ANGLE0);
	Mat kerny = PrewittKernel(ANGLE90);

	Mat x, y;
	conv2(src, y, kerny, depth, CONVOLUTION_SAME);
	conv2(src, x, kernx, depth, CONVOLUTION_SAME);
	dst.set((x.pow(2) + y.pow(2)).sqrt());
}


void lzh::HomomorphicFilter(InputArray src_, OutputArray dst, Val D0, Val n, Val rL, Val rH)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Array img;
	Array imgHls;
	vec_array vHls;

	if (src.channels() != 1)
	{
		cvtColor::ConverTo(src, imgHls, ColorGamutHSV);
		split(imgHls, vHls);
		vHls[2].copyTo(img);
	}
	else
		src.copyTo(img);

	//DFT
	//cout<<"DFT "<<endl;
	Array img_complex;
	/*Array fpower = */FourierTransform(img, img_complex);
	dftShift(img_complex);
	//dftShift(fpower);

	Array filter_complex;
	filter_complex = ButterworthFilter(img_complex.size(), D0, n, rH, rL);
	//do mulSpectrums on the full dft
	mulSpectrums(img_complex, filter_complex, filter_complex);

	dftShift(filter_complex);

	//IDFT
	Array result;
	InvFourierTransform(filter_complex, result);

	if (src.channels() == 3)
	{
		vHls[2] = result(Rect(0, 0, src.cols, src.rows));
		merge(vHls, imgHls);
		cvtColor::HSV2RGB(imgHls, dst);
	}
	else {
		//resize(result, dst, src.size(), InterpolationMothed::INTER_LINEAR);
		dst = result(Rect(0, 0, src.cols, src.rows)).clone();
	}
}
void lzh::GaussianFilter(InputArray src_, OutputArray dst, Size ksize, Val sigma, BorderType borderType)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG

}
void lzh::unevenLightCompensate(InputArray image_, OutputArray dst, Size blockSize)
{
	Array image(image_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(image);
#endif //LZH_DEBUG
	cvtColor::ConverTo(image, image, ColorGamutGray);
	mat_t average = image.mean();
	int32 rows_new = int32(ceil(mat_t(image.rows) / _T(blockSize.h)));
	int32 cols_new = int32(ceil(mat_t(image.cols) / _T(blockSize.w)));
	Array blockImage;
	blockImage = Array::zeros(cols_new, rows_new, image.depth());
	for (int32 i = 0; i < rows_new; i++)
	{
		for (int32 j = 0; j < cols_new; j++)
		{
			int32 rowmin = i * blockSize.h;
			int32 rowmax = (i + 1) * blockSize.h;
			if (rowmax > image.rows) rowmax = image.rows;
			int32 colmin = j * blockSize.w;
			int32 colmax = (j + 1) * blockSize.w;
			if (colmax > image.cols) colmax = image.cols;
			Mat imageROI = image(Range(rowmin, rowmax), Range(colmin, colmax));
			mat_t temaver = imageROI.mean();
			blockImage.set(i, j, temaver);
		}
	}
	blockImage = blockImage - average;
	Array blockImage2;
	resize(blockImage, blockImage2, image.size(), INTER_LINEAR);
	dst.set(image - blockImage2);
}
void lzh::GaussianBlur(InputArray src, OutputArray dst, Size ksize, Val sigma, BorderType borderType)
{
	Filter2D(src, dst, GaussianKernel(ksize, sigma), src.depth(), Point(-1, -1), Size(1, 1), true, borderType);
}
void lzh::medianBlur(InputArray src_, OutputArray dst, int32 ksize)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src_);
	if (ksize % 2 == 0)THROW_INFO(ERR_INFO_UNLESS);
#endif //LZH_DEBUG
	medianBlur(src_, dst, Size(ksize, ksize));
}
void lzh::medianBlur(InputArray src, OutputArray dst, Size ksize)
{
	auto Median = [](const Array& img, const Array& kern, int32 h, int32 w, int32 k, int32 c) {
		int32 idx = 0; vec_m vec(kern.len());
		for (int32 i = 0; i < kern.rows; ++i)
			for (int32 j = 0; j < kern.cols; ++j)
				vec[idx++] = img(h + i, w + j, k);
		return median(vec); };
	filter(src, dst, Mat(ksize), src.depth(), Median, Point(-1, -1), Size(1, 1), true, BORDER_REPLICATE);
}
void lzh::blur(InputArray src, OutputArray dst, Size kernel, Point anchor, int32 iteration)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	dst = src;
	Mat kern = Mat::value(1.0 / kernel.area(), kernel.w, kernel.h);
	auto Blur = [](const Array& img, const Array& kern, int32 h, int32 w, int32 k, int32 c) {
		int32 idx = 0; vec_m vec(kern.len());
		for (int32 i = 0; i < kern.rows; ++i)
			for (int32 j = 0; j < kern.cols; ++j)
				vec[idx++] = img(h + i, w + j, k);
		return median(vec); };
	while (iteration--)
		filter(dst, dst, kern, src.depth(), Blur, anchor, Size(1, 1), true, BORDER_REPLICATE);
}
void lzh::erode(InputArray src, OutputArray dst, InputArray kern, int32 iteration, Point anchor)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	//int32 kern_row = kernel.rows(), kern_col = kernel.cols();
	//int32 left, right, top, bottom;
	//Size3 size = CalSize(src.size3(), kernel.size3(), anchor, Size(1, 1), top, bottom, left, right);
	//Mat in = src;
	//Mat out(src.size3());
	//mat_t* img = in.data();
	//int32 c1 = in.channels();
	//int32 c = out.channels();
	//int32 offset = out.step().size();
	//while (iteration--) {
	//	for (int32 z = 0; z < in.channels(); z++) {
	//		mat_t* o = out.begin() + z;
	//		for (int32 h = top, x = 0; h < in.rows() - bottom; h++, x++) {
	//			for (int32 w = left, y = 0; w < in.cols() - right; w++, y++) {
	//				int32 v_idx = ((h - anchor.x) * in.s() + w - anchor.y) * in.o() + z;
	//				if (img[v_idx] != 0) {
	//					bool isContain = true;
	//					mat_t* k = kernel.data();
	//					for (int32 i = 0; i < kern_row; ++i) {
	//						for (int32 j = 0; j < kern_col; ++j) {
	//							int32 idx = v_idx + (i * in.s() + j) * in.o();
	//							if (k[j] != 0 && img[idx] == 0) {
	//								isContain = false; break;
	//							}
	//						}
	//						if (!isContain)break;
	//						k += kern_col;
	//					}
	//					o[y * c] = isContain ? img[v_idx] : mat_t(0);
	//				}
	//				else {
	//					o[y * c] = 0;
	//				}
	//			}
	//			o += offset;
	//		}
	//	}
	//	if (iteration == 0)break;
	//	out.setTo(in);
	//}
	auto Erode = [](const Array& src, const Array& kernel, int32 row, int32 col, int32 channel, int32 c) {
		//if (!src.isZero(src.toidx(row, col, channel))) {
			mat_t minv = src(row, col, channel);
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					mat_t v = src(row + i, col + j, channel);
					if (kernel(i, j) != 0 && v < minv) {
						minv = v;
					}
				}
			}
			return minv;
		//}
		//else
		//	return _T(0);
		//return src.range(row, row + kernel.rows, col, col + kernel.cols).channel(channel).min(kernel);
	};
	dst = src;
	while (iteration--)filter(dst, dst, kern, src.depth(), Erode, anchor, Size(1, 1), true, BORDER_REPLICATE);
}
void lzh::dilate(InputArray src, OutputArray dst, InputArray kern, int32 iteration, Point anchor)
{
	//Array src(src_);
	//Mat kernel = kern.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	//int32 kern_row = kernel.rows(), kern_col = kernel.cols();
	//int32 left, right, top, bottom;
	//if (anchor == Point(-1, -1)) {
	//	anchor.x = kern_row / 2;
	//	anchor.y = kern_col / 2;
	//}
	//Size3 size = CalSize(src.size3(), kernel.size3(), anchor, Size(1, 1), top, bottom, left, right);
	//Mat in = src;
	//Mat out(src.size3());
	//mat_t* img = in.data();
	//int32 c1 = in.channels();
	//int32 c = out.channels();
	//int32 offset = out.step().size();
	//while (iteration--) {
	//	for (int32 z = 0; z < in.channels(); z++) {
	//		mat_t* o = out.begin() + z;
	//		for (int32 h = top, x = 0; h < in.rows() - bottom; h++, x++) {
	//			for (int32 w = left, y = 0; w < in.cols() - right; w++, y++) {
	//				bool isContain = false;
	//				mat_t* k = kernel.data();
	//				int32 v_idx = ((h - anchor.x) * in.s() + w - anchor.y) * in.o() + z;
	//				//mat_t value = img[((h - anchor.x)* in.s() + w - anchor.y) * in.o() + z];
	//				for (int32 i = 0; i < kern_row; ++i) {
	//					for (int32 j = 0; j < kern_col; ++j) {
	//						int32 idx = v_idx + (i * in.s() + j) * in.o();
	//						if (k[j] != 0 && img[idx] != 0) {
	//							isContain = true; v_idx = idx; break;
	//						}
	//					}
	//					if (isContain)break;
	//					k += kern_col;
	//				}
	//				o[y * c] = isContain ? img[v_idx] : mat_t(0);
	//			}
	//			o += offset;
	//		}
	//	}
	//	if (iteration == 0)break;
	//	out.setTo(in);
	//}
	//dst.set(out);

	auto Dilate = [](const Array& src, const Array& kernel, int32 row, int32 col, int32 channel, int32 c) {
		mat_t maxv = 0;
		for (int32 i = 0; i < kernel.rows; ++i) {
			for (int32 j = 0; j < kernel.cols; ++j) {
				mat_t v = src(row + i, col + j, channel);
				if (kernel(i, j) != 0 && v > maxv) {
					maxv = v;
				}
			}
		}
		return maxv;
		//return src.range(row, row + kernel.rows, col, col + kernel.cols).channel(channel).max(kernel);
	};
	dst = src;
	while (iteration--)filter(dst, dst, kern, src.depth(), Dilate, anchor, Size(1, 1), true, BORDER_REPLICATE);
}
Array lzh::getStructuringElement(MorphShape morphshape, Size elementsize)
{
	Mat element;
	switch (morphshape)
	{
	case lzh::MORPH_RECT:element = Mat::ones(elementsize); break;
	case lzh::MORPH_CROSS: {
		element = Mat::zeros(elementsize);
		mat_t *m = element.data();
		int32 idx = element.w() / 2;
		FOR_RANGE(i, 0, element.rows())
			m[i * element.w() + idx] = _T(1);
		idx = element.h() / 2 * element.w();
		FOR_RANGE(i, 0, element.cols())
			m[idx + i] = _T(1);
		break;
	}
	case lzh::MORPH_ELLIPSE: {
		vec_p2_32s ps;
		if(elementsize.w == elementsize.h)
			BresenhamCircle(ps, 
				Point(elementsize.w % 2 == 0 ? elementsize.w / 2 - 1 : elementsize.w / 2, elementsize.h % 2 == 0 ? elementsize.h / 2 - 1 : elementsize.h / 2),
				MIN(elementsize.h, elementsize.w) % 2 == 0 ? MIN(elementsize.h, elementsize.w) / 2 - 1 : MIN(elementsize.h, elementsize.w) / 2);
		else
			BresenhamEllipse(ps, 
				Point(elementsize.w % 2 == 0 ? elementsize.w / 2 - 1 : elementsize.w / 2, elementsize.h % 2 == 0 ? elementsize.h / 2 - 1 : elementsize.h / 2),
				elementsize.w / 2, elementsize.h / 2);
		if (ps.empty())return element.toArray();
		element = Mat::zeros(elementsize);
		for (Point& p : ps)if (__RANGE(0, p.x, elementsize.w) && __RANGE(0, p.y, elementsize.h))element(p) = 1;
		floodFill(element, Point(elementsize.w / 2, elementsize.h / 2), Color(1));
		break;
	}
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return element.toArray();
}
void lzh::morphologyEx(InputArray src, OutputArray dst, MorphMethod morphtype, InputArray kernel, int32 iteration, Point anchor)
{
	Array t;
	switch (morphtype)
	{
	case lzh::MORPH_OPEN:
		erode(src, t, kernel, iteration, anchor);
		dilate(t, dst, kernel, iteration, anchor);
		break;
	case lzh::MORPH_CLOSE:
		dilate(src, t, kernel, iteration, anchor);
		erode(t, dst, kernel, iteration, anchor);
		break;
	case lzh::MORPH_GRADIENT: {
		Array a, b;
		dilate(src, a, kernel, iteration, anchor);
		erode(src, b, kernel, iteration, anchor);
		dst.set(a - b);
		break; }
	case lzh::MORPH_TOPHAT: {
		Array a;
		morphologyEx(src, a, MORPH_OPEN, kernel, iteration, anchor);
		dst.set(src.getArray() - a);
		break; }
	case lzh::MORPH_BLACKHAT: {
		Array a;
		morphologyEx(src, a, MORPH_CLOSE, kernel, iteration, anchor);
		dst.set(a - src.getArray());
		break; }
	case lzh::MORPH_ERODE:
		erode(src, dst, kernel, iteration, anchor);
		break;
	case lzh::MORPH_DILATE:
		dilate(src, dst, kernel, iteration, anchor);
		break;
	default:
		THROW_INFO(ERR_INFO_UNLESS);
	}
}
void lzh::reverse(InputArray src_, OutputArray dst, Val v)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	dst.set(-(src - v));
}
void lzh::matchTemplate(InputArray src, InputArray template_, OutputArray dst, MatchMethod method)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (src.channels() == template_.channels()), THROW_INFO(ERR_INFO_SIZE));
#endif //LZH_DEBUG
	Array img(src);
	Array _template(template_);
	img = img.convert(MAT_T) / 255;
	_template = _template.convert(MAT_T) / 255;
	FilterOp f = nullptr;
	switch (method)
	{
	case lzh::TEMPLATE_SQDIFF:
	{
		auto sqdiff = [](const Array& img, const Array& kernel, int32 h, int32 w, int32 c, int32 z) {
			mat_t v = 0;
			for (int32 i = 0; i < kernel.rows; ++i)
				for (int32 j = 0; j < kernel.cols; ++j)
					v += pow(img(h + i, w + j, c) - kernel(i, j, c), 2);
			return v;
		};
		f = sqdiff;
	}
	break;
	case lzh::TEMPLATE_SQDIFF_NORMED:
	{
		auto sqdiff_normed = [](const Array& img, const Array& kernel, int32 h, int32 w, int32 c, int32 z) {
			Vec3m val(0, 0, 0);
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					Vec2m v(img(h + i, w + j, c), kernel(i, j, c));
					val[0] += v[0] * v[0];
					val[1] += v[1] * v[1];
					val[2] += pow(v[0] - v[1], 2);
				}
			}
			return val[2] / sqrt((val[0] * val[1]));
		};
		f = sqdiff_normed;
	}
	break;
	case lzh::TEMPLATE_CCORR: 
	{
		auto ccorr = [](const Array& img, const Array& kernel, int32 h, int32 w, int32 c, int32 z) {
			mat_t v = 0;
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					v += img(h + i, w + j, c) * kernel(i, j, c);
				}
			}
			return v;
		};
		f = ccorr;
	}
	break;
	case lzh::TEMPLATE_CCORR_NORMED:
	{
		auto ccorr_normed = [](const Array& img, const Array& kernel, int32 h, int32 w, int32 c, int32 z) {
			Vec3m val(0, 0, 0);
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					Vec2m v(img(h + i, w + j, c), kernel(i, j, c));
					val[0] += v[0] * v[0];
					val[1] += v[1] * v[1];
					val[2] += v[0] * v[1];
				}
			}
			return val[2] / sqrt((val[0] * val[1]));
		};
		f = ccorr_normed;
	}
	break;
	case lzh::TEMPLATE_CCOEFF:
	{
		auto ccoeff = [](const Array& img, const Array& kernel, int32 h, int32 w, int32 c, int32 z) {
			Vec2m val(0, 0);
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					val[0] += img(h + i, w + j, c);
					val[1] += kernel(i, j, c);
				}
			}
			val = val / kernel.size().area();
			mat_t v = 0;
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					v += (img(h + i, w + j, c) - val[0]) * (kernel(i, j, c) - val[1]);
				}
			}
			return v;
		};
		f = ccoeff;
	}
	break;
	case lzh::TEMPLATE_CCOEFF_NORMED: 
	{
		auto ccoeff_normed = [](const Array& img, const Array& kernel, int32 h, int32 w, int32 c, int32 z) {
			Vec2m val(0, 0);
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					val[0] += img(h + i, w + j, c);
					val[1] += kernel(i, j, c);
				}
			}
			val = val / kernel.size().area();
			Vec3m val_(0, 0, 0);
			for (int32 i = 0; i < kernel.rows; ++i) {
				for (int32 j = 0; j < kernel.cols; ++j) {
					Vec2m v(img(h + i, w + j, c) - val[0], kernel(i, j, c) - val[1]);
					val_[0] += v[0] * v[0];
					val_[1] += v[1] * v[1];
					val_[2] += v[0] * v[1];
				}
			}
			return val_[2] / sqrt((val_[0] * val_[1]));
		};
		f = ccoeff_normed;
	}
	break;
	default:
		break;
	}
	Convolution(img, dst, _template, img.depth(), f, Point(-1, -1), Size(1, 1), true, BORDER_DEFAULT);
}
void lzh::inRange(InputArray src, InputArray lowerb, InputArray upperb, OutputArray dst)
{
	Array img(src);
	Array low(lowerb);
	Array up(upperb);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
	CHECK_MATRIX_(low);
	CHECK_MATRIX_(up);
	LZH_ACCESS(if (low.len() == up.len() && (low.len() == 1 || low.len() == img.channels())), THROW_INFO_STR("阈值长度应与通道数相同或为1"));
#endif //LZH_DEBUG
	Array mat(img.size(), img.depth(), ColorGamutGray);
	if (low.len() == 1) {
		FOR_RANGE(i, 0, mat.rows)FOR_RANGE(j, 0, mat.cols) {
			bool judge = true;
			FOR_RANGE(k, 0, img.channels()) {
				judge &= __RANGE(low(0), img(i, j, k), up(0));
				if (!judge)break;
			}
			mat.set(i, j, judge ? 255 : 0);
		}
	}
	else if (low.len() == img.channels()) {
		FOR_RANGE(i, 0, mat.rows)FOR_RANGE(j, 0, mat.cols) {
			bool judge = true;
			FOR_RANGE(k, 0, img.channels()) {
				judge &= __RANGE(low(k), img(i, j, k), up(k));
				if (!judge)break;
			}
			mat.set(i, j, judge ? 255 : 0);
		}
	}
	dst = mat;
}
void lzh::Threshold(InputArray src, OutputArray dst, Val boundary, Val lower, Val upper, int32 boundary2upper)
{
	Array src_(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src_);
#endif //LZH_DEBUG
	Array mark = isCompare(src, dst);
	switch (boundary2upper)
	{
	case -1:
		for (int32 ind = 0; ind < src_.len(); ind++)
			mark.set(ind, src_(ind) <= boundary ? lower : upper);
		break;
	case 0:
		for (int32 ind = 0; ind < src_.len(); ind++)
			mark.set(ind, src_(ind) < boundary ? lower : (src_(ind) == boundary ? boundary : upper));
		break;
	case 1:
		for (int32 ind = 0; ind < src_.len(); ind++)
			mark.set(ind, src_(ind) >= boundary ? upper : lower);
		break;
	default:
		THROW_INFO(ERR_INFO_UNLESS);
	}
	dst.set(mark);
}
mat_t lzh::Threshold(InputArray src_, OutputArray dst, Val thresh, Val maxval, ThreshMethod method)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Array img = isCompare(src_, dst);
	switch (method)
	{
	case lzh::THRESH_BINARY:
		FOR_RANGE(i, 0, img.rows) {
			FOR_RANGE(j, 0, img.cols) {
				if (src.compare(i, j, thresh, GREATER))
					img.set(i, j, maxval);
				else
					img.set(i, j, 0);
			}
		}
		break;
	case lzh::THRESH_BINARY_INV:
		FOR_RANGE(i, 0, img.rows) {
			FOR_RANGE(j, 0, img.cols) {
				if (!src.compare(i, j, thresh, GREATER))
					img.set(i, j, maxval);
				else
					img.set(i, j, 0);
			}
		}
		break;
	case lzh::THRESH_TRUNC:
		FOR_RANGE(i, 0, img.rows) {
			FOR_RANGE(j, 0, img.cols) {
				if (src.compare(i, j, thresh, GREATER))
					img.set(i, j, thresh);
				else
					img.set(i, j, src(i, j));
			}
		}
		break;
	case lzh::THRESH_TOZERO:
		FOR_RANGE(i, 0, img.rows) {
			FOR_RANGE(j, 0, img.cols) {
				if (src.compare(i, j, thresh, GREATER))
					img.set(i, j, src(i, j));
				else
					img.set(i, j, 0);
			}
		}
		break;
	case lzh::THRESH_TOZERO_INV:
		FOR_RANGE(i, 0, img.rows) {
			FOR_RANGE(j, 0, img.cols) {
				if (src.compare(i, j, thresh, GREATER))
					img.set(i, j, 0);
				else
					img.set(i, j, src(i, j));
			}
		}
		break;
	case lzh::THRESH_OTSU: {
		thresh = getOtsuThreshold(src);
		FOR_RANGE(i, 0, img.rows) {
			FOR_RANGE(j, 0, img.cols) {
				if (src.compare(i, j, thresh, GREATER))
					img.set(i, j, maxval);
				else
					img.set(i, j, 0);
			}
		}
		break;
	}
	default:
		THROW_INFO(ERR_INFO_UNLESS);
	}
	dst.set(img);
	return thresh;
}
mat_t lzh::getOtsuThreshold(InputArray src_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	mat_t pixel_sum = _T(1) / _T(src.len());
	Array pixel_pro = Array::zeros(256, MAT_T);
	Array pixel_counts = Array::zeros(256, MAT_32S);
	FOR_RANGE(i, 0, src.rows)
		FOR_RANGE(j, 0, src.cols)
		pixel_counts.at<int32>((int32)src(i, j) < 0 ? 0 : (int32)src(i, j) > 255 ? 255 : (int32)src(i, j)) += 1;
	FOR_RANGE(i, 0, 256)
		pixel_pro.at<mat_t>(i) = pixel_counts(i) * pixel_sum;
	Vec2m s_max(0, 0);
	FOR_RANGE(threshold, 0, 256) {
		mat_t w_0 = pixel_pro.colRange(0, threshold).sum();
		mat_t w_1 = pixel_pro.colRange(threshold, 255).sum();
		mat_t u_0 = 0, u_1 = 0;
		mat_t u = 0;
		if (w_0 > 0) {
			FOR_RANGE(i, 0, threshold) {
				u_0 += _T(i * pixel_pro(i));
			}
			u += u_0;
			u_0 /= w_0;
		}
		if (w_1 > 0) {
			FOR_RANGE(i, threshold, 256) {
				u_1 += _T(i * pixel_pro(i));
			}
			u += u_1;
			u_1 /= w_1;
		}
		mat_t g = w_0 * w_1 * pow((u_0 - u_1), 2);

		if (g > s_max[1])
			s_max = Vec2m(_T(threshold), g);
	}
	return s_max[0];
}
void lzh::adaptiveThreshold(InputArray src_, OutputArray dst_, Val minval, Val maxval, int32 area_pro, int32 percent)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Array dst;
	if (src.channels() != 1)
		cvtColor::RGB2GRAY(src, dst);
	else
		dst = isCompare(src_, dst_);
	//src.copyTo(dst);

	int32 x1, y1, x2, y2;
	int32 count = 0;
	mat_t sum = 0;
	int32 S = MIN(src.rows, src.cols);  //划分区域的大小S*S
	S = MIN(_I(area_pro / 100.0 * S), S);
	int32 T = percent;         //百分比，用来最后与阈值的比较
	int32 W = dst.cols;
	int32 H = dst.rows;
	Array Argv(dst.size(), MAT_T);
	for (int32 i = 0; i < W; i++)
	{
		sum = 0;
		for (int32 j = 0; j < H; j++)
		{
			sum += src(j, i);
			if (i == 0)
				Argv.at<mat_t>(j, i) = sum;
			else
				Argv.at<mat_t>(j, i) = Argv.at<mat_t>(j, i - 1) + sum;
		}
	}
	for (int32 i = 0; i < W; i++)
	{
		for (int32 j = 0; j < H; j++)
		{
			x1 = i - S / 2;
			x2 = i + S / 2;
			y1 = j - S / 2;
			y2 = j + S / 2;
			x1 = MAX(0, x1);
			y1 = MAX(0, y1);
			x2 = MIN(W - 1, x2);
			y2 = MIN(H - 1, y2);
			count = (x2 - x1) * (y2 - y1);
			sum = Argv.at<mat_t>(y2, x2) - Argv.at<mat_t>(y1, x2) - Argv.at<mat_t>(y2, x1) + Argv.at<mat_t>(y1, x1);
			if (src(j, i) * count < ((sum * T) / _T(100)))
				dst.set(j, i, minval);
			else
				dst.set(j, i, maxval);
		}
	}
	dst_.set(dst);
}
void lzh::copyMakeBorder(InputArray src_, OutputArray dst, Size newSize, BorderType borderType, const Color& value)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	if (newSize.h < src.rows || newSize.w < src.cols)
		THROW_INFO(ERR_INFO_SIZE);
#endif //LZH_DEBUG
	int32 h = newSize.h - src.rows;
	int32 w = newSize.w - src.cols;
	copyMakeBorder(src, dst, 0, h, 0, w, borderType, value);
}
//void lzh::copyMakeBorder(InputArray src_, OutputArray dst, Size newSize, BorderType borderType, const Color& value)
//{
//	Array src(src_);
//#ifdef LZH_DEBUG
//	CHECK_MATRIX_(src);
//	if (newSize.h < src.rows || newSize.w < src.cols || src.channels() > 4)
//		THROW_INFO(ERR_INFO_SIZE);
//#endif //LZH_DEBUG
//	int32 h = newSize.h - src.rows;
//	int32 w = newSize.w - src.cols;
//	Array mat(newSize, src.channels());
//	FOR_RANGE(i, 0, src.channels()) {
//		Mat res;
//		copyMakeBorder(src.channel(i), res, h / 2, Round(_T(h) / 2), w / 2, Round(_T(w) / 2), borderType, value[i]);
//		res.set(mat.channel(i));
//	}
//	dst = mat;
//}
//void lzh::copyMakeBorder(InputArray src_, OutputArray dst, int32 top, int32 bottom, int32 left, int32 right, BorderType borderType, const Color& value)
//{
//	Array src(src_);
//#ifdef LZH_DEBUG
//	CHECK_MATRIX_(src);
//	if (src.channels() > 4)
//		THROW_INFO(ERR_INFO_SIZE);
//#endif //LZH_DEBUG
//	Array mat(Size(src.cols + left + right, src.rows + top + bottom), src.channels(), src.depth());
//	FOR_RANGE(i, 0, src.channels()) {
//		Array res;
//		copyMakeBorder(src.channel(i), res, top, bottom, left, right, borderType, value[i]);
//		res.set(mat.channel(i));
//	}
//	dst = mat;
//}
void lzh::copyMakeBorder(InputArray src_, OutputArray dst, int32 top, int32 bottom, int32 left, int32 right, BorderType borderType, const Color& value)
{
	top = max(0, top);
	bottom = max(0, bottom);
	left = max(0, left);
	right = max(0, right);
	Array src(src_);
	Size3 size = src.size3();
	size.h += (top + bottom);
	size.w += (left + right);
	if (size == src.size3()) {
		dst = src; return;
	}
	Array mat(size, src.depth(), src.colorGamut());
	src.setTo(mat(Rect(left, top, src.cols, src.rows)));
	copyMakeBorder(mat, Rect(left, top, src.cols, src.rows), top, bottom, left, right, borderType, value);
	dst = mat;
	//top = max(0, top);
	//bottom = max(0, bottom);
	//left = max(0, left);
	//right = max(0, right);
	//Array src(src_);
	//Size3 size = src.size3();
	//size.h += (top + bottom);
	//size.w += (left + right);
	//bool more_once = false;
	//int32 top_ = top,
	//	left_ = left,
	//	bottom_ = bottom,
	//	right_ = right;
	//if (size == src.size3()) {
	//	dst = src;
	//}
	//Array mat(size, src.depth());
	//switch (borderType)
	//{
	//case BORDER_CONSTANT: {
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = 0; j < mat.cols; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < mat.rows; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
	//			}
	//		}
	//	}
	//	for (int32 i = top + src.rows; i < mat.rows; i++) {
	//		for (int32 j = 0; j < mat.cols; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < mat.rows; i++) {
	//		for (int32 j = left + src.cols; j < mat.cols; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
	//			}
	//		}
	//	}
	//	break;
	//}
	//case BORDER_REPLICATE: {
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = left; j < mat.cols - right; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(0, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < mat.rows - bottom; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, 0, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top + src.rows; i < mat.rows; i++) {
	//		for (int32 j = left; j < mat.cols - right; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 1, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < mat.rows - bottom; i++) {
	//		for (int32 j = left + src.cols; j < mat.cols; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, src.cols - 1, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(0, 0, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = mat.cols - right; j < mat.cols; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(0, src.cols - 1, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom; i < mat.rows; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 1, 0, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom; i < mat.rows; i++) {
	//		for (int32 j = mat.cols - right; j < mat.cols; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 1, src.cols - 1, z));
	//			}
	//		}
	//	}
	//	break;
	//}
	//case BORDER_REFLECT: {
	//	if (top >= src.rows)
	//	{
	//		more_once = true;
	//		top_ = top;
	//		top = src.rows;
	//	}
	//	if (bottom >= src.rows)
	//	{
	//		more_once = true;
	//		bottom_ = bottom;
	//		bottom = src.rows;
	//	}
	//	if (left >= src.cols)
	//	{
	//		more_once = true;
	//		left_ = left;
	//		left = src.cols;
	//	}
	//	if (right >= src.cols)
	//	{
	//		more_once = true;
	//		right_ = right;
	//		right = src.cols;
	//	}
	//	if (more_once) {
	//		size = src.size3();
	//		size.h += (top + bottom);
	//		size.w += (left + right);
	//		mat.create(size, src.depth());
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = left; j < src.cols + left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(top - i - 1, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < src.rows + top; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, left - j - 1, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top + src.rows, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = left; j < src.cols + left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 1 - x, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < src.rows + top; i++) {
	//		for (int32 j = left + src.cols, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, src.cols - 1 - y, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(top - i - 1, left - j - 1, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(top - i - 1, src.cols - 1 - y, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 1 - x, left - j - 1, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 1 - x, src.cols - 1 - y, z));
	//			}
	//		}
	//	}
	//	if (more_once) {
	//		copyMakeBorder(mat, mat, top_ - src.rows, bottom_ - src.rows, left_ - src.cols, right_ - src.cols, borderType);
	//		top = top_;
	//		bottom = bottom_;
	//		left = left_;
	//		right = right_;
	//	}
	//	break;
	//}
	//case BORDER_WRAP: {
	//	if (top >= src.rows)
	//	{
	//		more_once = true;
	//		top_ = top;
	//		top = src.rows;
	//	}
	//	if (bottom >= src.rows)
	//	{
	//		more_once = true;
	//		bottom_ = bottom;
	//		bottom = src.rows;
	//	}
	//	if (left >= src.cols)
	//	{
	//		more_once = true;
	//		left_ = left;
	//		left = src.cols;
	//	}
	//	if (right >= src.cols)
	//	{
	//		more_once = true;
	//		right_ = right;
	//		right = src.cols;
	//	}
	//	if (more_once) {
	//		size = src.size3();
	//		size.h += (top + bottom);
	//		size.w += (left + right);
	//		mat.create(size, src.depth());
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = left; j < src.cols + left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < src.rows + top; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, j, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top + src.rows, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = left; j < src.cols + left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(x, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < src.rows + top; i++) {
	//		for (int32 j = left + src.cols, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, y, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i, j, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i, y, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(x, j, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(x, y, z));
	//			}
	//		}
	//	}
	//	if (more_once) {
	//		copyMakeBorder(mat, mat, top_ - src.rows, bottom_ - src.rows, left_ - src.cols, right_ - src.cols, borderType);
	//		top = top_;
	//		bottom = bottom_;
	//		left = left_;
	//		right = right_;
	//	}
	//	break;
	//}
	//case BORDER_REFLECT_101: {
	//	if (top >= src.rows)
	//	{
	//		more_once = true;
	//		top = src.rows - 1;
	//	}
	//	if (bottom >= src.rows)
	//	{
	//		more_once = true;
	//		bottom = src.rows - 1;
	//	}
	//	if (left >= src.cols)
	//	{
	//		more_once = true;
	//		left = src.cols - 1;
	//	}
	//	if (right >= src.cols)
	//	{
	//		more_once = true;
	//		right = src.cols - 1;
	//	}
	//	if (more_once) {
	//		size = src.size3();
	//		size.h += (top + bottom);
	//		size.w += (left + right);
	//		mat.create(size, src.depth());
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = left; j < src.cols + left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(top - i, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < src.rows + top; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, left - j, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top + src.rows, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = left; j < src.cols + left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 2 - x, j - left, z));
	//			}
	//		}
	//	}
	//	for (int32 i = top; i < src.rows + top; i++) {
	//		for (int32 j = left + src.cols, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(i - top, src.cols - 2 - y, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(top - i, left - j, z));
	//			}
	//		}
	//	}
	//	for (int32 i = 0; i < top; i++) {
	//		for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(top - i, src.cols - 2 - y, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = 0; j < left; j++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 2 - x, left - j, z));
	//			}
	//		}
	//	}
	//	for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
	//		for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
	//			for (int32 z = 0; z < mat.channels(); z++) {
	//				mat.set(i, j, z, src(src.rows - 2 - x, src.cols - 2 - y, z));
	//			}
	//		}
	//	}
	//	if (more_once) {
	//		copyMakeBorder(mat, mat, top_ - src.rows, bottom_ - src.rows, left_ - src.cols, right_ - src.cols, borderType);
	//		top = top_;
	//		bottom = bottom_;
	//		left = left_;
	//		right = right_;
	//	}
	//	break;
	//}
	//						 //case BORDER_TRANSPARENT:
	//						 //	break;
	//						 //case BORDER_ISOLATED:
	//						 //	break;
	//default:
	//	break;
	//}
	//for (int32 i = 0; i < src.rows; i++) {
	//	for (int32 j = 0; j < src.cols; j++) {
	//		for (int32 z = 0; z < src.channels(); z++) {
	//			mat.set(i + top, j + left, z, src(i, j, z));
	//		}
	//	}
	//}
	//dst = mat;
}

void lzh::copyMakeBorder(InputOutputArray src_, Rect oldrect, int32 top, int32 bottom, int32 left, int32 right, BorderType borderType, const Color & value)
{
	top = max(0, top);
	bottom = max(0, bottom);
	left = max(0, left);
	right = max(0, right);
	Array mat(src_);
	Array src = mat(oldrect);
	int32 more_once = 0;
	int32 top_ = top,
		left_ = left,
		bottom_ = bottom,
		right_ = right;
	Size size;
	switch (borderType)
	{
	case BORDER_CONSTANT: {
		for (int32 i = 0; i < top; i++) {
			for (int32 j = 0; j < mat.cols; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
				}
			}
		}
		for (int32 i = 0; i < mat.rows; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
				}
			}
		}
		for (int32 i = top + src.rows; i < mat.rows; i++) {
			for (int32 j = 0; j < mat.cols; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
				}
			}
		}
		for (int32 i = 0; i < mat.rows; i++) {
			for (int32 j = left + src.cols; j < mat.cols; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, mat.channels() > 4 ? value[0] : value[z]);
				}
			}
		}
		break;
	}
	case BORDER_REPLICATE: {
		for (int32 i = 0; i < top; i++) {
			for (int32 j = left; j < mat.cols - right; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(0, j - left, z));
				}
			}
		}
		for (int32 i = top; i < mat.rows - bottom; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, 0, z));
				}
			}
		}
		for (int32 i = top + src.rows; i < mat.rows; i++) {
			for (int32 j = left; j < mat.cols - right; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 1, j - left, z));
				}
			}
		}
		for (int32 i = top; i < mat.rows - bottom; i++) {
			for (int32 j = left + src.cols; j < mat.cols; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, src.cols - 1, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(0, 0, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = mat.cols - right; j < mat.cols; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(0, src.cols - 1, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom; i < mat.rows; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 1, 0, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom; i < mat.rows; i++) {
			for (int32 j = mat.cols - right; j < mat.cols; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 1, src.cols - 1, z));
				}
			}
		}
		break;
	}
	case BORDER_REFLECT: {
		if (top > src.rows)
		{
			more_once |= 1;
			top = src.rows;
		}
		if (bottom > src.rows)
		{
			more_once |= 2;
			bottom = src.rows;
		}
		if (left > src.cols)
		{
			more_once |= 4;
			left = src.cols;
		}
		if (right > src.cols)
		{
			more_once |= 8;
			right = src.cols;
		}
		if (more_once != 0) {
			size = src.size();
			//size.h += (top + bottom);
			//size.w += (left + right);
			Rect rect = mat.rect();
			if (more_once & 0x01) {
				rect.y = top_ - src.rows;
				rect.height -= rect.y;
			}
			if (more_once & 0x02) {
				rect.height = rect.height - bottom_ + src.rows;
			}
			if (more_once & 0x04) {
				rect.x = left_ - src.cols;
				rect.width -= rect.x;
			}
			if (more_once & 0x08) {
				rect.width = rect.width - right_ + src.cols;
			}
			mat = mat(rect);
			oldrect = rect;
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = left; j < src.cols + left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(top - i - 1, j - left, z));
				}
			}
		}
		for (int32 i = top; i < src.rows + top; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, left - j - 1, z));
				}
			}
		}
		for (int32 i = top + src.rows, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = left; j < src.cols + left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 1 - x, j - left, z));
				}
			}
		}
		for (int32 i = top; i < src.rows + top; i++) {
			for (int32 j = left + src.cols, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, src.cols - 1 - y, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(top - i - 1, left - j - 1, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(top - i - 1, src.cols - 1 - y, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 1 - x, left - j - 1, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 1 - x, src.cols - 1 - y, z));
				}
			}
		}
		if (more_once) {
			copyMakeBorder(src_, oldrect, top_ - src.rows, bottom_ - src.rows, left_ - src.cols, right_ - src.cols, borderType);
		}
		break;
	}
	case BORDER_WRAP: {
		if (top > src.rows)
		{
			more_once |= 1;
			top = src.rows;
		}
		if (bottom > src.rows)
		{
			more_once |= 2;
			bottom = src.rows;
		}
		if (left > src.cols)
		{
			more_once |= 4;
			left = src.cols;
		}
		if (right > src.cols)
		{
			more_once |= 8;
			right = src.cols;
		}
		if (more_once != 0) {
			size = src.size();
			//size.h += (top + bottom);
			//size.w += (left + right);
			Rect rect = mat.rect();
			if (more_once & 0x01) {
				rect.y = top_ - src.rows;
				rect.height -= rect.y;
			}
			if (more_once & 0x02) {
				rect.height = rect.height - bottom_ + src.rows;
			}
			if (more_once & 0x04) {
				rect.x = left_ - src.cols;
				rect.width -= rect.x;
			}
			if (more_once & 0x08) {
				rect.width = rect.width - right_ + src.cols;
			}
			mat = mat(rect);
			oldrect = rect;
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = left; j < src.cols + left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i, j - left, z));
				}
			}
		}
		for (int32 i = top; i < src.rows + top; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, j, z));
				}
			}
		}
		for (int32 i = top + src.rows, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = left; j < src.cols + left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(x, j - left, z));
				}
			}
		}
		for (int32 i = top; i < src.rows + top; i++) {
			for (int32 j = left + src.cols, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, y, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i, j, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i, y, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(x, j, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(x, y, z));
				}
			}
		}
		if (more_once) {
			copyMakeBorder(src_, oldrect, top_ - src.rows, bottom_ - src.rows, left_ - src.cols, right_ - src.cols, borderType);
		}
		break;
	}
	case BORDER_REFLECT_101: {
		if (top >= src.rows)
		{
			more_once |= 1;
			top = src.rows - 1;
		}
		if (bottom >= src.rows)
		{
			more_once |= 2;
			bottom = src.rows - 1;
		}
		if (left >= src.cols)
		{
			more_once |= 4;
			left = src.cols - 1;
		}
		if (right >= src.cols)
		{
			more_once |= 8;
			right = src.cols - 1;
		}
		if (more_once != 0) {
			size = src.size();
			//size.h += (top + bottom);
			//size.w += (left + right);
			Rect rect = mat.rect();
			if (more_once & 0x01) {
				rect.y = top_ - src.rows + 1;
				rect.height -= rect.y;
			}
			if (more_once & 0x02) {
				rect.height = rect.height - bottom_ + src.rows - 1;
			}
			if (more_once & 0x04) {
				rect.x = left_ - src.cols + 1;
				rect.width -= rect.x;
			}
			if (more_once & 0x08) {
				rect.width = rect.width - right_ + src.cols - 1;
			}
			mat = mat(rect);
			oldrect = rect;
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = left; j < src.cols + left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(top - i, j - left, z));
				}
			}
		}
		for (int32 i = top; i < src.rows + top; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, left - j, z));
				}
			}
		}
		for (int32 i = top + src.rows, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = left; j < src.cols + left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 2 - x, j - left, z));
				}
			}
		}
		for (int32 i = top; i < src.rows + top; i++) {
			for (int32 j = left + src.cols, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(i - top, src.cols - 2 - y, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(top - i, left - j, z));
				}
			}
		}
		for (int32 i = 0; i < top; i++) {
			for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(top - i, src.cols - 2 - y, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = 0; j < left; j++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 2 - x, left - j, z));
				}
			}
		}
		for (int32 i = mat.rows - bottom, x = 0; i < mat.rows; i++, x++) {
			for (int32 j = mat.cols - right, y = 0; j < mat.cols; j++, y++) {
				for (int32 z = 0; z < mat.channels(); z++) {
					mat.set(i, j, z, src(src.rows - 2 - x, src.cols - 2 - y, z));
				}
			}
		}
		if (more_once) {
			copyMakeBorder(src_, oldrect, top_ - src.rows + 1, bottom_ - src.rows + 1, left_ - src.cols + 1, right_ - src.cols + 1, borderType);
		}
		break;
	}
							 //case BORDER_TRANSPARENT:
							 //	break;
							 //case BORDER_ISOLATED:
							 //	break;
	default:
		break;
	}
}

void lzh::Filter2D(InputArray src, OutputArray dst, InputArray kern, int32 depth, Point anchor, Size strides, bool is_copy_border, BorderType borderType, Val value)
{
	Array src_(src);
	Array kern_(kern);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src_);
	CHECK_MATRIX_(kern_);
	if (kern_.dims() > 2)
		THROW_INFO(ERR_INFO_NOT2D);
#endif //LZH_DEBUG
	filter(src, dst, kern, depth, FilterOperator, anchor, strides, is_copy_border, borderType, value);
}
void lzh::filter(InputArray src, OutputArray dst, InputArray kern, int32 depth, FilterOp op, Point anchor, Size strides, bool is_copy_border, BorderType borderType, Val value)
{
	Array src_(src);
	Array kern_(kern);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src_);
	CHECK_MATRIX_(kern_);
	if (kern_.dims() > 2)
		THROW_INFO(ERR_INFO_NOT2D);
#endif //LZH_DEBUG
	int32 kern_row = kern_.rows;
	int32 kern_col = kern_.cols;
	int32 left, right, top, bottom;
	Size3 size = CalSize(src_.size3(), kern_.size3(), anchor, strides, top, bottom, left, right);
	size.c = src_.channels();
	Array out, in;
	if (is_copy_border) {
		copyMakeBorder(src_, in, top, bottom, left, right, borderType, Color(value));
		out.create(src_.cols / strides.w, src_.rows / strides.h, size.c, depth);
	}
	else {
		in = src_;
		out.create(size, depth);
	}
	if (anchor == Point(-1, -1)) {
		anchor.x = kern_row / 2;
		anchor.y = kern_col / 2;
	}
	for (int32 z = 0; z < in.channels(); z++) {
		int32 y = 0;
		for (int32 h = top; h < in.rows - bottom; h += strides.h) {
			int32 x = 0;
			for (int32 w = left; w < in.cols - right; w += strides.w) {
				out.set(y, x, z, op(in, kern_, h - anchor.x, w - anchor.y, z, 0));
				x += 1;
			}
			y += 1;
		}
	}
	dst = out;
}
mat_t lzh::FilterOperator(const Array& src, const Array& kernel, int32 row, int32 col, int32 channel, int32 kernelchannel)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	CHECK_MATRIX_(kernel);
#endif //LZH_DEBUG
	mat_t value = 0;
	for (int32 i = 0; i < kernel.rows; ++i)
		for (int32 j = 0; j < kernel.cols; ++j)
			value += src(row + i, col + j, channel) * kernel(i, j, kernelchannel > 0 ? kernelchannel : 0);
	return value;
}
void lzh::Convolution(InputArray src_, OutputArray dst, InputArray kern, int32 depth, FilterOp op, Point anchor, Size strides, bool is_copy_border, BorderType borderType, Val value)
{
	Array src(src_);
	Array kernel(kern);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	CHECK_MATRIX_(kernel);
	CHECK_PTR(op);
#endif //LZH_DEBUG
	Array in, out;
	int32 kern_row = kernel.rows;
	int32 kern_col = kernel.cols;
	int32 left, right, top, bottom;
	Size3 size = CalSize(src.size3(), kernel.size3(), anchor, strides, top, bottom, left, right);
	if (is_copy_border) {
		copyMakeBorder(src, in, top, bottom, left, right, borderType, Color(value));
		out = Array::zeros(src.cols / strides.w, src.rows / strides.h, size.c, depth);
	}
	else {
		in = src;
		out = Array::zeros(size, depth);
	}
	if (anchor == Point(-1, -1)) {
		anchor.y = kern_row / 2;
		anchor.x = kern_col / 2;
	}
	for (int32 z = 0; z < size.c; z++) {
		for (int32 c = 0; c < src_.channels(); c++) {
			int32 y = 0;
			for (int32 h = top; h < in.rows - bottom; h += strides.h) {
				int32 x = 0;
				for (int32 w = left; w < in.cols - right; w += strides.w) {
					out.add(y, x, z, op(in, kernel, h - anchor.y, w - anchor.x, c, z * src.channels() + c));
					x += 1;
				}
				y += 1;
			}
		}
	}
	dst = out;
}
void lzh::img2col(InputArray src_, OutputArray dst, Size kernel, Point anchor, Size strides, bool is_copy_border, BorderType borderType, Val value)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src_);
#endif //LZH_DEBUG
	int32 kern_row = kernel.h;
	int32 kern_col = kernel.w;
	int32 left, right, top, bottom;
	if (anchor == Point(-1, -1)) {
		anchor.x = kern_row / 2;
		anchor.y = kern_col / 2;
	}
	Size3 size = CalSize(src.size3(), kernel, anchor, strides, top, bottom, left, right);
	size.c = src.channels();
	Mat out, in;
	if (is_copy_border) {
		copyMakeBorder(src, in, top, bottom, left, right, borderType, Color(value));
		out.create(src.cols / strides.h, src.rows / strides.w, size.c);
	}
	else {
		in = src;
		out.create(size);
	}
	int32 conv_count = (in.rows() - bottom - top) * (in.cols() - right - left) * size.c;
	Mat mat(kernel.area(), conv_count);
	mat_t* m = mat.data();
	mat_t* img = in.data();
	int32 c = in.channels();
	int32 offset = out.step().size();
	for (int32 z = 0; z < in.channels(); z++) {
		for (int32 h = top, x = 0; h < in.rows() - bottom; h += strides.h, x++) {
			for (int32 w = left, y = 0; w < in.cols() - right; w += strides.w, y++) {
				for (int32 i = 0; i < kern_row; ++i) {
					for (int32 j = 0; j < kern_col; ++j) {
						*m = img[((h + i - anchor.x) * in.step()[0] + w + j - anchor.y) * c + z];
						m++;
					}
				}
			}
		}
	}
	dst.set(mat);
}


//void lzh::fastFilter2D(InputArray src, OutputArray dst, InputArray kern, Point anchor, Size strides, bool is_copy_border, BorderType borderType, mat_t value)
//{
//	Mat src_ = src;
//	Mat kern_ = kern;
//#ifdef LZH_DEBUG
//	CHECK_MATRIX_(src_);
//	CHECK_MATRIX_(kern_);
//	if (kern_.dims() != 2)
//		THROW_INFO(ERR_INFO_NOT2D);
//#endif //LZH_DEBUG
//	Mat mat;
//	img2col(src, mat, kern_.size(), anchor, strides, is_copy_border, borderType, value);
//	kern_.reshape(1, -1);
//	Dot(mat, kern_, dst);
//}
void lzh::filter2D(InputArray src, OutputArray dst, InputArray kern, int32 depth, ConvolutionType type, BorderType border_types, Val value)
{
	Array img, dest;
	img = src.getArray();
	Array source = img.clone();
	Mat kernel(kern);
	if (CONVOLUTION_FULL == type)
	{
		source = Array();
		const int32 additionalRows = kernel.rows() - 1, additionalCols = kernel.cols() - 1;
		copyMakeBorder(img, source, (additionalRows + 1) / 2, additionalRows / 2, (additionalCols + 1) / 2, additionalCols / 2, border_types, Color(0));
	}
	Point anchor(kernel.rows() - kernel.rows() / 2 - 1, kernel.cols() - kernel.cols() / 2 - 1);
	Filter2D(source, dest, kernel, depth, anchor, Size(1, 1), true, border_types, value);
	if (CONVOLUTION_VALID == type)
	{
		dest = dest.colRange((kernel.cols() - 1) / 2, dest.cols - kernel.cols() / 2).rowRange((kernel.rows() - 1) / 2, dest.rows - kernel.rows() / 2);
	}
	dst.set(dest);
}
void lzh::SumChannel(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Array m = Array::zeros(src.size3(), src.depth());
	for (int32 i = 0; i < src.rows; i++)
		for (int32 j = 0; j < src.cols; j++) {
			mat_t sum = 0;
			for (int32 z = 0; z < src.channels(); z++)
				sum += src(i, j, z);
			m.set(i, j, sum);
		}
	dst.set(m);
}
void lzh::rotate(InputArray src_, RotateAngle dice, OutputArray dst_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Array dst;
	switch (dice)
	{
	case lzh::ROTATE_90_ANGLE:
	{
		Array mat(src.rows, src.cols, src.channels(), src.depth());
		for (int32 h = 0; h < src.rows; ++h)
			for (int32 w = 0; w < src.cols; ++w)
				for (int32 depth = 0; depth < src.channels(); ++depth)
					mat.set(w, src.rows - 1 - h, depth, src(h, w, depth));
		dst = mat;
	}
	break;
	case lzh::ROTATE_180_ANGLE:
	{
		Array mat(src.cols, src.rows, src.channels(), src.depth());
		for (int32 h = 0, y = src.rows - 1; h < src.rows && y >= 0; ++h, --y)
			for (int32 w = 0, x = src.cols - 1; w < src.cols && x >= 0; ++w, --x)
				for (int32 depth = 0; depth < src.channels(); ++depth)
					mat.set(h, w, depth, src(y, x, depth));
		dst = mat;
	}
	break;
	case lzh::ROTATE_270_ANGLE:
	{
		Array mat(src.rows, src.cols, src.channels(), src.depth());
		for (int32 h = 0; h < src.rows; ++h)
			for (int32 w = 0; w < src.cols; ++w)
				for (int32 depth = 0; depth < src.channels(); ++depth)
					mat.set(src.cols - 1 - w, h, depth, src(h, w, depth));
		dst = mat;
	}
	break;
	default:
		break;
	}
	dst_.set(dst);
}
void lzh::rotate(InputArray src, OutputArray dst, int32 angle)
{
	int32 angle_;
	Array img = src;
	switch (angle)
	{
	case lzh::ANGLE0:dst = img; return;
	case lzh::ANGLE90:rotate(src, ROTATE_90_ANGLE, dst); return;
	case lzh::ANGLE180:rotate(src, ROTATE_180_ANGLE, dst); return;
	case lzh::ANGLE270:rotate(src, ROTATE_270_ANGLE, dst); return;
	case lzh::ANGLE45:angle_ = 45; break;
	case lzh::ANGLE135:angle_ = 135; break;
	case lzh::ANGLE225:angle_ = 225; break;
	case lzh::ANGLE315:angle_ = 315; break;
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	warpAffine(src, dst, getRotationMat2D(Point(img.cols / 2, img.rows / 2), angle_));
}
OMat lzh::getRotationMat2D(Point2f center, Val angle)
{
	//angle = -angle;
	Mat rot_mat = Mat::zeros(3, 3);
	Mat tran_1_mat = Mat::eye(3);
	Mat tran_2_mat = Mat::eye(3);
	angle = angle * PI / _T(180);
	rot_mat(0, 0) = cos(angle);
	rot_mat(0, 1) = -sin(angle);
	rot_mat(1, 0) = -rot_mat(0, 1);
	rot_mat(1, 1) = rot_mat(0, 0);
	rot_mat(2, 2) = _T(1);
	tran_1_mat(0, 2) = center.x;
	tran_1_mat(1, 2) = center.y;
	tran_2_mat(0, 2) = -center.x;
	tran_2_mat(1, 2) = -center.y;
	rot_mat = tran_1_mat * rot_mat * tran_2_mat;
	return rot_mat(Rect(0, 0, 3, 2)).clone();
}
OMat lzh::getRotationMat3D(Point2f center, Val angle)
{
	Mat rotate_mat = Mat::zeros(3, 3);
	angle = angle * PI / _T(180);
	rotate_mat(0, 0) = cos(angle);
	rotate_mat(0, 1) = -sin(angle);
	rotate_mat(1, 0) = sin(angle);
	rotate_mat(1, 1) = cos(angle);
	rotate_mat(0, 2) = center.x;
	rotate_mat(1, 2) = center.y;
	rotate_mat(2, 2) = _T(1);
	return rotate_mat;
}
void lzh::Conv2D(InputArray src, OutputArray dst, InputArray kern, int32 depth, Point anchor, const Size& strides, bool is_copy_border, BorderType border_types, const Color & value)
{
	Array src_(src);
	Array kern_(kern);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src_);
	CHECK_MATRIX_(kern_);
#endif //LZH_DEBUG
	Array in, out;
	int32 kern_row = kern_.rows;
	int32 kern_col = kern_.cols;
	int32 left, right, top, bottom;
	Size3 size = CalSize(src_.size3(), kern_.size3(), anchor, strides, top, bottom, left, right);
	if (is_copy_border) {
		copyMakeBorder(src_, in, top, bottom, left, right, border_types, value);
		out = Array::zeros(src_.cols / strides.w, src_.rows / strides.h, size.c, depth);
	}
	else {
		in = src_;
		out = Array::zeros(size, depth);
	}
	if (anchor == Point(-1, -1)) {
		anchor.y = kern_row / 2;
		anchor.x = kern_col / 2;
	}
	for (int32 kern_c = 0; kern_c < size.c; kern_c++)
		for (int32 in_c = 0; in_c < src_.channels(); in_c++) {
			int32 c = kern_c * src_.channels() + in_c;
			for (int32 h = top, x = 0; h < in.rows - bottom; h += strides.h, x++)
				for (int32 w = left, y = 0; w < in.cols - right; w += strides.w, y++)
					out.add(x, y, kern_c, FilterOperator(in, kern_, h - anchor.y, w - anchor.x, in_c, kern_c));
		}
}
void lzh::conv2(InputArray src, OutputArray dst, InputArray kern, int32 depth, ConvolutionType type, BorderType border_types, const Color& value)
{
	Array kernel, img, dest;
	flip(kern, kernel, -1);
	img = Array(src);
	Array source(src);
	if (CONVOLUTION_FULL == type)
	{
		source = Array();
		const int32 additionalRows = kernel.rows - 1, additionalCols = kernel.cols - 1;
		copyMakeBorder(img, source, (additionalRows + 1) / 2, additionalRows / 2, (additionalCols + 1) / 2, additionalCols / 2, border_types, value);
	}
	Point anchor(kernel.rows / 2, kernel.cols / 2);
	Filter2D(source, dest, kernel, depth, anchor);
	if (CONVOLUTION_VALID == type)
	{
		dest = dest.colRange((kernel.cols - 1) / 2, dest.cols - kernel.cols / 2).rowRange((kernel.rows - 1) / 2, dest.rows - kernel.rows / 2);
	}
	dst.set(dest);
}
void lzh::flip(InputArray src_, OutputArray dst, int32 flipCode)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Array v(src.size3(), src.depth());
	if (flipCode > 0)
		FOR_RANGE(row, 0, v.rows)
		FOR_RANGE(col, 0, v.cols)
		FOR_RANGE(z, 0, v.channels())
		v.set(v.rows - row - 1, col, z, src(row, col, z));
	else if (flipCode == 0)
		FOR_RANGE(row, 0, v.rows)
		FOR_RANGE(col, 0, v.cols)
		FOR_RANGE(z, 0, v.channels())
		v.set(row, v.cols - col - 1, z, src(row, col, z));
	else
		FOR_RANGE(row, 0, v.rows)
		FOR_RANGE(col, 0, v.cols)
		FOR_RANGE(z, 0, v.channels())
		v.set(v.rows - row - 1, v.cols - col - 1, z, src(row, col, z));
	dst.set(v);
}
void lzh::HighPass(InputArray src, OutputArray dst, InputArray kernel, Val r, BorderType borderTypes)
{
	Array img(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Array result;
	Filter2D(src, result, kernel, src.depth(), Point(-1, -1), Size(1, 1), true, borderTypes);
	dst.set(img + r * (img - result)); //高反差保留算法
}
void lzh::HighPass(InputArray src, OutputArray dst, Val r, Size ksize, Val sigma)
{
	Array img(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Array result;
	GaussianBlur(img, result, ksize, sigma);
	dst.set(img + r * (img - result)); //高反差保留算法
}
Array lzh::ButterworthFilter(Size sz, Val D, Val n, Val high_h_v_TB, Val low_h_v_TB)
{
	Array single(sz.w, sz.h, MAT_T);
	Point centre = Point(sz.w / 2, sz.h / 2);
	mat_t* p = single.data;
	ButterworthSpectrum bs(D, n, high_h_v_TB, low_h_v_TB);
	for (int32 i = 0; i < sz.h; i++) {
		for (int32 j = 0; j < sz.w; j++) {
			*p = bs(i - centre.x, j - centre.y);
			p++;
		}
	}
	Array butterworth_complex;
	//make two channels to match complex
	Array butterworth_channels[] = { single, single/*Mat::zeros(sz)*/ };
	merge(butterworth_channels, 2, butterworth_complex);
	return butterworth_complex;
}
Array lzh::FourierTransform(InputArray frame_bw, OutputArray image_complex, Mat* image_phase, Mat* image_mag)
{
	Array frame_log(frame_bw);
	frame_log = frame_log.convert(MAT_T) / _T(255);
	/*Take the natural log of the input (compute log(1 + Mag)*/
	frame_log += 1;
	frame_log = frame_log.log();

	Array padded;
	//int32 M = getOptimalDFTSize(frame_log.rows);
	//int32 N = getOptimalDFTSize(frame_log.cols);
	//copyMakeBorder(frame_log, padded, 0, M - frame_log.rows, 0, N - frame_log.cols, BORDER_CONSTANT, 0);
	copyBorderDFTSize(frame_log, padded);

	Array image_planes[] = { padded, Array::zeros(padded.size(), MAT_T) };
	merge(image_planes, 2, image_complex);

	dft(image_complex, image_complex);

	split(image_complex, image_planes);

	if (image_phase) phase(image_planes[0], image_planes[1], *image_phase);
	if (image_mag) magnitude(image_planes[0], image_planes[1], *image_mag);

	//Power
	image_planes[0] = image_planes[0].pow(2);
	image_planes[1] = image_planes[1].pow(2);

	Array Power = image_planes[0] + image_planes[1];

	normalize(Power, Power, 0, 255, NORM_MINMAX);
	//imshow(Power.convert(MAT_8U));
	//waitKey();
	return Power;
}
void lzh::InvFourierTransform(InputArray input, OutputArray inverseTransform)
{
	Mat result;
	idft(input, result);
	/*Take the exponential*/
	result = result.exp();

	vec_mat planes;
	split(result, planes);
	magnitude(planes[0], planes[1], planes[0]);
	planes[0] = planes[0] - 1.0;
	normalize(planes[0], inverseTransform, 0, 255, NORM_MINMAX);
}
void lzh::SobelFact(Mat & kern, int32 kernelSize, bool XOrder)
{
	vec_32s kerI(kernelSize + 1);
	int32 ksize = kernelSize;
	int32 i, j;
	int32 order = XOrder ? 1 : 0;//修改X，Y的值
	int32 oldval, newval;
	kerI[0] = 1;
	//归一化其他vector数组中的值
	for (i = 0; i < ksize; i++)
		kerI[i + 1] = 0;

	for (i = 0; i < ksize - order - 1; i++)
	{
		oldval = kerI[0];
		for (j = 1; j <= ksize; j++)
		{
			newval = kerI[j] + kerI[j - 1];
			kerI[j - 1] = oldval;
			oldval = newval;
		}
	}

	for (i = 0; i < order; i++)
	{
		oldval = -kerI[0];
		for (j = 1; j <= ksize; j++)
		{
			newval = kerI[j - 1] - kerI[j];
			kerI[j - 1] = oldval;
			oldval = newval;
		}
	}
	kerI.resize(kernelSize);
	kern = kerI;
}
OMat lzh::SobelFact(int32 ksize, int32 angle)
{
	Mat kernel;
	Mat x, y;
	SobelFact(x, ksize, true);
	SobelFact(y, ksize, false);
	return y.t() * x;
}
OMat lzh::RobertsKernel(int32 angle)
{
	switch (angle)
	{
	case lzh::ANGLE0:	return (Mat_(2, 1) << -1, 1);
	case lzh::ANGLE45:	return (Mat_(2, 2) << -1, 0, 0, 1);
	case lzh::ANGLE90:	return (Mat_(1, 2) << -1, 1);
	case lzh::ANGLE135:	return (Mat_(2, 2) << 0, -1, 1, 0);
	case lzh::ANGLE180:	return (Mat_(2, 1) << 1, -1);
	case lzh::ANGLE225:	return (Mat_(2, 2) << 1, 0, 0, -1);
	case lzh::ANGLE270:	return (Mat_(2, 1) << 1, -1);
	case lzh::ANGLE315:	return (Mat_(2, 2) << 0, 1, -1, 0);
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return Mat();
}
OMat lzh::PrewittKernel(int32 angle)
{
	switch (angle)
	{
	case lzh::ANGLE0:
		return (Mat_(3, 3) <<
			-1, 0, 1,
			-1, 0, 1,
			-1, 0, 1);
	case lzh::ANGLE45:
		return (Mat_(3, 3) <<
			-1, -1, 0,
			-1, 0, 1,
			0, 1, 1);
	case lzh::ANGLE90:
		return (Mat_(3, 3) <<
			-1, -1, -1,
			0, 0, 0,
			1, 1, 1);
	case lzh::ANGLE135:
		return (Mat_(3, 3) <<
			0, -1, -1,
			1, 0, -1,
			1, 1, 0);
	case lzh::ANGLE180:
		return (Mat_(3, 3) <<
			1, 0, -1,
			1, 0, -1,
			1, 0, -1);
	case lzh::ANGLE225:
		return (Mat_(3, 3) <<
			1, 1, 0,
			1, 0, -1,
			0, -1, -1);
	case lzh::ANGLE270:
		return (Mat_(3, 3) <<
			1, 1, 1,
			0, 0, 0,
			-1, -1, -1);
	case lzh::ANGLE315:
		return (Mat_(3, 3) <<
			0, 1, 1,
			-1, 0, 1,
			-1, -1, 0);
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return Mat();
}
OMat lzh::SobelKernel(int32 angle)
{
	switch (angle)
	{
	case lzh::ANGLE0:
		return (Mat_(3, 3) <<
			-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1);
	case lzh::ANGLE45:
		return (Mat_(3, 3) <<
			-2, -1, 0,
			-1, 0, 1,
			0, 1, 2);
	case lzh::ANGLE90:
		return (Mat_(3, 3) <<
			-1, -2, -1,
			0, 0, 0,
			1, 2, 1);
	case lzh::ANGLE135:
		return (Mat_(3, 3) <<
			0, -1, -2,
			1, 0, -1,
			2, 1, 0);
	case lzh::ANGLE180:
		return (Mat_(3, 3) <<
			1, 0, -1,
			2, 0, -2,
			1, 0, -1);
	case lzh::ANGLE225:
		return (Mat_(3, 3) <<
			2, 1, 0,
			1, 0, -1,
			0, -1, -2);
	case lzh::ANGLE270:
		return (Mat_(3, 3) <<
			1, 2, 1,
			0, 0, 0,
			-1, -2, -1);
	case lzh::ANGLE315:
		return (Mat_(3, 3) <<
			0, 1, 2,
			-1, 0, 1,
			-2, -1, 0);
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return Mat();
}
OMat lzh::LapacianKernel(NeighborhoodType neighbourhood)
{
	switch (neighbourhood)
	{
	case lzh::Four_Dire:
		return (Mat_(3, 3) <<
			0, 1, 0,
			1, -4, 1,
			0, 1, 0);
	case lzh::Eight_Dire:
		return (Mat_(3, 3) <<
			1, 1, 1,
			1, -8, 1,
			1, 1, 1);
	default:THROW_INFO(ERR_INFO_UNLESS);
	}
	return Mat();
}
void lzh::Sobel(InputArray src_, OutputArray dst, int32 depth)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Mat kernx = SobelKernel(ANGLE0);
	Mat kerny = SobelKernel(ANGLE90);

	Mat x, y;
	conv2(src, y, kerny, depth, CONVOLUTION_SAME, BORDER_DEFAULT);
	conv2(src, x, kernx, depth, CONVOLUTION_SAME, BORDER_DEFAULT);
	dst.set((x.pow(2) + y.pow(2)).sqrt());
}
//bool CannyMark(InputOutputArray img, Point point, Object* p)
//{
//	if (p == nullptr)return false;
//	Array image = img;
//	if (image(point) == p[0].as<mat_t>() || image(point) == p[1].as<mat_t>()) {
//		image.set(point, p[2].as<mat_t>());
//		return true;
//	}
//	else {
//		//img(y, x) = 50;
//		return false;
//	}
//}
void lzh::Canny(InputArray image, OutputArray edges, Val threshold1, Val threshold2, bool L2gradient, Size ksize, Val sigma)
{
	Array img, grad[2], theta, G;
	if (image.getArray().channels() == 3)
		cvtColor::RGB2GRAY(image, img);
	else
		img = image.getArray();
	int32 type = img.depth();
	if (ksize.area() > 1 && sigma != 0)
		GaussianBlur(img, img, ksize, sigma);
	Sobel(img, grad[0], MAT_T, ANGLE0); Sobel(img, grad[1], MAT_T, ANGLE270);
	Canny(grad[0], grad[1], edges, threshold1, threshold2, L2gradient);
}
void lzh::Canny(InputArray dx_, InputArray dy_, OutputArray edges, Val threshold1, Val threshold2, bool L2gradient)
{
	Array dx(dx_), dy(dy_);
	Array G = (L2gradient ? (dx.pow(2) + dy.pow(2)).sqrt() : dx.abs() + dy.abs());
	//if (G.depth() != MAT_8U) {
	//	normalize(G, G, 0, 255, NORM_MINMAX);
	//}
	Array flag = Array::zeros(G.size3(), MAT_8U);
	mat_t Gp1, Gp2, angle, t, Gp;
	FOR_RANGE(i, 0, G.rows) {
		FOR_RANGE(j, 0, G.cols) {
			Gp1 = Gp2 = 0;
			Gp = G(i, j);
			if (Gp <= 0)continue;
			t = atan2(dy(i, j), dx(i, j));
			angle = t * rad2ang_;
			angle = angle < 0 ? 180 + angle : angle;
			if (0 < angle && angle < 45) {
				t = tan(t);
				if (!__RANGE(0, i - 1, G.rows) || !__RANGE(0, j + 1, G.cols))Gp1 = 0;
				else Gp1 = G(i, j + 1) + (G(i - 1, j + 1) - G(i, j + 1)) * t;
				if (!__RANGE(0, i + 1, G.rows) || !__RANGE(0, j - 1, G.cols))Gp2 = 0;
				else Gp2 = G(i, j - 1) + (G(i + 1, j - 1) - G(i, j - 1)) * t;
			}
			else if (45 < angle && angle < 90) {
				t = tan(PI_ / 2 - t);
				if (!__RANGE(0, i - 1, G.rows) || !__RANGE(0, j + 1, G.cols))Gp1 = 0;
				else Gp1 = G(i - 1, j) + (G(i - 1, j + 1) - G(i - 1, j)) * t;
				if (!__RANGE(0, i + 1, G.rows) || !__RANGE(0, j - 1, G.cols))Gp2 = 0;
				else Gp2 = G(i + 1, j) + (G(i + 1, j - 1) - G(i + 1, j)) * t;
			}
			else if (90 < angle && angle < 135) {
				t = tan(t - PI_ / 2);
				if (!__RANGE(0, i - 1, G.rows) || !__RANGE(0, j - 1, G.cols))Gp1 = 0;
				else Gp1 = G(i - 1, j) + (G(i - 1, j - 1) - G(i - 1, j)) * t;
				if (!__RANGE(0, i + 1, G.rows) || !__RANGE(0, j + 1, G.cols))Gp2 = 0;
				else Gp2 = G(i + 1, j) + (G(i + 1, j + 1) - G(i + 1, j)) * t;
			}
			else if (135 < angle && angle < 180) {
				t = tan(PI_ - t);
				if (!__RANGE(0, i - 1, G.rows) || !__RANGE(0, j - 1, G.cols))Gp1 = 0;
				else Gp1 = G(i, j - 1) + (G(i - 1, j - 1) - G(i, j - 1)) * t;
				if (!__RANGE(0, i + 1, G.rows) || !__RANGE(0, j + 1, G.cols))Gp2 = 0;
				else Gp2 = G(i, j + 1) + (G(i + 1, j + 1) - G(i, j + 1)) * t;
			}
			else if (angle == 0 || angle == 180) {
				if (!__RANGE(0, j + 1, G.cols))Gp1 = 0;
				else Gp1 = G(i, j + 1);
				if (!__RANGE(0, j - 1, G.cols))Gp2 = 0;
				else Gp2 = G(i, j - 1);
			}
			else if (angle == 45) {
				if (!__RANGE(0, i - 1, G.rows) || !__RANGE(0, j + 1, G.cols))Gp1 = 0;
				else Gp1 = G(i - 1, j + 1);
				if (!__RANGE(0, i + 1, G.rows) || !__RANGE(0, j - 1, G.cols))Gp2 = 0;
				else Gp2 = G(i + 1, j - 1);
			}
			else if (angle == 135) {
				if (!__RANGE(0, i - 1, G.rows) || !__RANGE(0, j - 1, G.cols))Gp1 = 0;
				else Gp1 = G(i - 1, j - 1);
				if (!__RANGE(0, i + 1, G.rows) || !__RANGE(0, j + 1, G.cols))Gp2 = 0;
				else Gp2 = G(i + 1, j + 1);
			}
			else if (angle == 90) {
				if (!__RANGE(0, i - 1, G.rows))Gp1 = 0;
				else Gp1 = G(i - 1, j);
				if (!__RANGE(0, i + 1, G.rows))Gp2 = 0;
				else Gp2 = G(i + 1, j);
			}
			if (Gp <= Gp1 || Gp <= Gp2) {
				flag.at<uint8>(i, j) = 0;
			}
			//else if ()
			//{
			//	flag.at<int32>(i, j) = 0;
			//}
			else {
				if (Gp >= threshold1 && Gp < threshold2) {
					flag.at<uint8>(i, j) = 128;
				}
				else if (Gp >= threshold2) {
					flag.at<uint8>(i, j) = 255;
				}
				else if (Gp < threshold1) {
					flag.at<uint8>(i, j) = 0;
				}
			}
		}
	}
	Object obj[3] = { _T(255), _T(128), _T(200) };
	//FOR_RANGE(i, 0, G.rows) {
	//	FOR_RANGE(j, 0, G.cols) {
	//		if (flag.at<int32>(i, j) == 0) {
	//			Edges.set(i, j, 0);
	//		}
	//		else if (flag.at<int32>(i, j) == 1) {
	//			Edges.set(i, j, obj[1].as<mat_t>());
	//		}
	//		else if (flag.at<int32>(i, j) == 2) {
	//			Edges.set(i, j, obj[0].as<mat_t>());
	//		}
	//	}
	//}
	auto CannyMark = [](InputOutputArray img, Point point, Object* p)
	{
		if (p == nullptr)return false;
		int32 index = 0;
		Array image = img.getArray();
		if (image(point) == p[1].as<mat_t>()) {
			image.set(point, p[0].as<mat_t>());
			return true;
		}
//		if (image(point) == p[0].as<mat_t>()) {
///*			if (image(CHECK_INDEX_IMAGE(point.y - 1, image.rows), CHECK_INDEX_IMAGE(point.x - 1, image.cols)) == p[0].as<mat_t>())index += 1;
//			if (image(CHECK_INDEX_IMAGE(point.y - 1, image.rows), CHECK_INDEX_IMAGE(point.x, image.cols)) == p[0].as<mat_t>())index += 1;
//			if (image(CHECK_INDEX_IMAGE(point.y - 1, image.rows), CHECK_INDEX_IMAGE(point.x + 1, image.cols)) == p[0].as<mat_t>())index += 1;
//			if (image(CHECK_INDEX_IMAGE(point.y, image.rows), CHECK_INDEX_IMAGE(point.x - 1, image.cols)) == p[0].as<mat_t>())index += 1;
//			if (image(CHECK_INDEX_IMAGE(point.y, image.rows), CHECK_INDEX_IMAGE(point.x + 1, image.cols)) == p[0].as<mat_t>())index += 1;
//			if (image(CHECK_INDEX_IMAGE(point.y + 1, image.rows), CHECK_INDEX_IMAGE(point.x - 1, image.cols)) == p[0].as<mat_t>())index += 1;
//			if (image(CHECK_INDEX_IMAGE(point.y + 1, image.rows), CHECK_INDEX_IMAGE(point.x, image.cols)) == p[0].as<mat_t>())index += 1;
//			if (image(CHECK_INDEX_IMAGE(point.y + 1, image.rows), CHECK_INDEX_IMAGE(point.x + 1, image.cols)) == p[0].as<mat_t>())index += 1;
//			if(index <= 3)*/ /*image.set(point, p[2].as<mat_t>());*/
//			return true;
//		}
		/*else*/ 
		//if (image(point) == p[1].as<mat_t>()) {
		//	if (image(CHECK_INDEX_IMAGE(point.y - 1, image.rows), CHECK_INDEX_IMAGE(point.x - 1, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (image(CHECK_INDEX_IMAGE(point.y - 1, image.rows), CHECK_INDEX_IMAGE(point.x, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (image(CHECK_INDEX_IMAGE(point.y - 1, image.rows), CHECK_INDEX_IMAGE(point.x + 1, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (image(CHECK_INDEX_IMAGE(point.y, image.rows), CHECK_INDEX_IMAGE(point.x - 1, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (image(CHECK_INDEX_IMAGE(point.y, image.rows), CHECK_INDEX_IMAGE(point.x + 1, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (image(CHECK_INDEX_IMAGE(point.y + 1, image.rows), CHECK_INDEX_IMAGE(point.x - 1, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (image(CHECK_INDEX_IMAGE(point.y + 1, image.rows), CHECK_INDEX_IMAGE(point.x, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (image(CHECK_INDEX_IMAGE(point.y + 1, image.rows), CHECK_INDEX_IMAGE(point.x + 1, image.cols)) == p[0].as<mat_t>())index += 1;
		//	if (index != 0) {
		//		image.set(point, p[0].as<mat_t>()); 
		//		return true;
		//	}else return false;
		//}
		else {
			//img(y, x) = 50;
			return false;
		}
	};
	Dfs(ITERATION, Eight_Dire).DfsMark(flag, 128, CannyMark, obj);
	//FOR_RANGE(i, 0, flag.rows) {
	//	FOR_RANGE(j, 0, flag.cols) {
	//		if (Edges(i, j) == obj[2].as<mat_t>()) {
	//			Edges.set(i, j, obj[0].as<mat_t>());
	//		}
	//		else {
	//			Edges.set(i, j, 0);
	//		}
	//	}
	//}
	//imwrite("C:/Users/lzh/Desktop/dst.bmp", flag);
	edges = flag;
}
lzh::Dfs::Dfs(DFSMethod method, NeighborhoodType type) 
	: dire(type), method(method)
{
}
void Dfs::DfsMark(InputOutputArray image, Val thresh, DfsOperator op, Object* pre, DfsCallBack callback)
{
	switch (method)
	{
	case lzh::RECURSION:
		DfsMarkRecursion(image, thresh, op, pre, callback);
		break;
	case lzh::ITERATION:
		DfsMarkIteration(image, thresh, op, pre, callback);
		break;
	default:
		break;
	}
}
void Dfs::DfsMark(InputOutputArray image, Point seedpoint, DfsOperator op, Object* pre, DfsCallBack callback)
{
	switch (method)
	{
	case lzh::RECURSION:
		DfsMarkRecursion(image, seedpoint, op, pre, callback);
		break;
	case lzh::ITERATION:
		DfsMarkIteration(image, seedpoint, op, pre, callback);
		break;
	default:
		break;
	}
}
void Dfs::DfsMarkRecursion(InputOutputArray image, Val thresh, DfsOperator op, Object* pre, DfsCallBack callback)
{
	Array img(image);
	FOR_RANGE(i, 0, img.rows) {
		FOR_RANGE(j, 0, img.cols) {
			if (img(i, j) >= thresh) {
				if (callback)callback(pre);
				DfsMarkRecursion(image, j, i, op, pre);
			}
		}
	}
}
void Dfs::DfsMarkRecursion(InputOutputArray image, Point seedpoint, DfsOperator op, Object* pre, DfsCallBack callback)
{
	if (callback)callback(pre);
	DfsMarkRecursion(image, seedpoint.x, seedpoint.y, op, pre);
}
void Dfs::DfsMarkRecursion(InputOutputArray image, int32 col, int32 row, DfsOperator op, Object* pre, int32 angle)
{
	if (row >= image.rows() || col >= image.cols() || col < 0 || row < 0)
		return;
	//lzh::output(Point(col, row));
	if (op(image, Point(col, row), pre)) {
		if (!MAT_SWITCH(angle, ANGLE0))		DfsMarkRecursion(image, col + 1, row, op, pre, angle | ANGLE180);
		if (!MAT_SWITCH(angle, ANGLE45))	DfsMarkRecursion(image, col + 1, row - 1, op, pre, angle | ANGLE225);
		if (!MAT_SWITCH(angle, ANGLE90))	DfsMarkRecursion(image, col, row - 1, op, pre, angle | ANGLE270);
		if (!MAT_SWITCH(angle, ANGLE135))	DfsMarkRecursion(image, col - 1, row - 1, op, pre, angle | ANGLE315);
		if (!MAT_SWITCH(angle, ANGLE180))	DfsMarkRecursion(image, col + 1, row, op, pre, angle | ANGLE0);
		if (!MAT_SWITCH(angle, ANGLE225))	DfsMarkRecursion(image, col - 1, row + 1, op, pre, angle | ANGLE45);
		if (!MAT_SWITCH(angle, ANGLE270))	DfsMarkRecursion(image, col, row + 1, op, pre, angle | ANGLE90);
		if (!MAT_SWITCH(angle, ANGLE315))	DfsMarkRecursion(image, col + 1, row + 1, op, pre, angle | ANGLE135);
	}
}
bool Dfs::DfsBackDirection(Point& p, int32& state, int32& angle)
{
	if (angle >> 8 == NO_ANGLE)
		return false;
	int32 s = angle >> 8;
	int32 off = 0;
	int32 idx = 0;
	while (s) {
		if ((s & 0x01) == 0x01)
			idx++;
		s >>= 1;
	}
	off = state >> 2 >> ((idx - 1) * 3); //获取最高3位的回退方向(栈顶)
	switch (off)
	{
	case lzh::ANGLE0_OFFSET:	p = Point(p.x + 1, p.y);		break;//往右退
	case lzh::ANGLE45_OFFSET:	p = Point(p.x + 1, p.y - 1);	break;//往右上退
	case lzh::ANGLE90_OFFSET:	p = Point(p.x, p.y - 1);		break;//往上退
	case lzh::ANGLE135_OFFSET:	p = Point(p.x - 1, p.y - 1);	break;//往左上退
	case lzh::ANGLE180_OFFSET:	p = Point(p.x - 1, p.y);		break;//往左退
	case lzh::ANGLE225_OFFSET:	p = Point(p.x - 1, p.y + 1);	break;//往左下退
	case lzh::ANGLE270_OFFSET:	p = Point(p.x, p.y + 1);		break;//往下退
	case lzh::ANGLE315_OFFSET:	p = Point(p.x + 1, p.y + 1);	break;//往右下退
	default:break;
	}
	state = (state & ~(0x07 << ((idx - 1) * 3 + 2)) & (~0x03)) | (state & 0x03); //消去最高3位回退方向(弹出栈顶)
	return true;
}
int32 Dfs::DfsCalDirection(int32& angle, int32 angle_offset)
{
	int32 idx = 0;
	int32 state = angle >> 8; //回退方向
	while (state) { //获取回退方向的个数
		if ((state & 0x01) == 0x01)
			idx++;
		state >>= 1;
	}
	angle |= 1 << _I(angle_offset);	//记录前进方向
	angle |= (1 << _I(angle_offset)) << 8;	//记录回退方向
	return angle_offset << (idx * 3); //低2位表示状态, 高位每3位表示后退方向优先级
}
bool Dfs::DfsForwardDirection(int32* stateArr, Point& p, int32& angle, int32 angle_flag, int32 rows, int32 cols, int32 dy, int32 dx)
{
	Point pos;
	angle = angle | angle_flag;
	if (!((dx >= 0 ? p.x + dx >= cols : p.x + dx < 0) || (dy >= 0 ? p.y + dy >= rows : p.y + dy < 0))) {
		pos = Point(p.x + dx, p.y + dy);
		if ((stateArr[pos.y * cols + pos.x] & 0x03) != 0x01) {
			p = pos;
			return true;
		}
	}
	return false;
}
bool Dfs::DfsForwardDirection(int32* angleArr, int32* stateArr, Point& p, int32& angle, int32 rows, int32 cols)
{
	int32 angle_offset = -1;
	switch (dire)
	{
	case Eight_Dire:
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE45))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE45, rows, cols, -1, 1))
				angle_offset = ANGLE225_OFFSET;
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE135))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE135, rows, cols, -1, -1))
				angle_offset = ANGLE315_OFFSET;
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE225))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE225, rows, cols, 1, -1))
				angle_offset = ANGLE45_OFFSET;
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE315))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE315, rows, cols, 1, 1))
				angle_offset = ANGLE135_OFFSET;
	case Four_Dire:
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE0))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE0, rows, cols, 0, 1))
				angle_offset = ANGLE180_OFFSET;
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE90))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE90, rows, cols, -1, 0))
				angle_offset = ANGLE270_OFFSET;
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE180))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE180, rows, cols, 0, -1))
				angle_offset = ANGLE0_OFFSET;
		if (angle_offset == -1 && !MAT_SWITCH(angle, ANGLE270))
			if (DfsForwardDirection(stateArr, p, angle, ANGLE270, rows, cols, 1, 0))
				angle_offset = ANGLE90_OFFSET;
		break;
	default:break;
	}
	if (angle_offset == -1)
		return false;
	else
		stateArr[p.y * cols + p.x] |= DfsCalDirection(angleArr[p.y * cols + p.x], angle_offset) << 2; //回退方向压入堆栈
	return true;
}
void Dfs::DfsMarkIteration(InputOutputArray image, Val thresh, DfsOperator op, Object* pre, DfsCallBack callback)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	int32* angleArr = (int32*)fastMalloc(img.size().area() * sizeof(int32));
	int32* stateArr = (int32*)fastMalloc(img.size().area() * sizeof(int32));
#ifdef LZH_DEBUG
	CHECK_PTR(angleArr);
	CHECK_PTR(stateArr);
#endif //LZH_DEBUG
	memset(angleArr, 0, img.size().area() * sizeof(int32));
	memset(stateArr, 0, img.size().area() * sizeof(int32));
	FOR_RANGE(i, 0, img.rows) {
		FOR_RANGE(j, 0, img.cols) {
			if (img(i, j) >= thresh && (stateArr[i * img.cols + j] & 0x03) == 0x00) { //状态为0 => 未经过该点
				DfsMarkIteration(image, angleArr, stateArr, Point(j, i), op, pre, callback);
			}
		}
	}
	fastFree(angleArr); angleArr = 0;
	fastFree(stateArr); stateArr = 0;
}
void Dfs::DfsMarkIteration(InputOutputArray image, Point seedpoint, DfsOperator op, Object* pre, DfsCallBack callback)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(image);
#endif //LZH_DEBUG
	int32* angleArr = (int32*)fastMalloc(img.size().area() * sizeof(int32));
	int32* stateArr = (int32*)fastMalloc(img.size().area() * sizeof(int32));
#ifdef LZH_DEBUG
	CHECK_PTR(angleArr);
	CHECK_PTR(stateArr);
#endif //LZH_DEBUG
	memset(angleArr, 0, img.size().area() * sizeof(int32));
	memset(stateArr, 0, img.size().area() * sizeof(int32));
	DfsMarkIteration(image, angleArr, stateArr, seedpoint, op, pre, callback);
	fastFree(angleArr); angleArr = 0;
	fastFree(stateArr); stateArr = 0;
}
void Dfs::DfsMarkIteration(InputOutputArray image, int32* angleArr, int32* stateArr, Point seedpoint, DfsOperator op, Object* pre, DfsCallBack callback)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(image);
	CHECK_PTR(angleArr);
	CHECK_PTR(stateArr);
#endif //LZH_DEBUG
	if (callback)callback(pre);
	Point p = seedpoint;
	while (1) {
		int32& state = stateArr[p.y * image.cols() + p.x];//当前状态
		int32& angle = angleArr[p.y * image.cols() + p.x];//当前方向累计
		if ((state & 0x03) != 0x01) { //状态为1 => 经过该区域
			//lzh::print(p);
			//imshow(image); waitKey(30);
			if ((state & 0x03) == 0x02 || op(image, p, pre)) { //状态为-1 => 递归返回该点 || 调用回调确认是否符合条件
				state = (state & (~0x03)) | 0x01; //标记区域为已经过状态
				if (!DfsForwardDirection(angleArr, stateArr, p, angle, image.rows(), image.cols())) {//前进方向
					if (!DfsBackDirection(p, state, angle)) //后退方向
						break;
					stateArr[p.y * image.cols() + p.x] = (stateArr[p.y * image.cols() + p.x] & (~0x03)) | 0x02;//递归返回
				}
			}
			else {
				state = (state & (~0x03)) | 0x01; //标记区域为已经过状态
				if (!DfsBackDirection(p, state, angle))//后退方向
					break;
				stateArr[p.y * image.cols() + p.x] = (stateArr[p.y * image.cols() + p.x] & (~0x03)) | 0x02;//递归返回
			}
		}
		else {
			if (!DfsBackDirection(p, state, angle))//后退方向
				break;
			stateArr[p.y * image.cols() + p.x] = (stateArr[p.y * image.cols() + p.x] & (~0x03)) | 0x02;//递归返回
		}
	}
}


void lzh::MarkMainArea(InputOutputArray image, int32 col, int32 row, int32 dire)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	if (row >= img.rows || col >= img.cols || col < 0 || row < 0)
		return;
	if (img.isValue(img.toidx(row, col), 255)) {
		img.set(row, col, 127);
		if ((dire & 1) != 1)
			MarkMainArea(image, col, row + 1, (dire | 2));
		if ((dire & 8) != 8)
			MarkMainArea(image, col + 1, row, (dire | 4));
		if ((dire & 2) != 2)
			MarkMainArea(image, col, row - 1, (dire | 1));
		if ((dire & 4) != 4)
			MarkMainArea(image, col - 1, row, (dire | 8));
	}
}
void lzh::SearchMainArea(InputOutputArray image)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	for (int32 i = 0; i < img.rows; i++) {
		for (int32 j = 0; j < img.cols; j++) {
			if (img.isValue(img.toidx(i, j), 127))
				img.set(i, j, 0);
			else img.set(i, j, 255);
		}
	}
}
void lzh::floodFill(InputOutputArray image, Point seedPoint, Color newVal)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Object obj[] = { _T(0), &newVal };
	auto floodFillMark = [](InputOutputArray img, Point point, Object* p)
	{
		if (p == nullptr)return false;
		if (img.getArray().isValue(point, p[0].as<mat_t>())) {
			img.getArray().set(point.y, point.x, p[1].as<Color>());
			return true;
		}
		return false;
	};
	Dfs(ITERATION, Four_Dire).DfsMark(img, seedPoint, floodFillMark, obj);
}
void lzh::floodFill(InputOutputArray image, InputOutputArray mark, Point seedPoint, Color newVal)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Image img_ = mark.getArray();
	Object obj[] = { _T(0), &newVal, &img_ };
	auto floodFillMark = [](InputOutputArray img, Point point, Object* p)
	{
		if (p == nullptr)return false;
		if (p[2].data() != nullptr) {
			if (p[2].as<Array>().isZero(point))return false;
		}
		if (img.getArray().isValue(point, p[0].as<mat_t>())) {
			img.getArray().set(point.y, point.x, p[1].as<Color>());
			return true;
		}
		return false;
	};
	Dfs(ITERATION, Four_Dire).DfsMark(img, seedPoint, floodFillMark, obj);
}
void lzh::floodHole(InputArray src, OutputArray dst, Color newVal)
{
	Array mark = src.getArray().clone();
	floodFill(mark, Point(0, 0), newVal);
	dst = src.getArray() | ~mark;
}
//bool ContoursMark(InputOutputArray img, Point point, Object* p)
//{
//	if (p == nullptr)return false;
//	if (img.getArray().isValue(point, p[1].as<mat_t>())) {
//		//v = 128;
//		vecs_p2_32s* vec = p[0].ptr<vecs_p2_32s>();
//		if (vec->empty())
//			vec->push_back(vec_p2_32s());
//		vec->at(vec->size() - 1).push_back(point);
//		return true;
//	}
//	return false;
//}
//void ContoursUpdate(Object* p)
//{
//	if (p == nullptr)return;
//	((vecs_p2_32s*)p[0])->push_back(vec_p2_32s());
//}
void lzh::findContours(InputArray image, OutputArrayOfArrays contours, Val threshold1, Val threshold2, bool L2gradient, Size ksize, Val sigma)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Canny(img, img, threshold1, threshold2, L2gradient, ksize, sigma);

	vecs_p2_32s vecs;
	Object obj[2] = { (const void*)&vecs, _T(255) };
	auto mark = [](InputOutputArray img, Point point, Object* p)
	{
		if (p == nullptr)return false;
		if (img.getArray().isValue(point, p[1].as<mat_t>())) {
			//v = 128;
			vecs_p2_32s* vec = p[0].ptr<vecs_p2_32s>();
			if (vec->empty())
				vec->push_back(vec_p2_32s());
			vec->at(vec->size() - 1).push_back(point);
			return true;
		}
		return false;
	};
	auto update = [](Object* p)
	{
		if (p == nullptr)return;
		((vecs_p2_32s*)p[0])->push_back(vec_p2_32s());
	};
	Dfs(ITERATION, Eight_Dire).DfsMark(img, 255, mark, obj, update);
	contours.createSameSize(vecs, contours.depth());
	contours.setTo(vecs);
}
Rect lzh::bouningRect(InputArray contour)
{
	Array points(contour);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(points);
#endif //LZH_DEBUG
	Point p = points.row(0).toPoint();
	Point pos[2] = { p, p };
	FOR_RANGE(i, 1, points.rows) {
		p = points.row(i).toPoint();
		if (p.x < pos[0].x)pos[0].x = p.x;
		if (p.y < pos[0].y)pos[0].y = p.y;
		if (p.x > pos[1].x)pos[1].x = p.x;
		if (p.y > pos[1].y)pos[1].y = p.y;
	}
	return Rect(pos[0].x, pos[0].y, pos[1].x - pos[0].x + 1, pos[1].y - pos[0].y + 1);
}
RectF lzh::bouningRectf(InputArray contour)
{
	Array points(contour);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(points);
#endif //LZH_DEBUG
	Point2f p = points.row(0).toPoint2d();
	Point2f pos[2] = { p, p };
	FOR_RANGE(i, 1, points.rows) {
		p = points.row(i).toPoint2d();
		if (p.x < pos[0].x)pos[0].x = p.x;
		if (p.y < pos[0].y)pos[0].y = p.y;
		if (p.x > pos[1].x)pos[1].x = p.x;
		if (p.y > pos[1].y)pos[1].y = p.y;
	}
	return RectF(pos[0].x, pos[0].y, pos[1].x - pos[0].x + 1, pos[1].y - pos[0].y + 1);
}
Point lzh::bouningCenter(InputArray contour)
{
	Array points(contour);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(points);
#endif //LZH_DEBUG
	Point2f p(0, 0);
	FOR_RANGE(i, 0, points.len() / 2) {
		p += Point2f(points.get<float32>(i * 2), points.get<float32>(i * 2 + 1));
	}
	return Point(p / (points.len() / 2.0f));
}
Point2f lzh::bouningCenterf(InputArray contour)
{
	Array points(contour);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(points);
#endif //LZH_DEBUG
	Point2f p(0, 0);
	FOR_RANGE(i, 0, points.len() / 2) {
		p += Point2f(points.get<float32>(i * 2), points.get<float32>(i * 2 + 1));
	}
	return p / float32(points.len() / 2.0);
}
lzh::Moments lzh::moments(InputArray _array, bool binaryImage)
{
	Array image(_array);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(image);
	LZH_ACCESS(if (_array.isStdVector() || image.dims() == 2), THROW_INFO(ERR_INFO_DIM));
	LZH_ACCESS(if (_array.isImage() || _array.isStdVector()), THROW_INFO(ERR_INFO_DIM));
#endif //LZH_DEBUG
	if (_array.isImage() && binaryImage) {
		Threshold(image, image, 0, 255, THRESH_OTSU);
	}
	//else if (_array.isStdVector()) {
	//	Rect rect = bouningRect(_array);
	//	Array img = Array::zeros(rect.width, rect.height, MAT_8U);
	//	drawContour(img, _array, Color::White);
	//	image = img;
	//}
	Moments mu;
	if (_array.isImage()) {
		FOR_RANGE(y, 0, image.rows) {
			FOR_RANGE(x, 0, image.cols) {
				float64 v = image.get<float64>(y, x);
				mu.m00 += v;
				mu.m01 += v * y;
				mu.m10 += v * x;
				mu.m20 += v * x * x;
				mu.m11 += v * x * y;
				mu.m02 += v * y * y;
				mu.m30 += v * x * x * x;
				mu.m21 += v * x * x * y;
				mu.m12 += v * x * y * y;
				mu.m03 += v * y * y * y;
			}
		}
		Point2d center = mu.center();
		FOR_RANGE(y, 0, image.rows) {
			FOR_RANGE(x, 0, image.cols) {
				float64 v = image.get<float64>(y, x);
				mu.mu20 += v * pow(x - center.x, 2);
				mu.mu11 += v * (x - center.x) * (y - center.y);
				mu.mu02 += v * pow(y - center.y, 2);
				mu.mu30 += v * pow(x - center.x, 3);
				mu.mu21 += v * pow(x - center.x, 2) * (y - center.y);
				mu.mu12 += v * (x - center.x) * pow(y - center.y, 2);
				mu.mu03 += v * pow(y - center.y, 3);
			}
		}
	}
	else if (_array.isStdVector()) {
		FOR_RANGE(i, 0, image.rows) {
			float64 v = 1;
			float64 x = image.row(i)(0);
			float64 y = image.row(i)(1);
			mu.m00 += v;
			mu.m01 += v * y;
			mu.m10 += v * x;
			mu.m20 += v * x * x;
			mu.m11 += v * x * y;
			mu.m02 += v * y * y;
			mu.m30 += v * x * x * x;
			mu.m21 += v * x * x * y;
			mu.m12 += v * x * y * y;
			mu.m03 += v * y * y * y;
		}
		Point2d center = mu.center();
		FOR_RANGE(i, 0, image.rows) {
			float64 v = 1;
			float64 x = image.row(i)(0);
			float64 y = image.row(i)(1);
			mu.mu20 += v * pow(x - center.x, 2);
			mu.mu11 += v * (x - center.x) * (y - center.y);
			mu.mu02 += v * pow(y - center.y, 2);
			mu.mu30 += v * pow(x - center.x, 3);
			mu.mu21 += v * pow(x - center.x, 2) * (y - center.y);
			mu.mu12 += v * (x - center.x) * pow(y - center.y, 2);
			mu.mu03 += v * pow(y - center.y, 3);
		}
	}
	mu.nu20 = mu.mu20 / pow(mu.m00, 2);
	mu.nu11 = mu.mu11 / pow(mu.m00, 2);
	mu.nu02 = mu.mu02 / pow(mu.m00, 2);
	mu.nu30 = mu.mu30 / pow(mu.m00, 2.5);
	mu.nu21 = mu.mu21 / pow(mu.m00, 2.5);
	mu.nu12 = mu.mu12 / pow(mu.m00, 2.5);
	mu.nu03 = mu.mu03 / pow(mu.m00, 2.5);
	return mu;
}
void lzh::HuMoments(const Moments & mu, mat_t hu[7])
{
	hu[0] = mu.nu20 + mu.nu02;
	hu[1] = (mu.nu20 - mu.nu02) * (mu.nu20 - mu.nu02) + 4 * mu.nu11 * mu.nu11;
	hu[2] = pow(mu.nu30 - 3 * mu.nu12, 2) + pow(3 * mu.nu21 - mu.nu03, 2);
	hu[3] = pow(mu.nu30 + mu.nu12, 2) + pow(mu.nu21 + mu.nu03, 2);
	hu[4] = (mu.nu30 - 3 * mu.nu12)*(mu.nu30 + mu.nu12)*(pow(mu.nu30 + mu.nu12, 2) - 3 * pow(mu.nu21 + mu.nu03, 2)) + (3 * mu.nu21 - mu.nu03)*(mu.nu21 + mu.nu03)*(3 * pow(mu.nu30 + mu.nu12, 2) - pow(mu.nu21 + mu.nu03, 2));
	hu[5] = (mu.nu20 - mu.nu02)*(pow(mu.nu30 + mu.nu12, 2) - pow(mu.nu21 + mu.nu03, 2)) + 4 * mu.nu11*(mu.nu30 + mu.nu12)*(mu.nu21 + mu.nu03);
	hu[6] = (3 * mu.nu21 - mu.nu03)*(mu.nu30 + mu.nu12)*(pow(mu.nu30 + mu.nu12, 2) - 3 * pow(mu.nu21 + mu.nu03, 2)) - (mu.nu30 - 3 * mu.nu12)*(mu.nu21 + mu.nu03)*(3 * pow(mu.nu30 + mu.nu12, 2) - pow(mu.nu21 + mu.nu03, 2));
}
void lzh::HuMoments(const Moments & m, OutputArray hu)
{
	Mat h(7);
	HuMoments(m, h.data());
	hu.set(h);
}
void lzh::SetPixel(InputOutputArray src_, Point point, int32 x, int32 y, const Color& color)
{
	Array src(src_);
	if (src.channels() == 3) {
		src.set(CHECK_INDEX_IMAGE(point.y + y, src.rows), CHECK_INDEX_IMAGE(point.x + x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -y, src.rows), CHECK_INDEX_IMAGE(point.x + x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + y, src.rows), CHECK_INDEX_IMAGE(point.x + -x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -y, src.rows), CHECK_INDEX_IMAGE(point.x + -x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + x, src.rows), CHECK_INDEX_IMAGE(point.x + y, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -x, src.rows), CHECK_INDEX_IMAGE(point.x + y, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + x, src.rows), CHECK_INDEX_IMAGE(point.x + -y, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -x, src.rows), CHECK_INDEX_IMAGE(point.x + -y, src.cols), color);
	}
	else {
		src.set(CHECK_INDEX_IMAGE(point.y + y, src.rows), CHECK_INDEX_IMAGE(point.x + x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -y, src.rows), CHECK_INDEX_IMAGE(point.x + x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + y, src.rows), CHECK_INDEX_IMAGE(point.x + -x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -y, src.rows), CHECK_INDEX_IMAGE(point.x + -x, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + x, src.rows), CHECK_INDEX_IMAGE(point.x + y, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -x, src.rows), CHECK_INDEX_IMAGE(point.x + y, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + x, src.rows), CHECK_INDEX_IMAGE(point.x + -y, src.cols), color);
		src.set(CHECK_INDEX_IMAGE(point.y + -x, src.rows), CHECK_INDEX_IMAGE(point.x + -y, src.cols), color);
	}
}
void lzh::BresenhamEllipse(OutputArray ellipsePoint, Point center, int32 a, int32 b)
{
	int32 sqa = a * a;
	int32 sqb = b * b;
	int32 x = 0;
	int32 y = b;
	int32 d = 2 * sqb - 2 * b * sqa + sqa;
	vec_p2_32s point;
	EllipsePlot(point, center, x, y);
	int32 P_x = Round((float32)sqa / sqrt((float32)(sqa + sqb)));
	while (x <= P_x){
		if (d < 0){
			d += 2 * sqb * (2 * x + 3);
		}
		else
		{
			d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
			y--;
		}
		x++;
		EllipsePlot(point, center, x, y);
	}
	d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
	while (y >= 0)
	{
		EllipsePlot(point, center, x, y);
		y--;
		if (d < 0)
		{
			x++;
			d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
		}
		else
		{
			d = d - 2 * sqa * y - sqa;
		}
	}
	ellipsePoint.set(point);
}
void lzh::drawEllipse(InputOutputArray circlePoints, Point point, int32 a, int32 b)
{
	Ellipse ellipse(point, a, b);
	vec_p2_32s points;
	Mat ps = ellipse(range(0, 1, 0.01));
	for (int32 i = 0; i < ps.cols(); i++) {
		points.push_back(ps.col(i).toPoint());
	}
	circlePoints.set(points);
}
void lzh::BresenhamCircle(InputOutputArray circlePoints, Point point, int32 radius, int32 lineWidth)
{
	vec_p2_32s contour;
	if (radius > 1) {
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			for (int32 x = 0, y = radius + wid, p = 3 - 2 * (radius + wid); x <= y; x++) {
				addContour(contour, point, x, y);
				if (p >= 0) {
					p += 4 * (x - y) + 10;
					y--;
				}
				else {
					p += 4 * x + 6;
				}
			}
	}
	else {
		contour.push_back(point);
	}
	circlePoints.createSameSize(contour, circlePoints.depth());
	circlePoints.setTo(contour);
}
void lzh::BresenhamCircle(InputOutputArray src_, Point point, int32 radius, const Color& color, int32 lineWidth, bool fill)
{
	Array src(src_);
	if (radius > 1) {
		radius -= 1;
		//vec_p2_32s contour;
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			for (int32 x = 0, y = radius + wid, p = 3 - 2 * (radius + wid); x <= y; x++) {
				SetPixel(src, point, x, y, color);
				//addContour(contour, point, x, y, src.rows(), src.cols());
				if (p >= 0) {
					p += 4 * (x - y) + 10;
					y--;
				}
				else {
					p += 4 * x + 6;
				}
			}
	}
	else {
		if (src.channels() == 3) src.set(CHECK_INDEX_IMAGE(point.y, src.rows), CHECK_INDEX_IMAGE(point.x, src.cols), color);
		else src.set(CHECK_INDEX_IMAGE(point.y, src.rows), CHECK_INDEX_IMAGE(point.x, src.cols), color);
	}
	if (fill)floodFill(src, point, color);
}
void lzh::drawPoint(InputOutputArray src_, Point point, const Color& color)
{
	Array src(src_);
	if (src.channels() == 3) {
		src.set(CHECK_INDEX_IMAGE(point.y, src.rows), CHECK_INDEX_IMAGE(point.x, src.cols), color);
	}
	else {
		src.set(CHECK_INDEX_IMAGE(point.y, src.rows), CHECK_INDEX_IMAGE(point.x, src.cols), color);
	}
}
void lzh::circle(InputOutputArray src, Point p, int32 radius, const Color& color, int32 lineWidth, bool fill)
{
	BresenhamCircle(src, p, radius, color, lineWidth, fill);
}
void lzh::circle(InputOutputArray src, int32 x, int32 y, int32 radius, const Color& color, int32 lineWidth, bool fill)
{
	BresenhamCircle(src, Point(x, y), radius, color, lineWidth, fill);
}
void lzh::drawFill(InputOutputArray src, vec_p2_32s& contour, const Color& color)
{
}


void lzh::lines(OutputArray src, Point p1, Point p2)
{
	if (!src.empty())
		src.clear();
	int32 x1 = p1.x;
	int32 x2 = p2.x;
	int32 y1 = p1.y;
	int32 y2 = p2.y;
	int32 dx, dy;//横轴纵轴差量
	int32 e;
	int32 x, y;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	y = y1;
	x = x1;
	int32 cx, cy;//表明x、y方向的增量  
	vec_p2_32s vec;
	if (x1 > x2) {
		cx = -1;//x递减方向  
	}
	else {
		cx = 1;//x递增方向  
	}
	if (y1 > y2) {
		cy = -1;//y递减方向  
	}
	else {
		cy = 1;//y递增方向  
	}
	if (dx == 0 && dy == 0) {
		//return; src.at<Vec3b>(CHECK_INDEX_IMAGE(y, row), CHECK_INDEX_IMAGE(x, col)) = color;
	}
	else if (dy == 0) {
		for (x = x1; (cx == 1 ? x <= x2 : x >= x2); x += cx) {
			vec.push_back(Point(x, y));
		}
	}
	else if (dx == 0) {
		for (y = y1; (cy == 1 ? y <= y2 : y >= y2); y += cy) {
			vec.push_back(Point(x, y));
		}
	}
	else if (dx >= dy) {
		e = -dx;
		for (x = x1; (cx == 1 ? x <= x2 : x >= x2); x += cx) {
			vec.push_back(Point(x, y));
			e += dy << 1;
			if (e > 0) {
				y += cy;
				e -= dx << 1;
			}
		}
	}
	else {
		e = -dy;
		for (y = y1; (cy >= 0 ? y <= y2 : y >= y2); y += cy) {
			vec.push_back(Point(x, y));
			e += dx << 1;
			if (e > 0) {
				x += cx;
				e -= dy << 1;
			}

		}
	}
	src.set(vec);
}
void lzh::lines(InputOutputArray src_, Point p1, Point p2, const Color& color, int32 lineWidth, bool fill)
{
	Array src(src_);
	int32 row = src.rows;
	int32 col = src.cols;
	int32 thickness = lineWidth;
	int32 x1 = p1.x;
	int32 x2 = p2.x;
	int32 y1 = p1.y;
	int32 y2 = p2.y;
	int32 dx, dy;//横轴纵轴差量
	int32 e;
	int32 x, y;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	y = y1;
	x = x1;
	int32 cx, cy;//表明x、y方向的增量  
	if (x1 > x2) {
		cx = -1;//x递减方向  
	}
	else {
		cx = 1;//x递增方向  
	}
	if (y1 > y2) {
		cy = -1;//y递减方向  
	}
	else {
		cy = 1;//y递增方向  
	}
	if (dx == 0 && dy == 0) {
		//return; src.set(CHECK_INDEX_IMAGE(y, row), CHECK_INDEX_IMAGE(x, col), color);
	}
	else if (dy == 0) {
		for (x = x1; (cx == 1 ? x <= x2 : x >= x2); x += cx) {
			src.set(CHECK_INDEX_IMAGE(y, row), CHECK_INDEX_IMAGE(x, col), color);
			for (int32 i = 1; i <= thickness; i++)
				src.set(CHECK_INDEX_IMAGE(y + i, row), CHECK_INDEX_IMAGE(x, col), color);
		}
	}
	else if (dx == 0) {
		for (y = y1; (cy == 1 ? y <= y2 : y >= y2); y += cy) {
			src.set(CHECK_INDEX_IMAGE(y, row), CHECK_INDEX_IMAGE(x, col), color);
			for (int32 i = 1; i <= thickness; i++)
				src.set(CHECK_INDEX_IMAGE(y, row), CHECK_INDEX_IMAGE(x + i, col), color);
		}
	}
	else if (dx >= dy) {
		e = -dx;
		for (x = x1; (cx == 1 ? x <= x2 : x >= x2); x += cx) {
			src.set(CHECK_INDEX_IMAGE(y, row), CHECK_INDEX_IMAGE(x, col), color);
			e += dy << 1;
			if (e > 0) {
				y += cy;
				e -= dx << 1;
			}
		}
	}
	else {
		e = -dy;
		for (y = y1; (cy >= 0 ? y <= y2 : y >= y2); y += cy) {
			src.set(CHECK_INDEX_IMAGE(y, row), CHECK_INDEX_IMAGE(x, col), color);
			e += dx << 1;
			if (e > 0) {
				x += cx;
				e -= dy << 1;
			}

		}
	}
}
void lzh::lines(InputOutputArray src, InputArray points, const Color& color, int32 lineWidth, bool fill)
{
	Array contours(points);
	FOR_RANGE(i, 0, contours.rows - 1)
		lines(src, contours.row(i).toPoint(), contours.row(i + 1).toPoint(), color, lineWidth, fill);
	lines(src, contours.row(contours.rows - 1).toPoint(), contours.row(0).toPoint(), color, lineWidth, fill);
}
void lzh::crossShape(OutputArray src, Point center, Size size, int32 lineWidth, Size in_size)
{
	vec_p2_32s ps;
	for (int32 i = -size.w / 2; i <= size.w / 2; ++i)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			if (abs(i) > in_size.w)ps.push_back(Point(center.x + i, center.y + wid));
	for (int32 i = -size.h / 2; i <= size.h / 2; ++i)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			if (abs(i) > in_size.h)ps.push_back(Point(center.x + wid, center.y + i));
	src.set(ps);
}
void lzh::crossShape(InputOutputArray src, int32 x, int32 y, int32 width, int32 height, const Color & color, int32 lineWidth, int32 in_width, int32 in_height)
{
	crossShape(src, Point(x, y), Size(width, height), color, lineWidth, Size(in_width, in_height));
}
void lzh::crossShape(InputOutputArray src_, Point center, Size size, const Color & color, int32 lineWidth, Size in_size)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	for (int32 i = -size.w / 2; i <= size.w / 2; ++i)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			if (abs(i) >= in_size.w)src.set(CHECK_INDEX_IMAGE(center.y + wid, src.rows), CHECK_INDEX_IMAGE(center.x + i, src.cols), color);
	for (int32 i = -size.h / 2; i <= size.h / 2; ++i)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			if (abs(i) >= in_size.h)src.set(CHECK_INDEX_IMAGE(center.y + i, src.rows), CHECK_INDEX_IMAGE(center.x + wid, src.cols), color);
}
void lzh::rectangle(InputOutputArray src_, int32 x1, int32 y1, int32 x2, int32 y2, const Color& color, int32 lineWidth, bool fill)
{
	Array src(src_);
	int32 xmin = MIN(x1, x2);
	int32 ymin = MIN(y1, y2);
	int32 xmax = MAX(x1, x2);
	int32 ymax = MAX(y1, y2);
	x1 = xmin;
	x2 = xmax;
	y1 = ymin;
	y2 = ymax;
	for (int32 col = MAX(x1, 0); col <= MIN(src.cols - 1, x2); ++col)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			src.set(CHECK_INDEX_IMAGE(y1 + wid, src.rows), CHECK_INDEX_IMAGE(col, src.cols), color);
	for (int32 col = MAX(x1, 0); col <= MIN(src.cols - 1, x2); ++col)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			src.set(CHECK_INDEX_IMAGE(y2 + wid, src.rows), CHECK_INDEX_IMAGE(col, src.cols), color);
	for (int32 row = MAX(y1, 0); row <= MIN(src.rows - 1, y2); ++row)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			src.set(CHECK_INDEX_IMAGE(row + wid, src.rows), CHECK_INDEX_IMAGE(x1 + wid, src.cols), color);
	for (int32 row = MAX(y1, 0); row <= MIN(src.rows - 1, y2); ++row)
		for (int32 wid = -lineWidth / 2; wid <= lineWidth / 2; ++wid)
			src.set(CHECK_INDEX_IMAGE(row + wid, src.rows), CHECK_INDEX_IMAGE(x2 + wid, src.cols), color);
	if (fill)
	{
		for (int32 col = MAX(x1, 0); col <= MIN(src.cols - 1, x2); ++col)
			for (int32 row = MAX(y1, 0); row <= MIN(src.rows - 1, y2); ++row)
				src.set(row, col, color);
	}
}
void lzh::rectangle(InputOutputArray src, Rect rect, const Color& color, int32 lineWidth, bool fill)
{
	rectangle(src, rect.x, rect.y, rect.x + rect.width, rect.y + rect.height, color, lineWidth, fill);
}
void lzh::rectangle(InputOutputArray src, Point p1, Point p2, const Color & color, int32 lineWidth, bool fill)
{
	rectangle(src, p1.x, p1.y, p2.x, p2.y, color, lineWidth, fill);
}
void lzh::drawContours(InputOutputArray src, InputArray contours_, const Color& color, int32 radius, int32 lineWidth, bool fill)
{
	Array contours(contours_);
	FOR_RANGE(i, 0, contours.rows)
		circle(src, contours.row(i).toPoint(), radius, color, fill);
}
void lzh::drawContours(InputOutputArray src_, const vecs_p2_32s& contours, int32 index, int32 radius, const Color& color, int32 lineWidth, bool fill)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	if (index == -1) {
		for (const std::vector<Point>& contour : contours) {
			for (const Point& p : contour)
				circle(src, p, radius, color, lineWidth);
			if (fill) floodFill(src, bouningCenter(contour), color);
		}
	}
	else {
		for (const Point& p : contours[index])
			circle(src, p, radius, color, lineWidth);
		if (fill) floodFill(src, bouningCenter(contours[index]), color);
	}
}
void lzh::drawContour(InputOutputArray src, const vec_p2_32s& contours, const Color& color, int32 radius, int32 lineWidth, bool fill)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	for (const Point& p : contours)
		circle(src, p, radius, color, lineWidth);
	if (fill) floodFill(src, bouningCenter(contours), color);
}
void lzh::drawContour(InputOutputArray src, InputArray contours, const Color & color, int32 radius, int32 lineWidth, bool fill)
{
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	CHECK_MATRIX_(contours);
#endif
	Array contour(contours);
	FOR_RANGE(i, 0, contour.rows) {
		Point p = contour.row(i).toPoint();
		circle(src, p, radius, color, lineWidth);
	}
	if (fill) floodFill(src, bouningCenter(contours), color);
}
void lzh::projection(InputArray src_, OutputArray vertical_, OutputArray horizontal_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	if (src.empty())return;
	Array horizontal = Array::zeros(src.cols, 1, src.channels(), MAT_T);
	Array vertical = Array::zeros(1, src.rows, src.channels(), MAT_T);
	FOR_IMAGE_(i, src, 1) {
		FOR_IMAGE_(j, src, 2) {
			FOR_IMAGE_(k, src, 3) {
				horizontal.at<mat_t>(0, j, k) += src(i, j, k);
				vertical.at<mat_t>(i, 0, k) += src(i, j, k);
			}
		}
	}
	vertical_.set(vertical);
	horizontal_.set(horizontal);
}
void lzh::verticalProjection(InputArray src_, OutputArray vertical_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	Array vertical = Array::zeros(1, src.rows, src.channels(), MAT_T);
	FOR_IMAGE_(i, src, 1) {
		FOR_IMAGE_(k, src, 3) {
			mat_t sum = 0;
			FOR_IMAGE_(j, src, 2) {
				sum += src(i, j, k);
			}
			vertical.at<mat_t>(i, 0, k) = sum;
		}
	}
	vertical_.set(vertical);
}
void lzh::horizontalProjection(InputArray src_, OutputArray horizontal_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	Array horizontal = Array::zeros(src.cols, 1, src.channels(), MAT_T);
	FOR_IMAGE_(i, src, 2) {
		FOR_IMAGE_(k, src, 3) {
			mat_t sum = 0;
			FOR_IMAGE_(j, src, 1) {
				sum += src(j, i, k);
			}
			horizontal.at<mat_t>(0, i, k) = sum;
		}
	}
	horizontal_.set(horizontal);
}
void lzh::mosaic(InputArray src_, OutputArray dst, Rect mosaicRange, int32 ksize, MosaicMethod method)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif //LZH_DEBUG
	Array out = src.clone();
	for (int32 h = mosaicRange.y; h < mosaicRange.y + mosaicRange.height; h += ksize)
		for (int32 w = mosaicRange.x; w < mosaicRange.x + mosaicRange.width; w += ksize)
			mosaic(out, Point(w, h), ksize, method);
	dst = out;
}
void lzh::mosaic(InputOutputArray image, Point mosaicRange, int32 ksize, MosaicMethod method)
{
	Array img(image);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	int32 i, j;
	switch (MosaicAngle0)
	{
	case lzh::MosaicRandom:i = rand() % ksize; j = rand() % ksize; break;
	case lzh::MosaicAngle0:i = ksize / 2; j = ksize - 1; break;
	case lzh::MosaicAngle45:i = 0; j = ksize - 1; break;
	case lzh::MosaicAngle90:i = 0; j = ksize / 2; break;
	case lzh::MosaicAngle135:i = 0; j = 0; break;
	case lzh::MosaicAngle180:i = ksize / 2; j = 0; break;
	case lzh::MosaicAngle225:i = ksize - 1; j = 0; break;
	case lzh::MosaicAngle270:i = ksize - 1; j = ksize / 2; break;
	case lzh::MosaicAngle315:i = ksize - 1; j = ksize - 1; break;
	default:break;
	}
	for (int32 z = 0; z < img.channels(); z++) {
		mat_t v = img(mosaicRange.y + i, mosaicRange.x + j, z);
		for (int32 py = 0; py < ksize; ++py) {
			for (int32 px = 0; px < ksize; ++px) {
				img.set(mosaicRange.y + py, mosaicRange.x + px, z, v);
			}
		}
	}
}
void lzh::mosaic(InputArray src, OutputArray dst, vec_p2_32s mosaicRange, int32 ksize, MosaicMethod method)
{
	Array img(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Array out = img.clone();
	for (Point& p : mosaicRange)
		mosaic(out, p, ksize, method);
	dst.set(out);
}

void lzh::histogram(InputArray src, OutputArray _histogram)
{
	Array img(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Array histogram_ = Array::zeros(256, img.channels(), MAT_32S);
	for (int32 c = 0; c < img.channels(); c++) {
		for (int32 row = 0; row < img.rows; row++) {
			for (int32 col = 0; col < img.cols; col++) {
				histogram_.at<int32>(c, lzh::saturate_cast<uint8>(Round(img(row, col, c)))) += 1;
			}
		}
	}
	_histogram.set(histogram_);
}

void lzh::equalizeHistogram(InputArray src, OutputArray dst)
{
	Array img(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif //LZH_DEBUG
	Array _histogram;
	histogram(img, _histogram);
	int32 numberOfPixel = img.size().area();
	Array LUT(256, MAT_8U);
	int32 sum = 0;
	for (int32 i = 0; i < _histogram.len(); ++i)
	{
		sum += _histogram.at<int32>(i);
		LUT.at<uint8>(i) = _I(1.0*sum / numberOfPixel * 255);
	}
	Array res(img.size3(), img.depth(), img.colorGamut());
	for (int32 i = 0; i < img.rows; ++i) {
		for (int32 j = 0; j < img.cols; ++j) {
			res.set(i, j, LUT.at<uint8>(img.get<int32>(i, j)));
		}
	}
	dst.set(res);
}

void cvtColor::ConverTo(InputArray src, OutputArray dst, ColorGamut code)
{
	Array img(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(img);
#endif
	ColorGamut cg = img.colorGamut();
	if (code == cg) {
		dst.set(img);
	}
	else {
		switch (cg)
		{
		case lzh::ColorGamutCustom:
			switch (code)
			{
			case lzh::ColorGamutGray:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2GRAY(src, dst);
				else if (img.channels() == 4)
					RGBA2GRAY(src, dst);
				else if (img.channels() == 1)
					dst.set(img);
				break;
			case lzh::ColorGamutRGB:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					dst.set(img);
				else if (img.channels() == 4)
					RGBA2RGB(src, dst);
				else if (img.channels() == 1)
					GRAY2RGB(src, dst);
				break;
			case lzh::ColorGamutBGR:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					dst.set(img);
				else if (img.channels() == 4)
					BGRA2BGR(src, dst);
				else if (img.channels() == 1)
					GRAY2BGR(src, dst);
				break;
			case lzh::ColorGamutRGBA:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2RGBA(src, dst);
				else if (img.channels() == 4)
					dst.set(img);
				else if (img.channels() == 1)
					GRAY2RGBA(src, dst);
				break;
			case lzh::ColorGamutBGRA:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2BGRA(src, dst);
				else if (img.channels() == 4)
					dst.set(img);
				else if (img.channels() == 1)
					GRAY2BGRA(src, dst);
				break;
			case lzh::ColorGamutCMYK:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2CMYK(src, dst);
				else if (img.channels() == 4)
					dst.set(img);
				else if (img.channels() == 1) {
					GRAY2RGB(src, dst);
					RGB2BGRA(dst, dst);
				}
				break;
			case lzh::ColorGamutHSV:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2HSV(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					RGB2HSV(dst, dst);
				}
				else if (img.channels() == 1) {
					GRAY2RGB(src, dst);
					RGB2HSV(dst, dst);
				}
				break;
			case lzh::ColorGamutHSL:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2HSL(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					RGB2HSL(dst, dst);
				}
				else if (img.channels() == 1) {
					GRAY2RGB(src, dst);
					RGB2HSL(dst, dst);
				}
				break;
			case lzh::ColorGamutXYZ:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2XYZ(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					RGB2XYZ(dst, dst);
				}
				else if (img.channels() == 1) {
					GRAY2RGB(src, dst);
					RGB2XYZ(dst, dst);
				}
				break;
			case lzh::ColorGamutYXY:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2YXY(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					RGB2YXY(dst, dst);
				}
				else if (img.channels() == 1) {
					GRAY2RGB(src, dst);
					RGB2YXY(dst, dst);
				}
				break;
			case lzh::ColorGamutLab:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2Lab(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					RGB2Lab(dst, dst);
				}
				else if (img.channels() == 1) {
					GRAY2RGB(src, dst);
					RGB2Lab(dst, dst);
				}
				break;
			case lzh::ColorGamutLuv:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					RGB2Luv(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					RGB2Luv(dst, dst);
				}
				else if (img.channels() == 1) {
					GRAY2RGB(src, dst);
					RGB2Luv(dst, dst);
				}
				break;
			case lzh::ColorGamutMean:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					toMean(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					toMean(dst, dst);
				}
				else if (img.channels() == 1) {
					dst.set(img);
				}
				break;
			case lzh::ColorGamutMaxMin:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					toMaxMin(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					toMaxMin(dst, dst);
				}
				else if (img.channels() == 1) {
					dst.set(img);
				}
				break;
			case lzh::ColorGamutR:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					toRad(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					toRad(dst, dst);
				}
				else if (img.channels() == 1) {
					dst.set(img);
				}
				break;
			case lzh::ColorGamutG:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					toGreen(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					toGreen(dst, dst);
				}
				else if (img.channels() == 1) {
					dst.set(img);
				}
				break;
			case lzh::ColorGamutB:
				LZH_ACCESS(if (!(img.channels() == 2 || img.channels() > 4)), THROW_INFO_STR("通道上不符合要求"));
				if (img.channels() == 3)
					toBlue(src, dst);
				else if (img.channels() == 4) {
					RGBA2RGB(src, dst);
					toBlue(dst, dst);
				}
				else if (img.channels() == 1) {
					dst.set(img);
				}
				break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutGray:
			switch (code)
			{
			case lzh::ColorGamutMean:
			case lzh::ColorGamutMaxMin:
			case lzh::ColorGamutR:
			case lzh::ColorGamutG:
			case lzh::ColorGamutB:
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutRGB:GRAY2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:GRAY2BGR(src, dst); break;
			case lzh::ColorGamutRGBA:GRAY2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:GRAY2BGRA(src, dst); break;
			case lzh::ColorGamutHSL:GRAY2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutHSV:GRAY2RGB(src, dst); RGB2HSV(dst, dst); break;
			case lzh::ColorGamutXYZ:GRAY2RGB(src, dst); RGB2XYZ(dst, dst); break;
			case lzh::ColorGamutYXY:GRAY2RGB(src, dst); RGB2YXY(dst, dst); break;
			case lzh::ColorGamutLab:GRAY2RGB(src, dst); RGB2Lab(dst, dst); break;
			case lzh::ColorGamutLuv:GRAY2RGB(src, dst); RGB2Luv(dst, dst); break;
			case lzh::ColorGamutCMYK:GRAY2RGB(src, dst); RGB2CMYK(dst, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutRGB:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:RGB2GRAY(src, dst); break;
			case lzh::ColorGamutBGR:RGB2BGR(src, dst); break;
			case lzh::ColorGamutRGBA:RGB2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:RGB2BGRA(src, dst); break;
			case lzh::ColorGamutCMYK:RGB2CMYK(src, dst); break;
			case lzh::ColorGamutHSL:RGB2HSL(src, dst); break;
			case lzh::ColorGamutHSV:RGB2HSV(src, dst); break;
			case lzh::ColorGamutXYZ:RGB2XYZ(src, dst); break;
			case lzh::ColorGamutYXY:RGB2YXY(src, dst); break;
			case lzh::ColorGamutLab:RGB2Lab(src, dst); break;
			case lzh::ColorGamutLuv:RGB2Luv(src, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutBGR:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:BGR2GRAY(src, dst); break;
			case lzh::ColorGamutRGB:BGR2RGB(src, dst);	break;
			case lzh::ColorGamutRGBA:BGR2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:BGR2BGRA(src, dst); break;
			case lzh::ColorGamutCMYK:BGR2CMYK(src, dst); break;
			case lzh::ColorGamutHSL:BGR2HSL(src, dst); break;
			case lzh::ColorGamutHSV:BGR2HSV(src, dst); break;
			case lzh::ColorGamutXYZ:BGR2XYZ(src, dst); break;
			case lzh::ColorGamutYXY:BGR2YXY(src, dst); break;
			case lzh::ColorGamutLab:BGR2Lab(src, dst); break;
			case lzh::ColorGamutLuv:BGR2Luv(src, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutRGBA:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:RGBA2GRAY(src, dst); break;
			case lzh::ColorGamutRGB:RGBA2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:RGBA2BGR(src, dst);	break;
			case lzh::ColorGamutBGRA:RGBA2BGRA(src, dst); break;
			case lzh::ColorGamutCMYK:RGBA2CMYK(src, dst); break;
			case lzh::ColorGamutHSL:RGBA2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutHSV:RGBA2RGB(src, dst); RGB2HSV(dst, dst); break;
			case lzh::ColorGamutXYZ:RGBA2RGB(src, dst); RGB2XYZ(dst, dst); break;
			case lzh::ColorGamutYXY:RGBA2RGB(src, dst); RGB2YXY(dst, dst); break;
			case lzh::ColorGamutLab:RGBA2RGB(src, dst); RGB2Lab(dst, dst); break;
			case lzh::ColorGamutLuv:RGBA2RGB(src, dst); RGB2Luv(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutBGRA:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:BGRA2GRAY(src, dst); break;
			case lzh::ColorGamutRGB:BGRA2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:BGRA2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:BGRA2RGBA(src, dst); break;
			case lzh::ColorGamutCMYK:BGRA2CMYK(src, dst); break;
			case lzh::ColorGamutHSL:BGRA2BGR(src, dst); BGR2HSL(dst, dst); break;
			case lzh::ColorGamutHSV:BGRA2BGR(src, dst); BGR2HSV(dst, dst); break;
			case lzh::ColorGamutXYZ:BGRA2BGR(src, dst); BGR2XYZ(dst, dst); break;
			case lzh::ColorGamutYXY:BGRA2BGR(src, dst); BGR2YXY(dst, dst); break;
			case lzh::ColorGamutLab:BGRA2BGR(src, dst); BGR2Lab(dst, dst); break;
			case lzh::ColorGamutLuv:BGRA2BGR(src, dst); BGR2Luv(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutHSV:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:HSV2RGB(src, dst); RGB2GRAY(dst, dst); break;
			case lzh::ColorGamutRGB:HSV2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:HSV2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:HSV2RGB(src, dst); RGB2RGBA(dst, dst); break;
			case lzh::ColorGamutBGRA:HSV2BGR(src, dst); BGR2BGRA(dst, dst); break;
			case lzh::ColorGamutCMYK:HSV2RGB(src, dst); RGB2CMYK(dst, dst); break;
			case lzh::ColorGamutHSL:HSV2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutXYZ:HSV2RGB(src, dst); RGB2XYZ(dst, dst); break;
			case lzh::ColorGamutYXY:HSV2RGB(src, dst); RGB2YXY(dst, dst); break;
			case lzh::ColorGamutLab:HSV2RGB(src, dst); RGB2Lab(dst, dst); break;
			case lzh::ColorGamutLuv:HSV2RGB(src, dst); RGB2Luv(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutHSL:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:HSL2RGB(src, dst); RGB2GRAY(dst, dst); break;
			case lzh::ColorGamutRGB:HSL2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:HSL2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:HSL2RGB(src, dst); RGB2RGBA(dst, dst); break;
			case lzh::ColorGamutBGRA:HSL2BGR(src, dst); BGR2BGRA(dst, dst); break;
			case lzh::ColorGamutCMYK:HSL2RGB(src, dst); RGB2CMYK(dst, dst); break;
			case lzh::ColorGamutHSV:HSL2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutXYZ:HSL2RGB(src, dst); RGB2XYZ(dst, dst); break;
			case lzh::ColorGamutYXY:HSL2RGB(src, dst); RGB2YXY(dst, dst); break;
			case lzh::ColorGamutLab:HSL2RGB(src, dst); RGB2Lab(dst, dst); break;
			case lzh::ColorGamutLuv:HSL2RGB(src, dst); RGB2Luv(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutXYZ:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:XYZ2RGB(src, dst); RGB2GRAY(dst, dst); break;
			case lzh::ColorGamutRGB:XYZ2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:XYZ2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:XYZ2RGB(src, dst); RGB2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:XYZ2BGR(src, dst); BGR2BGRA(src, dst); break;
			case lzh::ColorGamutCMYK:XYZ2RGB(src, dst); RGB2CMYK(dst, dst); break;
			case lzh::ColorGamutHSV:XYZ2RGB(src, dst); RGB2HSV(dst, dst); break;
			case lzh::ColorGamutHSL:XYZ2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutYXY:XYZ2YXY(src, dst); break;
			case lzh::ColorGamutLuv:XYZ2Luv(src, dst); break;
			case lzh::ColorGamutLab:XYZ2Lab(src, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutYXY:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:YXY2RGB(src, dst); RGB2GRAY(dst, dst); break;
			case lzh::ColorGamutRGB:YXY2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:YXY2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:YXY2RGB(src, dst); RGB2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:YXY2BGR(src, dst); BGR2BGRA(src, dst); break;
			case lzh::ColorGamutCMYK:YXY2RGB(src, dst); RGB2CMYK(dst, dst); break;
			case lzh::ColorGamutHSV:YXY2RGB(src, dst); RGB2HSV(dst, dst); break;
			case lzh::ColorGamutHSL:YXY2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutXYZ:YXY2XYZ(src, dst); break;
			case lzh::ColorGamutLuv:YXY2XYZ(src, dst); XYZ2Luv(dst, dst); break;
			case lzh::ColorGamutLab:YXY2XYZ(src, dst); XYZ2Lab(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutLab:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:Lab2RGB(src, dst); RGB2GRAY(dst, dst); break;
			case lzh::ColorGamutRGB:Lab2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:Lab2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:Lab2RGB(src, dst); RGB2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:Lab2BGR(src, dst); BGR2BGRA(src, dst); break;
			case lzh::ColorGamutCMYK:Lab2RGB(src, dst); RGB2CMYK(dst, dst); break;
			case lzh::ColorGamutHSV:Lab2RGB(src, dst); RGB2HSV(dst, dst); break;
			case lzh::ColorGamutHSL:Lab2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutXYZ:Lab2XYZ(src, dst); break;
			case lzh::ColorGamutYXY:Lab2XYZ(src, dst); XYZ2YXY(dst, dst); break;
			case lzh::ColorGamutLuv:Lab2XYZ(src, dst); XYZ2Luv(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutLuv:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:Luv2RGB(src, dst); RGB2GRAY(dst, dst); break;
			case lzh::ColorGamutRGB:Luv2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:Luv2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:Luv2RGB(src, dst); RGB2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:Luv2BGR(src, dst); BGR2BGRA(src, dst); break;
			case lzh::ColorGamutCMYK:Luv2RGB(src, dst); RGB2CMYK(dst, dst); break;
			case lzh::ColorGamutHSV:Luv2RGB(src, dst); RGB2HSV(dst, dst); break;
			case lzh::ColorGamutHSL:Luv2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutXYZ:Luv2XYZ(src, dst); break;
			case lzh::ColorGamutYXY:Luv2XYZ(src, dst); XYZ2YXY(dst, dst); break;
			case lzh::ColorGamutLab:Luv2XYZ(src, dst); XYZ2Lab(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		case lzh::ColorGamutCMYK:
			switch (code)
			{
			case lzh::ColorGamutCustom:dst.set(src); break;
			case lzh::ColorGamutGray:CMYK2RGB(src, dst); RGB2GRAY(dst, dst); break;
			case lzh::ColorGamutRGB:CMYK2RGB(src, dst);	break;
			case lzh::ColorGamutBGR:CMYK2BGR(src, dst);	break;
			case lzh::ColorGamutRGBA:CMYK2RGBA(src, dst); break;
			case lzh::ColorGamutBGRA:CMYK2BGRA(src, dst); break;
			case lzh::ColorGamutHSV:CMYK2RGB(src, dst); RGB2HSV(dst, dst); break;
			case lzh::ColorGamutHSL:CMYK2RGB(src, dst); RGB2HSL(dst, dst); break;
			case lzh::ColorGamutXYZ:CMYK2RGB(src, dst);  RGB2YXY(dst, dst); break;
			case lzh::ColorGamutYXY:CMYK2RGB(src, dst); RGB2YXY(dst, dst); break;
			case lzh::ColorGamutLab:CMYK2RGB(src, dst); RGB2Lab(dst, dst); break;
			case lzh::ColorGamutLuv:CMYK2RGB(src, dst); RGB2Luv(dst, dst); break;
			case lzh::ColorGamutMean:toMean(src, dst); break;
			case lzh::ColorGamutMaxMin:toMaxMin(src, dst); break;
			case lzh::ColorGamutR:toRad(src, dst); break;
			case lzh::ColorGamutG:toGreen(src, dst); break;
			case lzh::ColorGamutB:toBlue(src, dst); break;
			default:THROW_INFO_STR("不支持的色域");
			}
			break;
		default:THROW_INFO_STR("不支持的色域");
		}
	}
}
Array cvtColor::Convert(InputArray src, ColorGamut code)
{
	Array dst; ConverTo(src, dst, code);
	return dst;
}
void cvtColor::CMYK2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutCMYK || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutRGB, Color::Cmyk, Color::Rgb);
}
void cvtColor::CMYK2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutCMYK || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Cmyk, Color::Bgr);
}
void lzh::cvtColor::CMYK2RGBA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutCMYK || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array RGBA(src.size3(), src.depth(), ColorGamutRGBA);
	FOR_RANGE(i, 0, src.rows) {
		FOR_RANGE(j, 0, src.cols) {
			Color pix = src.pixel(i, j, Color::Cmyk);
			if (src.depth() == MAT_8U) pix = pix.inv();
			RGBA.setPixel(i, j, pix.toRgb().Alpha(255));
		}
	}
	dst.set(RGBA);
}
void lzh::cvtColor::CMYK2BGRA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutCMYK || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array BGRA(src.size3(), src.depth(), ColorGamutBGRA);
	FOR_RANGE(i, 0, src.rows) {
		FOR_RANGE(j, 0, src.cols) {
			Color pix = src.pixel(i, j, Color::Cmyk);
			if (src.depth() == MAT_8U) pix = pix.inv();
			BGRA.setPixel(i, j, pix.toBgr().Alpha(255));
		}
	}
	dst.set(BGRA);
}
void cvtColor::RGB2CMYK(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutCMYK, Color::Rgb, Color::Cmyk);
}
void cvtColor::BGR2CMYK(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutCMYK, Color::Bgr, Color::Cmyk);
}
void cvtColor::RGBA2CMYK(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGBA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutCMYK, Color::Rgb, Color::Cmyk);
}
void cvtColor::BGRA2CMYK(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGRA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutCMYK, Color::Bgr, Color::Cmyk);
}
void cvtColor::Luv2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutLuv || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutRGB, Color::Luv, Color::Rgb);
}
void cvtColor::Luv2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutLuv || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Luv, Color::Bgr);
}
void cvtColor::RGB2Luv(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutLuv, Color::Rgb, Color::Luv);
}
void cvtColor::BGR2Luv(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutLuv, Color::Bgr, Color::Luv);
}
void cvtColor::Luv2XYZ(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3 || src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutLuv || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutXYZ, Color::Luv, Color::Xyz);
}
void cvtColor::XYZ2Luv(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutXYZ || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutLuv, Color::Xyz, Color::Luv);
}
void cvtColor::Lab2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutLab || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutRGB, Color::Lab, Color::Rgb);
}
void cvtColor::Lab2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutLab || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Lab, Color::Bgr);
}
void cvtColor::RGB2Lab(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutLab, Color::Rgb, Color::Lab);
}
void cvtColor::BGR2Lab(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutLab, Color::Bgr, Color::Lab);
}
void cvtColor::Lab2XYZ(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3 || src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutLab || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutXYZ, Color::Lab, Color::Xyz);
}
void cvtColor::XYZ2Lab(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutXYZ || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutLab, Color::Xyz, Color::Lab);
}
void cvtColor::XYZ2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutXYZ || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Xyz, Color::Rgb);
}
void cvtColor::XYZ2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutXYZ || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Xyz, Color::Bgr);
}
void cvtColor::RGB2XYZ(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutXYZ, Color::Rgb, Color::Xyz);
}
void cvtColor::BGR2XYZ(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutXYZ, Color::Bgr, Color::Xyz);
}
void cvtColor::XYZ2YXY(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutXYZ || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutYXY, Color::Xyz, Color::Yxy);
}
void cvtColor::YXY2XYZ(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutYXY || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutXYZ, Color::Yxy, Color::Xyz);
}
void cvtColor::YXY2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutYXY || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Yxy, Color::Bgr);
}
void cvtColor::YXY2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutYXY || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutRGB, Color::Yxy, Color::Rgb);
}
void cvtColor::BGR2YXY(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutYXY, Color::Bgr, Color::Yxy);
}
void cvtColor::RGB2YXY(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutYXY, Color::Rgb, Color::Yxy);
}
void cvtColor::HSV2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutHSV || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Hsv, Color::Bgr);
}
void cvtColor::HSV2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutHSV || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutRGB, Color::Hsv, Color::Rgb);
}
void cvtColor::BGR2HSV(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutHSV, Color::Bgr, Color::Hsv);
}
void cvtColor::RGB2HSV(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutHSV, Color::Rgb, Color::Hsv);
}
void cvtColor::HSL2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutHSL || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutBGR, Color::Hsl, Color::Bgr);
}
void cvtColor::HSL2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutHSL || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutRGB, Color::Hsl, Color::Rgb);
}
void cvtColor::RGB2HSL(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutHSL, Color::Rgb, Color::Hsl);
}
void cvtColor::BGR2HSL(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	main_convert(src, dst, ColorGamutHSL, Color::Bgr, Color::Hsl);
}
void cvtColor::BGR2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array rgb(src.size3(), src.depth(), ColorGamutRGB);
	FOR_RANGE(i, 0, 3)
		src.channel(i).setTo(rgb.channel(2 - i));
	dst.set(rgb);
}
void cvtColor::RGB2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	if (src.empty())return;
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array bgr(src.size3(), src.depth(), ColorGamutBGR);
	FOR_RANGE(i, 0, 3)
		src.channel(i).setTo(bgr.channel(2 - i));
	dst.set(bgr);
}
void cvtColor::BGR2BGRA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, 4, src.depth(), ColorGamutBGRA);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, 2, src(i, j, 0));
			img.set(i, j, 1, src(i, j, 1));
			img.set(i, j, 0, src(i, j, 2));
			img.set(i, j, 3, 255);
		}
	}
	dst.set(img);
}
void cvtColor::RGB2RGBA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, 4, src.depth(), ColorGamutRGBA);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, 0, src(i, j, 0));
			img.set(i, j, 1, src(i, j, 1));
			img.set(i, j, 2, src(i, j, 2));
			img.set(i, j, 3, 255);
		}
	}
	dst.set(img);
}
void cvtColor::GRAY2BGRA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 1), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutGray || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array v(4);
	FOR_RANGE(i, 0, 3)
		v[i] = src;
	v[3] = Array::zeros(src.size(), src.depth()) + 255;
	Array bgra; merge(v, bgra);
	bgra.setColorGamut(ColorGamutBGRA);
	dst.set(bgra);
}
void cvtColor::GRAY2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	if (src.empty())return;
	LZH_ACCESS(if (src.channels() == 1), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutGray || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array v(3);
	FOR_RANGE(i, 0, 3)
		v[i] = src;
	Array bgr; merge(v, bgr);
	bgr.setColorGamut(ColorGamutBGR);
	dst.set(bgr);
}
void cvtColor::GRAY2RGBA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 1), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutGray || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array v(4);
	FOR_RANGE(i, 0, 3) v[i] = src;
	v[3] = Array::zeros(src.size(), src.depth()) + 255;
	Array rgba; merge(v, rgba);
	rgba.setColorGamut(ColorGamutRGBA);
	dst.set(rgba);
}
void cvtColor::GRAY2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	if (src.empty())return;
	LZH_ACCESS(if (src.channels() == 1), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutGray || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array v(3);
	FOR_RANGE(i, 0, 3) v[i] = src;
	Array rgb; merge(v, rgb);
	rgb.setColorGamut(ColorGamutRGB);
	dst.set(rgb);
}
void cvtColor::RGBA2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGBA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array rgba; split(src, rgba);
	Array rgb; merge(rgba, 3, rgb);
	rgb.setColorGamut(ColorGamutRGB);
	dst.set(rgb);
}
void cvtColor::BGRA2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGRA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array bgra; split(src, bgra);
	Array rgb; merge(bgra, 3, rgb);
	rgb.setColorGamut(ColorGamutBGR);
	dst.set(rgb);
}
void cvtColor::RGBA2BGRA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGBA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array rgba;
	split(src, rgba);
	lzh::Swap(rgba[0], rgba[2]);
	Array bgra; merge(rgba, 4, bgra);
	bgra.setColorGamut(ColorGamutBGRA);
	dst.set(bgra);
}
void cvtColor::BGRA2RGBA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGRA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array bgra;
	split(src, bgra);
	lzh::Swap(bgra[0], bgra[2]);
	Array rgba; merge(bgra, 4, rgba);
	rgba.setColorGamut(ColorGamutRGBA);
	dst.set(rgba);
}
void cvtColor::RGB2BGRA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, 4, src.depth(), ColorGamutBGRA);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, 0, src(i, j, 2));
			img.set(i, j, 1, src(i, j, 1));
			img.set(i, j, 2, src(i, j, 0));
			img.set(i, j, 3, 255);
		}
	}
	dst.set(img);
}
void cvtColor::BGR2RGBA(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, 4, src.depth(), ColorGamutRGBA);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, 0, src(i, j, 2));
			img.set(i, j, 1, src(i, j, 1));
			img.set(i, j, 2, src(i, j, 0));
			img.set(i, j, 3, 255);
		}
	}
	dst.set(img);
}
void cvtColor::RGBA2BGR(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGBA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array rgba;
	split(src, rgba);
	lzh::Swap(rgba[0], rgba[2]);
	Array bgr; merge(rgba, 3, bgr);
	bgr.setColorGamut(ColorGamutBGR);
	dst.set(bgr);
}
void cvtColor::BGRA2RGB(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGRA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	vec_array bgra;
	split(src, bgra);
	lzh::Swap(bgra[0], bgra[2]);
	Array rgb; merge(bgra, 3, rgb);
	rgb.setColorGamut(ColorGamutRGB);
	dst.set(rgb);
}
void cvtColor::RGB2GRAY(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGB || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, src.pixel(i, j, Color::Rgb).toGray()[0]);
		}
	}
	dst.set(img);
}
void cvtColor::RGBA2GRAY(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutRGBA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, src.pixel(i, j, Color::Rgb).toGray()[0]);
		}
	}
	dst.set(img);
}
void cvtColor::BGR2GRAY(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 3), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGR || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, src.pixel(i, j, Color::Rgb).toBgr().toGray()[0]);
		}
	}
	dst.set(img);
}
void cvtColor::BGRA2GRAY(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() == 4), THROW_INFO_STR("通道数不符合要求!"));
	ColorGamut colorGamut = src.colorGamut();
	LZH_ACCESS(if (colorGamut == ColorGamutBGRA || colorGamut == ColorGamutCustom), THROW_INFO_STR("图像类型错误"));
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, src.pixel(i, j, Color::Rgb).toBgr().toGray()[0]);
		}
	}
	dst.set(img);
}

void lzh::cvtColor::toMean(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			mat_t v = 0;
			for (int32 c = 0; c < src.channels(); ++c) {
				v += src(i, j, c) / src.channels();
			}
			img.set(i, j, v);
		}
	}
	dst.set(img);
}

void lzh::cvtColor::toMaxMin(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			Array m = img.range(i, i + 1, j, j + 1);
			img.set(i, j, (m.max() + m.min()) / 2.0);
		}
	}
	dst.set(img);
}

void lzh::cvtColor::toRad(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() >= 1), THROW_INFO_STR("通道数不符合要求!"));
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, src(i, j, 0));
		}
	}
	dst.set(img);
}

void lzh::cvtColor::toGreen(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() >= 2), THROW_INFO_STR("通道数不符合要求!"));
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, src(i, j, 1));
		}
	}
	dst.set(img);
}

void lzh::cvtColor::toBlue(InputArray src_, OutputArray dst)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	LZH_ACCESS(if (src.channels() >= 3), THROW_INFO_STR("通道数不符合要求!"));
	Array img(src.cols, src.rows, src.depth(), ColorGamutGray);
	for (int32 i = 0; i < src.rows; ++i) {
		for (int32 j = 0; j < src.cols; ++j) {
			img.set(i, j, src(i, j, 2));
		}
	}
	dst.set(img);
}

void lzh::cvtColor::operator()(InputArray src, OutputArray dst, ColorGamut code) const
{
	ConverTo(src, dst, code);
}

const Array lzh::cvtColor::operator()(InputArray src, ColorGamut code) const
{
	Array img;
	ConverTo(src, img, code);
	return img;
}

void lzh::cvtColor::main_convert(const Array &src, OutputArray dst, ColorGamut code, Color::Spec input_spec, Color::Spec output_spec)
{
	Array res(src.size3(), src.depth(), code);
	FOR_RANGE(i, 0, src.rows) {
		FOR_RANGE(j, 0, src.cols) {
			Color pix = src.pixel(i, j, input_spec);
			if (src.depth() == MAT_8U) pix = pix.inv();
			pix = pix.toSpec(output_spec);
			if (!isType<uint8>(src.depth())) res.setPixel(i, j, pix);
			else res.setPixel(i, j, pix.Uint8());
		}
	}
	dst = res;
}

void lzh::subSampled(InputArray src, OutputArray dst, const Size & dstsize, BorderType borderType)
{
	Array img(src);
	Size newsize;
	if (dstsize.area() == 0) {
		newsize = Size(img.cols / 2, img.rows / 2);
	}
	else {
		newsize = dstsize;
	}
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (newsize.w < img.size().w && newsize.h < img.size().h), THROW_INFO_STR("降采样方法只支持缩小操作"));
	LZH_ACCESS(if (borderType != BORDER_CONSTANT), THROW_INFO_STR("降采样方法不支持常量边缘扩充"));
#endif
	GaussianBlur(src, dst, Size(5, 5), 1, borderType);
	resize(dst, dst, newsize, INTER_SAMPLE);
}

void lzh::subSampled(InputArray src, OutputArray dst, const Size & dstsize, void(*zoom_out)(InputArray, OutputArray, Size), BorderType borderType)
{
	Array img(src);
	Size newsize;
	if (dstsize.area() == 0) {
		newsize = Size(img.cols / 2, img.rows / 2);
	}
	else {
		newsize = dstsize;
	}
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (newsize.w < img.size().w && newsize.h < img.size().h), THROW_INFO_STR("降采样方法只支持缩小操作"));
	LZH_ACCESS(if (borderType != BORDER_CONSTANT), THROW_INFO_STR("降采样方法不支持常量边缘扩充"));
#endif
	GaussianBlur(src, dst, Size(5, 5), 1, borderType);
	zoom_out(dst, dst, newsize);
}

void lzh::upSampling(InputArray src, OutputArray dst, const Size & dstsize, BorderType borderType)
{
	Array img(src);
	Size newsize;
	if (dstsize.area() == 0) {
		newsize = Size(img.cols * 2, img.rows * 2);
	}
	else {
		newsize = dstsize;
	}
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (newsize.w > img.size().w && newsize.h > img.size().h), THROW_INFO_STR("降采样方法只支持缩小操作"));
	LZH_ACCESS(if (borderType != BORDER_CONSTANT), THROW_INFO_STR("降采样方法不支持常量边缘扩充"));
#endif
	resize(src, dst, newsize, INTER_SAMPLE);
	GaussianBlur(dst, dst, Size(5, 5), 1, borderType);
}

void lzh::upSampling(InputArray src, OutputArray dst, const Size & dstsize, void(*zoom_in)(InputArray, OutputArray, Size), BorderType borderType)
{
	Array img(src);
	Size newsize;
	if (dstsize.area() == 0) {
		newsize = Size(img.cols * 2, img.rows * 2);
	}
	else {
		newsize = dstsize;
	}
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif	
	zoom_in(src, dst, newsize);
	GaussianBlur(dst, dst, Size(5, 5), 1, borderType);
}

void lzh::warpAffine(InputArray src_, OutputArray dst, InputArray M)
{
	Array src(src_);
	Mat m = M.getMat();
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
#endif
	int32 ymax = src.rows - 1;
	int32 xmax = src.cols - 1;
	Array mat(src.size3(), src.depth())/* + 255*/;
	mat_t *m_ = m.data();
	//Mat mark = Mat::zeros(src.size3(), MAT_32S);
	FOR_RANGE(i, 0, mat.rows) {
		FOR_RANGE(j, 0, mat.cols) {
			Point pos;
			pos.x = Round(m_[0] * j + m_[1] * i + m_[2]);
			pos.y = Round(m_[3] * j + m_[4] * i + m_[5]);
			if (_RANGE_(-1, pos.y, src.rows) && _RANGE_(-1, pos.x, src.cols)) {
				FOR_RANGE(k, 0, mat.channels())
					mat.set(i, j, k, src(pos.y, pos.x, k));
			}
			else {
				FOR_RANGE(k, 0, mat.channels())
					mat.set(i, j, k, 0);
			}
		}
	}
	dst.set(mat);
}


void lzh::dft(InputArray src, OutputArray dst)
{
	Array m(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif
	bool dim = m.dims() >= 2;
	bool canfast = isOptimalDFTSize(m.size());
	dst = dim ? (canfast ? fft2D(src) : dft2D(src)) : (canfast ? fft1D(src) : dft1D(src));
}
void lzh::idft(InputArray src, OutputArray dst)
{
	Array m(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(m);
#endif
	bool dim = m.dims() >= 2;
	bool canfast = isOptimalDFTSize(m.size());
	dst = dim ? (canfast ? ifft2D(src) : idft2D(src)) : (canfast ? ifft1D(src) : idft1D(src));
}
//OMat lzh::fft1D(InputOutputArray src)
//{
//	Mat y = src.getMat();
//	bool dir = y.cols() > y.rows() ? true : false;
//#ifdef LZH_DEBUG
//	CHECK_MATRIX_(y);
//	LZH_ACCESS(if (y.channels() == 2 || y.channels() == 1), THROW_INFO(ERR_INFO_CHANNEL););
//#endif
//	int32 l = dir ? y.cols() : y.rows();
//	Complex u, t;
//	brc(y); // 调用反转置换
//	mat_t* p = y.data();
//	int32 stride = y.step()[1];
//	//int32 stride = dir ? y.step()[1] : y.step().size();
//	for (int32 h = 2; h <= l; h <<= 1) // 控制层数
//	{
//		// 初始化单位复根
//		Complex wn(cos(-D_PI_ / h), sin(-D_PI_ / h));
//		for (int32 j = 0; j < l; j += h) // 控制起始下标
//		{
//			Complex w(1, 0); // 初始化螺旋因子
//			for (int32 k = j; k < j + h / 2; k++) // 配对
//			{
//				mat_t* val_k = p + k * stride;
//				mat_t* val_h = p + (k + h / 2) * stride;
//				u = Complex(val_k[0], val_k[1]);
//				t = w * Complex(val_h[0], val_h[1]);
//				Complex x = u + t;
//				val_k[0] = x.r;
//				val_k[1] = x.i;
//				x = u - t;
//				val_h[0] = x.r;
//				val_h[1] = x.i;
//				w = w * wn;
//			} // 据说上面的操作叫蝴蝶操作…
//		}
//	}
//	return y;
//}

Array lzh::dft2D(InputArray src_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (src.channels() == 2 || src.channels() == 1), THROW_INFO(ERR_INFO_CHANNEL););
#endif
	src = src.convert(MAT_T);
	int32 height = src.rows, width = src.cols;
	Array mat(width, height, 2, src.depth());
	for (int32 v = 0; v < width; v++)
		dft1D(src.col(v)).setTo(mat.col(v));
	for (int32 v = 0; v < height; v++)
		dft1D(mat.row(v)).setTo(mat.row(v));
	// now we have the transformed vector, keep the info
	return mat;
}
Array lzh::idft2D(InputArray src_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (src.channels() == 2), THROW_INFO(ERR_INFO_CHANNEL));
#endif
	int32 height = src.rows, width = src.cols;
	Array mat(width, height, 2, src.depth());
	for (int32 v = 0; v < width; v++)
		idft1D(src.col(v)).setTo(mat.col(v));
	for (int32 v = 0; v < height; v++)
		idft1D(mat.row(v)).setTo(mat.row(v));
	// now we have the inverse transformed matrix, keep the info
	return mat;
}
Array lzh::fft2D(InputArray src_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (src.channels() == 2 || src.channels() == 1), THROW_INFO(ERR_INFO_CHANNEL););
#endif
	src = src.convert(MAT_T);
	int32 height = src.rows, width = src.cols;
	if ((height & height - 1) != 0 || (width & width - 1))
		THROW_INFO(ERR_INFO_FFT);
	Array mat(width, height, 2, src.depth());
	for (int32 v = 0; v < width; v++)
		fft1D(src.col(v)).setTo(mat.col(v));
	for (int32 v = 0; v < height; v++)
		fft1D(mat.row(v)).setTo(mat.row(v));
	// now we have the transformed vector, keep the info
	return mat;
}
Array lzh::ifft2D(InputArray src_)
{
	Array src(src_);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(src);
	LZH_ACCESS(if (src.channels() == 2), THROW_INFO(ERR_INFO_CHANNEL));
#endif
	src = src.convert(MAT_T);
	int32 height = src.rows, width = src.cols;
	if ((height & height - 1) != 0 || (width & width - 1))
		THROW_INFO(ERR_INFO_FFT);
	Array mat(width, height, 2, src.depth());
	for (int32 v = 0; v < width; v++)
		ifft1D(src.col(v)).setTo(mat.col(v));
	for (int32 v = 0; v < height; v++)
		ifft1D(mat.row(v)).setTo(mat.row(v));
	// now we have the inverse transformed matrix, keep the info
	return mat;
}
void lzh::dftShift(InputOutputArray src)
{
	Array v(src);
	int32 w = v.cols / 2;
	int32 h = v.rows / 2;

	Array a = v(Rect(0, 0, w, h)).clone();
	Array b = v(Rect(w, 0, w, h)).clone();
	Array c = v(Rect(w, h, w, h)).clone();
	Array d = v(Rect(0, h, w, h)).clone();

	a.setTo(v(Rect(w, h, w, h)));
	b.setTo(v(Rect(0, h, w, h)));
	c.setTo(v(Rect(0, 0, w, h)));
	d.setTo(v(Rect(w, 0, w, h)));
}
void lzh::idftShift(InputOutputArray src)
{
	dftShift(src);
}
void lzh::dftShift(InputArray src, OutputArray dst)
{
	Array s(src);
	Array v(s.size3(), s.depth());
	int32 w = v.cols / 2;
	int32 h = v.rows / 2;

	Array a = s(Rect(0, 0, w, h));
	Array b = s(Rect(w, 0, w, h));
	Array c = s(Rect(w, h, w, h));
	Array d = s(Rect(0, h, w, h));

	a.setTo(v(Rect(w, h, w, h)));
	b.setTo(v(Rect(0, h, w, h)));
	c.setTo(v(Rect(0, 0, w, h)));
	d.setTo(v(Rect(w, 0, w, h)));

	dst.set(v);
}
void lzh::ifftShift(InputArray src, OutputArray dst)
{
	dftShift(src, dst);
}
bool lzh::isOptimalDFTSize(int32 size)
{
	return ((size & (size - 1)) == 0);
}
bool lzh::isOptimalDFTSize(Size size)
{
	return (isOptimalDFTSize(size.h) && isOptimalDFTSize(size.w));
}
int32 lzh::getOptimalDFTSize(int32 size, bool canResize)
{
	if ((size & (size - 1)) == 0)return size;
	int32 l = 1;
	while (l < size) l <<= 1;
	if(canResize)return (size - (l >> 1)) > (l - size) ? l : l >> 1;
	else return l;
}
Size lzh::getOptimalDFTSize(Size size, bool canResize)
{
	return Size(getOptimalDFTSize(size.w, canResize), getOptimalDFTSize(size.h, canResize));
}
void lzh::copyBorderDFTSize(InputArray src, OutputArray dst, bool canResize)
{
	Size sz = src.getArray().size();
	Size newSz = getOptimalDFTSize(sz, canResize);
	if (sz != newSz)
		if (sz.h < newSz.h && sz.w < newSz.w)
			copyMakeBorder(src, dst, newSz, BORDER_CONSTANT, Color(0));
		else if (sz.h > newSz.h && sz.w > newSz.w)
			resize(src, dst, newSz, INTER_LINEAR);
		else {
			Mat img;
			copyMakeBorder(src, img, Size(MAX(newSz.w, sz.w), MAX(newSz.h, sz.h)), BORDER_CONSTANT, Color(0));
			resize(img, dst, newSz, INTER_LINEAR);
		}
	else
		dst.set(src);
}
void lzh::normalize(InputArray src, OutputArray dst, Val alpha, Val beta, NormType norm_type)
{
	Array s(src);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(s);
#endif // LZH_DEBUG
	Array norm = s.clone();
	mat_t a = MIN(alpha.v, beta.v);
	mat_t b = MAX(alpha.v, beta.v);
	FOR_RANGE(i, 0, norm.channels()) {
		Array in = norm.channel(i);
		switch (norm_type)
		{
		case lzh::NORM_SUM:		(in * b / in.sum()).setTo(in); break;
		case lzh::NORM_L1:		(in * b / in.sum(1, true)).setTo(in); break;
		case lzh::NORM_L2:		(in * b / sqrt(in.sum(2))).setTo(in); break;
		case lzh::NORM_INF:		(in * b / in.max(true)).setTo(in); break;
		case lzh::NORM_MINMAX: {
			mat_t min_v = in.min(), max_v = in.max();
			if (max_v - min_v != 0)
				((in - min_v) / (max_v - min_v) * (b - a) + a).setTo(in);
			else
				((in - min_v) + a).setTo(in);
			break;
		}
		case lzh::NORM_IMAGE: {
			mat_t max_v = in.max();
			//mat_t min_v = in.min();
			//Mat t = in;
			Array t = Compare(max_v, b, EQUAL) ? in : in / max_v * b;
			//if (max_v > b) {
			//	Min(b, t, t);
			//}
			//if (min_v < a){
			//	Max(a, t, t);
			//}
			if (!t.empty())t.setTo(in);
			break;
		}
		case lzh::NORM_ZSCORE: {
			Array x = in;
			x -= in.mean();
			x /= in.std();
			x.setTo(in);
			break;
		}
		default:THROW_INFO(ERR_INFO_UNLESS);
		}
	}
	dst.set(norm);
}
Array lzh::getPerspectiveTransform(const Point2f* src, const Point2f* dst)
{
	Array a = Array::ones(3, 3, MAT_T);
	Array b = Array::ones(3, 3, MAT_T);
	FOR_RANGE(i, 0, 3) {
		a.at<mat_t>(0, i) = src[i].x;
		a.at<mat_t>(1, i) = src[i].y;
		b.at<mat_t>(0, i) = dst[i].x;
		b.at<mat_t>(1, i) = dst[i].y;
	}
	return b * a.inv();
}
void lzh::getPerspectiveTransform(InputArray src, InputArray dst, OutputArray transform)
{
	Array a(src);
	Array b(dst);
#ifdef LZH_DEBUG
	CHECK_MATRIX_(a);
	CHECK_MATRIX_(b);
#endif // LZH_DEBUG
	//if (a.rows() != 3 || b.rows() != 3)
	//	THROW_INFO(ERR_INFO_SIZE);
	Array m = b * a.inv();
	transform.set(m);
}

void lzh::resize(InputArray src_, OutputArray dst, Val xRatio, Val yRatio, InterpolationMothed mothed, mat_t alpha)
{
	Array src(src_);
	if (src.empty())return;
	if (yRatio == 1 && xRatio == 1) {
		dst.set(src_); return;
	}
	int32 rows = saturate_cast<int32>(src.rows * yRatio);
	int32 cols = saturate_cast<int32>(src.cols * xRatio);
	Array img = Array::zeros(cols, rows, src.channels(), src.depth(), src.colorGamut());
	switch (mothed)
	{
	case lzh::INTER_SAMPLE: 
	{
		for (int32 i = 0; i < rows; i++) {
			int32 h = static_cast<int32>((i + 1) / yRatio + _T(0.5)) - 1;
			for (int32 j = 0; j < cols; j++) {
				int32 w = static_cast<int32>((j + 1) / xRatio + _T(0.5)) - 1;
				for (int32 z = 0; z < src.channels(); z++) {
					img.set(i, j, z, src(h, w, z));//取得采样像素
				}
			}
		}
	}
	break;
	case lzh::INTER_MEAN:
	{
		LZH_ACCESS(if (yRatio < 1 && xRatio < 1), THROW_INFO_STR("此方法只支持缩小"));
		FOR_RANGE(z, 0, src.channels()) {	
			int32 lastRow = 0;
			int32 lastCol = 0;
			Array m = src.channel(z);
			for (int32 i = 0; i < rows; i++) {
				int32 h = saturate_cast<int32>((i + 1) / yRatio);
				for (int32 j = 0; j < cols; j++) {
					int32 w = saturate_cast<int32>((j + 1) / xRatio);
					img.set(i, j, z, m.range(lastRow, h, lastCol, w).mean());
					lastCol = w; //下一个子块左上角的列坐标，行坐标不变
				}
				lastCol = 0; //子块的左上角列坐标，从0开始
				lastRow = h; //子块的左上角行坐标
			}
		}
	}
	break;
	case lzh::INTER_MAX: 
	{
		LZH_ACCESS(if (yRatio < 1 && xRatio < 1), THROW_INFO_STR("此方法只支持缩小"));
		FOR_RANGE(z, 0, src.channels()) {
			int32 lastRow = 0;
			int32 lastCol = 0;
			Array m = src.channel(z);
			for (int32 i = 0; i < rows; i++) {
				int32 h = saturate_cast<int32>((i + 1) / yRatio);
				for (int32 j = 0; j < cols; j++) {
					int32 w = saturate_cast<int32>((j + 1) / xRatio);
					img.set(i, j, z, m.range(lastRow, h, lastCol, w).max());
					lastCol = w; //下一个子块左上角的列坐标，行坐标不变
				}
				lastCol = 0; //子块的左上角列坐标，从0开始
				lastRow = h; //子块的左上角行坐标
			}
		}
	}
	break;
	case lzh::INTER_MIN: 
	{
		LZH_ACCESS(if (yRatio < 1 && xRatio < 1), THROW_INFO_STR("此方法只支持缩小"));
		FOR_RANGE(z, 0, src.channels()) {
			int32 lastRow = 0;
			int32 lastCol = 0;
			Array m = src.channel(z);
			for (int32 i = 0; i < rows; i++) {
				int32 h = saturate_cast<int32>((i + 1) / yRatio);
				for (int32 j = 0; j < cols; j++) {
					int32 w = saturate_cast<int32>((j + 1) / xRatio);
					img.set(i, j, z, m.range(lastRow, h, lastCol, w).min());
					lastCol = w; //下一个子块左上角的列坐标，行坐标不变
				}
				lastCol = 0; //子块的左上角列坐标，从0开始
				lastRow = h; //子块的左上角行坐标
			}
		}
	}
	break;
	case lzh::INTER_NEAREST: 
	{
		yRatio = 1 / yRatio;
		xRatio = 1 / xRatio;
		img.fill(0);
		for (int32 i = 0; i < rows; i++)
		{
			for (int32 j = 0; j < cols; j++)
			{
				int32 i_scale = Floor(yRatio * i);  //依照高的比例计算原图相应坐标中的y，这里采用的是向下取整，四舍五入在整数倍放大时图像边缘有越界风险
				int32 j_scale = Floor(xRatio * j);  //依照宽的比例计算原图相应坐标中的x
				//cout << "i_scale: " << i_scale <<" j_scale: "<< j_scale << endl;
				for (int32 k = 0; k < img.channels(); k++)
					img.set(i, j, k, src(i_scale, j_scale, k));
			}
		}
	}
	break;
	case lzh::INTER_LINEAR: 
	{
		mat_t h_scale_rate = 1 / yRatio;
		mat_t w_scale_rate = 1 / xRatio;
		img.fill(0);
		copyMakeBorder(src, src, 0, Round(yRatio), 0, Round(xRatio), BORDER_REPLICATE);
		int32 c = img.channels();
		for (int32 i = 0; i < rows; i++)
		{
			mat_t i_scale = h_scale_rate * i;
			int32 raw_i = _I(i_scale);
			mat_t u = i_scale - raw_i;
			for (int32 j = 0; j < cols; j++)
			{
				mat_t j_scale = w_scale_rate * j;

				int32 raw_j = _I(j_scale);
				mat_t v = j_scale - raw_j;

				//注意处理边界问题，容易越界
				if (raw_i + 1 >= src.rows || raw_j + 1 >= src.cols)
				{
					for (int32 k = 0; k < img.channels(); k++)
						img.set(i, j, k, src(raw_i, raw_j, k));
				}
				else {
					for (int32 k = 0; k < img.channels(); k++) {
						mat_t x1 = src(raw_i, raw_j, k);			//f(i,j)
						mat_t x2 = src(raw_i, raw_j + 1, k);		//f(i,j+1)
						mat_t x3 = src(raw_i + 1, raw_j, k);		//(i+1,j)
						mat_t x4 = src(raw_i + 1, raw_j + 1, k);	//f(i+1,j+1) 
						img.set(i, j, k, ((1 - u) * (1 - v) * x1 + (1 - u) * v * x2 + u * (1 - v) * x3 + u * v * x4));
					}
				}
			}
		}
	}
	break;
	case lzh::INTER_AREA:
		if (yRatio > 1 && xRatio > 1 && yRatio == xRatio)resize(src, img, yRatio, xRatio, INTER_NEAREST);
		else if (yRatio < 1 && xRatio < 1 && yRatio == xRatio)resize(src, img, yRatio, xRatio, INTER_MEAN);
		else resize(src, img, yRatio, xRatio, INTER_LINEAR);
		break;
	case lzh::INTER_CUBIC: 
	{
		auto sw = [=](float64 x)
		{
			x = abs(x);
			if (0 <= x && x < 1)
				return 1 - (alpha + 3) * x * x + (alpha + 2) * x * x * x;
			else if (1 <= x && x < 2)
				return -4 * alpha + 8 * alpha * x - 5 * alpha * x * x + alpha * x * x * x;
			else
				return 0.0;
		};
		copyMakeBorder(src.toMat(), src, 1, 2, 1, 2, BORDER_REPLICATE);
		FOR_RANGE(i, 0, img.rows) {
			mat_t y_ = i / yRatio;
			mat_t y = y_ - int32(y_);
			int32 i1 = int32(y_) + 1;
			Mat A = (Array_<mat_t>(4) << sw(1 + y), sw(y), sw(1 - y), sw(2 - y));
			if (i1 + 2 >= src.rows) i1 = src.rows - 3;
			if (i1 < 1)i1 = 1;
			FOR_RANGE(j, 0, img.cols) {
				mat_t x_ = j / xRatio;
				mat_t x = x_ - int32(x_);
				int32 j1 = int32(x_) + 1;
				if (j1 + 2 >= src.cols) j1 = src.cols - 3;
				if (j1 < 1)j1 = 1;
				Mat C = (Mat_(1, 4) << sw(1 + x), sw(x), sw(1 - x), sw(2 - x));
				FOR_RANGE(z, 0, src.channels()) {
					Mat B = src.channel(z).range(i1 - 1, i1 + 3, j1 - 1, j1 + 3);
					//img.set(i, j, z, (A*B*C)(0));
					img.set(i, j, z,
						(A(0)*B(0, 0) + A(1)*B(1, 0) + A(2)*B(2, 0) + A(3)*B(3, 0))*C(0) + 
						(A(0)*B(0, 1) + A(1)*B(1, 1) + A(2)*B(2, 1) + A(3)*B(3, 1))*C(1) +
						(A(0)*B(0, 2) + A(1)*B(1, 2) + A(2)*B(2, 2) + A(3)*B(3, 2))*C(2) +
						(A(0)*B(0, 3) + A(1)*B(1, 3) + A(2)*B(2, 3) + A(3)*B(3, 3))*C(3));
				}
			}
		}
	}
	break;
	case lzh::INTER_LANCZOS4: 
	{
		copyMakeBorder(src, src, 3, 4, 3, 4, BORDER_REPLICATE);
		for (int32 j = 0; j < img.rows; ++j)
		{
			float32 fy = (float32)((j + 0.5) / yRatio - 0.5);
			int32 sy = Floor(fy);
			fy -= sy;
			sy = MIN(sy, src.rows - 5);
			sy = MAX(3, sy);

			const float64 s45 = 0.70710678118654752440084436210485;
			const float64 cs[][2] = {
			{1, 0}, {-s45, -s45},
			{0, 1}, {s45, -s45},
			{-1, 0}, {s45, s45},
			{0, -1}, {-s45, s45} };
			float32 coeffsY[8];

			if (fy < FLT_EPSILON) {
				for (int32 t = 0; t < 8; t++)
					coeffsY[t] = 0;
				coeffsY[3] = 1;
			}
			else {
				float32 sum = 0;
				float64 y0 = -(fy + 3) * PI * 0.25, s0 = sin(y0), c0 = cos(y0);

				for (int32 t = 0; t < 8; ++t)
				{
					float64 dy = -(fy + 3 - t) * PI * 0.25;
					coeffsY[t] = (float)((cs[t][0] * s0 + cs[t][1] * c0) / (dy * dy));
					sum += coeffsY[t];
				}

				sum = 1.f / sum;
				for (int32 t = 0; t < 8; ++t)
					coeffsY[t] *= sum;
			}

			for (int32 i = 0; i < img.cols; ++i)
			{
				float32 fx = (float32)((i + 0.5) / xRatio - 0.5);
				int32 sx = Floor(fx);
				fx -= sx;

				if (sx < 3) {
					fx = 0, sx = 3;
				}
				if (sx >= src.cols - 5) {
					fx = 0, sx = src.cols - 5;
				}

				float32 coeffsX[8];

				if (fx < FLT_EPSILON) {
					for (int32 t = 0; t < 8; t++)
						coeffsX[t] = 0;
					coeffsX[3] = 1;
				}
				else {
					float32 sum = 0;
					float64 x0 = -(fx + 3) * PI * 0.25, s0 = sin(x0), c0 = cos(x0);

					for (int32 t = 0; t < 8; ++t)
					{
						float64 dx = -(fx + 3 - t) * PI * 0.25;
						coeffsX[t] = (float32)((cs[t][0] * s0 + cs[t][1] * c0) / (dx * dx));
						sum += coeffsX[t];
					}

					sum = 1.f / sum;
					for (int32 t = 0; t < 8; ++t)
						coeffsX[t] *= sum;
				}

				for (int32 k = 0; k < src.channels(); ++k)
				{
					img.set(j, i, k,
						src(sy - 3, sx - 3, k) * coeffsX[0] * coeffsY[0] + src(sy - 2, sx - 3, k) * coeffsX[0] * coeffsY[1] +
						src(sy - 1, sx - 3, k) * coeffsX[0] * coeffsY[2] + src(sy, sx - 3, k) * coeffsX[0] * coeffsY[3] +
						src(sy + 1, sx - 3, k) * coeffsX[0] * coeffsY[4] + src(sy + 2, sx - 3, k) * coeffsX[0] * coeffsY[5] +
						src(sy + 3, sx - 3, k) * coeffsX[0] * coeffsY[6] + src(sy + 4, sx - 3, k) * coeffsX[0] * coeffsY[7] +

						src(sy - 3, sx - 2, k) * coeffsX[1] * coeffsY[0] + src(sy - 2, sx - 2, k) * coeffsX[1] * coeffsY[1] +
						src(sy - 1, sx - 2, k) * coeffsX[1] * coeffsY[2] + src(sy, sx - 2, k) * coeffsX[1] * coeffsY[3] +
						src(sy + 1, sx - 2, k) * coeffsX[1] * coeffsY[4] + src(sy + 2, sx - 2, k) * coeffsX[1] * coeffsY[5] +
						src(sy + 3, sx - 2, k) * coeffsX[1] * coeffsY[6] + src(sy + 4, sx - 2, k) * coeffsX[1] * coeffsY[7] +

						src(sy - 3, sx - 1, k) * coeffsX[2] * coeffsY[0] + src(sy - 2, sx - 1, k) * coeffsX[2] * coeffsY[1] +
						src(sy - 1, sx - 1, k) * coeffsX[2] * coeffsY[2] + src(sy, sx - 1, k) * coeffsX[2] * coeffsY[3] +
						src(sy + 1, sx - 1, k) * coeffsX[2] * coeffsY[4] + src(sy + 2, sx - 1, k) * coeffsX[2] * coeffsY[5] +
						src(sy + 3, sx - 1, k) * coeffsX[2] * coeffsY[6] + src(sy + 4, sx - 1, k) * coeffsX[2] * coeffsY[7] +

						src(sy - 3, sx, k) * coeffsX[3] * coeffsY[0] + src(sy - 2, sx, k) * coeffsX[3] * coeffsY[1] +
						src(sy - 1, sx, k) * coeffsX[3] * coeffsY[2] + src(sy, sx, k) * coeffsX[3] * coeffsY[3] +
						src(sy + 1, sx, k) * coeffsX[3] * coeffsY[4] + src(sy + 2, sx, k) * coeffsX[3] * coeffsY[5] +
						src(sy + 3, sx, k) * coeffsX[3] * coeffsY[6] + src(sy + 4, sx, k) * coeffsX[3] * coeffsY[7] +

						src(sy - 3, sx + 1, k) * coeffsX[4] * coeffsY[0] + src(sy - 2, sx + 1, k) * coeffsX[4] * coeffsY[1] +
						src(sy - 1, sx + 1, k) * coeffsX[4] * coeffsY[2] + src(sy, sx + 1, k) * coeffsX[4] * coeffsY[3] +
						src(sy + 1, sx + 1, k) * coeffsX[4] * coeffsY[4] + src(sy + 2, sx + 1, k) * coeffsX[4] * coeffsY[5] +
						src(sy + 3, sx + 1, k) * coeffsX[4] * coeffsY[6] + src(sy + 4, sx + 1, k) * coeffsX[4] * coeffsY[7] +

						src(sy - 3, sx + 2, k) * coeffsX[5] * coeffsY[0] + src(sy - 2, sx + 2, k) * coeffsX[5] * coeffsY[1] +
						src(sy - 1, sx + 2, k) * coeffsX[5] * coeffsY[2] + src(sy, sx + 2, k) * coeffsX[5] * coeffsY[3] +
						src(sy + 1, sx + 2, k) * coeffsX[5] * coeffsY[4] + src(sy + 2, sx + 2, k) * coeffsX[5] * coeffsY[5] +
						src(sy + 3, sx + 2, k) * coeffsX[5] * coeffsY[6] + src(sy + 4, sx + 2, k) * coeffsX[5] * coeffsY[7] +

						src(sy - 3, sx + 3, k) * coeffsX[6] * coeffsY[0] + src(sy - 2, sx + 3, k) * coeffsX[6] * coeffsY[1] +
						src(sy - 1, sx + 3, k) * coeffsX[6] * coeffsY[2] + src(sy, sx + 3, k) * coeffsX[6] * coeffsY[3] +
						src(sy + 1, sx + 3, k) * coeffsX[6] * coeffsY[4] + src(sy + 2, sx + 3, k) * coeffsX[6] * coeffsY[5] +
						src(sy + 3, sx + 3, k) * coeffsX[6] * coeffsY[6] + src(sy + 4, sx + 3, k) * coeffsX[6] * coeffsY[7] +

						src(sy - 3, sx + 4, k) * coeffsX[7] * coeffsY[0] + src(sy - 2, sx + 4, k) * coeffsX[7] * coeffsY[1] +
						src(sy - 1, sx + 4, k) * coeffsX[7] * coeffsY[2] + src(sy, sx + 4, k) * coeffsX[7] * coeffsY[3] +
						src(sy + 1, sx + 4, k) * coeffsX[7] * coeffsY[4] + src(sy + 2, sx + 4, k) * coeffsX[7] * coeffsY[5] +
						src(sy + 3, sx + 4, k) * coeffsX[7] * coeffsY[6] + src(sy + 4, sx + 4, k) * coeffsX[7] * coeffsY[7]);
				}
			}
		}
	}
	break;
	default:break;
	}
	dst.set(img);
}
void lzh::resize(InputArray src, OutputArray dst, Size newSize, InterpolationMothed mothed, mat_t alpha)
{
	if (newSize.area() <= 0)return;
	resize(src, dst, newSize.w / _T(src.cols()), newSize.h / _T(src.rows()), mothed, alpha);
}

void lzh::QuadrantNorm(InputArray src, OutputArray dst, Val minimum, Val maximum, Val value, int32 is_min_or_max)
{
	Array img(src);
	Array out = Array(img.size3(), img.depth(), img.colorGamut());
	for (int32 i = 0; i < img.len(); i++)
	{
		if ((is_min_or_max & 0x01) == 0x01 && img(i) < minimum) {
			out.set(i, img(i) + value);
		}
		else if ((is_min_or_max & 0x02) == 0x02 && img(i) > maximum) {
			out.set(i, img(i) - value);
		}
		else
			out.set(i, img(i) - value);
	}
	dst = out;
}
void lzh::createCorrelationPatch(InputOutputArrayOfArrays patch, Val angle_1, Val angle_2, Val radius)
{
	vec_mat Patch(4);
	// width and height
	int32 width = int32(radius * 2 + 1);
	int32 height = int32(radius * 2 + 1);

	// initialize template
	FOR_RANGE(i, 0, 4)Patch[i] = Mat::zeros(Size(width, height));

	// midpoint
	mat_t mu = radius + 1;
	mat_t mv = radius + 1;

	// compute normals from angles
	Mat n1 = (Mat_(2) << -sin(angle_1), cos(angle_1));
	Mat n2 = (Mat_(2) << -sin(angle_2), cos(angle_2));

	// for all points in template do
	for (int32 u = 1; u <= width; u++) {
		for (int32 v = 1; v <= height; v++) {
			// vector
			Mat vec = { u - mu, v - mv };
			mat_t dist = vec.norm(2);
			// check on which side of the normals we are
			Mat s = vec * n1.t();
			mat_t s1 = s(0, 0);
			s = vec * n2.t();
			mat_t s2 = s(0, 0);
			if ((s1 <= -0.1) && (s2 <= -0.1))
				Patch[0](v - 1, u - 1) = normpdf(dist, 0, radius / 2);
			else if ((s1 >= 0.1) && (s2 >= 0.1))
				Patch[2](v - 1, u - 1) = normpdf(dist, 0, radius / 2);
			else if ((s1 <= -0.1) && (s2 >= 0.1))
				Patch[1](v - 1, u - 1) = normpdf(dist, 0, radius / 2);
			else if ((s1 >= 0.1) && (s2 <= -0.1))
				Patch[3](v - 1, u - 1) = normpdf(dist, 0, radius / 2);
		}
	}
	patch.create(4, MAT_T);
	// normalize
	FOR_RANGE(i, 0, 4) {
		patch.setMatRef(Patch[i] / Patch[i].sum(), i);
	}
}
void lzh::nonMaximumSuppression(InputArray src, OutputArray dst, int32 n, Val tau, int32 margin)
{
	Array img(src);
	// extract parameters
	int32 width = img.cols;
	int32 height = img.rows;

	// init maxima list
	vector<Point2d> maxima;

	// non maximum suppression
	for (int32 i = n + margin; i < width - n - margin; i += (n + 1)) {
		for (int32 j = n + margin; j < height - n - margin; j += (n + 1)) {
			int32 maxi = i;
			int32 maxj = j;
			mat_t maxval = img(j, i);
			for (int32 i2 = i; i2 <= i + n; i2++) {
				for (int32 j2 = j; j2 <= j + n; j2++) {
					mat_t currval = img(j2, i2);
					if (currval > maxval) {
						maxi = i2;
						maxj = j2;
						maxval = currval;
						//printf("%d, %d, %lf\n", maxi, maxj, maxval);
					}
				}
			}
			bool failed = false;
			for (int32 i2 = maxi - n; i2 <= min(maxi + n, width - margin); i2++) {
				for (int32 j2 = maxj - n; j2 <= min(maxj + n, height - margin); j2++) {
					mat_t currval = img(j2, i2);
					if (currval > maxval && (i2<i || i2>i + n || j2<j || j2>j + n)) {
						failed = true;
						break;
					}
				}
				if (failed)
					break;
			}
			if (maxval >= tau && !failed) {
				maxima.push_back(Point2d(maxi, maxj));
			}
		}
	}
	dst = maxima;
}
void lzh::findModesMeanShift(InputArray hist, OutputArray modes, OutputArray histSmoothed, Val sigma)
{
	Array hist_(hist);
	Mat hist_smoothed;
	//efficient mean - shift approximation by histogram smoothing
	//compute smoothed histogram
	int32 length = hist.rows()*hist.cols();
	hist_smoothed = Mat::zeros(length);
	Mat j, idx, index;
	for (int32 i = 0; i < length; i++) {
		j = mRange((int32)-round(2 * sigma), (int32)+round(2 * sigma));
		idx = mod(i + j, length) + 1;
		index = Mat(idx.rows()*idx.cols());
		for (int32 iter = 0; iter < idx.rows()*idx.cols(); iter++) {
			index(iter) = hist_((int32)idx(iter) - 1);
		}
		hist_smoothed(i) = ((index.mul(normpdf(j, 0, sigma))).sum());
	}
	vector<Vec3m> modes_;
	// check if at least one entry is non - zero
	// (otherwise mode finding may run infinitly)
	//if (abs(hist_smoothed - hist_smoothed.at<double>(0)) < 1e-5)
	//		return;
	length = hist.rows()*hist.cols();
	// mode finding
	for (int32 i = 0; i < length; i++) {
		int32 j = i;
		while (true) {
			mat_t h0 = hist_smoothed(j);
			int32 j1 = mod(j + 1, length);
			int32 j2 = mod(j - 1, length);
			mat_t h1 = hist_smoothed(j1);
			mat_t h2 = hist_smoothed(j2);
			if ((h1 > h0) && (h1 > h2))
				j = j1;
			else if (h2 > h0 && h2 > h1)
				j = j2;
			else
				break;
		}
		if (modes_.size() == 0 || !any(Mat(modes_).col(0), j + 1))
			modes_.push_back(Vec3m(j + 1, hist_smoothed(j)));
	}

	sort(modes_.begin(), modes_.end(), [](const Vec3d &x, const Vec3d &y) {return x[1] > y[1]; });
	modes = modes_;
}
mat_t lzh::cornerCorrelationScore(InputArray iou, InputArray imgWeight, Vec2m v1, Vec2m v2)
{
	Mat img = iou.getMat();
	Mat img_weight = imgWeight.getMat();
	// center
	Mat c = Mat::ones(Size(2, 1))*(img_weight.rows() + 1) / 2.0;
	Mat V1(v1);
	Mat V2(v2);
	// compute gradient filter kernel(bandwith = 3 px)
	Mat img_filter = -1 * Mat::ones(img_weight.size());
	for (int32 x = 1; x <= img_weight.cols(); x++) {
		for (int32 y = 1; y <= img_weight.rows(); y++) {
			Mat p1 = Vec2m(x, y) - c;
			Mat p2 = p1 * V1.t()*V1;
			Mat p3 = p1 * V2.t()*V2;
			if ((p1 - p2).norm(2) <= 1.5 || (p1 - p3).norm(2) <= 1.5) {
				img_filter(y - 1, x - 1) = 1;
			}
		}
	}
	// convert into vectors
	Mat vec_weight = img_weight.t().reshape(-1);
	Mat vec_filter = img_filter.t().reshape(-1);
	mat_t vec_weight_mean = vec_weight.mean();
	mat_t vec_weight_std = vec_filter.std();
	mat_t vec_filter_mean = vec_weight.mean();
	mat_t vec_filter_std = vec_filter.std();
	// normalize
	vec_weight = (vec_weight - vec_weight_mean) / vec_weight_std;
	vec_filter = (vec_filter - vec_filter_mean) / vec_filter_std;
	// compute gradient score
	mat_t score_gradient = MAX(vec_weight.mul(vec_filter).sum() / (vec_weight.rows()*vec_weight.cols() - 1), 0);

	// create intensity filter kernel
	vec_mat temp;
	createCorrelationPatch(temp, atan2(v1[1], v1[0]), atan2(v2[1], v2[0]), c(1) - 1);
	Mat v = { 0,0,0,0 };
	for (int32 i : range(4))v[i] = temp[i].t().reshape(-1).t().mul(img.t().reshape(-1).t()).sum();

	// mean
	mat_t mu = v.sum() / 4;

	// case 1: a = white, b = black
	mat_t score_a = min(v[0] - mu, v[1] - mu);
	mat_t score_b = min(mu - v[2], mu - v[3]);
	mat_t score_1 = min(score_a, score_b);

	// case 2: b = white, a = black
	score_a = min(mu - v[0], mu - v[1]);
	score_b = min(v[2] - mu, v[3] - mu);
	mat_t score_2 = min(score_a, score_b);

	// intensity score : max.of the 2 cases
	mat_t score_intensity = max(max(score_1, score_2), _T(0));

	// final score: product of gradient and intensity score
	mat_t score = score_gradient * score_intensity;
	return score;
}
void lzh::edgeOrientations(InputArray img_angle, InputArray img_weight, Vec2m &v1, Vec2m &v2)
{
	// number of bins(histogram parameter)
	int32 bin_num = 32;

	// convert images to vectors
	Mat vec_angle = img_angle.getMat().t().reshape(-1);
	Mat vec_weight = img_weight.getMat().t().reshape(-1);
	// convert angles from normals to directions
	vec_angle = vec_angle + PI / 2;
	QuadrantNorm(vec_angle, vec_angle, 0, PI, PI, 0x02);
	// create histogram
	Mat angle_hist = Mat::zeros(bin_num);
	for (int32 i = 0; i < vec_angle.cols(); i++) {
		int32 bin = max(min(Floor(vec_angle(i) / (PI / _T(bin_num))), bin_num - 1), 0);
		angle_hist(bin) += vec_weight(i);

	}
	// find modes of smoothed histogram
	vector<Vec3m> modes;
	Mat angle_hist_smoothed;
	findModesMeanShift(angle_hist, modes, angle_hist_smoothed, 1);

	// if only one or no mode = > return invalid corner
	if (modes.size() <= 1)
		return;

	// compute orientation at modes
	for (size_t i = 0; i < modes.size(); i++)
		modes[i][2] = (modes[i][0] - 1)*PI_ / bin_num;

	// extract 2 strongest modes and sort by angle
	modes.erase(modes.begin() + 2, modes.end());
	std::sort(modes.begin(), modes.end(), [](const Vec3m &x, const Vec3m &y) {return x[2] < y[2]; });

	// compute angle between modes
	mat_t delta_angle = min(modes[1][2] - modes[0][2], modes[0][2] + PI_ - modes[1][2]);

	// if angle too small = > return invalid corner
	if (delta_angle <= 0.3)
		return;

	// set statistics : orientations
	v1 = Vec2d(cos(modes[0][2]), sin(modes[0][2]));
	v2 = Vec2d(cos(modes[1][2]), sin(modes[1][2]));
}
void lzh::refineScoreCorners(InputArray src, InputOutputArray corners, OutputArray refinescore, int32 scales, int32 r, Val tau, bool subPix)
{
	Array image = cvtColor()(src, ColorGamutGray);
	Mat img = image.convert(MAT_T);
	img = img / 255;
	// sobel masks
	Mat du = (Mat_(3, 3) <<
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1);
	Mat dv = du.t();

	// compute image derivatives(for principal axes estimation)
	Mat img_du, img_dv;
	conv2(img, img_du, du, MAT_T, CONVOLUTION_SAME);
	conv2(img, img_dv, dv, MAT_T, CONVOLUTION_SAME);
	Mat img_angle, img_weight;
	arcTan2(img_dv, img_du, img_angle);
	img_weight = ((img_dv.pow(2)) + (img_du.pow(2))).sqrt();

	// correct angle to lie in between[0, pi]
	QuadrantNorm(img_angle, img_angle, 0, PI, PI);

	// 3 scales
	vec_32s rad;
	// temp properties
	if (scales & FourRadius)
		rad.push_back(4);
	if (scales & EightRadius)
		rad.push_back(8);
	if (scales & TwelveRadius)
		rad.push_back(12);

	Array radius(rad);
	Array corner(corners);
	// image dimensions
	int32 width = img_du.cols();
	int32 height = img_dv.rows();

	// init orientations to invalid(corner is invalid iff orientation = 0)
	std::vector<Vec<mat_t, 5>> score(corner.rows);

	// for all corners do
	for (int32 i = 0; i < corner.rows; i++) {

		// extract current corner location
		int32 cu = (int32)corner(i, 0);
		int32 cv = (int32)corner(i, 1);

		// estimate edge orientations
		Rect rect(max(cu - r, 0), max(cv - r, 0), min(cu + r + 1, width - 1) - max(cu - r, 0), min(cv + r + 1, height - 1) - max(cv - r, 0));
		Mat img_angle_sub = img_angle(rect).clone();
		Mat img_weight_sub = img_weight(rect).clone();
		Vec2m V1, V2;
		edgeOrientations(img_angle_sub, img_weight_sub, V1, V2);
		score[i] = Vec<mat_t, 5>(V1[0], V1[1], V2[0], V2[1], 0);

		// continue, if invalid edge orientations
		if ((V1(0) == 0 && V1(1) == 0) || (V1(0) == 0 && V2(1) == 0)) {
			continue;
		}
		if (subPix) {
			Mat v1 = (Mat_(1, 2) << V1(0), V1(1));
			Mat v2 = (Mat_(1, 2) << V2(0), V2(1));
			// corner orientation refinement
			Mat A1 = Mat::zeros(Size(2, 2));
			Mat A2 = Mat::zeros(Size(2, 2));

			for (int32 u = max(cu - r, 0); u < min(cu + r + 1, width); u++) {
				for (int32 v = max(cv - r, 0); v < min(cv + r + 1, height); v++) {

					// pixel orientation vector
					Mat o = (Mat_(2) << img_du(v, u), img_dv(v, u));
					mat_t nor = o.norm(2);
					if (nor < 0.1)
						continue;
					o = o / nor;

					Mat dst = o * v1;
					// robust refinement of orientation 1
					if (abs(dst(0)) < 0.25) // inlier ? 
					{
						A1(0, 0) = A1(0, 0) + img_du(v, u) * img_du(v, u);
						A1(0, 1) = A1(0, 1) + img_du(v, u) * img_dv(v, u);
						A1(1, 0) = A1(1, 0) + img_dv(v, u) * img_du(v, u);
						A1(1, 1) = A1(1, 1) + img_dv(v, u) * img_dv(v, u);
					}
					dst = o * v2;
					// robust refinement of orientation 2
					if (abs(dst(0)) < 0.25) // inlier ? 
					{
						A2(0, 0) = A2(0, 0) + img_du(v, u) * img_du(v, u);
						A2(0, 1) = A2(0, 1) + img_du(v, u) * img_dv(v, u);
						A2(1, 0) = A2(1, 0) + img_dv(v, u) * img_du(v, u);
						A2(1, 1) = A2(1, 1) + img_dv(v, u) * img_dv(v, u);
					}

				}
			}
			// set new corner orientation
			eig(A1, v1);
			V1 = Vec2m(v1(1, 0), v1(1, 1));
			if (V1[0] == 1 || V1[1] == 1) {
				score[i][0] = V1[1]; score[i][1] = V1[0];
			}
			else {
				score[i][0] = -V1[0]; score[i][1] = -V1[1];
			}
			eig(A2, v2);
			V2 = Vec2m(v2(1, 0), v2(1, 1));
			if (V2[0] == 1 || V2[1] == 1) {
				score[i][2] = V2[1]; score[i][3] = V2[0];
			}
			else {
				score[i][2] = -V2[0]; score[i][3] = -V2[1];
			}
			v1 = Mat(V1).t();
			v2 = Mat(V2).t();
			//  corner location refinement  %
			Mat G = Mat::zeros(Size(2, 2));
			Mat b = Mat::zeros(Size(1, 2));
			for (int32 u = max(cu - r, 0); u < min(cu + r + 1, width); u++) {
				for (int32 v = max(cv - r, 0); v < min(cv + r + 1, height); v++) {

					// pixel orientation vector
					Mat o = (Mat_(2) << img_du(v, u), img_dv(v, u));
					mat_t nor = o.norm(2);
					if (nor < 0.1)
						continue;
					o = o / nor;

					// robust subpixel corner estimation
					if (u != cu || v != cv) { // do not consider center pixel
											  // compute rel.position of pixel and distance to vectors
						Mat w = (Mat_(2) << (u - cu), (v - cv));
						//w.show();
						mat_t d1 = (w - w * v1*v1.t()).norm(2);
						mat_t d2 = (w - w * v2*v2.t()).norm(2);
						Mat o1 = o * v1;
						Mat o2 = o * v2;
						// if pixel corresponds with either of the vectors / directions
						if ((d1 < 3 && abs(o1(0)) < 0.25) || (d2 < 3 && abs(o2(0)) < 0.25)) {
							mat_t du = img_du(v, u);
							mat_t dv = img_dv(v, u);
							Mat d = Vec2m(du, dv);
							Mat H = d.t()*d;
							G = G + H;
							b = b + H * Vec2m(u, v).t();
						}
					}
				}
			}
			G.show();
			// set new corner location if G has full rank
			if (rank(G) == 2) {
				Array corner_pos_old = corner.row(i);
				Mat corner_pos_new = (G.inv() * b).t();
				corner_pos_new.setTo(corner_pos_old);

				// set corner to invalid, if position update is very large
				if ((corner_pos_new - corner_pos_old).norm(2) >= 4) {
					score[i] = Vec<mat_t, 5>(0, 0, 0, 0, 0);
				}
			}
			// otherwise: set corner to invalid
			else {
				score[i] = Vec<mat_t, 5>(0, 0, 0, 0, 0);
			}
		}
		if (score[i][0] != 0 || score[i][1] != 0) {

			// corner location
			int32 u = (int32)round(corner(i, 0));
			int32 v = (int32)round(corner(i, 1));
			Vec3d score_;
			// compute corner statistics @ radius 1
			for (int32 j = 0; j < radius.rows*radius.cols; j++) {
				score_[j] = 0;
				if (u >= radius.at<int32>(j) && u < width - radius.at<int32>(j) && v >= radius.at<int32>(j) && v < height - radius.at<int32>(j)) {
					Rect rect(u - radius.at<int32>(j), v - radius.at<int32>(j), 2 * radius.at<int32>(j) + 1, 2 * radius.at<int32>(j) + 1);
					Mat img_sub = img(rect);
					Mat img_angle_sub = img_angle(rect).clone();
					Mat img_weight_sub = img_weight(rect).clone();
					score_[j] = cornerCorrelationScore(img_sub, img_weight_sub, Vec2m(score[i][0], score[i][1]), Vec2m(score[i][2], score[i][3]));
				}
			}
			// take highest score
			score[i][4] = Max(score_);
		}
	}

	// remove corners without edges
	vector<Vec<mat_t, 5>>::iterator pit;
	// remove low scoring corners
	for (pit = score.begin(); pit != score.end();)
	{
		if ((*pit)[4] < tau || ((*pit)[0] == 0 && (*pit)[1] == 0)) {
			pit = score.erase(pit);
		}
		else
			++pit;
		// make v1(:, 1) + v1(:, 2) positive(= > comparable to c++ code)
		if (pit != score.end() && (*pit)[0] + (*pit)[1] < 0) {
			(*pit)[0] = -(*pit)[0]; (*pit)[1] = -(*pit)[1];
		}
	}
	// make all coordinate systems right - handed(reduces matching ambiguities from 8 to 4)
	for (int32 i = 0; i < (int32)score.size(); i++) {
		mat_t v = score[i][0] * score[i][2] - score[i][1] * score[i][3];
		int32 flip;
		if (v < 0) {
			flip = 1;
		}
		else if (v > 0) {
			flip = -1;
		}
		else {
			flip = 0;
		}
		score[i][2] = flip * score[i][2];
		score[i][3] = flip * score[i][3];
	}
	refinescore.set(score);
	corners.set(corner);
}
void lzh::findConcers(InputArray src, OutputArray concers, OutputArray dst, int32 scales, int32 n, Val tau, int32 margin)
{
	Array image = cvtColor()(src, ColorGamutGray);
	Mat img = image.convert(MAT_T);
	// 3 scales
	vec_32s r;
	// temp properties
	vecs_m props;
	if (scales & FourRadius) {
		r.push_back(4);
		props.push_back({ 0, PI_ / 2, 4 });
		props.push_back({ PI_ / 4, -PI_ / 4, 4 });
	}
	if (scales & EightRadius) {
		r.push_back(8);
		props.push_back({ 0, PI_ / 2, 8 });
		props.push_back({ PI_ / 4, -PI_ / 4, 8 });
	}
	if (scales & TwelveRadius) {
		r.push_back(12);
		props.push_back({ 0, PI_ / 2, 12 });
		props.push_back({ PI_ / 4, -PI_ / 4, 12 });
	}
	Array radius(r);
	Mat template_props(props);

	// scale input image
	normalize(img, img, 0, 1, NORM_MINMAX);

	// filter image
	Mat img_corners = Mat::zeros(img.size3());
	Mat img_corners_a, img_corners_b, img_corners_1, img_corners_2;
	for (int32 template_class = 0; template_class < template_props.rows(); template_class++) {

		// create correlation temp
		vector<Mat> temp;
		createCorrelationPatch(temp, template_props(template_class, 0), template_props(template_class, 1), template_props(template_class, 2));
		// filter image according with current temp
		Mat img_corners_patch[4];
		std::thread th[4];
		for (int32 i : range(4)) {
			th[i] = std::thread([&](int32 idx) {
				conv2(img, img_corners_patch[idx], temp[idx], MAT_T, CONVOLUTION_SAME);
			}, i);
		}
		for (std::thread& t : th)t.join();
		// compute mean
		Mat img_corners_mu = (img_corners_patch[0] + img_corners_patch[1] + img_corners_patch[2] + img_corners_patch[3]) / 4;

		// case 1: a = white, b = black
		Min(img_corners_patch[0] - img_corners_mu, img_corners_patch[2] - img_corners_mu, img_corners_a);
		Min(img_corners_mu - img_corners_patch[1], img_corners_mu - img_corners_patch[3], img_corners_b);
		Min(img_corners_a, img_corners_b, img_corners_1);

		// case 2: b = white, a = black
		Min(img_corners_mu - img_corners_patch[0], img_corners_mu - img_corners_patch[2], img_corners_a);
		Min(img_corners_patch[1] - img_corners_mu, img_corners_patch[3] - img_corners_mu, img_corners_b);
		Min(img_corners_a, img_corners_b, img_corners_2);

		// update corner map
		Max(img_corners, img_corners_1, img_corners);
		Max(img_corners, img_corners_2, img_corners);
	}
	if(!dst.empty())
		dst = img_corners;
	// extract corner candidates via non maximum suppression
	nonMaximumSuppression(img_corners, concers, n, tau, margin);
}
void lzh::cornerSubPix(InputArray img, InputOutputArray corners, int32 scales, int32 r, Val tau)
{
	refineScoreCorners(img, corners, Mat(), scales, r, tau, true);
}