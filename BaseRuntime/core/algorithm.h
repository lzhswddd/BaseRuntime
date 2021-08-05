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
	@brief softmax����
	Si = exp(y - max(y)) / sum(exp(y - max(y)))
	*/
	extern LZHAPI OMat softmax(IMat x);
	/**
	@brief L1����
	E = |y - y0|
	*/
	extern LZHAPI OMat L1(IMat y, IMat y0);
	/**
	@brief L2����
	E = (y - y0)^2
	*/
	extern LZHAPI OMat L2(IMat y, IMat y0);
	/**
	@brief quadratic����
	E = 1/2 * (y - y0)^2
	*/
	extern LZHAPI OMat Quadratic(IMat y, IMat y0);
	/**
	@brief relative ����
	E = abs(y - y0)/y0
	*/
	extern LZHAPI OMat Relative(IMat y, IMat y0);
	/**
	@brief crossentropy����
	E = -(y * log(y0))
	*/
	extern LZHAPI OMat CrossEntropy(IMat y, IMat y0);
	/**
	@brief softmax + crossentropy����
	E = -(y * log(softmax(y0)))
	*/
	extern LZHAPI OMat SoftmaxCrossEntropy(IMat y, IMat y0);
	/**
	@brief sigmoid����
	y = 1/(1 + exp(-x))
	*/
	extern LZHAPI OMat sigmoid(IMat x);
	/**
	@brief sigmoid����
	y = 1/(1 + exp(-x))
	*/
	extern LZHAPI OMat sigmoid(IMat x);
	/**
	@brief tanh����
	y = (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	*/
	extern LZHAPI OMat tanh(IMat x);
	/**
	@brief relu����
	y = {x if x > 0; 0 if x < 0}
	*/
	extern LZHAPI OMat relu(IMat x);
	/**
	@brief elu����
	y = {x if x > 0; a*(exp(x) - 1) if x < 0}
	*/
	extern LZHAPI OMat elu(IMat x);
	/**
	@brief selu����
	y = scale*Elu(x)
	*/
	extern LZHAPI OMat selu(IMat x);
	/**
	@brief leaky relu����
	y = {x if x > 0; a*x if x < 0}
	*/
	extern LZHAPI OMat leaky_relu(IMat x);
	/**
	@brief softplus����
	y = log(1 + e^x)
	*/
	extern LZHAPI OMat softplus(IMat x);

	/**
	@brief softmax����
	y = y*(1 - y),
	*/
	extern LZHAPI OMat d_softmax(IMat y);
	extern LZHAPI OMat D_L1(IMat y, IMat y0);
	/**
	@brief L2����
	y = y0/(y0^2 - y^2)^(1/2),
	*/
	extern LZHAPI OMat D_L2(IMat y, IMat y0);
	/**
	@brief quadratic����
	y = y0 - y
	*/
	extern LZHAPI OMat D_Quadratic(IMat y, IMat y0);
	/**
	@brief quadratic����
	y = sign(y - y0)/y0
	*/
	extern LZHAPI OMat D_Relative(IMat y, IMat y0);
	/**
	@brief crossentropy����
	y = y0 - y
	*/
	extern LZHAPI OMat D_CrossEntropy(IMat y, IMat y0);
	/**
	@brief softmax + crossentropy������
	y = y0 - y
	*/
	extern LZHAPI OMat D_SoftmaxCrossEntropy(IMat y, IMat y0);
	/**
	@brief sigmoid����
	y = sigmoid(x) * (1 - sigmoid(x)),
	*/
	extern LZHAPI OMat d_sigmoid(IMat x);
	/**
	@brief tanh����
	y = 4 * d_sigmoid(x),
	*/
	extern LZHAPI OMat d_tanh(IMat x);
	/**
	@brief relu����
	y = {1 if x > 0; 0 if x < 0}
	*/
	extern LZHAPI OMat d_relu(IMat x);
	/**
	@brief elu����
	y = {1 if x > 0; a*exp(x) if x < 0}
	*/
	extern LZHAPI OMat d_elu(IMat x);
	/**
	@brief selu����
	y = scale*d_Elu(x)
	*/
	extern LZHAPI OMat d_selu(IMat x);
	/**
	@brief leaky relu����
	y = {1 if x > 0; a if x < 0}
	*/
	extern LZHAPI OMat d_leaky_relu(IMat x);
	/**
	@brief softplus����
	y = e^x / (e^x + 1)
	*/
	extern LZHAPI OMat d_softplus(IMat x);

	/**
	�����㷨��
	�ṩ��
	ð������	=> bubble;
	��������	=> insert;
	ѡ������	=> select;
	������		=> comb;
	�ؾ�����	=> gnome;
	������		=> heap;
	ϣ������	=> shell;
	��������	=> quick;
	�鲢����	=> merge;
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
		ð������
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void bubble(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		ð������
		@param m		��������
		@param order	˳��
		*/
		static void bubble(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		��������
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void insert(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		��������
		@param m		��������
		@param order	˳��
		*/
		static void insert(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		ѡ������
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void select(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		ѡ������
		@param m		��������
		@param order	˳��
		*/
		static void select(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		������
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void comb(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		������
		@param m		��������
		@param order	˳��
		*/
		static void comb(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		�ؾ�����
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void gnome(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		�ؾ�����
		@param m		��������
		@param order	˳��
		*/
		static void gnome(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		������
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void heap(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		������
		@param m		��������
		@param order	˳��
		*/
		static void heap(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		ϣ������
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void shell(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		ϣ������
		@param m		��������
		@param order	˳��
		*/
		static void shell(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		��������
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void quick(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		��������
		@param m		��������
		@param order	˳��
		*/
		static void quick(InputArray src, OutputArray dst, ORDER order = MIN_TO_MAX);
		/**
		�鲢����
		@param m		��������
		@param order	˳��
		*/
		template<typename _Tp, typename _Ptr> static void merge(_Tp *begin, _Tp *end, _Ptr compare = comp_min);
		/**
		@brief ����������
		�鲢����
		@param m		��������
		@param order	˳��
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
	@brief �����������
	���ϸ�˹�ֲ�
	��СΪsize
	@param size		�����С
	@param n1		��ǰͨ��
	@param n2		��ǰͨ��
	*/
	extern LZHAPI OMat Xavier(Size3 size, int32 n1, int32 n2);
	/**
	@brief �����������
	���ϸ�˹�ֲ�
	��СΪh_*w_*c_
	@param h_		��������
	@param w_		��������
	@param c_	����ͨ����
	@param n1		��ǰͨ��
	@param n2		��ǰͨ��
	*/
	extern LZHAPI OMat Xavier(int32 w, int32 h, int32 c, int32 n1, int32 n2);
	/**
	@brief �����������
	���ϸ�˹�ֲ�
	��СΪsize, Ԫ�ط�Χ[0, 1]
	@param size		�����С
	*/
	extern LZHAPI OMat Random(Size3 size);
	/**
	@brief �����������
	���ϸ�˹�ֲ�
	��СΪh_*w_*c_, Ԫ�ط�Χ[0, 1]
	@param h_		��������
	@param w_		��������
	@param c_	����ͨ����
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
	@brief ���ظ����˷�
	������ƽ̯��ת�û� O(logn)
	*/
	extern LZHAPI void brc(InputOutputArray src);
	/**
	һά���ٸ���Ҷ�任
	FFT O(nlogn)
	*/
	extern LZHAPI OMat fft1D(InputArray src);
	/**
	һά�����渵��Ҷ�任
	FFT O(nlogn)
	*/
	extern LZHAPI OMat ifft1D(InputArray src);
	/**
	��ά����Ҷ�任
	*/
	extern LZHAPI OMat dft1D(InputArray src);
	/**
	��ά�渵��Ҷ�任
	*/
	extern LZHAPI OMat idft1D(InputArray src);
	/**
	@brief ����a*b
	ʹ�ø���Ҷ�任����
	*/
	extern LZHAPI const LargeNumber fftMult(const LargeNumber &a, const LargeNumber &b);
	/**
	@brief ����a*b
	ʹ�ø���Ҷ�任����
	*/
	extern LZHAPI void fftMult(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���ظ����˷�
	@param a ����
	@param b ����
	*/
	extern LZHAPI void complexDot(InputArray a, InputArray b, OutputArray dst);
	/**
	����
	*/
	extern LZHAPI void mSort(InputArray src, OutputArray dst, ORDER flag = MIN_TO_MAX, RANK r = ROW);
	/**
	����
	*/
	extern LZHAPI void mSort(IMat src, OutputArray dst, bool(*comp)(IMat a, IMat b), RANK r = ROW);
	/**
	����
	*/
	extern LZHAPI void mSort(IMat src, OutputArray dst, bool(*comp)(InputArray a, InputArray b), RANK r = ROW);
	/**
	@brief �����������
	*/
	extern LZHAPI mat_t Cross(IMat p0, IMat p1);
	/**
	@brief ���������Ƿ�˳����˳ʱ��
	*/
	extern LZHAPI bool Clockwise(IMat p0, IMat p1, IMat p2);
	/**
	@brief ����͹�����
	͹��
	@param in �����㼯
	@param convexhull ͹���㼯
	*/
	extern LZHAPI void ConvexHull(InputArray in, OutputArray convexhull);
	/**
	@brief ������Ͻ��
	��С���˷�
	@param x �Ա���
	@param y �����
	*/
	extern LZHAPI void LeastSquare(InputArray x, InputArray y, OutputArray out, Mat *r = nullptr);
	/**
	@brief ����a
	�������y=a(0)*x+a(1)
	y��x����Ϊ������
	��֤y��x��������ͬ
	@param x �Ա���
	@param y �����
	*/
	extern LZHAPI void regress(InputArray x, InputArray y, OutputArray out, Mat *r = nullptr);
	/**
	@brief ����P
	����ʽ���y=P(1)*x^n + P(2)*x^(n-1) +...+ P(n)*x + P(n+1)
	y��x����Ϊ������
	��֤y��x��������ͬ
	@param x �Ա���
	@param y �����
	*/
	extern LZHAPI void polyfit(InputArray x, InputArray y, uint32 n, OutputArray out, Mat *r = nullptr);
	/**
	@brief ������Ͻ��
	���Բ
	p����Ϊ������
	@param p �㼯
	*/
	extern LZHAPI void circlefit(InputArray p, OutputArray circle, Mat *r = nullptr);
	/**
	@brief ������Ͻ��
	���Բ
	p����Ϊ������
	@param p �㼯
	*/
	extern LZHAPI void circlefit(InputArray x, InputArray y, OutputArray circle, Mat *r = nullptr);
	/**
	@brief ������Ͻ��
	��������С���˷�
	y��x����Ϊ������
	��֤y��x��������ͬ
	@param x		�Ա���
	@param y		�����
	@param a0		��ʼ����
	@param f		����ָ�� f(a, x) = y
	@param stride		���²���
	@param error	���(С������������)
	*/
	extern LZHAPI void NonLinearLeastSqures(InputArray x, InputArray y, InputArray a0, OutputArray out, F f, Mat *r = nullptr, Val stride = 1e-2, Val error = 1e-6);
	/**
	@brief �����ݶ�
	@param y	�����
	@param x	�Ա���(ȱʡ��ֵΪ1)
	*/
	extern LZHAPI void gradient(InputArray y, OutputArray grad, InputArray x = Mat());
	/**
	@brief ������ֵ�ݶ�
	@param f		����ָ�� f(x) = y
	@param x		�Ա���
	@param epsilon	��ֵ
	*/
	extern LZHAPI mat_t NumericGradient(NF f, Val x, Val epsilon = 1e-3);
	/**
	@brief ������ֵ�ݶ�
	@param f		����ָ�� f(x) = y
	@param x		�Ա���
	@param epsilon	��ֵ
	*/
	extern LZHAPI void NumericGradient(InputArray x, Fun f, OutputArray grad, Val epsilon = 1e-3);
	/**
	@brief ������ֵ�ݶ�
	@param f		����ָ�� f(a, x) = y
	@param a		�Ա���
	@param x		����
	@param epsilon	��ֵ
	*/
	extern LZHAPI void NumericGradient(InputArray a, InputArray x, F f, OutputArray grad, Val epsilon = 1e-3);
	/**
	@brief ���ػ���ֵ
	ʹ��ŷ��������ֵ����
	@param f		����ָ�� f(x) = y
	@param low		��������
	@param high		��������
	@param epsilon	�������
	*/
	extern LZHAPI mat_t EulerInt(NF f, Val low, Val high, Val epsilon = 1e-3);
	/**
	@brief ���ػ���ֵ
	ʹ�����η�����ֵ����
	@param f		����ָ�� f(x) = y
	@param low		��������
	@param high		��������
	@param epsilon	�������
	*/
	extern LZHAPI mat_t TrapezoidInt(NF f, Val low, Val high, Val epsilon = 1e-3);
	/**
	@brief ���ػ���ֵ
	ʹ���Ľ�����-����������ֵ����
	@param f		����ָ�� f(x) = y
	@param low		��������
	@param high		��������
	@param epsilon	�������
	*/
	extern LZHAPI mat_t RungeKuttaInt(NF f, Val low, Val high, Val epsilon = 1e-3);
	/**
	@brief ����ֱ��
	����ȷ��ֱ��
	@param p1		��1
	@param p2		��2
	*/
	extern LZHAPI Vec2m SolveLinear(const Point2m & p1, const Point2m & p2);
	/**
	@brief ���ؽ���
	����ֱ���󽻵�
	@param a1	ֱ��1б��
	@param b1	ֱ��1ƫ��
	@param a2	ֱ��2б��
	@param b2	ֱ��2ƫ��
	*/
	extern LZHAPI Point2m LinearIntersection(Val a1, Val b1, Val a2, Val b2);
	/**
	@brief ���ؽ���
	����ֱ���󽻵�
	@param line1	ֱ��1
	@param line2	ֱ��2
	*/
	extern LZHAPI Point2m LinearIntersection(const Vec2m & line1, const Vec2m & line2);
	/**
	@brief ����y
	����ʽ���y=P(1)*x^n + P(2)*x^(n-1) +...+ P(n)*x + P(n+1)
	x����Ϊ������
	@param a ����
	@param x �Ա���
	*/
	extern LZHAPI void polynomial(InputArray a, InputArray x, OutputArray y);
	/**
	@brief ���������
	���ϸ�˹�ֲ��������
	@param mu		����ֵ
	@param sigma	��׼��
	*/
	extern LZHAPI mat_t generateGaussianNoise(Val mu = 0, Val sigma = 1);
	/**
	@brief ���ؾ�����
	K��ֵ����
	@param P			����
	@param k			�Ե㼯�ķ�����
	@param K			�������
	@param iteration	��������
	@param error		���(С������������)
	*/
	extern LZHAPI void kmeans(InputArray P, InputOutputArray k, OutputArray classifier, int32 K, const uint32 iteration, const Val error = 1e-7);
	/**
	�����󻯳ɼ򻯽��ݾ���
	@param src		����
	@param dst		���ݾ���
	*/
	extern LZHAPI void Trapezoidal(InputArray src, OutputArray dst, bool normalization = true);
	/**
	����������������
	@param src		����
	@param dst		��������
	@rank			����(ָ��Ϊ�ղ�������
	*/
	extern LZHAPI void RowSimplest(InputArray src, OutputArray dst, int32 *rank = nullptr);
	/**
	@brief ��������
	����������������
	@param src		����
	@param dst		��������
	@rank			����(ָ��Ϊ�ղ�������
	*/
	extern LZHAPI void ColSimplest(InputArray src, OutputArray dst, int32 *rank = nullptr);
	/**
	@brief �������״̬
	����������Է�����
	������Է�����ʹ��addzeros�ֶ����0
	@param src		�������Է�����
	@param dst		������ϵ
	@param simplest	����о���
	@param mark		���״̬(���ɽ�Ϊ0,�ؽ�Ϊ1,�޽�Ϊnullptr)
	*/
	extern LZHAPI EQUATION SolveLinearEquation(InputArray src, OutputArray dst, Mat *simplest = nullptr, Mat *mark = nullptr);

}
#include "algorithm_inl.hpp"
#endif // !__ALGORITHM_H__