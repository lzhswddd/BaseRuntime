#pragma once
#ifndef _MAT_OPERATOR_H__
#define _MAT_OPERATOR_H__

#include "mat.h"
#include "interface.h"

namespace lzh
{
	class LZHAPI FunOp
	{
	public:
		static mat_t angle2radian(mat_t);
		static mat_t radian2angle(mat_t);
		static mat_t sign(mat_t);
		static mat_t EuclideanDistance_2D(mat_t, mat_t);
		static mat_t mul(mat_t, mat_t);
		static mat_t inv_mul(mat_t, mat_t);
	};
	extern LZHAPI bool Compare(Val v1, Val v2, CompareType compareType, mat_t epsilon = 1e-7);
	extern LZHAPI Array isCompare(InputArray a, InputArray b);
	/**
	@brief ���ش�low��top�ȷֳɵ�1*len�ľ���
	@param low �½�
	@param top �Ͻ�
	@param gap ���
	*/
	extern LZHAPI Array mRange(Val low, Val top, Val gap = 1);
	/**
	@brief ���ش�low��top�ȷֳɵ�1*len�ľ���
	@param low �½�
	@param top �Ͻ�
	@param len �ȷָ���
	*/
	extern LZHAPI Array linspace(Val low, Val top, int32 len);
	/**
	@brief ���ؾ�������ֵ
	@param src		����
	@param isAbs	�Ƿ�ȡ����ֵ
	*/
	extern LZHAPI mat_t Max(InputArray src, bool isAbs = false);
	/**
	@brief ���ؾ������Сֵ
	@param src		����
	@param isAbs	�Ƿ�ȡ����ֵ
	*/
	extern LZHAPI mat_t Min(InputArray src, bool isAbs = false);
	/**
	@brief ���ؾ��������ʽ
	@param src	����
	*/
	extern LZHAPI mat_t det(InputArray src);
	/**
	@brief ���ؾ���ļ�
	@param src	����
	*/
	extern LZHAPI mat_t trace(InputArray src);
	/**
	@brief ������������ֵ
	@param src	����
	*/
	extern LZHAPI mat_t median(InputArray src);
	/**
	@brief ���ؾ����Ӧ����������ʽ
	@param src	����
	@param x	������
	@param y	������
	*/
	extern LZHAPI mat_t cof(InputArray src, int32 x, int32 y);
	/**
	@brief ���������
	@param min		��Сֵ
	@param max		���ֵ
	@param isdouble �Ƿ����������
	*/
	extern LZHAPI mat_t getRandData(int32 min, int32 max, bool isdouble = false);
	/**
	@brief ���������
	@param min		��Сֵ
	@param max		���ֵ
	@param isdouble �Ƿ����������
	*/
	extern LZHAPI mat_t getRandData(mat_t min, mat_t max);
	/**
	@brief ���ؾ�����
	@param src ����
	@param num ������
	*/
	extern LZHAPI mat_t Norm(InputArray src, int32 num = 1);
	/**
	@brief ���ؾ���ľ���
	@param a	����
	@param b	����
	@param num	������
	*/
	extern LZHAPI mat_t Dist(InputArray a, InputArray b, int32 num = 2);
	/**
	@brief ��������ľ���Ԫ��
	@param src ����
	*/
	extern LZHAPI mat_t randSample(InputArray src);
	/**
	@brief ���ؾ���ĵ�c_��ͨ��
	@param src		����
	@param c_	ͨ������
	*/
	extern LZHAPI void split(InputArray src, int32 c, OutputArray dst);
	/**
	@brief ���ذ������ͨ��������
	@param src �������
	@param dst �������ͨ�����ľ�������
	*/
	extern LZHAPI void split(InputArray src, OutputArrayOfArrays dst);
	/**
	@brief ���ذ������ͨ��������
	@param src �������
	@param dst �������ͨ�����ľ�������
	*/
	extern LZHAPI void split(const Mat & src, Mat *dst);
	/**
	@brief ���ذ������ͨ��������
	@param src �������
	@param dst �������ͨ�����ľ�������
	*/
	extern LZHAPI void split(const Array & src, Array *dst);
	/**
	@brief ���ذ�ͨ���ϲ��ľ���
	@param src		��������
	@param channels ͨ����
	*/
	extern LZHAPI void merge(const Array *src, int32 channels, OutputArray dst);
	/**
	@brief ���ذ�ͨ���ϲ��ľ���
	@param src		��������
	@param channels ͨ����
	*/
	extern LZHAPI void merge(InputArrayOfArrays src, OutputArray dst);
	/**
	@brief ���ذ�ͨ���ϲ��ľ���
	@param src		��������
	@param channels ͨ����
	*/
	extern LZHAPI void merge(InputArrayOfArrays src, int32 channels, OutputArray dst);
	/**
	@brief ���ذ����������и�ľ���
	@param src			����
	@param Row_Start	��ȡ�г�ʼ����ֵ
	@param Row_End		��ȡ�н�������ֵ
	@param Col_Start	��ȡ�г�ʼ����ֵ
	@param Col_End		��ȡ�н�������ֵ
	*/
	extern LZHAPI void Block(InputArray src, OutputArray dst, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End, int32 Chennel_Start = 0, int32 Chennel_End = 0);
	/**
	@brief �����������Ԫ��size.x*size.y*size.z����
	@param low		�½�
	@param top		�Ͻ�
	@param size		�����С
	@param isdouble �Ƿ����ɸ�����
	*/
	extern LZHAPI Array Rand(mat_t low, mat_t top, Size3 size, bool isdouble = false);
	/**
	@brief ����������󣬾�����Ϊһά����
	@param src	����
	*/
	extern LZHAPI void reverse(InputArray src, OutputArray dst);
	/**
	@brief ��������h*w*c�ľ��������ȡ����srcԪ�����
	@param src	����
	@param h	��������
	@param w	��������
	@param c	����ͨ����
	*/
	extern LZHAPI void randSample(InputArray src, OutputArray dst, int32 w, int32 h, int32 c = 1);
	/**
	@brief ���������ȡnum�ξ���src���л�����ɵľ���
	@param src	����
	@param rc	��ȡ��ʽ
	@param num	��ȡ����
	*/
	extern LZHAPI void randSample(InputArray src, OutputArray dst, RANK rc, int32 num = 1);
	/**
	@brief ����op�������
	@param src ����
	*/
	extern LZHAPI void Operator(InputArray src, OutputArray dst, mat_t(*op)(mat_t));
	/**
	@brief ����op�������
	@param src ����
	*/
	extern LZHAPI void Operator(InputArray m1, InputArray m2, OutputArray dst, mat_t(*op)(mat_t, mat_t));
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_not(InputOutputArray a);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_not(InputArray a, OutputArray dst);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_and(InputOutputArray a, InputArray b);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_and(InputOutputArray a, Val b);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_and(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_and(InputArray a, Val b, OutputArray dst);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_or(InputOutputArray a, Val b);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_or(InputOutputArray a, InputArray b);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_or(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���ذ�λ��
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Bitwise_or(InputArray a, Val b, OutputArray dst);
	/**
	@brief ����cos���
	@param src ����
	*/
	extern LZHAPI void Cos(InputArray src, OutputArray dst);
	/**
	@brief ����cosh���
	@param src ����
	*/
	extern LZHAPI void Cosh(InputArray src, OutputArray dst);
	/**
	@brief ����sin���
	@param src ����
	*/
	extern LZHAPI void Sin(InputArray src, OutputArray dst);
	/**
	@brief ����sinh���
	@param src ����
	*/
	extern LZHAPI void Sinh(InputArray src, OutputArray dst);
	/**
	@brief ����tan���
	@param src ����
	*/
	extern LZHAPI void Tan(InputArray src, OutputArray dst);
	/**
	@brief ����tanh���
	@param src ����
	*/
	extern LZHAPI void Tanh(InputArray src, OutputArray dst);
	/**
	@brief ����acos���
	@param src ����
	*/
	extern LZHAPI void arcCos(InputArray src, OutputArray dst);
	/**
	@brief ����acosh���
	@param src ����
	*/
	extern LZHAPI void arcCosh(InputArray src, OutputArray dst);
	/**
	@brief ����asin���
	@param src ����
	*/
	extern LZHAPI void arcSin(InputArray src, OutputArray dst);
	/**
	@brief ����asinh���
	@param src ����
	*/
	extern LZHAPI void arcSinh(InputArray src, OutputArray dst);
	/**
	@brief ����atan���
	@param src ����
	*/
	extern LZHAPI void arcTan(InputArray src, OutputArray dst);
	/**
	@brief ����atanh���
	@param src ����
	*/
	extern LZHAPI void arcTanh(InputArray src, OutputArray dst);
	/**
	@brief ����sign���
	@param src ����
	*/
	extern LZHAPI void sign(InputArray src, OutputArray dst);
	/**
	@brief ����atan2���
	@param src ����
	*/
	extern LZHAPI void arcTan2(InputArray y, InputArray x, OutputArray dst);
	/**
	@brief ����ת�Ƕ�
	@param src ����
	*/
	extern LZHAPI void Radian2Angle(InputArray radian, OutputArray angle);
	/**
	@brief �Ƕ�ת����
	@param src ����
	*/
	extern LZHAPI void Angle2Radian(InputArray angle, OutputArray radian);
	/**
	@brief ���ؾ���İ������
	@param src ����
	*/
	extern LZHAPI void adj(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ���������
	@param src ����
	*/
	extern LZHAPI void inv(InputArray src, OutputArray dst);
	/**
	@brief ����QR�ֽ�
	@param src ����
	*/
	extern LZHAPI void QR(InputArray src, OutputArray Q, OutputArray R);
	/**
	@brief ���ؾ�������ֵ
	@param src ����
	@param eigenvalues ����ֵ
	@param eigenvectors ��������
	*/
	extern LZHAPI void eig(InputArray src, OutputArray eigenvalues, OutputArray eigenvectors = noArray(), int iterator_num = 50);
	/**
	@brief ���ضԽ��߾���
	@param src	����
	@param k	��k���Խ���
	*/
	extern LZHAPI void diag(InputArray src, OutputArray dst, int32 k = 0);
	/**
	@brief ���ضԽ�������
	@param src	����
	@param dst	�Խ�������
	*/
	extern LZHAPI void diagonal(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ����α�����
	@param src	����
	@param dire α�����ļ��㷽ʽ
	*/
	extern LZHAPI void pinv(InputArray src, OutputArray dst, Dire dire = LEFT);
	/**
	@brief ���ؾ����ת�þ���
	@param src ����
	*/
	extern LZHAPI void tran(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ���ľ���ֵ����
	@param src ����
	*/
	extern LZHAPI void Abs(InputArray src, OutputArray dst);
	/**
	@brief ����angle��2*2����ת����
	@param angle �Ƕ�
	*/
	extern LZHAPI OMat getRotationMat2D(Point2f center, Val angle);
	/**
	@brief ����angle��3*3����ת����
	@param angle �Ƕ�
	*/
	extern LZHAPI OMat getRotationMat3D(Point2f center, Val angle);
	/**
	@brief ���ؾ���num����
	@param src ����
	@param num ����
	*/
	extern LZHAPI void POW(InputArray src, OutputArray dst, int32 num);
	/**
	@brief ���ؾ���ȡ��
	@param src ����
	*/
	extern LZHAPI void Opp(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ����л���֮��
	@param src	����
	@param rc	��͵ķ���
	*/
	extern LZHAPI void Sum(InputArray src, OutputArray dst, RANK rc);
	/**
	@brief ���ؾ���Ԫ��ȡָ��
	@param src ����
	*/
	extern LZHAPI void Exp(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ���Ԫ��ȡ����
	@param src ����
	*/
	extern LZHAPI void Log(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ���Ԫ��ȡ����
	@param src ����
	*/
	extern LZHAPI void Log10(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ���Ԫ��ȡ����
	@param src ����
	*/
	extern LZHAPI void Sqrt(InputArray src, OutputArray dst);
	/**
	@brief ���ؾ���Ԫ��ȡnum����
	@param src ����
	@param num ����
	*/
	extern LZHAPI void Pow(InputArray src, OutputArray dst, Val num);
	/**
	@brief ���ؾ���val/src��Ԫ�س�
	@param src ����
	@param val ����
	*/
	extern LZHAPI void Div(InputArray src, OutputArray dst, Val val, Dire dire = RIGHT);
	/**
	@brief ���ؾ������
	@param a	��������
	@param b	������
	@param dire ������ʽ
	*/
	extern LZHAPI void Div(InputArray a, InputArray b, OutputArray dst, Dire dire = RIGHT);
	/**
	@brief ���ع������
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Mult(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���ع������
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Hadamard(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���ؾ���˷�
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Dot(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���ز�ֵ
	@param a ����
	@param b ����
	*/
	extern LZHAPI void Sub(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���غ�ֵ
	@param a ����
	@param b ����
	*/
	extern LZHAPI void add(InputArray a, InputArray b, OutputArray dst);
	extern LZHAPI void add(InputArray src1, InputArray src2, OutputArray dst, InputArray mask = noArray(), int32 dtype = -1);
	extern LZHAPI void subtract(InputArray src1, InputArray src2, OutputArray dst, InputArray mask = noArray(), int32 dtype = -1);
	extern LZHAPI void multiply(InputArray src1, InputArray src2, OutputArray dst, InputArray mask = noArray(), mat_t scale = 1, int32 dtype = -1);
	extern LZHAPI void divide(InputArray src1, InputArray src2, OutputArray dst, InputArray mask = noArray(), mat_t scale = 1, int32 dtype = -1);
	extern LZHAPI void divide(mat_t scale, InputArray src2, OutputArray dst, InputArray mask = noArray(), int32 dtype = -1);
	extern LZHAPI void scaleAdd(InputArray src1, mat_t alpha, InputArray src2, OutputArray dst);
	extern LZHAPI void addWeighted(InputArray src1, mat_t alpha, InputArray src2, mat_t beta, mat_t gamma, OutputArray dst, int32 dtype = -1);
	extern LZHAPI void convertScaleAbs(InputArray src, OutputArray dst, mat_t alpha = 1, mat_t beta = 0);
	extern LZHAPI void LUT(InputArray src, InputArray lut, OutputArray dst);
	extern LZHAPI int32 countNonZero(InputArray src);
	extern LZHAPI void findNonZero(InputArray src, OutputArray idx);
	extern LZHAPI void absdiff(InputArray src1, InputArray src2, OutputArray dst);
	extern LZHAPI void meanStdDev(InputArray src, mat_t& mean, mat_t& stddev);
	extern LZHAPI void meanStdDev(InputArray src, OutputArray mean, OutputArray stddev, InputArray mask = noArray());
	extern LZHAPI void phase(InputArray x, InputArray y, OutputArray angle, bool angleInDegrees = false);
	extern LZHAPI void magnitude(InputArray x, InputArray y, OutputArray magnitude);
	extern LZHAPI void mulSpectrums(InputArray a, InputArray b, OutputArray c, bool conjB = false);
	/**
	@brief ���ؾ���Ԫ��ȡa��b֮������ֵ
	@param a �Ƚ�ֵ
	@param b �ȽϾ���
	*/
	extern LZHAPI void Max(Val a, InputArray b, OutputArray dst);
	/**
	@brief ���ؾ���Ԫ��ȡa��b֮������ֵ
	@param a �ȽϾ���
	@param b �ȽϾ���
	*/
	extern LZHAPI void Max(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief ���ؾ���Ԫ��ȡa��b֮�����Сֵ
	@param a �Ƚ�ֵ
	@param b �ȽϾ���
	*/
	extern LZHAPI void Min(Val a, InputArray b, OutputArray dst);
	/**
	@brief ���ؾ���Ԫ��ȡa��b֮�����Сֵ
	@param a �ȽϾ���
	@param b �ȽϾ���
	*/
	extern LZHAPI void Min(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief �����޸�ά�ȵľ���
	������ı���󳤶�
	@param src	����
	@param size �����С
	*/
	extern LZHAPI void Reshape(InputArray src, Size3 size, OutputArray dst);
	/**
	@brief ���ؾ���
	��ͨ�����
	@param src	����
	*/
	extern LZHAPI void SumChannel(InputArray src, OutputArray dst);


	extern LZHAPI const Array operator + (const Array & m, Val val);
	extern LZHAPI const Array operator + (const Array & m, const Array & a);
	extern LZHAPI const Array operator + (Val val, const Array & m);

	extern LZHAPI const Array operator - (const Array & m);
	extern LZHAPI const Array operator - (const Array & m, Val val);
	extern LZHAPI const Array operator - (const Array & m, const Array & a);
	extern LZHAPI const Array operator - (Val val, const Array & m);

	extern LZHAPI const Array operator * (const Array & m, Val val);
	extern LZHAPI const Array operator * (const Array & m, const Array & a);
	extern LZHAPI const Array operator * (Val val, const Array & m);

	extern LZHAPI const Array operator / (const Array & m, Val val);
	extern LZHAPI const Array operator / (const Array & m, const Array & a);
	extern LZHAPI const Array operator / (Val val, const Array & m);

	extern LZHAPI const Array operator ^ (const Array & m, Val power);

	extern LZHAPI const Array operator ~ (const Array & m);
	extern LZHAPI const Array operator ! (const Array & m);

	extern LZHAPI const Array operator & (const Array & m, Val val);
	extern LZHAPI const Array operator & (const Array & m, const Array & a);
	extern LZHAPI const Array operator & (Val val, const Array & m);

	extern LZHAPI const Array operator | (const Array & m, Val val);
	extern LZHAPI const Array operator | (const Array & m, const Array & a);
	extern LZHAPI const Array operator | (Val val, const Array & m);

	extern LZHAPI void operator += (Array m, Val val);
	extern LZHAPI void operator += (Array m, const Array & a);

	extern LZHAPI void operator -= (Array m, Val val);
	extern LZHAPI void operator -= (Array m, const Array & a);

	extern LZHAPI void operator *= (Array m, Val val);
	extern LZHAPI void operator *= (Array m, const Array & a);

	extern LZHAPI void operator /= (Array m, Val val);
	extern LZHAPI void operator /= (Array m, const Array & a);

	extern LZHAPI void operator &= (Array m, Val val);
	extern LZHAPI void operator &= (Array m, const Array & a);

	extern LZHAPI void operator |= (Array m, Val val);
	extern LZHAPI void operator |= (Array m, const Array & a);

	extern LZHAPI OMat operator + (IMat m, Val val);
	extern LZHAPI OMat operator + (IMat m, IMat a);
	extern LZHAPI OMat operator + (Val val, IMat m);

	extern LZHAPI OMat operator - (IMat m);
	extern LZHAPI OMat operator - (IMat m, Val val);
	extern LZHAPI OMat operator - (IMat m, IMat a);
	extern LZHAPI OMat operator - (Val val, IMat m);

	extern LZHAPI OMat operator * (IMat m, Val val);
	extern LZHAPI OMat operator * (IMat m, IMat a);
	extern LZHAPI OMat operator * (Val val, IMat m);

	extern LZHAPI OMat operator / (IMat m, Val val);
	extern LZHAPI OMat operator / (IMat m, IMat a);
	extern LZHAPI OMat operator / (Val val, IMat m);

	extern LZHAPI OMat operator ^ (IMat m, Val power);

	extern LZHAPI void operator += (Mat m, Val val);
	extern LZHAPI void operator += (Mat m, IMat a);

	extern LZHAPI void operator -= (Mat m, Val val);
	extern LZHAPI void operator -= (Mat m, IMat a);

	extern LZHAPI void operator *= (Mat m, Val val);
	extern LZHAPI void operator *= (Mat m, IMat a);

	extern LZHAPI void operator /= (Mat m, Val val);
	extern LZHAPI void operator /= (Mat m, IMat a);
}

#endif // !_MAT_OPERATOR_H__