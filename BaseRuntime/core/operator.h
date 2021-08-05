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
	@brief 返回从low到top等分成的1*len的矩阵
	@param low 下界
	@param top 上界
	@param gap 间距
	*/
	extern LZHAPI Array mRange(Val low, Val top, Val gap = 1);
	/**
	@brief 返回从low到top等分成的1*len的矩阵
	@param low 下界
	@param top 上界
	@param len 等分个数
	*/
	extern LZHAPI Array linspace(Val low, Val top, int32 len);
	/**
	@brief 返回矩阵的最大值
	@param src		矩阵
	@param isAbs	是否取绝对值
	*/
	extern LZHAPI mat_t Max(InputArray src, bool isAbs = false);
	/**
	@brief 返回矩阵的最小值
	@param src		矩阵
	@param isAbs	是否取绝对值
	*/
	extern LZHAPI mat_t Min(InputArray src, bool isAbs = false);
	/**
	@brief 返回矩阵的行列式
	@param src	矩阵
	*/
	extern LZHAPI mat_t det(InputArray src);
	/**
	@brief 返回矩阵的迹
	@param src	矩阵
	*/
	extern LZHAPI mat_t trace(InputArray src);
	/**
	@brief 返回向量的中值
	@param src	向量
	*/
	extern LZHAPI mat_t median(InputArray src);
	/**
	@brief 返回矩阵对应索引的余子式
	@param src	矩阵
	@param x	列索引
	@param y	行索引
	*/
	extern LZHAPI mat_t cof(InputArray src, int32 x, int32 y);
	/**
	@brief 返回随机数
	@param min		最小值
	@param max		最大值
	@param isdouble 是否随机非整数
	*/
	extern LZHAPI mat_t getRandData(int32 min, int32 max, bool isdouble = false);
	/**
	@brief 返回随机数
	@param min		最小值
	@param max		最大值
	@param isdouble 是否随机非整数
	*/
	extern LZHAPI mat_t getRandData(mat_t min, mat_t max);
	/**
	@brief 返回矩阵范数
	@param src 矩阵
	@param num 几范数
	*/
	extern LZHAPI mat_t Norm(InputArray src, int32 num = 1);
	/**
	@brief 返回矩阵的距离
	@param a	矩阵
	@param b	矩阵
	@param num	几范数
	*/
	extern LZHAPI mat_t Dist(InputArray a, InputArray b, int32 num = 2);
	/**
	@brief 返回随机的矩阵元素
	@param src 矩阵
	*/
	extern LZHAPI mat_t randSample(InputArray src);
	/**
	@brief 返回矩阵的第c_的通道
	@param src		矩阵
	@param c_	通道索引
	*/
	extern LZHAPI void split(InputArray src, int32 c, OutputArray dst);
	/**
	@brief 返回按矩阵的通道数复制
	@param src 输入矩阵
	@param dst 输入矩阵通道数的矩阵数组
	*/
	extern LZHAPI void split(InputArray src, OutputArrayOfArrays dst);
	/**
	@brief 返回按矩阵的通道数复制
	@param src 输入矩阵
	@param dst 输入矩阵通道数的矩阵数组
	*/
	extern LZHAPI void split(const Mat & src, Mat *dst);
	/**
	@brief 返回按矩阵的通道数复制
	@param src 输入矩阵
	@param dst 输入矩阵通道数的矩阵数组
	*/
	extern LZHAPI void split(const Array & src, Array *dst);
	/**
	@brief 返回按通道合并的矩阵
	@param src		矩阵数组
	@param channels 通道数
	*/
	extern LZHAPI void merge(const Array *src, int32 channels, OutputArray dst);
	/**
	@brief 返回按通道合并的矩阵
	@param src		矩阵数组
	@param channels 通道数
	*/
	extern LZHAPI void merge(InputArrayOfArrays src, OutputArray dst);
	/**
	@brief 返回按通道合并的矩阵
	@param src		矩阵数组
	@param channels 通道数
	*/
	extern LZHAPI void merge(InputArrayOfArrays src, int32 channels, OutputArray dst);
	/**
	@brief 返回按索引区域切割的矩阵
	@param src			矩阵
	@param Row_Start	截取行初始索引值
	@param Row_End		截取行结束索引值
	@param Col_Start	截取列初始索引值
	@param Col_End		截取列结束索引值
	*/
	extern LZHAPI void Block(InputArray src, OutputArray dst, int32 Row_Start, int32 Row_End, int32 Col_Start, int32 Col_End, int32 Chennel_Start = 0, int32 Chennel_End = 0);
	/**
	@brief 返回随机生成元素size.x*size.y*size.z矩阵
	@param low		下界
	@param top		上界
	@param size		矩阵大小
	@param isdouble 是否生成浮点数
	*/
	extern LZHAPI Array Rand(mat_t low, mat_t top, Size3 size, bool isdouble = false);
	/**
	@brief 返回逆序矩阵，矩阵需为一维向量
	@param src	矩阵
	*/
	extern LZHAPI void reverse(InputArray src, OutputArray dst);
	/**
	@brief 返回生成h*w*c的矩阵，随机抽取矩阵src元素填充
	@param src	矩阵
	@param h	矩阵行数
	@param w	矩阵列数
	@param c	矩阵通道数
	*/
	extern LZHAPI void randSample(InputArray src, OutputArray dst, int32 w, int32 h, int32 c = 1);
	/**
	@brief 返回随机抽取num次矩阵src的行或列组成的矩阵
	@param src	矩阵
	@param rc	抽取方式
	@param num	抽取次数
	*/
	extern LZHAPI void randSample(InputArray src, OutputArray dst, RANK rc, int32 num = 1);
	/**
	@brief 返回op操作结果
	@param src 矩阵
	*/
	extern LZHAPI void Operator(InputArray src, OutputArray dst, mat_t(*op)(mat_t));
	/**
	@brief 返回op操作结果
	@param src 矩阵
	*/
	extern LZHAPI void Operator(InputArray m1, InputArray m2, OutputArray dst, mat_t(*op)(mat_t, mat_t));
	/**
	@brief 返回按位非
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_not(InputOutputArray a);
	/**
	@brief 返回按位非
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_not(InputArray a, OutputArray dst);
	/**
	@brief 返回按位与
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_and(InputOutputArray a, InputArray b);
	/**
	@brief 返回按位与
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_and(InputOutputArray a, Val b);
	/**
	@brief 返回按位与
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_and(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回按位与
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_and(InputArray a, Val b, OutputArray dst);
	/**
	@brief 返回按位或
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_or(InputOutputArray a, Val b);
	/**
	@brief 返回按位或
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_or(InputOutputArray a, InputArray b);
	/**
	@brief 返回按位或
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_or(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回按位或
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Bitwise_or(InputArray a, Val b, OutputArray dst);
	/**
	@brief 返回cos结果
	@param src 矩阵
	*/
	extern LZHAPI void Cos(InputArray src, OutputArray dst);
	/**
	@brief 返回cosh结果
	@param src 矩阵
	*/
	extern LZHAPI void Cosh(InputArray src, OutputArray dst);
	/**
	@brief 返回sin结果
	@param src 矩阵
	*/
	extern LZHAPI void Sin(InputArray src, OutputArray dst);
	/**
	@brief 返回sinh结果
	@param src 矩阵
	*/
	extern LZHAPI void Sinh(InputArray src, OutputArray dst);
	/**
	@brief 返回tan结果
	@param src 矩阵
	*/
	extern LZHAPI void Tan(InputArray src, OutputArray dst);
	/**
	@brief 返回tanh结果
	@param src 矩阵
	*/
	extern LZHAPI void Tanh(InputArray src, OutputArray dst);
	/**
	@brief 返回acos结果
	@param src 矩阵
	*/
	extern LZHAPI void arcCos(InputArray src, OutputArray dst);
	/**
	@brief 返回acosh结果
	@param src 矩阵
	*/
	extern LZHAPI void arcCosh(InputArray src, OutputArray dst);
	/**
	@brief 返回asin结果
	@param src 矩阵
	*/
	extern LZHAPI void arcSin(InputArray src, OutputArray dst);
	/**
	@brief 返回asinh结果
	@param src 矩阵
	*/
	extern LZHAPI void arcSinh(InputArray src, OutputArray dst);
	/**
	@brief 返回atan结果
	@param src 矩阵
	*/
	extern LZHAPI void arcTan(InputArray src, OutputArray dst);
	/**
	@brief 返回atanh结果
	@param src 矩阵
	*/
	extern LZHAPI void arcTanh(InputArray src, OutputArray dst);
	/**
	@brief 返回sign结果
	@param src 矩阵
	*/
	extern LZHAPI void sign(InputArray src, OutputArray dst);
	/**
	@brief 返回atan2结果
	@param src 矩阵
	*/
	extern LZHAPI void arcTan2(InputArray y, InputArray x, OutputArray dst);
	/**
	@brief 弧度转角度
	@param src 矩阵
	*/
	extern LZHAPI void Radian2Angle(InputArray radian, OutputArray angle);
	/**
	@brief 角度转弧度
	@param src 矩阵
	*/
	extern LZHAPI void Angle2Radian(InputArray angle, OutputArray radian);
	/**
	@brief 返回矩阵的伴随矩阵
	@param src 矩阵
	*/
	extern LZHAPI void adj(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵的逆矩阵
	@param src 矩阵
	*/
	extern LZHAPI void inv(InputArray src, OutputArray dst);
	/**
	@brief 返回QR分解
	@param src 矩阵
	*/
	extern LZHAPI void QR(InputArray src, OutputArray Q, OutputArray R);
	/**
	@brief 返回矩阵特征值
	@param src 矩阵
	@param eigenvalues 特征值
	@param eigenvectors 特征向量
	*/
	extern LZHAPI void eig(InputArray src, OutputArray eigenvalues, OutputArray eigenvectors = noArray(), int iterator_num = 50);
	/**
	@brief 返回对角线矩阵
	@param src	向量
	@param k	第k条对角线
	*/
	extern LZHAPI void diag(InputArray src, OutputArray dst, int32 k = 0);
	/**
	@brief 返回对角线向量
	@param src	矩阵
	@param dst	对角线向量
	*/
	extern LZHAPI void diagonal(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵的伪逆矩阵
	@param src	矩阵
	@param dire 伪逆矩阵的计算方式
	*/
	extern LZHAPI void pinv(InputArray src, OutputArray dst, Dire dire = LEFT);
	/**
	@brief 返回矩阵的转置矩阵
	@param src 矩阵
	*/
	extern LZHAPI void tran(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵的绝对值矩阵
	@param src 矩阵
	*/
	extern LZHAPI void Abs(InputArray src, OutputArray dst);
	/**
	@brief 返回angle度2*2的旋转矩阵
	@param angle 角度
	*/
	extern LZHAPI OMat getRotationMat2D(Point2f center, Val angle);
	/**
	@brief 返回angle度3*3的旋转矩阵
	@param angle 角度
	*/
	extern LZHAPI OMat getRotationMat3D(Point2f center, Val angle);
	/**
	@brief 返回矩阵num次幂
	@param src 矩阵
	@param num 次幂
	*/
	extern LZHAPI void POW(InputArray src, OutputArray dst, int32 num);
	/**
	@brief 返回矩阵取反
	@param src 矩阵
	*/
	extern LZHAPI void Opp(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵按行或列之和
	@param src	矩阵
	@param rc	求和的方向
	*/
	extern LZHAPI void Sum(InputArray src, OutputArray dst, RANK rc);
	/**
	@brief 返回矩阵按元素取指数
	@param src 矩阵
	*/
	extern LZHAPI void Exp(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵按元素取对数
	@param src 矩阵
	*/
	extern LZHAPI void Log(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵按元素取对数
	@param src 矩阵
	*/
	extern LZHAPI void Log10(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵按元素取开方
	@param src 矩阵
	*/
	extern LZHAPI void Sqrt(InputArray src, OutputArray dst);
	/**
	@brief 返回矩阵按元素取num次幂
	@param src 矩阵
	@param num 次幂
	*/
	extern LZHAPI void Pow(InputArray src, OutputArray dst, Val num);
	/**
	@brief 返回矩阵val/src按元素除
	@param src 矩阵
	@param val 除数
	*/
	extern LZHAPI void Div(InputArray src, OutputArray dst, Val val, Dire dire = RIGHT);
	/**
	@brief 返回矩阵除法
	@param a	被除矩阵
	@param b	除矩阵
	@param dire 除法方式
	*/
	extern LZHAPI void Div(InputArray a, InputArray b, OutputArray dst, Dire dire = RIGHT);
	/**
	@brief 返回哈达玛积
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Mult(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回哈达玛积
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Hadamard(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回矩阵乘法
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Dot(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回差值
	@param a 矩阵
	@param b 矩阵
	*/
	extern LZHAPI void Sub(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回和值
	@param a 矩阵
	@param b 矩阵
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
	@brief 返回矩阵按元素取a和b之间的最大值
	@param a 比较值
	@param b 比较矩阵
	*/
	extern LZHAPI void Max(Val a, InputArray b, OutputArray dst);
	/**
	@brief 返回矩阵按元素取a和b之间的最大值
	@param a 比较矩阵
	@param b 比较矩阵
	*/
	extern LZHAPI void Max(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回矩阵按元素取a和b之间的最小值
	@param a 比较值
	@param b 比较矩阵
	*/
	extern LZHAPI void Min(Val a, InputArray b, OutputArray dst);
	/**
	@brief 返回矩阵按元素取a和b之间的最小值
	@param a 比较矩阵
	@param b 比较矩阵
	*/
	extern LZHAPI void Min(InputArray a, InputArray b, OutputArray dst);
	/**
	@brief 返回修改维度的矩阵
	不允许改变矩阵长度
	@param src	输入
	@param size 矩阵大小
	*/
	extern LZHAPI void Reshape(InputArray src, Size3 size, OutputArray dst);
	/**
	@brief 返回矩阵
	按通道求和
	@param src	输入
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