#pragma once
#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include "interface.h"
#include "structure.h"

namespace lzh
{
	typedef OMat(*Act)(IMat);
	typedef OMat(*LF)(IMat, IMat);

	/**
	@brief softmax函数
	Si = exp(y - max(y)) / sum(exp(y - max(y)))
	*/
	extern LZHAPI OMat softmax(IMat x);
	/**
	@brief L1函数
	E = |y - y0|
	*/
	extern LZHAPI OMat L1(IMat y, IMat y0);
	/**
	@brief L2函数
	E = (y - y0)^2
	*/
	extern LZHAPI OMat L2(IMat y, IMat y0);
	/**
	@brief quadratic函数
	E = 1/2 * (y - y0)^2
	*/
	extern LZHAPI OMat Quadratic(IMat y, IMat y0);
	/**
	@brief relative 函数
	E = abs(y - y0)/y0
	*/
	extern LZHAPI OMat Relative(IMat y, IMat y0);
	/**
	@brief crossentropy函数
	E = -(y * log(y0))
	*/
	extern LZHAPI OMat CrossEntropy(IMat y, IMat y0);
	/**
	@brief softmax + crossentropy函数
	E = -(y * log(softmax(y0)))
	*/
	extern LZHAPI OMat SoftmaxCrossEntropy(IMat y, IMat y0);
	/**
	@brief sigmoid函数
	y = 1/(1 + exp(-x))
	*/
	extern LZHAPI OMat sigmoid(IMat x);
	/**
	@brief sigmoid函数
	y = 1/(1 + exp(-x))
	*/
	extern LZHAPI OMat sigmoid(IMat x);
	/**
	@brief tanh函数
	y = (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	*/
	extern LZHAPI OMat tanh(IMat x);
	/**
	@brief relu函数
	y = {x if x > 0; 0 if x < 0}
	*/
	extern LZHAPI OMat relu(IMat x);
	/**
	@brief elu函数
	y = {x if x > 0; a*(exp(x) - 1) if x < 0}
	*/
	extern LZHAPI OMat elu(IMat x);
	/**
	@brief selu函数
	y = scale*Elu(x)
	*/
	extern LZHAPI OMat selu(IMat x);
	/**
	@brief leaky relu函数
	y = {x if x > 0; a*x if x < 0}
	*/
	extern LZHAPI OMat leaky_relu(IMat x);
	/**
	@brief softplus函数
	y = log(1 + e^x)
	*/
	extern LZHAPI OMat softplus(IMat x);

	/**
	@brief softmax导数
	y = y*(1 - y),
	*/
	extern LZHAPI OMat d_softmax(IMat y);
	extern LZHAPI OMat D_L1(IMat y, IMat y0);
	/**
	@brief L2导数
	y = y0/(y0^2 - y^2)^(1/2),
	*/
	extern LZHAPI OMat D_L2(IMat y, IMat y0);
	/**
	@brief quadratic导数
	y = y0 - y
	*/
	extern LZHAPI OMat D_Quadratic(IMat y, IMat y0);
	/**
	@brief quadratic导数
	y = sign(y - y0)/y0
	*/
	extern LZHAPI OMat D_Relative(IMat y, IMat y0);
	/**
	@brief crossentropy导数
	y = y0 - y
	*/
	extern LZHAPI OMat D_CrossEntropy(IMat y, IMat y0);
	/**
	@brief softmax + crossentropy导函数
	y = y0 - y
	*/
	extern LZHAPI OMat D_SoftmaxCrossEntropy(IMat y, IMat y0);
	/**
	@brief sigmoid导数
	y = sigmoid(x) * (1 - sigmoid(x)),
	*/
	extern LZHAPI OMat d_sigmoid(IMat x);
	/**
	@brief tanh导数
	y = 4 * d_sigmoid(x),
	*/
	extern LZHAPI OMat d_tanh(IMat x);
	/**
	@brief relu导数
	y = {1 if x > 0; 0 if x < 0}
	*/
	extern LZHAPI OMat d_relu(IMat x);
	/**
	@brief elu导数
	y = {1 if x > 0; a*exp(x) if x < 0}
	*/
	extern LZHAPI OMat d_elu(IMat x);
	/**
	@brief selu导数
	y = scale*d_Elu(x)
	*/
	extern LZHAPI OMat d_selu(IMat x);
	/**
	@brief leaky relu导数
	y = {1 if x > 0; a if x < 0}
	*/
	extern LZHAPI OMat d_leaky_relu(IMat x);
	/**
	@brief softplus函数
	y = e^x / (e^x + 1)
	*/
	extern LZHAPI OMat d_softplus(IMat x);

	/**
	排序算法类
	提供了
	冒泡排序	=> bubble;
	插入排序	=> insert;
	选择排序	=> select;
	梳排序		=> comb;
	地精排序	=> gnome;
	堆排序		=> heap;
	希尔排序	=> shell;
	快速排序	=> quick;
	归并排序	=> merge;
	*/
	class LZHAPI Sort
	{
	public:
		static bool compare_angle(InputArray a, InputArray b);
		static bool compare_min(InputArray a, InputArray b);
		static bool compare_max(InputArray a, InputArray b);
		template<typename _Tp> static bool comp_min(_Tp a, _Tp b);
		template<typename _Tp> static bool comp_max(_Tp a, _Tp b);
		/**
		冒泡排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void bubble(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		冒泡排序
		@param m		排序数据
		@param order	顺序
		*/
		static void bubble(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		插入排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void insert(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		插入排序
		@param m		排序数据
		@param order	顺序
		*/
		static void insert(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		选择排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void select(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		选择排序
		@param m		排序数据
		@param order	顺序
		*/
		static void select(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		梳排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void comb(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		梳排序
		@param m		排序数据
		@param order	顺序
		*/
		static void comb(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		地精排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void gnome(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		地精排序
		@param m		排序数据
		@param order	顺序
		*/
		static void gnome(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		堆排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void heap(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		堆排序
		@param m		排序数据
		@param order	顺序
		*/
		static void heap(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		希尔排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void shell(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		希尔排序
		@param m		排序数据
		@param order	顺序
		*/
		static void shell(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		快速排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void quick(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		快速排序
		@param m		排序数据
		@param order	顺序
		*/
		static void quick(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		归并排序
		@param m		排序数据
		@param order	顺序
		*/
		template<typename _Tp, typename _Ptr> static void merge(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief 返回排序结果
		归并排序
		@param m		排序数据
		@param order	顺序
		*/
		static void merge(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		static void Register(const void*);
		static void* pre;
	protected:
		template<typename _Tp, typename _Ptr> static void heapdown(_Tp *m, int32 i, int32 n, _Ptr compare = comp_min);
		template<typename _Tp, typename _Ptr> static void heapup(_Tp *m, int32 i, _Ptr compare = comp_min);
		template<typename _Tp, typename _Ptr> static void makeheap(_Tp *m, int32 length, _Ptr compare = comp_min);
		template<typename _Tp, typename _Ptr> static void heaparray(_Tp *m, int32 i, int32 n, _Ptr compare = comp_min);
		template<typename _Tp, typename _Ptr> static void mergearray(_Tp * a, _Tp * b, int32 start, int32 mid, int32 end, _Ptr compare = comp_min);
		template<typename _Tp, typename _Ptr> static void _merge(_Tp * a, _Tp * b, int32 start, int32 end, _Ptr compare = comp_min);
		template<typename _Tp, typename _Ptr> static void _quick(_Tp *m, int32 low, int32 high, _Ptr compare = comp_min);
	};
	template<typename _Tp> TEMPLATE_API void Swap(_Tp &a, _Tp &b);
	/**
	@brief 返回随机矩阵
	符合高斯分布
	大小为size
	@param size		矩阵大小
	@param n1		先前通道
	@param n2		当前通道
	*/
	extern LZHAPI OMat Xavier(Size3 size, int32 n1, int32 n2);
	/**
	@brief 返回随机矩阵
	符合高斯分布
	大小为h_*w_*c_
	@param h_		矩阵行数
	@param w_		矩阵列数
	@param c_	矩阵通道数
	@param n1		先前通道
	@param n2		当前通道
	*/
	extern LZHAPI OMat Xavier(int32 w, int32 h, int32 c, int32 n1, int32 n2);
	/**
	@brief 返回随机矩阵
	符合高斯分布
	大小为size, 元素范围[0, 1]
	@param size		矩阵大小
	*/
	extern LZHAPI OMat Random(Size3 size);
	/**
	@brief 返回随机矩阵
	符合高斯分布
	大小为h_*w_*c_, 元素范围[0, 1]
	@param h_		矩阵行数
	@param w_		矩阵列数
	@param c_	矩阵通道数
	*/
	extern LZHAPI OMat Random(int32 w, int32 h, int32 c = 1);
	extern LZHAPI mat_t generateGaussian(Val x, Val sigma);
	extern LZHAPI mat_t generateGaussian(Val x, Val y, Val sigma);
	extern LZHAPI mat_t generateLoG(Val x, Val sigma);
	extern LZHAPI mat_t generateLoG(Val x, Val y, Val sigma);
	extern LZHAPI mat_t generateDoG(Val x, Val sigma1, Val sigma2);
	extern LZHAPI mat_t generateDoG(Val x, Val y, Val sigma1, Val sigma2);
	extern LZHAPI int32 mod(int32 x, int32 b);
	extern LZHAPI OMat mod(IMat x, int32 b);
	extern LZHAPI bool any(InputArray s);
	extern LZHAPI bool any(InputArray s, Val value, CompareType op = EQUAL);
	// Return NaN for out of range parameters.
	extern LZHAPI mat_t normpdf(Val x, Val mu, Val sigma);
	extern LZHAPI OMat normpdf(InputArray x, Val mu, Val sigma);
	extern LZHAPI int32 rank(InputArray G, mat_t error = 1e-6);
	extern LZHAPI mat_t fastAtan2(mat_t y, mat_t x);
	/**
	@brief 返回复数乘法
	二进制平摊反转置换 O(logn)
	*/
	extern LZHAPI void brc(InputOutputArray src);
	/**
	一维快速傅里叶变换
	FFT O(nlogn)
	*/
	extern LZHAPI OMat fft1D(InputArray src);
	/**
	一维快速逆傅里叶变换
	FFT O(nlogn)
	*/
	extern LZHAPI OMat ifft1D(InputArray src);
	/**
	二维傅里叶变换
	*/
	extern LZHAPI OMat dft1D(InputArray src);
	/**
	二维逆傅里叶变换
	*/
	extern LZHAPI OMat idft1D(InputArray src);
	/**
	@brief 返回a*b
	使用傅里叶变换计算
	*/
	extern LZHAPI const LargeNumber fftMult(const LargeNumber &a, const LargeNumber &b);
	/**
	@brief 返回a*b
	使用傅里叶变换计算
	*/
	extern LZHAPI void fftMult(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回复数乘法
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void complexDot(InputArray a, InputArray b, OutputArray dst);
	/**
	排序
	*/
	extern LZHAPI void mSort(InputArray src, OutputArray dst, ORDER flag = MIN_TO_MAX, RANK r = ROW);
	/**
	排序
	*/
	extern LZHAPI void mSort(IMat src, OutputArray dst, bool(*comp)(IMat a, IMat b), RANK r = ROW);
	/**
	排序
	*/
	extern LZHAPI void mSort(IMat src, OutputArray dst, bool(*comp)(InputArray a, InputArray b), RANK r = ROW);
	/**
	@brief 返回向量叉积
	*/
	extern LZHAPI mat_t Cross(IMat p0, IMat p1);
	/**
	@brief 返回三点是否顺序是顺时针
	*/
	extern LZHAPI bool Clockwise(IMat p0, IMat p1, IMat p2);
	/**
	@brief 返回凸包结果
	凸包
	@param in 轮廓点集
	@param convexhull 凸包点集
	*/
	extern LZHAPI void ConvexHull(InputArray in, OutputArray convexhull);
	/**
	@brief 返回拟合结果
	最小二乘法
	@param x 自变量
	@param y 因变量
	*/
	extern LZHAPI void LeastSquare(InputArray x, InputArray y, OutputArray out, Mat *r = nullptr);
	/**
	@brief 返回a
	线性拟合y=a(0)*x+a(1)
	y和x必须为行向量
	保证y和x的行数相同
	@param x 自变量
	@param y 因变量
	*/
	extern LZHAPI void regress(InputArray x, InputArray y, OutputArray out, Mat *r = nullptr);
	/**
	@brief 返回P
	多项式拟合y=P(1)*x^n + P(2)*x^(n-1) +...+ P(n)*x + P(n+1)
	y和x必须为行向量
	保证y和x的行数相同
	@param x 自变量
	@param y 因变量
	*/
	extern LZHAPI void polyfit(InputArray x, InputArray y, uint32 n, OutputArray out, Mat *r = nullptr);
	/**
	@brief 返回拟合结果
	拟合圆
	p必须为行向量
	@param p 点集
	*/
	extern LZHAPI void circlefit(InputArray p, OutputArray circle, Mat *r = nullptr);
	/**
	@brief 返回拟合结果
	拟合圆
	p必须为行向量
	@param p 点集
	*/
	extern LZHAPI void circlefit(InputArray x, InputArray y, OutputArray circle, Mat *r = nullptr);
	/**
	@brief 返回拟合结果
	非线性最小二乘法
	y和x必须为行向量
	保证y和x的行数相同
	@param x		自变量
	@param y		因变量
	@param a0		初始参数
	@param f		函数指针 f(a, x) = y
	@param stride		更新步长
	@param error	误差(小于误差结束更新)
	*/
	extern LZHAPI void NonLinearLeastSqures(InputArray x, InputArray y, InputArray a0, OutputArray out, F f, Mat *r = nullptr, Val stride = 1e-2, Val error = 1e-6);
	/**
	@brief 返回梯度
	@param y	因变量
	@param x	自变量(缺省差值为1)
	*/
	extern LZHAPI void gradient(InputArray y, OutputArray grad, InputArray x = Mat());
	/**
	@brief 返回数值梯度
	@param f		函数指针 f(x) = y
	@param x		自变量
	@param epsilon	差值
	*/
	extern LZHAPI mat_t NumericGradient(NF f, Val x, Val epsilon = 1e-3);
	/**
	@brief 返回数值梯度
	@param f		函数指针 f(x) = y
	@param x		自变量
	@param epsilon	差值
	*/
	extern LZHAPI void NumericGradient(InputArray x, Fun f, OutputArray grad, Val epsilon = 1e-3);
	/**
	@brief 返回数值梯度
	@param f		函数指针 f(a, x) = y
	@param a		自变量
	@param x		输入
	@param epsilon	差值
	*/
	extern LZHAPI void NumericGradient(InputArray a, InputArray x, F f, OutputArray grad, Val epsilon = 1e-3);
	/**
	@brief 返回积分值
	使用欧拉法求数值积分
	@param f		函数指针 f(x) = y
	@param low		积分下限
	@param high		积分上限
	@param epsilon	采样间隔
	*/
	extern LZHAPI mat_t EulerInt(NF f, Val low, Val high, Val epsilon = 1e-3);
	/**
	@brief 返回积分值
	使用梯形法求数值积分
	@param f		函数指针 f(x) = y
	@param low		积分下限
	@param high		积分上限
	@param epsilon	采样间隔
	*/
	extern LZHAPI mat_t TrapezoidInt(NF f, Val low, Val high, Val epsilon = 1e-3);
	/**
	@brief 返回积分值
	使用四阶龙格-库塔法求数值积分
	@param f		函数指针 f(x) = y
	@param low		积分下限
	@param high		积分上限
	@param epsilon	采样间隔
	*/
	extern LZHAPI mat_t RungeKuttaInt(NF f, Val low, Val high, Val epsilon = 1e-3);
	/**
	@brief 返回直线
	两点确定直线
	@param p1		点1
	@param p2		点2
	*/
	extern LZHAPI Vec2m SolveLinear(const Point2m & p1, const Point2m & p2);
	/**
	@brief 返回交点
	两条直线求交点
	@param a1	直线1斜率
	@param b1	直线1偏置
	@param a2	直线2斜率
	@param b2	直线2偏置
	*/
	extern LZHAPI Point2m LinearIntersection(Val a1, Val b1, Val a2, Val b2);
	/**
	@brief 返回交点
	两条直线求交点
	@param line1	直线1
	@param line2	直线2
	*/
	extern LZHAPI Point2m LinearIntersection(const Vec2m & line1, const Vec2m & line2);
	/**
	@brief 返回y
	多项式拟合y=P(1)*x^n + P(2)*x^(n-1) +...+ P(n)*x + P(n+1)
	x必须为行向量
	@param a 参数
	@param x 自变量
	*/
	extern LZHAPI void polynomial(InputArray a, InputArray x, OutputArray y);
	/**
	@brief 返回随机数
	符合高斯分布的随机数
	@param mu		期望值
	@param sigma	标准差
	*/
	extern LZHAPI mat_t generateGaussianNoise(Val mu = 0, Val sigma = 1);
	/**
	@brief 返回聚类结果
	K均值聚类
	@param P			输入
	@param k			对点集的分类结果
	@param K			聚类个数
	@param iteration	迭代次数
	@param error		误差(小于误差结束迭代)
	*/
	extern LZHAPI void kmeans(InputArray P, InputOutputArray k, OutputArray classifier, int32 K, const uint32 iteration, const Val error = 1e-7);
	/**
	将矩阵化成简化阶梯矩阵
	@param src		输入
	@param dst		阶梯矩阵
	*/
	extern LZHAPI void Trapezoidal(InputArray src, OutputArray dst, bool normalization = true);
	/**
	计算矩阵的行最简矩阵
	@param src		输入
	@param dst		行最简矩阵
	@rank			行秩(指针为空不做操作
	*/
	extern LZHAPI void RowSimplest(InputArray src, OutputArray dst, int32 *rank = nullptr);
	/**
	@brief 返回列秩
	计算矩阵的列最简矩阵
	@param src		输入
	@param dst		列最简矩阵
	@rank			列秩(指针为空不做操作
	*/
	extern LZHAPI void ColSimplest(InputArray src, OutputArray dst, int32 *rank = nullptr);
	/**
	@brief 返回求解状态
	求解非齐次线性方程组
	齐次线性方程组使用addzeros手动添加0
	@param src		输入线性方程组
	@param dst		基础解系
	@param simplest	最简行矩阵
	@param mark		解的状态(自由解为0,特解为1,无解为nullptr)
	*/
	extern LZHAPI EQUATION SolveLinearEquation(InputArray src, OutputArray dst, Mat *simplest = nullptr, Mat *mark = nullptr);

}
#include "algorithm_inl.hpp"
#endif // !__ALGORITHM_H__