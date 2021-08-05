#pragma once
#ifndef __IMAGE_PROCESS_H__
#define __IMAGE_PROCESS_H__

#include "core.h"
#include "interface.h"

namespace lzh
{
	enum
	{
		NO_ANGLE = 0,

		ANGLE0_OFFSET = 0,
		ANGLE45_OFFSET = 1,
		ANGLE90_OFFSET = 2,
		ANGLE135_OFFSET = 3,
		ANGLE180_OFFSET = 4,
		ANGLE225_OFFSET = 5,
		ANGLE270_OFFSET = 6,
		ANGLE315_OFFSET = 7,

		ANGLE0 = 1 << ANGLE0_OFFSET,
		ANGLE45 = 1 << ANGLE45_OFFSET,
		ANGLE90 = 1 << ANGLE90_OFFSET,
		ANGLE135 = 1 << ANGLE135_OFFSET,
		ANGLE180 = 1 << ANGLE180_OFFSET,
		ANGLE225 = 1 << ANGLE225_OFFSET,
		ANGLE270 = 1 << ANGLE270_OFFSET,
		ANGLE315 = 1 << ANGLE315_OFFSET,
		ANGLEALL = 0xFF
	};
	enum DFSMethod
	{
		RECURSION = 0,
		ITERATION,
	};
	enum NeighborhoodType
	{
		Four_Dire = 4,
		Eight_Dire = 8
	};
	enum SharpenScale
	{
		SHARPEN_LEVEL1 = 1,
		SHARPEN_LEVEL2 = 2,
	};
	/**
	INTER_SAMPLE = 0,//采样
	INTER_MEAN = 1,//均值(只支持缩小)
	INTER_MAX = 2,//最大(只支持缩小)
	INTER_MIN = 3,//最小(只支持缩小)
	INTER_NEAREST = 4,//最邻近插值
	INTER_LINEAR = 5,//双线性插值(4邻域)
	INTER_AREA = 6,//放大使用双线性插值,缩小时行列缩放比例相同使用最邻近插值,行列缩放比例不同使用均值
	INTER_CUBIC = 7,//三次样条插值(8邻域)
	INTER_LANCZOS4 = 8 //Lanczos插值(15邻域)
	*/
	enum InterpolationMothed
	{
		INTER_SAMPLE = 0,
		INTER_MEAN = 1,
		INTER_MAX = 2,
		INTER_MIN = 3,
		INTER_NEAREST = 4,
		INTER_LINEAR = 5,
		INTER_AREA = 6,
		INTER_CUBIC = 7,
		INTER_LANCZOS4 = 8,
	};
	/**
	旋转方向顺时针
	ROTATE_90_ANGLE 90度
	ROTATE_180_ANGLE 180度
	ROTATE_270_ANGLE 270度
	*/
	enum RotateAngle
	{
		ROTATE_90_ANGLE = 0,
		ROTATE_180_ANGLE,
		ROTATE_270_ANGLE
	};
	/**
	二值化方法
	THRESH_BINARY = 0,
	THRESH_BINARY_INV = 1,
	THRESH_TRUNC = 2,
	THRESH_TOZERO = 3,
	THRESH_TOZERO_INV = 4,
	THRESH_OTSU = 5
	*/
	enum ThreshMethod
	{
		THRESH_BINARY = 0,
		THRESH_BINARY_INV = 1,
		THRESH_TRUNC = 2,
		THRESH_TOZERO = 3,
		THRESH_TOZERO_INV = 4,
		THRESH_OTSU = 5
	};
	/**
	归一化方法
	NORM_L1 = 0,
	NORM_L2 = 1,
	NORM_INF = 2,
	NORM_MINMAX = 3,
	NORM_SUM = 4,
	NORM_IMAGE = 5,
	NORM_ZSCORE = 6
	*/
	enum NormType
	{
		NORM_L1 = 0,
		NORM_L2 = 1,
		NORM_INF = 2,
		NORM_MINMAX = 3,
		NORM_SUM = 4,
		NORM_IMAGE = 5,
		NORM_ZSCORE = 6
	};
	/**
	模板匹配方法
	TEMPLATE_SQDIFF = 0,
	TEMPLATE_SQDIFF_NORMED,
	TEMPLATE_CCORR,
	TEMPLATE_CCORR_NORMED,
	TEMPLATE_CCOEFF,
	TEMPLATE_CCOEFF_NORMED
	*/
	enum MatchMethod
	{
		TEMPLATE_SQDIFF = 0,
		TEMPLATE_SQDIFF_NORMED,
		TEMPLATE_CCORR,
		TEMPLATE_CCORR_NORMED,
		TEMPLATE_CCOEFF,
		TEMPLATE_CCOEFF_NORMED
	};
	/**
	形态学核类型
	MORPH_RECT = 0,
	MORPH_CROSS = 1,
	MORPH_ELLIPSE = 2
	*/
	enum MorphShape
	{
		MORPH_RECT = 0,
		MORPH_CROSS = 1,
		MORPH_ELLIPSE = 2
	};
	/**
	形态学方法
	MORPH_OPEN = 0,	//开运算
	MORPH_CLOSE = 1,//闭运算
	MORPH_GRADIENT = 2,//形态学梯度
	MORPH_TOPHAT = 3,//顶帽
	MORPH_BLACKHAT = 4,//黑帽
	MORPH_ERODE = 5,//腐蚀
	MORPH_DILATE = 6	//膨胀
	*/
	enum MorphMethod
	{
		MORPH_OPEN = 0,//开运算
		MORPH_CLOSE = 1,//闭运算
		MORPH_GRADIENT = 2,//形态学梯度
		MORPH_TOPHAT = 3,//顶帽
		MORPH_BLACKHAT = 4,//黑帽
		MORPH_ERODE = 5,//腐蚀
		MORPH_DILATE = 6	//膨胀
	};
	enum ConvolutionType {
		/* Return the full convolution, including border */
		CONVOLUTION_FULL,

		/* Return only the part that corresponds to the original image */
		CONVOLUTION_SAME,

		/* Return only the submatrix containing elements that were not influenced by the border */
		CONVOLUTION_VALID
	};
	enum MosaicMethod {
		MosaicRandom = 0,
		MosaicAngle0,
		MosaicAngle45,
		MosaicAngle90,
		MosaicAngle135,
		MosaicAngle180,
		MosaicAngle225,
		MosaicAngle270,
		MosaicAngle315,
	};
	enum
	{
		FourRadius = 0x01,
		EightRadius = 0x02,
		TwelveRadius = 0x04
	};
	class LZHAPI Moments
	{
	public:
		Moments();
		Moments(float64 m00, float64 m10, float64 m01, float64 m20, float64 m11,
			float64 m02, float64 m30, float64 m21, float64 m12, float64 m03);
		Point2d center()const;
		float64 theta()const;
		//空间矩
		float64 m00, m10, m01, m20, m11, m02, m30, m21, m12, m03;
		//中心矩
		float64 mu20, mu11, mu02, mu30, mu21, mu12, mu03;
		//中心归一化矩
		float64 nu20, nu11, nu02, nu30, nu21, nu12, nu03;
	};

	class FilterMask
	{
	public:
		virtual mat_t f(int32 x, int32 y)const = 0;
		mat_t operator ()(int32 x, int32 y)const;
	};
	class ButterworthSpectrum : public FilterMask
	{
	public:
		ButterworthSpectrum(mat_t d, mat_t n, mat_t th, mat_t tl)
			: d(d * d), n(n), upper(th), lower(tl) {}
		mat_t f(int32 x, int32 y)const;

		mat_t d;
		mat_t n;
		mat_t upper;
		mat_t lower;
	};
	class GaussianSpectrum : public FilterMask
	{
	public:
		GaussianSpectrum(mat_t sigma) : sigma(sigma) {}
		mat_t f(int32 x, int32 y)const;

		mat_t sigma;
	};
	typedef mat_t(*FilterOp)(const Array&, const Array&, int32, int32, int32, int32);
	/**
	@brief CalSize 计算卷积所需扩张的边界
	返回矩阵大小
	@param src		被卷积矩阵
	@param kern		卷积核
	@param anchor	像素对应卷积核坐标
	anchor默认为Point(-1,-1), 像素对应卷积核中心
	@param strides	滑动步长
	@param top		向上扩充几行
	@param bottom	向下扩充几行
	@param left		向左扩充几列
	@param right	向右扩充几列
	*/
	extern LZHAPI Size3 CalSize(Size3 src, Size3 kern, Point anchor, Size strides, int32 &top, int32 &bottom, int32 &left, int32 &right);
	/**
	@brief CalSize 计算卷积所需扩张的边界
	返回矩阵大小
	@param src		被卷积矩阵尺寸
	@param kern		卷积核尺寸
	@param anchor	像素对应卷积核坐标
	*/
	extern LZHAPI Size3 CalSize(Size3 src, Size3 kern, Point anchor, Size strides);
	/**
	@brief CalSize 计算卷积所需扩张的边界
	返回矩阵大小
	@param src		被卷积矩阵尺寸
	@param kern		卷积核尺寸
	@param anchor	像素对应卷积核坐标
	*/
	extern LZHAPI Size3 CalSize(Size3 src, Size kern, Point anchor, Size strides);
	extern LZHAPI void HomomorphicFilter(InputArray src, OutputArray dst, Val D0, Val n, Val rL, Val rH);
	extern LZHAPI void GaussianFilter(InputArray src, OutputArray dst, Size ksize, Val sigma, BorderType borderType = BORDER_DEFAULT);
	extern LZHAPI void unevenLightCompensate(InputArray image_, OutputArray dst, Size blockSize);
	extern LZHAPI void GaussianBlur(InputArray src, OutputArray dst, Size ksize, Val sigma, BorderType borderType = BORDER_DEFAULT);
	extern LZHAPI void medianBlur(InputArray src, OutputArray dst, int32 ksize);
	extern LZHAPI void medianBlur(InputArray src, OutputArray dst, Size ksize);
	extern LZHAPI void blur(InputArray src, OutputArray dst, Size kernel, Point anchor = Point(-1, -1), int32 iteration = 1);
	extern LZHAPI void erode(InputArray src, OutputArray dst, InputArray kernel, int32 iteration = 1, Point anchor = Point(-1, -1));
	extern LZHAPI void dilate(InputArray src, OutputArray dst, InputArray kernel, int32 iteration = 1, Point anchor = Point(-1, -1));
	extern LZHAPI Array getStructuringElement(MorphShape morphshape, Size elementsize);
	extern LZHAPI void morphologyEx(InputArray src, OutputArray dst, MorphMethod morphtype, InputArray kernel, int32 iteration = 1, Point anchor = Point(-1, -1));
	extern LZHAPI void reverse(InputArray src, OutputArray dst, Val v);
	extern LZHAPI void matchTemplate(InputArray src, InputArray template_, OutputArray dst, MatchMethod method);
	extern LZHAPI void inRange(InputArray src, InputArray lowerb, InputArray upperb, OutputArray dst);
	/**
	@brief 返回按boundary分界填充的矩阵
	返回矩阵大小等于输入矩阵大小
	@param src				输入矩阵
	@param boundary			分界值
	@param lower			小于boundary用lower填充
	@param upper			大于boundary用upper填充
	@param boundary2upper	当矩阵元素等于boundary时(为1归upper,为-1归lower, 为0不处理)
	*/
	extern LZHAPI void Threshold(InputArray src, OutputArray dst, Val boundary, Val lower, Val upper, int32 boundary2upper = 1);
	/**
	@brief 二值化
	返回矩阵大小等于输入矩阵大小
	@param src				输入矩阵
	@param dst				输出矩阵
	@param thresh			阈值
	@param maxval			大于boundary用thresh填充
	@param method			二值化方法
	*/
	extern LZHAPI mat_t Threshold(InputArray src, OutputArray dst, Val thresh, Val maxval, ThreshMethod method);
	/**
	@brief wellner自适应阈值二值化
	@param src				输入矩阵
	@param dst				输出矩阵
	@param minval			小于阈值填充值
	@param maxval			大于阈值填充值
	@param area_pro			二值化方法
	@param percent			百分比阈值
	*/
	extern LZHAPI void adaptiveThreshold(InputArray src, OutputArray dst, Val minval, Val maxval, int32 area_pro = 3, int32 percent = 15);
	extern LZHAPI mat_t getOtsuThreshold(InputArray src);
	/**
	@brief 返回边界扩充的矩阵
	@param src			输入矩阵
	@param newSize		新矩阵尺寸
	@param borderType	边界像素外推的插值方法
	@param value		常量插值的数值
	**/
	extern LZHAPI void copyMakeBorder(InputArray src, OutputArray dst, Size newSize, BorderType borderType, const Color& value = Color(0));
	/**
	@brief 返回边界扩充的图像
	@param src			输入图像
	@param newSize		新图像尺寸
	@param borderType	边界像素外推的插值方法
	@param value		常量插值的数值
	**/
	//extern LZHAPI void copyMakeBorder(InputArray src, OutputArray dst, Size newSize, BorderType borderType, const Color &value);
	/**
	@brief 返回边界扩充的矩阵
	@param src			输入矩阵
	@param top			向上扩充几行
	@param bottom		向下扩充几行
	@param left			向左扩充几列
	@param right		向右扩充几列
	@param borderType	边界像素外推的插值方法
	@param value		常量插值的数值
	**/
	//extern LZHAPI void copyMakeBorder(InputArray src, OutputArray dst, int32 top, int32 bottom, int32 left, int32 right, BorderType borderType, const Color &value);
	/**
	@brief 返回边界扩充的矩阵
	@param src			输入矩阵
	@param top			向上扩充几行
	@param bottom		向下扩充几行
	@param left			向左扩充几列
	@param right		向右扩充几列
	@param borderType	边界像素外推的插值方法
	@param value		常量插值的数值
	**/
	extern LZHAPI void copyMakeBorder(InputArray src, OutputArray dst, int32 top, int32 bottom, int32 left, int32 right, BorderType borderType = BORDER_CONSTANT, const Color &value = Color(0));
	extern LZHAPI void copyMakeBorder(InputOutputArray src, Rect oldrect, int32 top, int32 bottom, int32 left, int32 right, BorderType borderType = BORDER_CONSTANT, const Color &value = Color(0));
	/**
	@brief 返回矩阵2维卷积结果(只支持二维输入)
	返回矩阵大小为(input.h_/strides_x, input.w_/strides_y, 1)
	@param input			输入矩阵
	@param kern				卷积核
	@param anchor			矩阵元素对应卷积核的位置
	以卷积核的左上角为(0,0)点, 默认(-1,-1)为中心
	@param strides			滑动步长
	Size.hei为x轴,Size.wid为y轴
	@param is_copy_border	是否要扩展边界
	*/
	extern LZHAPI void Filter2D(InputArray src, OutputArray dst, InputArray kern, int32 depth, Point anchor = Point(-1, -1), Size strides = Size(1, 1), bool is_copy_border = true, BorderType borderType = BORDER_CONSTANT, Val value = 0);
	extern LZHAPI void filter(InputArray src, OutputArray dst, InputArray kern, int32 depth, FilterOp op, Point anchor = Point(-1, -1), Size strides = Size(1, 1), bool is_copy_border = true, BorderType borderType = BORDER_CONSTANT, Val value = 0);
	extern LZHAPI mat_t FilterOperator(const Array& src, const Array& kern, int32 row, int32 col, int32 channel, int32 kernelchannel = -1);
	extern LZHAPI void Convolution(InputArray src, OutputArray dst, InputArray kern, int32 depth, FilterOp op, Point anchor = Point(-1, -1), Size strides = Size(1, 1), bool is_copy_border = true, BorderType borderType = BORDER_CONSTANT, Val value = 0);
	extern LZHAPI void img2col(InputArray src, OutputArray dst, Size kernel, Point anchor = Point(-1, -1), Size strides = Size(1, 1), bool is_copy_border = true, BorderType borderType = BORDER_CONSTANT, Val value = 0);
	//extern LZHAPI void fastFilter2D(InputArray src, OutputArray dst, InputArray kern, Point anchor = Point(-1, -1), Size strides = Size(1, 1), bool is_copy_border = true, BorderType borderType = BORDER_CONSTANT, Val value = 0.0);
	extern LZHAPI void filter2D(InputArray src, OutputArray dst, InputArray kern, int32 depth, ConvolutionType type, BorderType border_types, Val value = 0);
	extern LZHAPI void fastFilter(InputArray src, OutputArray dst, InputArray kern, int32 depth, ConvolutionType type, BorderType border_types = BORDER_CONSTANT, const Color & value = Color(0));
	/**
	@brief 返回矩阵2维卷积结果(支持三维输入)
	返回矩阵大小为(input.h_/strides_x, input.w_/strides_y, 1)
	@param src				输入矩阵
	@param dst				输出矩阵
	@param kern				卷积核
	@param anchor			矩阵元素对应卷积核的位置
	以卷积核的左上角为(0,0)点, 默认(-1,-1)为中心
	@param strides			滑动步长
	Size.hei为x轴,Size.wid为y轴
	@param is_copy_border	是否要扩展边界
	*/
	extern LZHAPI void Conv2D(InputArray src, OutputArray dst, InputArray kern, int32 depth, Point anchor = Point(-1, -1), const Size & strides = Size(1, 1), bool is_copy_border = true, BorderType border_types = BORDER_CONSTANT, const Color & value = Color(0));
	extern LZHAPI void conv2(InputArray src, OutputArray dst, InputArray kern, int32 depth, ConvolutionType type, BorderType border_types = BORDER_CONSTANT, const Color & value = Color(0));
	/**
	实现矩阵在水平方向，垂直方向和水平垂直方向的翻转
	@param src				输入矩阵
	@param dst				输出矩阵
	@param filpCode			旋转类型，0代表x轴旋转，任意正数代表y轴旋转，任意负数代表x和y轴同时旋转
	*/
	extern LZHAPI void flip(InputArray src, OutputArray dst, int32 flipCode = 0);
	extern LZHAPI void HighPass(InputArray src, OutputArray dst, InputArray kernel, Val r, BorderType border_types = BORDER_DEFAULT);
	extern LZHAPI void HighPass(InputArray src, OutputArray dst, Val r = 3, Size ksize = Size(7, 7), Val sigma = 1.6);
	extern LZHAPI Array ButterworthFilter(Size sz, Val D, Val n, Val high_h_v_TB, Val low_h_v_TB);
	extern LZHAPI Array FourierTransform(InputArray frame_bw, OutputArray image_complex, Mat *image_phase = nullptr, Mat *image_mag = nullptr);
	extern LZHAPI void InvFourierTransform(InputArray input, OutputArray inverseTransform);
	extern LZHAPI void SobelFact(Mat &kern, int32 kernelSize, bool XOrder);
	extern LZHAPI OMat SobelFact(int32 ksize, int32 angle);
	extern LZHAPI OMat RobertsKernel(int32 angle);
	extern LZHAPI OMat PrewittKernel(int32 angle);
	extern LZHAPI OMat SobelKernel(int32 angle);
	extern LZHAPI OMat LapacianKernel(NeighborhoodType neighbourhood = Four_Dire);
	extern LZHAPI OMat GaussianKernel(Size ksize, Val sigma, bool isnorm = true);
	extern LZHAPI OMat LoGKernel(Size ksize, Val sigma, bool isnorm = true);
	extern LZHAPI OMat DoGKernel(Size ksize, Val sigma1, Val sigma2, bool isnorm = true);
	extern LZHAPI OMat SharpenKernel(SharpenScale level);
	extern LZHAPI void ConvKernel(InputArray src, OutputArray dst, int32 depth, OMat(*CreatKernel)(int32), int32 angle);
	extern LZHAPI void Sobel(InputArray src, OutputArray dst, int32 depth, int32 angle);
	extern LZHAPI void Sobel(InputArray src, OutputArray dst, int32 depth, int32 dx, int32 dy, int32 ksize = 3, mat_t scale = 1, mat_t delta = 0,BorderType borderType = BORDER_DEFAULT);
	extern LZHAPI void Roberts(InputArray src, OutputArray dst, int32 depth, int32 angle);
	extern LZHAPI void Prewitt(InputArray src, OutputArray dst, int32 depth, int32 angle);
	extern LZHAPI void Sharpen(InputArray src, OutputArray dst, int32 depth, SharpenScale level);
	extern LZHAPI void Lapacian(InputArray src, OutputArray dst, int32 depth, NeighborhoodType laplacetype = Four_Dire);
	extern LZHAPI void LoG(InputArray src, OutputArray dst, int32 depth, Size ksize, Val sigma);
	extern LZHAPI void DoG(InputArray src, OutputArray dst, int32 depth, Size ksize, Val sigma1, Val sigma2);
	extern LZHAPI void Roberts(InputArray src, OutputArray dst, int32 depth);
	extern LZHAPI void Prewitt(InputArray src, OutputArray dst, int32 depth);
	/**
	sobel算子卷积图像
	@param src				输入矩阵
	@param dst				输出矩阵
	*/
	extern LZHAPI void Sobel(InputArray src, OutputArray dst, int32 depth);

	extern LZHAPI void Canny(InputArray dx, InputArray dy, OutputArray edges, Val threshold1, Val threshold2, bool L2gradient = true);
	extern LZHAPI void Canny(InputArray image, OutputArray edges, Val threshold1, Val threshold2, bool L2gradient = true, Size ksize = Size(5, 5), Val sigma = 1);

	class LZHAPI Dfs
	{
	public:
		typedef bool(*DfsOperator)(InputOutputArray, Point, Object*);
		typedef void(*DfsCallBack)(Object*);

		explicit Dfs(DFSMethod method = ITERATION, NeighborhoodType type = Eight_Dire);
		void DfsMark(InputOutputArray image, Val thresh, DfsOperator op, Object* pre = nullptr, DfsCallBack callback = nullptr);
		void DfsMark(InputOutputArray image, Point seedpoint, DfsOperator op, Object* pre = nullptr, DfsCallBack callback = nullptr);
		void DfsMarkRecursion(InputOutputArray image, Val thresh, DfsOperator op, Object* pre = nullptr, DfsCallBack callback = nullptr);
		void DfsMarkRecursion(InputOutputArray image, int32 col, int32 row, DfsOperator op, Object*pre = nullptr, int32 dire = 0);
		void DfsMarkRecursion(InputOutputArray image, Point seedpoint, DfsOperator op, Object*pre = nullptr, DfsCallBack callback = nullptr);
		int32 DfsCalDirection(int32 &angle, int32 angle_offset);
		bool DfsBackDirection(Point &p, int32 &state, int32 &angle);
		bool DfsForwardDirection(int32 *stateArr, Point &p, int32 &angle, int32 angle_flag, int32 rows, int32 cols, int32 dy = 0, int32 dx = 0);
		bool DfsForwardDirection(int32 *angleArr, int32 *stateArr, Point &p, int32 &angle, int32 rows, int32 cols);
		void DfsMarkIteration(InputOutputArray image, Val thresh, DfsOperator op, Object*pre = nullptr, DfsCallBack callback = nullptr);
		void DfsMarkIteration(InputOutputArray image, Point seedpoint, DfsOperator op, Object*pre = nullptr, DfsCallBack callback = nullptr);
		void DfsMarkIteration(InputOutputArray image, int32 *angleArr, int32 *stateArr, Point seedpoint, DfsOperator op, Object * pre = nullptr, DfsCallBack callback = nullptr);
	private:
		DFSMethod method;
		NeighborhoodType dire;
	};

	extern LZHAPI void MarkMainArea(InputOutputArray img, int32 col, int32 row, int32 dire);
	extern LZHAPI void SearchMainArea(InputOutputArray img);

	extern LZHAPI void floodFill(InputOutputArray image, Point seedPoint, Color newVal);
	extern LZHAPI void floodFill(InputOutputArray image, InputOutputArray mark, Point seedPoint, Color newVal);
	extern LZHAPI void floodHole(InputArray src, OutputArray dst, Color newVal = Color(255));
	extern LZHAPI void findContours(InputArray image, OutputArrayOfArrays contours, Val threshold1, Val threshold2, bool L2gradient = true, Size ksize = Size(5, 5), Val sigma = 3);
	extern LZHAPI Rect bouningRect(InputArray contour);
	extern LZHAPI RectF bouningRectf(InputArray contour);
	extern LZHAPI Point bouningCenter(InputArray contour);
	extern LZHAPI Point2f bouningCenterf(InputArray contour);
	extern LZHAPI Moments moments(InputArray array, bool binaryImage = false);
	extern LZHAPI void HuMoments(const Moments& mu, mat_t hu[7]);
	extern LZHAPI void HuMoments(const Moments& m, OutputArray hu);

	/*********************************************************************
	绘制图像
	**********************************************************************/
	extern LZHAPI void SetPixel(InputOutputArray src, Point point, int32 x, int32 y, const Color & color);
	template<typename _Tp> TEMPLATE_API void EllipsePlot(std::vector<Point2_<_Tp>>& contour, Point center, int32 x, int32 y);
	template<typename _Tp> TEMPLATE_API void addContour(std::vector<Point2_<_Tp>>& contour, Point2_<_Tp> point, int32 x, int32 y);
	extern LZHAPI void BresenhamEllipse(OutputArray ellipsePoint, Point center, int32 a, int32 b);
	extern LZHAPI void drawEllipse(InputOutputArray ellipsePoints, Point point, int32 a, int32 b);
	extern LZHAPI void BresenhamCircle(InputOutputArray circlePoints, Point point, int32 radius, int32 lineWidth = 1);
	extern LZHAPI void BresenhamCircle(InputOutputArray src, Point point, int32 radius, const Color &color, int32 lineWidth, bool fill = false);
	extern LZHAPI void drawPoint(InputOutputArray src, Point p, const Color & color);
	extern LZHAPI void circle(InputOutputArray src, Point p, int32 radius, const Color &color, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void circle(InputOutputArray src, int32 x, int32 y, int32 radius, const Color &color, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void drawFill(InputOutputArray src, vec_p2_32s &contour, const Color & color);
	extern LZHAPI void lines(OutputArray src, Point p1, Point p2);
	extern LZHAPI void lines(InputOutputArray src, Point p1, Point p2, const Color & color, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void lines(InputOutputArray src, InputArray points, const Color & color, int32 lineWidth = 1, bool fill = false);
	//extern LZHAPI void frontSight(OutputArray src, Point center, Size size, int32 lineWidth = 1);
	//extern LZHAPI void frontSight(InputOutputArray src, int32 x, int32 y, int32 width, int32 height, const Color & color, int32 lineWidth = 1);
	//extern LZHAPI void frontSight(InputOutputArray src, Point center, Size size, const Color & color, int32 lineWidth = 1);
	extern LZHAPI void crossShape(OutputArray src, Point center, Size size, int32 lineWidth = 1, Size in_size = Size(0,0));
	extern LZHAPI void crossShape(InputOutputArray src, int32 x, int32 y, int32 width, int32 height, const Color & color, int32 lineWidth = 1, int32 in_width = 0, int32 in_height = 0);
	extern LZHAPI void crossShape(InputOutputArray src, Point center, Size size, const Color & color, int32 lineWidth = 1, Size in_size = Size(0, 0));
	extern LZHAPI void rectangle(InputOutputArray src, int32 x1, int32 y1, int32 x2, int32 y2, const Color & color, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void rectangle(InputOutputArray src, Rect rect, const Color & color, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void rectangle(InputOutputArray src, Point p1, Point p2, const Color & color, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void drawContours(InputOutputArray src, InputArray contours, const Color & color, int32 radius = 1, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void drawContours(InputOutputArray src, const vecs_p2_32s & contours, int32 index, int32 radius, const Color & color, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void drawContour(InputOutputArray src, const vec_p2_32s & contours, const Color & color, int32 radius = 1, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void drawContour(InputOutputArray src, InputArray contours, const Color & color, int32 radius = 1, int32 lineWidth = 1, bool fill = false);
	extern LZHAPI void projection(InputArray src, OutputArray vertical, OutputArray horizontal);
	extern LZHAPI void verticalProjection(InputArray src, OutputArray vertical);
	extern LZHAPI void horizontalProjection(InputArray src, OutputArray horizontal);
	extern LZHAPI void histogram(InputArray src, OutputArray _histogram);
	extern LZHAPI void equalizeHistogram(InputArray src, OutputArray dst);
	extern LZHAPI void mosaic(InputArray src, OutputArray dst, Rect mosaicRange, int32 ksize = 3, MosaicMethod method = MosaicAngle135);
	extern LZHAPI void mosaic(InputOutputArray image, Point mosaicRange, int32 ksize = 3, MosaicMethod method = MosaicAngle135);
	extern LZHAPI void mosaic(InputArray src, OutputArray dst, vec_p2_32s mosaicRange, int32 ksize = 3, MosaicMethod method = MosaicAngle135);
	/**
	色域转换类
	*/
	class LZHAPI cvtColor
	{
	public:
		/**
		按色域编码进行图像色域转换
		@param	src  输入
		@param	dst  输出
		@param	code  色域编码
		*/
		static void ConverTo(InputArray src, OutputArray dst, ColorGamut code);
		/**
		按色域编码进行图像色域转换
		@param	src  输入
		@param	dst  输出
		@param	code  色域编码
		*/
		static Array Convert(InputArray src, ColorGamut code);


		/**
		cmyk色域转rbg色域
		*/
		static void CMYK2RGB(InputArray src, OutputArray dst);
		/**
		cmyk色域转bgr色域
		*/
		static void CMYK2BGR(InputArray src, OutputArray dst);
		/**
		cmyk色域转rbga色域
		*/
		static void CMYK2RGBA(InputArray src, OutputArray dst);
		/**
		cmyk色域转bgra色域
		*/
		static void CMYK2BGRA(InputArray src, OutputArray dst);
		/**
		rbg色域转cmyk色域
		*/
		static void RGB2CMYK(InputArray src, OutputArray dst);
		/**
		bgr色域转cmyk色域
		*/
		static void BGR2CMYK(InputArray src, OutputArray dst);
		/**
		rbga色域转cmyk色域
		*/
		static void RGBA2CMYK(InputArray src, OutputArray dst);
		/**
		bgra色域转cmyk色域
		*/
		static void BGRA2CMYK(InputArray src, OutputArray dst);


		/**
		luv色域转rbg色域
		*/
		static void Luv2RGB(InputArray src, OutputArray dst);
		/**
		luv色域转bgr色域
		*/
		static void Luv2BGR(InputArray src, OutputArray dst);
		/**
		rbg色域转luv色域
		*/
		static void RGB2Luv(InputArray src, OutputArray dst);
		/**
		bgr色域转luv色域
		*/
		static void BGR2Luv(InputArray src, OutputArray dst);
		/**
		lab色域转xyz色域
		*/
		static void Luv2XYZ(InputArray src, OutputArray dst);
		/**
		xyz色域转lab色域
		*/
		static void XYZ2Luv(InputArray src, OutputArray dst);


		/**
		lab色域转rbg色域
		*/
		static void Lab2RGB(InputArray src, OutputArray dst);
		/**
		lab色域转bgr色域
		*/
		static void Lab2BGR(InputArray src, OutputArray dst);
		/**
		rbg色域转lab色域
		*/
		static void RGB2Lab(InputArray src, OutputArray dst);
		/**
		bgr色域转lab色域
		*/
		static void BGR2Lab(InputArray src, OutputArray dst);
		/**
		lab色域转xyz色域
		*/
		static void Lab2XYZ(InputArray src, OutputArray dst);
		/**
		xyz色域转lab色域
		*/
		static void XYZ2Lab(InputArray src, OutputArray dst);


		/**
		xyz色域转bgr色域
		*/
		static void XYZ2BGR(InputArray src, OutputArray dst);
		/**
		xyz色域转rbg色域
		*/
		static void XYZ2RGB(InputArray src, OutputArray dst);
		/**
		bgr色域转xyz色域
		*/
		static void BGR2XYZ(InputArray src, OutputArray dst);
		/**
		rgb色域转xyz色域
		*/
		static void RGB2XYZ(InputArray src, OutputArray dst);


		/**
		xyz色域转yxy色域
		*/
		static void XYZ2YXY(InputArray src, OutputArray dst);
		/**
		yxy色域转xyz色域
		*/
		static void YXY2XYZ(InputArray src, OutputArray dst);
		/**
		yxy色域转bgr色域
		*/
		static void YXY2BGR(InputArray src, OutputArray dst);
		/**
		yxy色域转rbg色域
		*/
		static void YXY2RGB(InputArray src, OutputArray dst);
		/**
		bgr色域转yxy色域
		*/
		static void BGR2YXY(InputArray src, OutputArray dst);
		/**
		rgb色域转yxy色域
		*/
		static void RGB2YXY(InputArray src, OutputArray dst);


		/**
		hsv色域转bgr色域
		*/
		static void HSV2BGR(InputArray src, OutputArray dst);
		/**
		hsv色域转rbg色域
		*/
		static void HSV2RGB(InputArray src, OutputArray dst);
		/**
		bgr色域转hsv色域
		*/
		static void BGR2HSV(InputArray src, OutputArray dst);
		/**
		rgb色域转hsv色域
		*/
		static void RGB2HSV(InputArray src, OutputArray dst);


		/**
		hsl色域转bgr色域
		*/
		static void HSL2BGR(InputArray src, OutputArray dst);
		/**
		hsl色域转rbg色域
		*/
		static void HSL2RGB(InputArray src, OutputArray dst);
		/**
		bgr色域转hsl色域
		*/
		static void BGR2HSL(InputArray src, OutputArray dst);
		/**
		rgb色域转hsl色域
		*/
		static void RGB2HSL(InputArray src, OutputArray dst);


		/**
		bgr色域转rbg色域
		*/
		static void BGR2RGB(InputArray src, OutputArray dst);
		/**
		rbg色域转bgr色域
		*/
		static void RGB2BGR(InputArray src, OutputArray dst);
		/**
		bgr色域转bgra色域
		*/
		static void BGR2BGRA(InputArray src, OutputArray dst);
		/**
		rgb色域转RGBA色域
		*/
		static void RGB2RGBA(InputArray src, OutputArray dst);


		/**
		灰度转bgra色域
		*/
		static void GRAY2BGRA(InputArray src, OutputArray dst);
		/**
		灰度转bgr色域
		*/
		static void GRAY2BGR(InputArray src, OutputArray dst);
		/**
		灰度转rgba色域
		*/
		static void GRAY2RGBA(InputArray src, OutputArray dst);
		/**
		灰度转rgb色域
		*/
		static void GRAY2RGB(InputArray src, OutputArray dst);


		/**
		rgba色域转rgb色域
		*/
		static void RGBA2RGB(InputArray src, OutputArray dst);
		/**
		rgba色域转rgb色域
		*/
		static void RGBA2BGR(InputArray src, OutputArray dst);
		/**
		bgra色域转bgr色域
		*/
		static void BGRA2BGR(InputArray src, OutputArray dst);
		/**
		rgba色域转rgb色域
		*/
		static void RGBA2BGRA(InputArray src, OutputArray dst);
		/**
		bgra色域转bgr色域
		*/
		static void BGRA2RGBA(InputArray src, OutputArray dst);
		/**
		rgba色域转rgb色域
		*/
		static void RGB2BGRA(InputArray src, OutputArray dst);
		/**
		bgra色域转bgr色域
		*/
		static void BGR2RGBA(InputArray src, OutputArray dst);
		/**
		bgra色域转bgr色域
		*/
		static void BGRA2RGB(InputArray src, OutputArray dst);
		/**
		rgb色域转灰度
		*/
		static void RGB2GRAY(InputArray src, OutputArray dst);
		/**
		rgba色域转灰度
		*/
		static void RGBA2GRAY(InputArray src, OutputArray dst);
		/**
		bgr色域转灰度
		*/
		static void BGR2GRAY(InputArray src, OutputArray dst);
		/**
		bgra色域转灰度
		*/
		static void BGRA2GRAY(InputArray src, OutputArray dst);
		/**
		rgb色域转平均灰度
		*/
		static void toMean(InputArray src, OutputArray dst);
		/**
		rgb色域转最大最小灰度
		*/
		static void toMaxMin(InputArray src, OutputArray dst);
		/**
		rgb色域取r通道转灰度
		*/
		static void toRad(InputArray src, OutputArray dst);
		/**
		rgb色域取g通道转灰度
		*/
		static void toGreen(InputArray src, OutputArray dst);
		/**
		rgb色域取b通道转灰度
		*/
		static void toBlue(InputArray src, OutputArray dst);

		void operator()(InputArray src, OutputArray dst, ColorGamut code) const;
		const Array operator ()(InputArray src, ColorGamut code)const;

	protected:
		static void main_convert(const Array &src, OutputArray dst, ColorGamut code, Color::Spec input_spec, Color::Spec output_spec);
	};

	extern LZHAPI void subSampled(InputArray src, OutputArray dst, const Size& dstsize = Size(), BorderType borderType = BORDER_DEFAULT);
	extern LZHAPI void subSampled(InputArray src, OutputArray dst, const Size&  dstsize, void(*zoom_out)(InputArray, OutputArray, Size), BorderType borderType = BORDER_DEFAULT);
	extern LZHAPI void upSampling(InputArray src, OutputArray dst, const Size& dstsize = Size(), BorderType borderType = BORDER_DEFAULT);
	extern LZHAPI void upSampling(InputArray src, OutputArray dst, const Size&  dstsize, void(*zoom_in)(InputArray, OutputArray, Size), BorderType borderType = BORDER_DEFAULT);
	/**
	仿射变换
	*/
	extern LZHAPI void warpAffine(InputArray src, OutputArray dst, InputArray M);
	/**
	傅里叶变换
	*/
	extern LZHAPI void dft(InputArray src, OutputArray dst);
	/**
	傅里叶逆变换
	*/
	extern LZHAPI void idft(InputArray src, OutputArray dst);
	/**
	二维傅里叶变换
	*/
	extern LZHAPI Array dft2D(InputArray src);
	/**
	二维逆傅里叶变换
	*/
	extern LZHAPI Array idft2D(InputArray src);
	/**
	二维快速傅里叶变换
	*/
	extern LZHAPI Array fft2D(InputArray src);
	/**
	二维快速逆傅里叶变换
	*/
	extern LZHAPI Array ifft2D(InputArray src);
	/**
	频谱搬移至中心
	*/
	extern LZHAPI void dftShift(InputOutputArray src);
	/**
	频谱还原
	*/
	extern LZHAPI void idftShift(InputOutputArray src);
	/**
	频谱搬移至中心
	*/
	extern LZHAPI void dftShift(InputArray src, OutputArray dst);
	/**
	频谱还原
	*/
	extern LZHAPI void ifftShift(InputArray src, OutputArray dst);
	/**
	返回是否符合快速傅里叶的要求尺寸
	*/
	extern LZHAPI bool isOptimalDFTSize(int32 size);
	/**
	返回是否符合快速傅里叶的要求尺寸
	*/
	extern LZHAPI bool isOptimalDFTSize(Size size);
	/**
	返回是否符合快速傅里叶的要求尺寸
	*/
	extern LZHAPI int32 getOptimalDFTSize(int32 size, bool canResize);
	/**
	返回快速傅里叶的要求尺寸
	*/
	extern LZHAPI Size getOptimalDFTSize(Size size, bool canResize);
	/**
	扩展图像至快速傅里叶的要求尺寸
	*/
	extern LZHAPI void copyBorderDFTSize(InputArray src, OutputArray dst, bool canResize = false);
	/**
	归一化
	*/
	extern LZHAPI void normalize(InputArray src, OutputArray dst, Val alpha = 1, Val beta = 0, NormType norm_type = NORM_L2);
	/**
	生成映射矩阵
	*/
	extern LZHAPI Array getPerspectiveTransform(const Point2f* src, const Point2f* dst);
	/**
	生成映射矩阵
	*/
	extern LZHAPI void getPerspectiveTransform(InputArray src, InputArray dst, OutputArray transform);
	/**
	@brief 返回旋转矩阵
	@param src	输入
	@param dice 旋转角度
	*/
	extern LZHAPI void rotate(InputArray src, RotateAngle dice, OutputArray dst);
	/**
	@brief 返回旋转矩阵
	@param src	输入
	@param dice 旋转角度
	*/
	extern LZHAPI void rotate(InputArray src, OutputArray dst, int32 angle);
	/**
	按比例缩放矩阵
	@param src		输入
	@param dst		输出
	@param xRatio	x缩放比例
	@param yRatio	y缩放比例
	@param mothed	缩放方法
	*/
	extern LZHAPI void resize(InputArray src, OutputArray dst, Val xRatio, Val yRatio, InterpolationMothed mothed = INTER_LINEAR, mat_t alpha = -0.75);
	/**
	按大小缩放矩阵
	@param src		输入
	@param dst		输出
	@param newSize	新的矩阵大小
	@param mothed	缩放方法
	*/
	extern LZHAPI void resize(InputArray src, OutputArray dst, Size newSize, InterpolationMothed mothed = INTER_LINEAR, mat_t alpha = -0.75);
	extern LZHAPI void QuadrantNorm(InputArray src, OutputArray dst, Val minimum, Val maximum, Val value, int32 is_min_or_max = 0x03);
	extern LZHAPI void createCorrelationPatch(InputOutputArrayOfArrays patch, Val angle_1, Val angle_2, Val radius);
	extern LZHAPI void nonMaximumSuppression(InputArray src, OutputArray dst, int32 n, Val tau, int32 margin);
	extern LZHAPI void findModesMeanShift(InputArray hist, OutputArray modes, OutputArray histSmoothed, Val sigma);
	extern LZHAPI mat_t cornerCorrelationScore(InputArray iou, InputArray imgWeight, Vec2m v1, Vec2m v2);
	extern LZHAPI void edgeOrientations(InputArray img_angle, InputArray img_weight, Vec2m &v1, Vec2m &v2);
	extern LZHAPI void refineScoreCorners(InputArray img, InputOutputArray corners, OutputArray refinescore, int32 scales, int32 r, Val tau = 0.01, bool subPix = false);
	extern LZHAPI void findConcers(InputArray src, OutputArray concers, OutputArray dst, int32 scales = FourRadius, int32 n = 3, Val tau = 0.01, int32 margin = 5);
	extern LZHAPI void cornerSubPix(InputArray img, InputOutputArray corners, int32 scales, int32 r, Val tau = 0.01);
}

template<typename _Tp> inline void lzh::addContour(std::vector<Point2_<_Tp>>& contour, Point2_<_Tp> point, int32 x, int32 y)
{
	contour.push_back(Point2_<_Tp>(point.x + x, point.y + y));
	contour.push_back(Point2_<_Tp>(point.x + x, point.y + -y));
	contour.push_back(Point2_<_Tp>(point.x + -x, point.y + y));
	contour.push_back(Point2_<_Tp>(point.x + -x, point.y + -y));
	contour.push_back(Point2_<_Tp>(point.x + y, point.y + x));
	contour.push_back(Point2_<_Tp>(point.x + y, point.y + -x));
	contour.push_back(Point2_<_Tp>(point.x + -y, point.y + x));
	contour.push_back(Point2_<_Tp>(point.x + -y, point.y + -x));
}
template<typename _Tp> inline void lzh::EllipsePlot(std::vector<Point2_<_Tp>>& contour, Point point, int32 x, int32 y)
{
	contour.push_back(Point2_<_Tp>(point.x + x, point.y + y));
	contour.push_back(Point2_<_Tp>(point.x + x, point.y - y));
	contour.push_back(Point2_<_Tp>(point.x - x, point.y + y));
	contour.push_back(Point2_<_Tp>(point.x - x, point.y - y));
}
#endif // !__IMAGE_PROCESS_H__
